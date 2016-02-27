#include "GameScene.h"
#include "GameOverScene.h"
#include "FinishScene.h"
USING_NS_CC;

Scene* GameScene::createScene()
{
    auto scene = Scene::createWithPhysics();
	scene->getPhysicsWorld()->setGravity(Vec2(0, 0));
    
    auto layer = GameScene::create();
	layer->setPhyWorld(scene->getPhysicsWorld());
	scene->addChild(layer);

    return scene;
}


bool GameScene::init()
{
	if (!Layer::init()){
		return false;
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto backgroundSprite = Sprite::create("breakoutbackground.jpg");
	backgroundSprite->setPosition(Point(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
	this->addChild(backgroundSprite);

	edgeSp = Sprite::create();
	auto boundBody = PhysicsBody::createEdgeBox(visibleSize, PHYSICSBODY_MATERIAL_DEFAULT, 3);
	boundBody->getShape(0)->setRestitution(1);
	boundBody->getShape(0)->setFriction(0);
	boundBody->getShape(0)->setDensity(1);
	edgeSp->setPosition(Point(visibleSize.width / 2, visibleSize.height / 2)); 
	edgeSp->setPhysicsBody(boundBody);
	boundBody->setContactTestBitmask(0x000001); 
	this->addChild(edgeSp);
	edgeSp->setTag(0);

	ball = Sprite::create("ball.png");
	ball->setPosition(visibleSize.width / 2, 65);
	auto ballBody = PhysicsBody::createCircle(ball->getContentSize().width / 2, PhysicsMaterial(1,1,0));
	ballBody->setGravityEnable(false); 
	Vect force = Vect(500000, 500000);
	ballBody->applyImpulse(force);
	ball->setPhysicsBody(ballBody);
	ballBody->setContactTestBitmask(0x000001); 
	ball->setTag(1);
	this->addChild(ball);

	paddle = Sprite::create("paddle.png");
	auto paddleBody = PhysicsBody::createBox(paddle->getContentSize(), PhysicsMaterial(10, 1, 0));
	paddleBody->setGravityEnable(false);
	paddleBody->setDynamic(false);
	paddle->setPosition(visibleSize.width / 2, 40);
	paddle->setPhysicsBody(paddleBody);
	paddleBody->setContactTestBitmask(0x000001); 
	ball->setTag(2);
	this->addChild(paddle);
	
	for (int i = 0; i < 15; i++) {
		int brickGap = 0;
		auto brick = Sprite::create("brick.png");
		brick->setColor(Color3B::BLUE);
		auto brickBody = PhysicsBody::createBox(brick->getContentSize() - Size(10, 10), PhysicsMaterial(10, 1, 0));
		brickBody->setDynamic(false);
		int xOffset = brickGap + brick->getContentSize().width / 2 +
			((brick->getContentSize().width + brickGap) * i);
		brick->setPosition(xOffset, 500);
		brickBody->setContactTestBitmask(0x000001);
		brick->setPhysicsBody(brickBody);
		brick->setTag(3);
		this->addChild(brick);
	}
	
	for (int i = 0; i < 15; i++) {
		int brickGap = 0;
		auto brick = Sprite::create("brick.png");
		brick->setColor(Color3B::GREEN);
		auto brickBody = PhysicsBody::createBox(brick->getContentSize() - Size(10, 10), PhysicsMaterial(10, 1, 0));
		brickBody->setDynamic(false);
		int xOffset = brickGap + brick->getContentSize().width / 2 +
			((brick->getContentSize().width + brickGap) * i);
		brick->setPosition(xOffset, 550);
		brickBody->setContactTestBitmask(0x000001);
		brick->setPhysicsBody(brickBody);
		brick->setTag(4);
		this->addChild(brick);
	}
	
	for (int i = 0; i < 15; i++) {
		static int brickGap = 0;
		auto brick = Sprite::create("brick.png");
		brick->setColor(Color3B::YELLOW);
		auto brickBody = PhysicsBody::createBox(brick->getContentSize() - Size(10, 10), PhysicsMaterial(10, 1, 0));
		brickBody->setDynamic(false);
		int xOffset = brickGap + brick->getContentSize().width / 2 +
			((brick->getContentSize().width + brickGap) * i);
		brick->setPosition(xOffset, 600);
		brickBody->setContactTestBitmask(0x000001);
		brick->setPhysicsBody(brickBody);
		brick->setTag(5);
		this->addChild(brick);
	}

	auto contactListener = EventListenerPhysicsContact::create();
	contactListener->onContactBegin = CC_CALLBACK_1(GameScene::onContactBegin, this);
	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(contactListener, this);

	auto eventListener = EventListenerKeyboard::create();
	eventListener->onKeyPressed = CC_CALLBACK_2(GameScene::updateDirection, this);
	eventListener->onKeyReleased = CC_CALLBACK_2(GameScene::onKeyRelease, this);
	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(eventListener, this);

	score = 0;
	scoreLabel = cocos2d::Label::create();
	scoreLabel->setString("Score: " + std::to_string(score));
	scoreLabel->setSystemFontSize(20);
	scoreLabel->setAlignment(cocos2d::TextHAlignment::CENTER);

	addChild(scoreLabel, 0);
	scoreLabel->setPosition(100, 700);
	this->scheduleUpdate();
	
	return true;
}

void GameScene::update(float dt) {
	x = paddle->getPositionX();
	y = paddle->getPositionY();

	switch (direction) {
	case LEFT:
		paddle->setPosition(x - 20, y);
		break;
	case RIGHT:
		paddle->setPosition(x + 20, y);
		break;
	case STOP:
		paddle->setPosition(x, y);
		break;
	}

	if (paddle->getPositionX() > 1335) {
		paddle->setPosition(1335, y);
	}

	if (paddle->getPositionX() < 65) {
		paddle->setPosition(65, y);
	}

	bool isWin = true;

	Vector<PhysicsBody*> bodies = pWorld->getAllBodies();

	for each(PhysicsBody* body in bodies) {
		if (body->getNode()->getTag() == 3 || body->getNode()->getTag() == 4 || body->getNode()->getTag() == 5){
			isWin = false; 
		}
	}

	if (isWin == true){
		auto scene = FinishScene::createScene();
		Director::getInstance()->replaceScene(TransitionFade::create(0.5, scene));
	}


}


void GameScene::updateDirection(EventKeyboard::KeyCode keyCode, Event* event) {
	switch (keyCode) {
	case EventKeyboard::KeyCode::KEY_A:
		direction = LEFT;
		break;
	case EventKeyboard::KeyCode::KEY_D:
		direction = RIGHT;
		break;
	}
};

void GameScene::onKeyRelease(EventKeyboard::KeyCode keyCode, Event* event) {
	switch (keyCode) {
	case EventKeyboard::KeyCode::KEY_A:
		direction = STOP;
		break;
	case EventKeyboard::KeyCode::KEY_D:
		direction = STOP;
		break;
	}
}
bool GameScene::onContactBegin(PhysicsContact& contact)
{
	auto spriteA = (Sprite*)contact.getShapeA()->getBody()->getNode();
	auto spriteB = (Sprite*)contact.getShapeB()->getBody()->getNode();

	int tagA = spriteA->getTag();
	int tagB = spriteB->getTag();

	if (tagA == 3){
		score++;
		this->removeChild(spriteA, true);
		scoreLabel->setString("Score: " + std::to_string(score));
		scoreLabel->setSystemFontSize(20);
		scoreLabel->setAlignment(cocos2d::TextHAlignment::CENTER);

		scoreLabel->setPosition(100, 700);
	}

	if (tagB == 3){
		score++;
		this->removeChild(spriteB, true); 

		scoreLabel->setString("Score: " + std::to_string(score));
		scoreLabel->setSystemFontSize(20);		
		scoreLabel->setAlignment(cocos2d::TextHAlignment::CENTER);

		scoreLabel->setPosition(100, 700);
	}

	if (tagA == 4){
		score = score + 2;
		this->removeChild(spriteA, true);
		scoreLabel->setString("Score: " + std::to_string(score));
		scoreLabel->setSystemFontSize(20);
		scoreLabel->setAlignment(cocos2d::TextHAlignment::CENTER);

		scoreLabel->setPosition(100, 700);
	}

	if (tagB == 4){
		score = score + 2;
		this->removeChild(spriteB, true);

		scoreLabel->setString("Score: " + std::to_string(score));
		scoreLabel->setSystemFontSize(20);
		scoreLabel->setAlignment(cocos2d::TextHAlignment::CENTER);

		scoreLabel->setPosition(100, 700);
	}

	if (tagA == 5) {
		score = score + 3;
		this->removeChild(spriteA, true);
		scoreLabel->setString("Score: " + std::to_string(score));
		scoreLabel->setSystemFontSize(20);
		scoreLabel->setAlignment(cocos2d::TextHAlignment::CENTER);

		scoreLabel->setPosition(100, 700);
	}

	if (tagB == 5) {
		score = score + 3;
		this->removeChild(spriteB, true);

		scoreLabel->setString("Score: " + std::to_string(score));
		scoreLabel->setSystemFontSize(20);
		scoreLabel->setAlignment(cocos2d::TextHAlignment::CENTER);

		scoreLabel->setPosition(100, 700);
	}

	if ((tagA == 0 || tagB == 0) & (ball->getPositionY() <= paddle->getPositionY())){
		auto scene = GameOverScene::createScene();
		Director::getInstance()->replaceScene(TransitionFade::create(0.5, scene));
	}

	return true;
}