#pragma once
#include <box2d/box2d.h>
#include <set>
#include <vector>
#include <iostream>
#include "Pig.h"
#include "Bird.h"

class GameContactListener : public b2ContactListener {
public:
    std::set<b2Body*> destructionQueue;

    std::vector<Pig*> pigs;
    std::vector<Bird*>* birds = nullptr;
    std::vector<int>* birdDamage = nullptr;
    int* launchedBird = nullptr;

    void BeginContact(b2Contact* contact) override {
        b2Body* bodyA = contact->GetFixtureA()->GetBody();
        b2Body* bodyB = contact->GetFixtureB()->GetBody();

        if (birds == nullptr || birdDamage == nullptr || launchedBird == nullptr) {
            return;
        }

        b2Body* launchedBirdBody = (*birds)[*launchedBird]->getBody();

        if (bodyA != launchedBirdBody && bodyB != launchedBirdBody) {
            return;
        }

        for (int i = 0; i < pigs.size(); i++) {
            Pig* pig = pigs[i];

            if (pig->checkIfPopped()) {
                continue;
            }

            b2Body* pigBody = pig->getBody();

            //only damage if the other body is a pig
            if (bodyA == pigBody || bodyB == pigBody) {
                pig->takeHit((*birdDamage)[*launchedBird]);

                std::cout << "Pig hit! Health: "
                    << pig->getHealth() << std::endl;

                if (pig->checkIfPopped()) {
                    destructionQueue.insert(pigBody);
                }

                break;
            }
        }
    }
};