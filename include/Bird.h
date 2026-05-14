#pragma once
#include "DynamicObject.h"
#include <SFML/Graphics.hpp>
#include <string>

class Bird : public DynamicObject {
protected:
    sf::Sprite    sp_rendered;
    sf::Texture   sf_tex;
    b2CircleShape b2_circleShape;
    float         f_attackValue;
    std::string   str_spriteLocation;

public:
    Bird(b2Vec2 b2_posIn, b2World& b2_world, std::string spritePath,
        int i_frameWidthIn, int i_frameHeightIn,
        float f_radiusIn, float f_scaleIn, float f_attackValueIn);

    void update() override;
    void draw(sf::RenderWindow& window) override;
    float getAttackValue() override;
    void launch(b2Vec2 impulse);
    //b2Body* getBody();

    virtual void useAbility() = 0;

    virtual ~Bird() = default;
};