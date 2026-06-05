#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>
#include <iostream>
#include "Pig.h"
#include "Bird.h"
#include "RedBird.h"
#include "FastBird.h"
#include "HeavyBird.h"
#include "Slingshot.h"
#include "ContactListener.h" 
#include "UI.h"
#include "PigPool.h"
#include <thread>
#include <chrono>
#include <cmath>
#include <vector>
#include <mutex>
#include <future>
#include <fstream>
#include <sstream>


std::mutex loadingMutex;
float threadLoadingProgress = 0.0f;

//fake loading task for sprite data
void loadSpriteData() {
    std::cout << "Loading sprite data.. " << std::endl;
    for (int i = 0; i < 50; i++) {
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        std::lock_guard<std::mutex> lock(loadingMutex);
        threadLoadingProgress += 1.0f;
    }

    std::cout << "Sprite data loaded " << std::endl;
}



//fake loading task for physics data
void loadPhysicsData() {
    std::cout << "Loading physics data.." << std::endl;

    for (int i = 0; i < 50; i++) {
        std::this_thread::sleep_for(std::chrono::milliseconds(50));

        std::lock_guard<std::mutex> lock(loadingMutex);
            threadLoadingProgress+= 1.0f;
    }

    std::cout << "Physics data loaded" << std::endl;
}

//async file processing
int processCSVFile() {
    std::ifstream file("../assets/Docs/UserDetails.csv");
    if (!file.is_open()) {
        std::cout << "Failed to open CSV file" << std::endl;
        return 0;
    }

    std::string line;
    int lineCount = 0;

    while (std::getline(file, line)) {
        lineCount++;
    }
    
    file.close();

    std::cout << "CSV lines Processed: " << lineCount << std::endl;
    return lineCount;
}



