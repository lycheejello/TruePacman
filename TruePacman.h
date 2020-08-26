/****************************************************************************
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.
 
 http://www.cocos2d-x.org
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "Actor.h"


class HelloWorld : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);

    void update(float) override;

    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);

private:
    // touch events
    void setupTouch();
    bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event);
    void onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *event);
    void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *event);
    void onTouchCancelled(cocos2d::Touch *touch, cocos2d::Event *event);

    void handleTouchEvents();
    bool isTouchDown;
    float initialTouchPos[2];
    float currentTouchPos[2];

    void setupColliderEvents();

    cocos2d::SpriteFrameCache* spriteCache;

    std::array<std::array<char, 19>, 22> grid;
    void initGrid(bool isReset);
    std::vector<cocos2d::Sprite*> walls;

    Actor* player;
    std::vector<Actor*> ghosts;
    void moveActor(Actor* actor, float movement);
    void moveGhost(Actor* ghost, float movement);

    bool onContactBegin(cocos2d::PhysicsContact& contact);

    int score = 0;
    int highScore = 100;
    cocos2d::Label* scoreLabel;
    cocos2d::Label* highScoreLabel;
    cocos2d::Label* centerLabel;
    float delayTime = 3;

    void onGameOver();
    bool isGameOver = false;
    void resetGame();

    int numCoins = 0;
    void eatCoin(cocos2d::Node* node);
};

#endif // __HELLOWORLD_SCENE_H__
