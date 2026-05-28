#include <gtest/gtest.h>
#include "Enemy.h"
#include <memory>

/// <summary>
///Taken from the GoogleTest primer. 
/// </summary>

// The fixture for testing class Foo.
class EnemyTest : public testing::Test {
public:
    std::unique_ptr<Enemy> enemy;
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

    }

    void TearDown() override {
        // Code here will be called immediately after each test (right
        // before the destructor).
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


//initial assertion test
TEST(Assertions, BasicNonFatalAssertions) {
    EXPECT_EQ(100, 100);
    EXPECT_GT(100, 50);
    EXPECT_LT(50, 100);
    SUCCEED() << "Basic non-fatal assertions passed";
}

//fatal assertion
TEST(Assertions, FatalAssertions) {
    int health = 100;
    ASSERT_EQ(health, 100);
    ASSERT_GT(health, 0);
}



int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}