int main() {

    // --- 1. WINDOW SETUP ---
    sf::RenderWindow window(sf::VideoMode(800, 600), "Annoyed_Flocks");
    window.setFramerateLimit(60);

    //title
    sf::Font font;
    if (!font.loadFromFile("../assets/fonts/angry-birds.ttf")) {
        std::cout << "Font failed to load" << std::endl;
    }

    sf::Text titleText;
    titleText.setFont(font);
    titleText.setString("Annoyed Flocks (drag with mouse to play)");
    titleText.setCharacterSize(20);
    titleText.setFillColor(sf::Color::Red);
    titleText.setPosition(50.0f, 10.0f);

    //start screen UI
    UI loadingUI;
    bool showStartScreen = true;
    sf::Clock startScreenClock;

    //threads used to replicate loading sprite and physics data
    std::thread spriteLoadingThread(loadSpriteData);
    std::thread physicsLoadingThread(loadPhysicsData);
    bool loadingThreadsJoined = false;

    //async csv processing
    std::future<int> csvFuture = std::async(std::launch::async, processCSVFile);
    bool csvProcessed = false;

    //adding static images on the start screen
    sf::Texture loadingBirdTexture;
    sf::Sprite loadingBirdLeft;
    sf::Sprite loadingBirdRight;

    loadingBirdTexture.loadFromFile("../assets/Ang_Birds/redBird2.png");

    loadingBirdLeft.setTexture(loadingBirdTexture);
    loadingBirdRight.setTexture(loadingBirdTexture);
    loadingBirdLeft.setPosition(150.0f, 180.0f);
    loadingBirdRight.setPosition(550.0f, 180.0f);
    loadingBirdLeft.setScale(0.2f, 0.2f);
    loadingBirdRight.setScale(0.2f, 0.2f);

    //Box2D works in meters. SFML works in pixels.
    const float SCALE = 30.0f;

    //Can set a definition for PI.
    const float PI = 3.1415927;

    //setup world.
    b2Vec2 b2_gravity(0.0f, 9.8f); // Earth-like gravity
    b2World world(b2_gravity);

    //Setup ground for the circle to move / bounce on.
    //Needs to have a body definition and a body. We use a raw pointer for the b2Body as Box2d does the management itself.
    //A body can be defined as having a position, velocity, and mass. 
    b2BodyDef b2_groundBodyDef;
    b2_groundBodyDef.position.Set(400.0f / SCALE, 590.0f / SCALE);
    b2Body* b2_groundBody = world.CreateBody(&b2_groundBodyDef);

    //Define a fixture shape that relates to the collision for the ground.
    b2PolygonShape b2_groundBox;
    b2_groundBox.SetAsBox(400.0f / SCALE, 10.0f / SCALE);
    b2_groundBody->CreateFixture(&b2_groundBox, 0.0f);

    //Set up the ground visualisation.
    sf::RectangleShape sf_groundVisual(sf::Vector2f(800.0f, 20.0f));
    sf_groundVisual.setOrigin(400.0f, 10.0f);
    sf_groundVisual.setFillColor(sf::Color(34, 139, 34)); // Forest Green

    //Setting up a wall for the ball to hit.
    b2BodyDef b2_wallDef;
    b2_wallDef.position.Set(750.0f / SCALE, 500.0f / SCALE);
    b2Body* b2_wallBody = world.CreateBody(&b2_wallDef);

    b2PolygonShape b2_wallBox;
    b2_wallBox.SetAsBox(10.0f / SCALE, 160.0f / SCALE);
    b2_wallBody->CreateFixture(&b2_wallBox, 0.0f);

    sf::RectangleShape sf_wallVisual(sf::Vector2f(20.0f, 160.0f));
    sf_wallVisual.setOrigin(10.0f, 80.0f);
    sf_wallVisual.setFillColor(sf::Color::Red);

    //Rather than having an immovable wall, we can use the dynamic body type to create one that can have velocity etc.
    b2BodyDef b2_plankDef;

    b2_plankDef.type = b2_dynamicBody;
    b2_plankDef.position.Set(550.0f / SCALE, 450.0f / SCALE);
    b2Body* b2_plankBody = world.CreateBody(&b2_plankDef);

    b2PolygonShape b2_plankBox;
    b2_plankBox.SetAsBox(10.0f / SCALE, 60.0f / SCALE);

    b2FixtureDef b2_plankFixture;
    b2_plankFixture.shape = &b2_plankBox;
    b2_plankFixture.density = 1.5f;   // Light wood
    b2_plankFixture.friction = 0.3f;
    b2_plankBody->CreateFixture(&b2_plankFixture);

    sf::RectangleShape sf_plankVisual(sf::Vector2f(20.0f, 120.0f));
    sf_plankVisual.setOrigin(10.0f, 60.0f);
    sf_plankVisual.setFillColor(sf::Color(139, 69, 19)); // Brown

    //Create a ball that is fired when space is pressed. We need to first have a dynamic ball to do it.
    b2BodyDef b2_ballDef;
    b2_ballDef.type = b2_dynamicBody;
    b2_ballDef.position.Set(100.0f / SCALE, 500.0f / SCALE);
    b2Body* b2_ballBody = world.CreateBody(&b2_ballDef);

    b2CircleShape b2_circleShape;
    b2_circleShape.m_radius = 15.0f / SCALE;

    b2FixtureDef b2_ballFixture;
    b2_ballFixture.shape = &b2_circleShape;
    b2_ballFixture.density = 1.0f;
    b2_ballFixture.restitution = 0.5f; // Bounciness
    b2_ballBody->CreateFixture(&b2_ballFixture);

    sf::CircleShape sf_ballVisual(15.0f);
    sf_ballVisual.setOrigin(15.0f, 15.0f);
    sf_ballVisual.setFillColor(sf::Color::Yellow);

    //BIRDS
    std::string redBirdSprite = "../assets/Ang_Birds/redBird2.png";
    std::string fastBirdSprite = "../assets/Ang_Birds/yellowBird.png";
    std::string heavyBirdSprite = "../assets/Ang_Birds/blackBird.png";

    //PIGS
    std::string pigSmall = "../assets/Ang_Birds/sprite_1.png";
    std::string pigMedium = "../assets/Ang_Birds/sprite_2.png";
    std::string pigLarge = "../assets/Ang_Birds/sprite_3.png";


    // bottom plank
    b2BodyDef b2_plank1Def;
    b2_plank1Def.position.Set(680.0f / SCALE, 530.0f / SCALE);
    b2Body* b2_plank1Body = world.CreateBody(&b2_plank1Def);
    b2PolygonShape b2_plank1Box;
    b2_plank1Box.SetAsBox(80.0f / SCALE, 5.0f / SCALE);
    b2_plank1Body->CreateFixture(&b2_plank1Box, 0.0f);

    sf::RectangleShape sf_plank1Visual(sf::Vector2f(160.0f, 10.0f));
    sf_plank1Visual.setOrigin(80.0f, 5.0f);
    sf_plank1Visual.setFillColor(sf::Color(139, 69, 19)); // Brown

    // top plank
    b2BodyDef b2_plank2Def;
    b2_plank2Def.position.Set(680.0f / SCALE, 470.0f / SCALE);
    b2Body* b2_plank2Body = world.CreateBody(&b2_plank2Def);
    b2PolygonShape b2_plank2Box;
    b2_plank2Box.SetAsBox(50.0f / SCALE, 5.0f / SCALE);
    b2_plank2Body->CreateFixture(&b2_plank2Box, 0.0f);

    sf::RectangleShape sf_plank2Visual(sf::Vector2f(100.0f, 10.0f));
    sf_plank2Visual.setOrigin(50.0f, 5.0f);
    sf_plank2Visual.setFillColor(sf::Color(139, 69, 19)); // Brown

    // planks health
    int i_plank1Health = 100;
    int i_plank2Health = 100;
    bool b_plank1Destroyed = false;
    bool b_plank2Destroyed = false; 

    //large pigs - 200 health
    Pig pig1(b2Vec2(640.0f / SCALE, 555.0f / SCALE), world, pigLarge, window, 250, 27.0f, SCALE, 100, 100);
    Pig pig2(b2Vec2(680.0f / SCALE, 555.0f / SCALE), world, pigLarge, window, 250, 27.0f, SCALE, 100, 100);
    Pig pig3(b2Vec2(720.0f / SCALE, 555.0f / SCALE), world, pigLarge, window, 250, 27.0f, SCALE, 100, 100);

    //medium pigs - 150 health
    Pig pig4(b2Vec2(655.0f / SCALE, 500.0f / SCALE), world, pigMedium, window, 200, 20.0f, SCALE, 96, 90);
    Pig pig5(b2Vec2(705.0f / SCALE, 500.0f / SCALE), world, pigMedium, window, 200, 20.0f, SCALE, 96, 90);

    //small pig - 100 health
    Pig pig6(b2Vec2(680.0f / SCALE, 445.0f / SCALE), world, pigSmall, window, 150, 15.0f, SCALE, 57, 57);

    //memory pool for pig enemies
    PigPool pigPool;
    pigPool.addPig(&pig1);
    pigPool.addPig(&pig2);
    pigPool.addPig(&pig3);
    pigPool.addPig(&pig4);
    pigPool.addPig(&pig5);
    pigPool.addPig(&pig6);

    std::cout << "Pig pool size: " << pigPool.getPoolSize() << std::endl;


    // keep pigs in place before game starts
    pig1.getBody()->SetGravityScale(0);
    pig2.getBody()->SetGravityScale(0);
    pig3.getBody()->SetGravityScale(0);
    pig4.getBody()->SetGravityScale(0);
    pig5.getBody()->SetGravityScale(0);
    pig6.getBody()->SetGravityScale(0);

    //press + drag
    bool isDragging = false;
    bool birdLaunched = false;
    sf::Vector2f dragStart;
    sf::Vector2f dragEnd;
    sf::Vector2f slingPosition(100.0f, 500.0f);
    float maxPullLeft = 80.0f;
    float maxPullDown = 80.0f;
    float launchStrength = 15.0f;

    RedBird redBird(b2Vec2(slingPosition.x / SCALE, slingPosition.y / SCALE), world, redBirdSprite, SCALE);
    FastBird fastBird(b2Vec2(slingPosition.x / SCALE, slingPosition.y / SCALE), world, fastBirdSprite, SCALE);
    HeavyBird heavyBird(b2Vec2(slingPosition.x / SCALE, slingPosition.y / SCALE), world, heavyBirdSprite, SCALE);

    std::vector<Bird*> birds;
    birds.push_back(&redBird);
    birds.push_back(&fastBird);
    birds.push_back(&heavyBird);

    birds[1]->getBody()->SetEnabled(false);
    birds[2]->getBody()->SetEnabled(false);

    int currentBird = 0;
    int launchedBird = 0;
    sf::Clock resetClock;
    bool resetTimerStarted = false;

    // bird damage values - red is neutral, fast does more, heavy does most
    std::vector<int> birdDamage = { 25, 50, 100 };


    GameContactListener contactListener;
    contactListener.pigs = { &pig1, &pig2, &pig3, &pig4, &pig5, &pig6 };
    contactListener.birds = &birds;
    contactListener.birdDamage = &birdDamage;
    contactListener.launchedBird = &launchedBird;
    world.SetContactListener(&contactListener);



    Slingshot slingshot;
    slingshot.loadBird("Red");

    // --- 7. MAIN LOOP ---
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            // INPUT HANDLING: Press SPACE to launch
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Space) {
                    // Reset position of the ball so that it can be fired again from its original poisition.
                    //b2_ballBody->SetTransform(b2Vec2(100.0f / SCALE, 500.0f / SCALE), 0);
                    //b2_ballBody->SetLinearVelocity(b2Vec2(0, 0));
                    //b2_ballBody->SetAngularVelocity(0);

                    // Apply impulse (X-axis, Y-axis) Negative Y is UP in Box2D because gravity is positive.
                    //b2_ballBody->ApplyLinearImpulse(b2Vec2(5.0f, -5.0f), b2_ballBody->GetWorldCenter(), true);

                    //std::cout << "Firing!!!!" << std::endl;

                    birds[currentBird]->getBody()->SetTransform(
                        b2Vec2(slingPosition.x / SCALE, slingPosition.y / SCALE), 0
                    );
                    birds[currentBird]->getBody()->SetLinearVelocity(b2Vec2(0, 0));
                    birds[currentBird]->getBody()->SetAngularVelocity(0);
                    birdLaunched = false;
                    resetTimerStarted = false;
                    std::cout << "Bird reset" << std::endl;
                }
            }

            //mouse pressed
            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left && !birdLaunched) {
                    isDragging = true;
                    dragStart = slingPosition;
                }
            }

            //mouse drag
            if (event.type == sf::Event::MouseMoved && isDragging) {
                float dx = event.mouseMove.x - slingPosition.x;
                float dy = event.mouseMove.y - slingPosition.y;

                float distance = std::sqrt(dx * dx + dy * dy);
                float maxDrag = 150.0f;

                if (dx > 0) dx = 0;
                if (dx < -maxPullLeft) dx = -maxPullLeft;
                if (dy > maxPullDown) dy = maxPullDown;
                if (dy < -maxPullDown) dy = -maxPullDown;

                dragEnd = sf::Vector2f(slingPosition.x + dx, slingPosition.y + dy);

                if (distance > maxDrag) {
                    float scale = maxDrag / distance;
                    dx *= scale;
                    dy *= scale;
                }

                dragEnd = sf::Vector2f(slingPosition.x + dx, slingPosition.y + dy);

                birds[currentBird]->getBody()->SetTransform(
                    b2Vec2(dragEnd.x / SCALE, dragEnd.y / SCALE), 0
                );
                birds[currentBird]->getBody()->SetLinearVelocity(b2Vec2(0, 0));
                birds[currentBird]->getBody()->SetAngularVelocity(0);
            }

            //mouse release
            if (event.type == sf::Event::MouseButtonReleased) {
                if (event.mouseButton.button == sf::Mouse::Left && isDragging) {
                    isDragging = false;

                    dragEnd = sf::Vector2f(event.mouseButton.x, event.mouseButton.y);

                    float dx = slingPosition.x - dragEnd.x;
                    if (dx < 0) dx = 0;
                    float dy = dragEnd.y - slingPosition.y;

                    float dragDistance = std::sqrt(dx * dx + dy * dy);
                    float maxDrag = 90.0f;

                    if (dragDistance > maxDrag) {
                        float scale = maxDrag / dragDistance;
                        dx *= scale;
                        dy *= scale;
                        dragDistance = maxDrag;
                    }

                    float pullX = slingPosition.x - dragEnd.x;
                    float pullY = dragEnd.y - slingPosition.y;
                    float pullDistance = std::sqrt(pullX * pullX + pullY * pullY);

                    if (pullDistance > 5.0f) {
                        int tension = static_cast<int>(pullDistance);
                        slingshot.pullBack(tension);

                        birds[currentBird]->launch(
                            b2Vec2(pullX / launchStrength, -pullY / launchStrength)
                        );

                        launchedBird = currentBird;
                        birdLaunched = true;
                        resetTimerStarted = true;
                        resetClock.restart();

                        std::cout << "Bird Type: " << slingshot.getBirdType() << std::endl;
                        std::cout << "Tension is: " << slingshot.getTension() << std::endl;

                        slingshot.release();
                    }
                }
            }

        }

        if (!csvProcessed) {
            int csvLines = csvFuture.get();
            std::cout << "CSV processing complete: " << csvLines << "lines" << std::endl;
            csvProcessed = true;
        }


        if (showStartScreen) {
            float progress;
            {
                std::lock_guard<std::mutex> lock(loadingMutex);
                progress = threadLoadingProgress;
            }

            if (progress >= 100.0f) {
                progress = 100.0f;

                if (!loadingThreadsJoined) {
                    spriteLoadingThread.join();
                    physicsLoadingThread.join();
                    loadingThreadsJoined = true;
                }


                showStartScreen = false;
            }

            loadingUI.update(progress);

            window.clear(sf::Color(135, 206, 235));
            window.draw(loadingBirdLeft);
            window.draw(loadingBirdRight);
            loadingUI.draw(window);
            window.display();

            continue;
        }

        // Update Physics
        world.Step(1.0f / 60.0f, 8, 3);


        for (b2Body* body : contactListener.bodiesToPush) {
            body->SetGravityScale(1.0f);
            body->SetFixedRotation(false);
            body->SetAwake(true);

            body->SetLinearVelocity(b2Vec2(5.0f, -2.0f));
        }

        contactListener.bodiesToPush.clear();

        for (b2Body* body : contactListener.destructionQueue) {
            body->SetEnabled(false);
        }

        contactListener.destructionQueue.clear();






        //All of the visuals needs to be synced with the physics.

        //Static objects usually don't move, but we set the position once.
        sf_groundVisual.setPosition(b2_groundBody->GetPosition().x * SCALE, b2_groundBody->GetPosition().y * SCALE);
        sf_wallVisual.setPosition(b2_wallBody->GetPosition().x * SCALE, b2_wallBody->GetPosition().y * SCALE);
        sf_plank1Visual.setPosition(b2_plank1Body->GetPosition().x * SCALE, b2_plank1Body->GetPosition().y * SCALE);
        sf_plank2Visual.setPosition(b2_plank2Body->GetPosition().x * SCALE, b2_plank2Body->GetPosition().y * SCALE);

        // Dynamic wall.
        sf_plankVisual.setPosition(b2_plankBody->GetPosition().x * SCALE, b2_plankBody->GetPosition().y * SCALE);
        sf_plankVisual.setRotation(b2_plankBody->GetAngle() * (180.0f / PI));

        //sync pig sprites to box2d
        pig1.update();
        pig2.update();
        pig3.update();
        pig4.update();
        pig5.update();
        pig6.update();

        if (resetTimerStarted && resetClock.getElapsedTime().asSeconds() >= 3.0f) {
            resetTimerStarted = false;
            birdLaunched = false;

            birds[currentBird]->getBody()->SetEnabled(false);

            currentBird = (currentBird + 1) % birds.size();

            birds[currentBird]->getBody()->SetEnabled(true);
            birds[currentBird]->getBody()->SetTransform(
                b2Vec2(slingPosition.x / SCALE, slingPosition.y / SCALE), 0
            );
            birds[currentBird]->getBody()->SetLinearVelocity(b2Vec2(0, 0));
            birds[currentBird]->getBody()->SetAngularVelocity(0);

            std::cout << "Next bird ready!" << std::endl;
        }


        //pig collision
        //auto checkPigHit = [&](Pig& pig, float f_pigRadius) {
            //if (pig.checkIfPopped()) return;
            //b2Vec2 ballPos = birds[launchedBird]->getBody()->GetPosition();
            //b2Vec2 pigPos = pig.getBody()->GetPosition();

            //float dx = ballPos.x - pigPos.x;
            //float dy = ballPos.y - pigPos.y;
            //float distance = std::sqrt(dx * dx + dy * dy);

            //if (distance < f_pigRadius + (15.0f / SCALE)) {
                //pig.getBody()->SetType(b2_dynamicBody);
                //pig.takeHit(birdDamage[launchedBird]);
                //std::cout << "Pig hit! Health: " << pig.getHealth() << std::endl;
            //}
            //};

            //if (birdLaunched) {
                //checkPigHit(pig1, 27.0f / SCALE);
                //checkPigHit(pig2, 27.0f / SCALE);
                //checkPigHit(pig3, 27.0f / SCALE);
                //checkPigHit(pig4, 20.0f / SCALE);
                //checkPigHit(pig5, 20.0f / SCALE);
                //checkPigHit(pig6, 15.0f / SCALE);
            //}


        if (!b_plank1Destroyed && birdLaunched) {
            b2Vec2 ballPos = birds[launchedBird]->getBody()->GetPosition();
            b2Vec2 plankPos = b2_plank1Body->GetPosition();
            float dx = ballPos.x - plankPos.x;
            float dy = ballPos.y - plankPos.y;
            if (std::sqrt(dx * dx + dy * dy) < 1.5f) {
                i_plank1Health -= birdDamage[launchedBird];
                if (i_plank1Health <= 0) {
                    b_plank1Destroyed = true;
                    //world.DestroyBody(b2_plank1Body);
                    std::cout << "Plank 1 Destroyed!" << std::endl;
                }
            }
        }

        if (!b_plank2Destroyed && birdLaunched) {
            b2Vec2 ballPos = birds[launchedBird]->getBody()->GetPosition();
            b2Vec2 plankPos = b2_plank2Body->GetPosition();
            float dx = ballPos.x - plankPos.x;
            float dy = ballPos.y - plankPos.y;
            if (std::sqrt(dx * dx + dy * dy) < 1.5f) {
                i_plank2Health -= birdDamage[launchedBird];
                if (i_plank2Health <= 0) {
                    b_plank2Destroyed = true;
                    //world.DestroyBody(b2_plank2Body);
                    std::cout << "PLank 2 destroyed" << std::endl;
                }
            }
        }


        //Render all of the content at each frame. Remember you need to clear the screen each iteration or artefacts remain.
        window.clear(sf::Color(135, 206, 235)); // Sky Blue

        window.draw(sf_groundVisual);
        window.draw(sf_wallVisual);
        window.draw(sf_plankVisual);
        window.draw(sf_plank1Visual);
        window.draw(sf_plank2Visual);

        if (birdLaunched) {
            birds[launchedBird]->update();
            birds[launchedBird]->draw(window);
        }
        else {
            birds[currentBird]->update();
            birds[currentBird]->draw(window);
        }

        //draw pigs to the window
        pig1.draw(window);
        pig2.draw(window);
        pig3.draw(window);
        pig4.draw(window);
        pig5.draw(window);
        pig6.draw(window);

        window.draw(titleText);

        window.display();
    }

    return 0;
}