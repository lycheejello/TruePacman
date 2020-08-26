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

#include "TruePacman.h"

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    return HelloWorld::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool HelloWorld::init() {

    // 1. super init first
    if (!Scene::initWithPhysics()) {
        return false;
    }

    setupTouch();
    setupColliderEvents();

    spriteCache = SpriteFrameCache::getInstance();
    spriteCache->addSpriteFramesWithFile("raccoon.plist");


    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    auto p1 = Label::createWithTTF("1 UP", "fonts/Marker Felt.ttf", 24);
    scoreLabel = Label::createWithTTF("0", "fonts/Marker Felt.ttf", 24);
    auto hs = Label::createWithTTF("HIGH SCORE", "fonts/Marker Felt.ttf", 24);
    highScoreLabel = Label::createWithTTF("100", "fonts/Marker Felt.ttf", 24);
    centerLabel = Label::createWithTTF("Ready!", "fonts/Marker Felt.ttf", 24);

    if (p1 == nullptr || scoreLabel == nullptr) {
        problemLoading("'fonts/Marker Felt.ttf'");
    } else {
        p1->setPosition( Vec2(20 + origin.x, origin.y + visibleSize.height - p1->getContentSize().height));
        auto pos = p1->getPosition();
        pos.add(Vec2(20.0f, 5 - scoreLabel->getContentSize().height));
        scoreLabel->setPosition(pos);

        hs->setPosition(Vec2(origin.x + visibleSize.width / 2,
                             origin.y + visibleSize.height - p1->getContentSize().height));
        pos = hs->getPosition();
        pos.add(Vec2(0.0, 5 - highScoreLabel->getContentSize().height));
        highScoreLabel->setPosition(pos);

        centerLabel->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));

        addChild(p1, 3);
        addChild(scoreLabel, 3);
        addChild(hs, 3);
        addChild(highScoreLabel, 3);
        addChild(centerLabel, 3);
    }

    initGrid(false);

    scheduleUpdate();

    return true;
}

