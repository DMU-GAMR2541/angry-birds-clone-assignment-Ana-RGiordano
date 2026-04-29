#include <gtest/gtest.h>
#include "Enemy.h"

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
        enemy = std::make_unique<Enemy>(50); // All enemnies in this test suite start with 50 HP.
                    
    }

    void TearDown() override {
        // Code here will be called immediately after each test (right
        // before the destructor).
    }


};

class ParamTest : public::testing::TestWithParam<int> {
protected:
    ParamTest() = default;
    ~ParamTest() = default;

    void SetUp()override {
    }
};

TEST_P(ParamTest, SimpleTest) {
    int i_test = GetParam();
    std::cout << "Param value::" << i_test << std::endl;
    EXPECT_GT(i_test, 1);
}

INSTANTIATE_TEST_SUITE_P(
    Simple,
    ParamTest, ::testing::Values(1, 2, 3, 4, 5)
);


//A single test, not a fixture. No setup is called.
TEST(Enemy, First_test) {
    Enemy e(100);
    std::cout << e.getHealth() << " " << 100 << std::endl;
    //EXPECT_GT(e.getHealth(), 100);
}

TEST_F(EnemyTest, LethalDamagePopsPig) {
    enemy->takeDamage(60);
    EXPECT_TRUE(enemy->checkIfPopped());
}

TEST_F(EnemyTest, NegativeDamageDoesNothing) {
    int initialHealth = enemy->getHealth();
    enemy->takeDamage(-10);
    EXPECT_EQ(enemy->getHealth(), initialHealth);
    EXPECT_FALSE(enemy->checkIfPopped());
} // checks if negative dmaage does nothing

TEST(Assertions, NonFatalEquality) {
    EXPECT_EQ(5, 5);
    EXPECT_FLOAT_EQ(1.5f, 1.5f);
    EXPECT_STREQ("hello", "hello");
} // tests that values are equal

TEST(Assertions, NonFatalEquality) {
    EXPECT_NE(5, 10);
    EXPECT_STREQ("hello", "world");
} // check values are not equal

TEST(Assertions, NonFatalComparisons) {
    EXPECT_GT(10, 5);
    EXPECT_LT(5, 10);
    EXPECT_GE(10, 10);
    EXPECT_LE(5, 5);
}// testing values that are <,<,>=,<=

TEST(NewTest, test) {
    std::string str = "Check";
    const char* c = str.c_str();
    EXPECT_STREQ(c, "Check");
}

int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
