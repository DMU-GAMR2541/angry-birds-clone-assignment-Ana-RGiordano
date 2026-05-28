#include <gtest/gtest.h>
#include "Enemy.h"
#include <memory>
#include <vector>
#include "Pig.h"
#include <box2d/box2d.h>
#include <SFML/Graphics.hpp>

/// <summary>
///Taken from the GoogleTest primer. 
/// </summary>

// The fixture for testing class Foo.

//enemy is now an abstract class so i adapted the fixture to use the pig class instead
class EnemyTest : public testing::Test {
public:
    std::unique_ptr<Pig> pig;
    std::unique_ptr<b2World> world;
    std::unique_ptr<sf::RenderWindow> window;
protected:
    // You can remove any or all of the following functions if their bodies would
    // be empty.

    EnemyTest() {
        // You can do set-up work for each test here.

    }

    ~EnemyTest() override {
        // You can do clean-up work that doesn't throw exceptions here.
    }

    // If the constructor and destructor are not enough for setting up
    // and cleaning up each test, you can define the following methods:

    void SetUp() override {
        // Code here will be called immediately after the constructor (right
        // before each test).
        //enemy = std::make_unique<Enemy>(50); // All enemnies in this test suite start with 50 HP.
        world = std::make_unique<b2World>(b2Vec2(0.0f, 9.8f));
        window = std::make_unique<sf::RenderWindow>();
        pig = std::make_unique<Pig>(b2Vec2(100.0f / 30.0f, 500.0f / 30.0f), *world, "C:/Users/abeat/source/repos/angry-birds-clone-assignment-Ana-RGiordano/assets/Ang_Birds/sprite_1.png", *window,
            50, 15.0f, 30.0f, 57, 57);


    }

    void TearDown() override {
        // Code here will be called immediately after each test (right
        // before the destructor).
        pig.reset();
        window.reset();
        world.reset();
    }


};

//A single test, not a fixture. No setup is called.
//TEST(Enemy, First_test) {
    //Enemy e(100);
    //EXPECT_GT(e.getHealth(), 100);
    //SUCCEED() << "Test test passed";
//}



//TEST_F(EnemyTest, LethalDamagePopsPig) {
   // enemy->takeDamage(60);
    //EXPECT_TRUE(enemy->checkIfPopped());
//}


//binary comparison initial assertion test 
TEST(Assertions, BasicNonFatalAssertions) {
    EXPECT_EQ(100, 100);
    EXPECT_GT(100, 50);
    EXPECT_LT(50, 100);
    SUCCEED() << "Basic non-fatal assertions passed";
}

//fatal assertions
TEST(Assertions, FatalAssertions) {
    int health = 100;
    ASSERT_EQ(health, 100);
    ASSERT_GT(health, 0);
}

//string, int and float comparisons
TEST(Assertions, TypeComparisons) {
    int health = 100;
    float scale = 30.0f;
    std::string birdType = "Red";

    EXPECT_EQ(health, 100);
    EXPECT_FLOAT_EQ(scale, 30.0f);
    EXPECT_EQ(birdType, "Red");
    EXPECT_NE(health, 50);
    EXPECT_NE(birdType, "Fast");
}
//fxiture tests

//tests starting health
TEST_F(EnemyTest, PigHealthStartsAtExpectedValue) {
    EXPECT_EQ(pig->getHealth(), 50);
}

// tests health reduction
TEST_F(EnemyTest, PigHealthCanBeReduced) {
    pig->takeHit(20);
    EXPECT_EQ(pig->getHealth(), 30);
}
// tests if the pig is popped after lethal damage
TEST_F(EnemyTest, LethalDamagePopsPig) {
    pig->takeHit(60);
    EXPECT_TRUE(pig->checkIfPopped());
}

//tests correctness of movement of a dynamic object across a spread of values
TEST(Physics, DynamicObjectMovesAcrossValues) {
    std::vector<float> impulseValues = { 2.0f, 5.0f, 10.0f };
    
    for (float impulse : impulseValues) {
        b2World world(b2Vec2(0.0f, 9.8f));
        
        b2BodyDef bodyDef;
        bodyDef.type = b2_dynamicBody;
        bodyDef.position.Set(1.0f, 1.0f);
        b2Body* body = world.CreateBody(&bodyDef);

        b2CircleShape circle;
        circle.m_radius = 0.5f;
        
        b2FixtureDef fixture;
        fixture.shape = &circle;
        fixture.density = 1.0f;
        body->CreateFixture(&fixture);

        float startX = body->GetPosition().x;
        body->ApplyLinearImpulseToCenter(b2Vec2(impulse, 0.0f), true);
        world.Step(1.0f / 60.0f, 8, 3);
        float endX = body->GetPosition().x;

        EXPECT_GT(endX, startX);

    }
}

int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}