void HelloWorld::initGrid(bool isReset) {
    //e empty
    //w wall
    //p player spawn
    //ghost
    //screen space bottom left x, y, correlates with map[x][sizeof(y) - y]

    //19x22
    //                 0    1    2    3    4    5    6    7    8    9    0    1    2    3    4    5    6    7    8
    grid = {{       {{'w', 'w', 'w', 'w', 'w', 'w', 'w', 'w', 'w', 'w', 'w', 'w', 'w', 'w', 'w', 'w', 'w', 'w', 'w'}},
                    {{'w', 'c', 'c', 'c', 'c', 'c', 'c', 'c', 'c', 'w', 'c', 'c', 'c', 'c', 'c', 'c', 'c', 'c', 'w'}},
                    {{'w', 'c', 'w', 'w', 'c', 'w', 'w', 'w', 'c', 'w', 'c', 'w', 'w', 'w', 'c', 'w', 'w', 'c', 'w'}},
                    {{'w', 'c', 'w', 'w', 'c', 'w', 'w', 'w', 'c', 'w', 'c', 'w', 'w', 'w', 'c', 'w', 'w', 'c', 'w'}},
                    {{'w', 'c', 'c', 'c', 'c', 'c', 'c', 'c', 'c', 'c', 'c', 'c', 'c', 'c', 'c', 'c', 'c', 'c', 'w'}},
                    {{'w', 'c', 'w', 'w', 'c', 'w', 'c', 'w', 'w', 'w', 'w', 'w', 'c', 'w', 'c', 'w', 'w', 'c', 'w'}},
                    {{'w', 'c', 'c', 'c', 'c', 'w', 'c', 'c', 'c', 'w', 'c', 'c', 'c', 'w', 'c', 'c', 'c', 'c', 'w'}},
                    {{'w', 'w', 'w', 'w', 'c', 'w', 'w', 'w', 'c', 'w', 'c', 'w', 'w', 'w', 'c', 'w', 'w', 'w', 'w'}},
                    {{'e', 'e', 'e', 'w', 'c', 'w', 'c', 'c', 'c', 'c', 'c', 'c', 'c', 'w', 'c', 'w', 'e', 'e', 'e'}},
                    {{'w', 'w', 'w', 'w', 'c', 'w', 'c', 'w', 'w', 'w', 'w', 'w', 'c', 'w', 'c', 'w', 'w', 'w', 'w'}},
                    {{'c', 'c', 'c', 'c', 'c', 'c', 'c', 'g', 'e', 'e', 'e', 'g', 'c', 'c', 'c', 'c', 'c', 'c', 'c'}},
                    {{'w', 'w', 'w', 'w', 'c', 'w', 'c', 'w', 'w', 'w', 'w', 'w', 'c', 'w', 'c', 'w', 'w', 'w', 'w'}},
                    {{'e', 'e', 'e', 'w', 'c', 'w', 'c', 'c', 'c', 'c', 'c', 'c', 'c', 'w', 'c', 'w', 'e', 'e', 'e'}},
                    {{'w', 'w', 'w', 'w', 'c', 'w', 'c', 'w', 'w', 'w', 'w', 'w', 'c', 'w', 'c', 'w', 'w', 'w', 'w'}},
                    {{'w', 'c', 'c', 'c', 'c', 'c', 'c', 'c', 'c', 'w', 'c', 'c', 'c', 'c', 'c', 'c', 'c', 'c', 'w'}},
                    {{'w', 'c', 'w', 'w', 'c', 'w', 'w', 'w', 'c', 'w', 'c', 'w', 'w', 'w', 'c', 'w', 'w', 'c', 'w'}},
                    {{'w', 'c', 'c', 'w', 'c', 'c', 'c', 'c', 'c', 'p', 'c', 'c', 'c', 'c', 'c', 'w', 'c', 'c', 'w'}},
                    {{'w', 'w', 'c', 'w', 'c', 'w', 'c', 'w', 'w', 'w', 'w', 'w', 'c', 'w', 'c', 'w', 'c', 'w', 'w'}},
                    {{'w', 'c', 'c', 'c', 'c', 'w', 'c', 'c', 'c', 'w', 'c', 'c', 'c', 'w', 'c', 'c', 'c', 'c', 'w'}},
                    {{'w', 'c', 'w', 'w', 'w', 'w', 'w', 'w', 'c', 'w', 'c', 'w', 'w', 'w', 'w', 'w', 'w', 'c', 'w'}},
                    {{'w', 'c', 'c', 'c', 'c', 'c', 'c', 'c', 'c', 'c', 'c', 'c', 'c', 'c', 'c', 'c', 'c', 'c', 'w'}},
                    {{'w', 'w', 'w', 'w', 'w', 'w', 'w', 'w', 'w', 'w', 'w', 'w', 'w', 'w', 'w', 'w', 'w', 'w', 'w'}} }};

    if (isReset) {
        player->getSprite()->removeFromParentAndCleanup(true);
        delete player;
        for (Actor* ghost : ghosts) {
            ghost->getSprite()->removeFromParentAndCleanup(true);
        }
        ghosts.clear();
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    float cellSize = visibleSize.width / grid[0].size();
    int bottomSpace = 5 * cellSize;

    for (int i = 0; i < grid.size(); i++) {
        for (int j = 0; j < grid[0].size(); j++) {
            switch (grid[i][j]) {
                case 'w': {
                    if (isReset) {
                        break;
                    }
                    auto sprite = Sprite::createWithSpriteFrameName("Wall.png");
                    if (sprite == nullptr) {
                        problemLoading("'wall.png'");
                    } else {
                        sprite = Sprite::create("wall.png");

                        float nodeScale = cellSize / sprite->getBoundingBox().size.width;
                        sprite->setScale(nodeScale);
                        sprite->setAnchorPoint(Vec2(0, 0));

                        Vec2 startPos = Vec2(origin.x + cellSize * j,
                                             origin.y + cellSize * (grid.size() - i) + bottomSpace);
                        sprite->setPosition(startPos);
                        sprite->setColor(Color3B(22, 22, 222));
                        addChild(sprite, 1);
                        walls.push_back(sprite);
                    }
                    break;
                }
                case 'p': {
                    Sprite *playerSprite = Sprite::createWithSpriteFrameName("Raccoon100-f.png");
                    if (playerSprite == nullptr) {
                        problemLoading("Raccoon100-f.png");
                    } else {

                        auto playerPhysics = PhysicsBody::createBox( playerSprite->getContentSize(), PhysicsMaterial(0.1f, 1.0f, 0.0f));
                        playerPhysics->setDynamic(false);
                        playerPhysics->setContactTestBitmask(0xFFFFFFFF);
                        playerSprite->addComponent(playerPhysics);
                        playerSprite->setTag(0);

                        float nodeScale = cellSize / playerSprite->getBoundingBox().size.height;
                        playerSprite->setScale(0.75f * nodeScale);
                        playerSprite->setAnchorPoint(Vec2(0, 0));

                        Vec2 startPos = Vec2(2 + origin.x + cellSize * j,
                                             2 + origin.y + cellSize * (grid.size() - i) + bottomSpace);
                        playerSprite->setPosition(startPos);

                        Rect aabb = playerSprite->getBoundingBox();
                        DrawNode* drawNode = DrawNode::create();
                        drawNode->drawRect(aabb.origin, aabb.origin + aabb.size, Color4F(1, 0, 0, 1));
                        addChild(drawNode, 100);

                        player = new Actor(playerSprite);
                        addChild(playerSprite, 2);
                    }
                    break;
                }
                case 'g': {

                    Sprite *ghostSprite = Sprite::createWithSpriteFrameName("AdBlock100.png");
                    //player = Sprite::create("Raccoon_Happy.png");
                    if (ghostSprite == nullptr) {
                        problemLoading("AdBlock100.png");
                    } else {

                        float nodeScale = cellSize / ghostSprite->getBoundingBox().size.height;
                        ghostSprite->setScale(0.75f * nodeScale);
                        ghostSprite->setAnchorPoint(Vec2(0, 0));


                        Vec2 startPos = Vec2(2 + origin.x + cellSize * j,
                                             2 + origin.y + cellSize * (grid.size() - i) + bottomSpace);
                        ghostSprite->setPosition(startPos);
                        ghostSprite->setColor(Color3B(255, 0, 0));

                        auto ghostPhysics = PhysicsBody::createBox(
                                ghostSprite->getContentSize(),
                                PhysicsMaterial(0.1f, 1.0f, 0.0f));
                        ghostPhysics->setDynamic(false);
                        ghostPhysics->setContactTestBitmask(0xFFFFFFFF);
                        ghostSprite->addComponent(ghostPhysics);
                        ghostSprite->setTag(1);

                        Actor* ghost = new Actor(ghostSprite);
                        ghost->setNextDirection(Actor::up);
                        addChild(ghostSprite, 2);
                        ghosts.push_back(ghost);
                    }
                    break;
                } case 'c': {
                    numCoins += 1;
                    auto coin = Sprite::createWithSpriteFrameName("Coin100.png");
                    if (coin == nullptr) {
                        problemLoading("Coin100.png");
                    } else {

                        float nodeScale = cellSize / coin->getBoundingBox().size.width;
                        coin->setScale(nodeScale * 0.25f);

                        Vec2 startPos = Vec2(origin.x + (cellSize / 2) + cellSize * j,
                                             origin.y + (cellSize / 2) + cellSize * (grid.size() - i) + bottomSpace);
                        coin->setPosition(startPos);

                        auto coinPhysics = PhysicsBody::createBox(coin->getContentSize(),
                                                                  PhysicsMaterial(0.1f, 1.0f,
                                                                                  0.0f));
                        coinPhysics->setDynamic(false);
                        coinPhysics->setContactTestBitmask(0xFFFFFFFF);
                        coin->addComponent(coinPhysics);
                        coin->setTag(2);

                        addChild(coin, 1);
                    }
                    break;
                }
            }
        }
    }
}

void HelloWorld::update(float delta) {

    if (isGameOver) {
        if (delayTime > 0) {
            delayTime -= delta;
            return;
        } else {
            resetGame();
        }
    }

    if (delayTime > 0) {
        delayTime -= delta;
        return;
    }
    centerLabel->setString("");

    //update high score
    if (score > highScore) {
        highScore = score;
        highScoreLabel->setString(std::to_string(highScore));
    }

    //all dots collected, reset
    if (numCoins == 0) {
        centerLabel->setString("Ready");
        delayTime = 3;
        initGrid(true);
    }

    handleTouchEvents();

    moveActor(player, delta * 50);

    for (Actor* ghost : ghosts) {
        moveGhost(ghost, delta * 51);
    }



}

void HelloWorld::setupTouch() {
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);

    listener->onTouchBegan = CC_CALLBACK_2(HelloWorld::onTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(HelloWorld::onTouchMoved, this);
    listener->onTouchEnded = CC_CALLBACK_2(HelloWorld::onTouchEnded, this);
    listener->onTouchCancelled = CC_CALLBACK_2(HelloWorld::onTouchCancelled, this);

    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    isTouchDown = false;

    initialTouchPos[0] = 0;
    initialTouchPos[1] = 0;
}

void HelloWorld::handleTouchEvents() {
    auto visibleSize = Director::getInstance()->getVisibleSize();
    //handle touch
    if (isTouchDown) {
        if (initialTouchPos[0] - currentTouchPos[0] > visibleSize.width * 0.05)
        {
            log("SWIPED LEFT");
            player->setNextDirection(Actor::left);
            isTouchDown = false;
        }
        else if (initialTouchPos[0] - currentTouchPos[0] < - visibleSize.width * 0.05)
        {
            log("SWIPED RIGHT");
            player->setNextDirection(Actor::right);
            isTouchDown = false;
        }
        else if (initialTouchPos[1] - currentTouchPos[1] > visibleSize.width * 0.05)
        {
            log("SWIPED DOWN");
            player->setNextDirection(Actor::down);
            isTouchDown = false;
        }
        else if (initialTouchPos[1] - currentTouchPos[1] < - visibleSize.width * 0.05)
        {
            log("SWIPED UP");
            player->setNextDirection(Actor::up);
            //player->getPhysicsBody()->setVelocity(Vec2(0, 100));
            isTouchDown = false;
        }
    }
}


bool HelloWorld::onTouchBegan(Touch *touch, Event *event)
{
    initialTouchPos[0] = touch->getLocation().x;
    initialTouchPos[1] = touch->getLocation().y;
    currentTouchPos[0] = touch->getLocation().x;
    currentTouchPos[1] = touch->getLocation().y;

    isTouchDown = true;

    //log("onTouchBegan ID: %d, X: %f, Y: %f", touch->getID(), touch->getLocation().x, touch->getLocation().y);

    return true;
}

void HelloWorld::onTouchMoved(Touch *touch, Event *event)
{
    currentTouchPos[0] = touch->getLocation().x;
    currentTouchPos[1] = touch->getLocation().y;
}

void HelloWorld::onTouchEnded(Touch *touch, Event *event)
{
    isTouchDown = false;
}

void HelloWorld::onTouchCancelled(Touch *touch, Event *event)
{
    onTouchEnded(touch, event);
}

bool HelloWorld::onContactBegin(PhysicsContact& contact) {
    auto nodeA = contact.getShapeA()->getBody()->getNode();
    auto nodeB = contact.getShapeB()->getBody()->getNode();

    if (nodeA && nodeB)
    {
        //0 player
        //1 ghost
        //2 coin
        if (nodeA->getTag() == 0 && nodeB->getTag() == 2) {
            eatCoin(nodeB);
        } else if (nodeB->getTag() == 0 && nodeA->getTag() == 2) {
            eatCoin(nodeA);
        } else if ( (nodeA->getTag() == 0 && nodeB->getTag() == 1) ||
                    (nodeA->getTag() == 1 && nodeB->getTag() == 0)) {
            onGameOver();
        }
    }
    return true;
}


void HelloWorld::setupColliderEvents() {
    auto contactListener = EventListenerPhysicsContact::create();
    contactListener->onContactBegin = CC_CALLBACK_1(HelloWorld::onContactBegin, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);
}

void HelloWorld::moveActor(Actor* actor, float movement) {

    Vec2 startPos = actor->getPosition();
    bool didTurn = false;

    //attempt to move to the next position if there is a turn queued
    if (actor->getCurrentDirection() != actor->getNextDirection()) {
        //check 3 steps ahead to make sure we can turn
        actor->setPosition(actor->getNextPos(movement * 4, true));
        //if turn not successful, return to original position
        if (actor->collidesWith(walls)) {
            log("collide on actor turn");
            actor->setPosition(startPos);
        } else {
            //reset and actually take the step
            actor->setPosition(startPos);
            actor->setPosition(actor->getNextPos(movement, true));
            didTurn = true;
            actor->updateCurrentDirection();
        }
    }

    //go straight if turn not queued or if turn not possible
    if (!didTurn) {
        actor->setPosition(actor->getNextPos(movement, false));
        if (actor->collidesWith(walls)) {
            log("collide on actor straight");
            actor->setMoving(false);
            actor->setPosition(startPos);
        } else {
            //Correct for looping to the other side of the screen
            Size visibleSize = Director::getInstance()->getVisibleSize();
            Vec2 origin = Director::getInstance()->getVisibleOrigin();

            auto pos = actor->getPosition();

            float actorHalfWidth = actor->getSprite()->getBoundingBox().size.width / 2;
            if (pos.x < origin.x) {
                pos.x = visibleSize.width + origin.x;
            }

            if (pos.x > visibleSize.width + origin.x) {
                pos.x = origin.x;
            }

            float actorHalfHeight = actor->getSprite()->getBoundingBox().size.height / 2;
            if (pos.y > visibleSize.height + origin.y) {
                pos.y = origin.y;
            }

            if (pos.y < origin.y) {
                pos.y = visibleSize.height + origin.y;
            }

            actor->setPosition(pos);
        }
    }
}

void HelloWorld::moveGhost(Actor* ghost, float movement) {
    //if the ghost is stuck, try the next direction
    if (!ghost->isMoving()) {
        switch(ghost->getCurrentDirection()) {
            case Actor::up:
                ghost->setNextDirection(Actor::right);
                break;
            case Actor::right:
                ghost->setNextDirection(Actor::down);
                break;
            case Actor::down:
                ghost->setNextDirection(Actor::left);
                break;
            case Actor::left:
                ghost->setNextDirection(Actor::up);
                break;
        }
        ghost->updateCurrentDirection();
    } else {
        //control the ghost with a simple heuristic - if turning is better than going straight, do it.
        auto ghostDir = ghost->getCurrentDirection();
        float margin = player->getSprite()->getContentSize().width;
        if (ghostDir == Actor::up || ghostDir == Actor::down) {
            if (ghost->getPosition().x > player->getPosition().x - margin) {
                ghost->setNextDirection(Actor::left);
            } else if (ghost->getPosition().x < player->getPosition().x + margin) {
                ghost->setNextDirection(Actor::right);
            }
        } else {
            if (ghost->getPosition().y > player->getPosition().y - margin) {
                ghost->setNextDirection(Actor::down);
            } else if (ghost->getPosition().y < player->getPosition().y + margin) {
                ghost->setNextDirection(Actor::up);
            }
        }
    }
    moveActor(ghost, movement);
}

void HelloWorld::eatCoin(Node* coin) {
    log("eat");
    coin->removeFromParentAndCleanup(true);
    numCoins -= 1;
    score += 10;
    scoreLabel->setString(std::to_string(score));
}

void HelloWorld::onGameOver() {
    isGameOver = true;
    delayTime = 3;
    centerLabel->setString("GAME OVER");
}

void HelloWorld::resetGame() {
    isGameOver = false;
    delayTime = 3;
    centerLabel->setString("READY");
    initGrid(true);
    score = 0;
    scoreLabel->setString(std::to_string(score));
}
