#pragma once
#include "GameObject.h"
#include <box2d/box2d.h>

class StaticObject : public GameObject {
protected:
    b2BodyDef   b2_bodyDef;
    b2Body*     b2_body = nullptr;
    float       f_scale;

public:
    StaticObject() = default;
    StaticObject(std::string str_spriteLoc, sf::Vector2f v2_position, float f_scaleIn)
        : GameObject(str_spriteLoc, v2_position), f_scale(f_scaleIn) {}


    virtual void update() = 0;
    virtual void draw(sf::RenderWindow& window) = 0;

    virtual float getAttackValue() {
        return 0.0f; 
    }

    b2Body* getBody() { 
        return b2_body; 
    }

    virtual ~StaticObject() = default;
};