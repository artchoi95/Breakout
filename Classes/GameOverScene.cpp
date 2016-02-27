#include "GameOverScene.h"

USING_NS_CC;

Scene* GameOverScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = GameOverScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool GameOverScene::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Layer::init())
	{
		return false;
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto backgroundSprite = Sprite::create("breakoutbackground.jpg");
	backgroundSprite->setPosition(Point(visibleSize.width / 2, visibleSize.height / 2));
	this->addChild(backgroundSprite);

	auto winSprite = Sprite::create("gameover.png");
	winSprite->setPosition(Point(visibleSize.width / 2, visibleSize.height / 2));
	this->addChild(winSprite);

	return true;
}