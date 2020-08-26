
#include "Actor.h"

USING_NS_CC;

Actor::Actor() { }

Actor::Actor(Sprite* s) {
    this->sprite = s;
}

Sprite* Actor::getSprite() {
    return sprite;
}

void Actor::setPosition(Vec2 pos) {
    sprite->setPosition(pos);
}

Vec2 Actor::getPosition() {
    return sprite->getPosition();
}

void Actor::updateCurrentDirection() {
    currentDirection = nextDirection;
}

Actor::Direction Actor::getCurrentDirection() {
    return currentDirection;
}

void Actor::setNextDirection(Direction d) {
    nextDirection = d;
    moving = true;
}

Actor::Direction Actor::getNextDirection() {
    return nextDirection;
}

bool Actor::isMoving() {
    return moving;
}

void Actor::setMoving(bool isMoving) {
    moving = isMoving;
}

Vec2 Actor::getNextPos(float moveSpeed, bool useNextDirection) {
    Vec2 pos = sprite->getPosition();
    if (!isMoving()) {
        return pos;
    }

    Direction direction = useNextDirection ? nextDirection : currentDirection;

    switch (direction) {
        case up:
            pos.y += moveSpeed;
            break;
        case right:
            pos.x += moveSpeed;
            break;
        case down:
            pos.y -= moveSpeed;
            break;
        case left:
            pos.x -= moveSpeed;
            break;
    };

    return pos;
}

bool Actor::collidesWith(std::vector<cocos2d::Sprite*> sprites) {
    for (Sprite* s : sprites) {
        if (s->getBoundingBox().intersectsRect(this->sprite->getBoundingBox())) {
            return true;
        }
    }
    return false;
}
