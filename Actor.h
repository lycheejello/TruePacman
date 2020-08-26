
#ifndef PROJ_ANDROID_ACTOR_H
#define PROJ_ANDROID_ACTOR_H

#include "cocos2d.h"

class Actor {

public:
    Actor();
    Actor(cocos2d::Sprite* sprite);

    cocos2d::Sprite* getSprite();

    enum Direction {up, down, left, right};

    void setPosition(cocos2d::Vec2 pos);
    cocos2d::Vec2 getPosition();
    void updateCurrentDirection();
    Direction getCurrentDirection();
    void setNextDirection(Direction d);
    Direction getNextDirection();
    bool isMoving();
    void setMoving(bool isMoving);

    cocos2d::Vec2 getNextPos(float moveSpeed, bool attemptTurn);

    bool collidesWith(std::vector<cocos2d::Sprite*> sprites);
private:
    cocos2d::Sprite* sprite;
    Direction currentDirection = left;
    Direction nextDirection = left;
    bool moving = true;


};


#endif //PROJ_ANDROID_ACTOR_H
