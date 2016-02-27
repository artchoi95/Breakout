#pragma once

#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"

using namespace cocos2d;

class GameScene : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    // implement the "static create()" method manually
    CREATE_FUNC(GameScene);

	Sprite* ball;
	Sprite* paddle;
	Sprite* edgeSp;
	PhysicsBody* paddleBody;
	cocos2d::Label* scoreLabel;
	PhysicsWorld* pWorld;
	void update(float dt);

private:
	int x, y, score;
	void setPhyWorld(PhysicsWorld* world) {pWorld = world;};
	enum Direction { UP, DOWN, LEFT, RIGHT, STOP };
	Direction direction;
	void updateDirection(EventKeyboard::KeyCode keyCode, Event* event);
	void onKeyRelease(EventKeyboard::KeyCode keyCode, Event* event);
	bool onContactBegin(cocos2d::PhysicsContact &contact);
};

#endif // __GAME_SCENE_H__
