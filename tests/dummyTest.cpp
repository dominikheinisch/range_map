#include <gtest/gtest.h>

struct DummyTest : public ::testing::Test {
    void SetUp() {}
    void TearDown() {}
};

TEST_F(DummyTest, assertTrue)
{
    ASSERT_TRUE(true);
}

