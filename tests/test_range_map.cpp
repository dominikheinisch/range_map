#include <gtest/gtest.h>

#include <limits>
#include <memory>

#include "range_map_testable.hpp"


const auto LOWEST = std::numeric_limits<int>::lowest();
const auto MAX = std::numeric_limits<int>::max();
const auto DEFAULT_RAW_MAP = std::map<int, char> {{LOWEST, '*'}, {0, 'a'}, {10, 'b'}, {15, '*'}};

struct RangeMapTest : public ::testing::Test {
    void SetUp() {
        sut = std::make_unique<range_map_testable<int, char>>('*');
    }

    void checkResult(const std::map<int, char>& expected) {
        const auto& result = sut->getRawMap();
        EXPECT_TRUE(std::equal(expected.begin(), expected.end(), result.begin()));
        EXPECT_EQ(expected.size(), result.size());
    }

    void setDefaultRawMap() {
        sut->setRawMap(DEFAULT_RAW_MAP);
    }

    std::map<int, char> getExtendedDefaultMap(const std::initializer_list<std::pair<const int, char>>& toExtend) {
        auto result = DEFAULT_RAW_MAP;
        result.insert(toExtend);
        return result;
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

TEST_F(RangeMapTest, testAddToBeginOfDefaultMap) {
    sut->add(LOWEST, LOWEST+1, 'w');
    auto expected = std::map<int, char> {{LOWEST, 'w'}, {LOWEST + 1, '*'}};
    checkResult(expected);
}

TEST_F(RangeMapTest, testAddInnerDifferentElem) {
    setDefaultRawMap();
    sut->add(1, 5, 'c');
    checkResult(getExtendedDefaultMap({{1, 'c'}, {5, 'a'}}));
}

TEST_F(RangeMapTest, testAddInnerSameElem) {
    setDefaultRawMap();
    sut->add(2, 5, 'a');
    checkResult(DEFAULT_RAW_MAP);
}

TEST_F(RangeMapTest, testAddToBeginOfElemRange) {
    setDefaultRawMap();
    sut->add(0, 1, 'c');
    auto expected = std::map<int, char>{{LOWEST, '*'}, {0, 'c'}, {1, 'a'}, {10, 'b'}, {15, '*'}};
    checkResult(expected);
}

TEST_F(RangeMapTest, testAddToEndOfElemRange) {
    setDefaultRawMap();
    sut->add(2, 10, 'c');
    checkResult(getExtendedDefaultMap({{2, 'c'}}));
}

TEST_F(RangeMapTest, testAddExtendBeforeElemRnge) {
    setDefaultRawMap();
    sut->add(-2, 0, 'a');
    checkResult(std::map<int, char>{{LOWEST, '*'}, {-2, 'a'}, {10, 'b'}, {15, '*'}});
}


TEST_F(RangeMapTest, testAddExtendElemRangeBelow) {
    setDefaultRawMap();
    sut->add(-2, 2, 'a');
    auto expected = std::map<int, char> {{LOWEST, '*'}, {-2, 'a'}, {10, 'b'}, {15, '*'}};
    checkResult(expected);
}

TEST_F(RangeMapTest, testAddExtendAfterElemRange) {
    setDefaultRawMap();
    sut->add(10, 12, 'a');
    auto expected = std::map<int, char> {{LOWEST, '*'}, {0, 'a'}, {12, 'b'}, {15, '*'}};
    checkResult(expected);
}

TEST_F(RangeMapTest, testAddExtendElemRangeAbove) {
    setDefaultRawMap();
    sut->add(5, 12, 'a');
    auto expected = std::map<int, char> {{LOWEST, '*'}, {0, 'a'}, {12, 'b'}, {15, '*'}};
    checkResult(expected);
}

TEST_F(RangeMapTest, testAddDifferentBelowAndMoveElem) {
    setDefaultRawMap();
    sut->add(-2, 2, 'c');
    auto expected = std::map<int, char> {{LOWEST, '*'}, {-2, 'c'}, {2, 'a'}, {10, 'b'}, {15, '*'}};
    checkResult(expected);
}

TEST_F(RangeMapTest, testAddDifferentAboveAndMoveElem) {
    setDefaultRawMap();
    sut->add(9, 11, 'c');
    auto expected = std::map<int, char> {{LOWEST, '*'}, {0, 'a'}, {9, 'c'}, {11, 'b'}, {15, '*'}};
    checkResult(expected);
}

TEST_F(RangeMapTest, testAddOverFewElems) {
    sut->setRawMap({{LOWEST, '*'}, {0, 'a'}, {10, 'b'}, {11, 'd'}, {12, 'e'}, {15, 'a'}, {20, '*'}});
    sut->add(9, 14, 'c');
    auto expected = std::map<int, char> {{LOWEST, '*'}, {0, 'a'}, {9, 'c'}, {14, 'e'}, {15, 'a'}, {20, '*'}};
    checkResult(expected);
}

TEST_F(RangeMapTest, testAddToBeginOfLastElem) {
    sut->setRawMap({{LOWEST, '*'}, {0, 'a'}, {3, '*'}});
    sut->add(2, 3, 'b');
    auto expected = std::map<int, char> {{LOWEST, '*'}, {0, 'a'}, {2, 'b'}, {3, '*'}};
    checkResult(expected);
}

TEST_F(RangeMapTest, testAddAndLookUpFewElems) {
    sut->add(10, 14, 'b');
    sut->add(11, 12, 'd');
    sut->add(0, 20, 'a');
    sut->add(12, 15, 'e');
    sut->add(9, 14, 'c');
    EXPECT_EQ((*sut)[13], 'c');
    EXPECT_EQ((*sut)[-100], '*');
    EXPECT_EQ((*sut)[14], 'e');
    EXPECT_EQ((*sut)[1], 'a');
    EXPECT_EQ((*sut)[17], 'a');
    auto expected = std::map<int, char> {{LOWEST, '*'}, {0, 'a'}, {9, 'c'}, {14, 'e'}, {15, 'a'}, {20, '*'}};
    checkResult(expected);
}
