#include "FinishScene.h"

USING_NS_CC;

Scene* FinishScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = FinishScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool FinishScene::init()
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

	auto winSprite = Sprite::create("finish.png");
	winSprite->setPosition(Point(visibleSize.width / 2, visibleSize.height / 2));
	this->addChild(winSprite);

	return true;
}