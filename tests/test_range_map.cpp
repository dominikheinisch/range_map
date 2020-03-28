#include <gtest/gtest.h>

#include <limits>
#include <memory>

#include "range_map_testable.hpp"


const auto LOWEST = std::numeric_limits<int>::lowest();

struct RangeMapTest : public ::testing::Test {
    void SetUp() {
        sut = std::make_unique<range_map_testable<int, char>>('*');
    }

    void checkResult(const std::map<int, char>& expected) {
        const auto& result = sut->getRawMap();
        EXPECT_TRUE(std::equal(expected.begin(), expected.end(), result.begin()));
        EXPECT_EQ(expected.size(), result.size());
    }

    std::unique_ptr<range_map_testable<int, char>> sut;
};

TEST_F(RangeMapTest, testCtor) {
    checkResult(std::map<int, char>{{LOWEST, '*'}});
}
