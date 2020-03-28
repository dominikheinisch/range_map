#include <gtest/gtest.h>

#include <limits>
#include <memory>

#include "range_map_testable.hpp"


const auto LOWEST = std::numeric_limits<int>::lowest();
const auto MAX = std::numeric_limits<int>::max();

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

TEST_F(RangeMapTest, testAddWrongRangeGreaterBegin) {
    sut->add(3, 2, 'c');
    checkResult(std::map<int, char>{{LOWEST, '*'}});
}

TEST_F(RangeMapTest, testAddWrongRangeBeginEqualEnd) {
    sut->add(3, 3, 'c');
    checkResult(std::map<int, char>{{LOWEST, '*'}});
}

TEST_F(RangeMapTest, testAddInnerWithMaxKeyEnd) {
    sut->add(10, MAX, 'b');
    auto expected = std::map<int, char> {{LOWEST, '*'}, {10, 'b'}, {MAX, '*'}};
    checkResult(expected);
}

TEST_F(RangeMapTest, testAddInnerSameToDefaultMap) {
    sut->add(-5, -3, '*');
    auto expected = std::map<int, char> {{LOWEST, '*'}};
    checkResult(expected);
}

TEST_F(RangeMapTest, testAddInnerNegative) {
    sut->add(-5, -2, 'a');
    auto expected = std::map<int, char> {{LOWEST, '*'}, {-5, 'a'}, {-2, '*'}};
    checkResult(expected);
}

TEST_F(RangeMapTest, testAddInnerPositive) {
    sut->add(2, 8, 'a');
    auto expected = std::map<int, char> {{LOWEST, '*'}, {2, 'a'}, {8, '*'}};
    checkResult(expected);
}

TEST_F(RangeMapTest, testAddOnBottomOfDefaultMap) {
    sut->add(LOWEST, LOWEST+1, 'w');
    auto expected = std::map<int, char> {{LOWEST, 'w'}, {LOWEST + 1, '*'}};
    checkResult(expected);
}