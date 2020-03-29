#include <gtest/gtest.h>

#include <limits>
#include <memory>

#include "range_map_testable.hpp"


const auto DOUBLE_MIN = std::numeric_limits<double>::lowest();

struct Key{
    Key(double d) : d(d) {}
    friend bool operator<(const Key& l, const Key& r)
    {
        return l.d < r.d;
    }

    double d;
};

namespace std {
    template<> class numeric_limits<Key> {
    public:
       static Key lowest() {return Key(DOUBLE_MIN);};
    };
}

struct Value{
    Value(std::string str) : str(str) {}
    bool operator== (const Value& other) {
        return (str.compare(other.str) == 0);
    }

    std::string str;
};

struct TestRangeMapCustomStructure : public ::testing::Test {
    void SetUp() {
        sut = std::make_unique<range_map_testable<Key, Value>>(Value("---"));
    }

    std::unique_ptr<range_map_testable<Key, Value>> sut;
};

TEST_F(TestRangeMapCustomStructure, testAddFewElems)
{
    sut->add(0.0, 3.0, Value("aa"));
    sut->add(0.1, 3.14, Value("aa"));
    sut->add(0.1, 3.14, Value("bb"));
    sut->add(0.05, 2.14, Value("aa"));
    auto result = sut->getRawMap();
    std::map<Key, Value> expected = {
        {Key(DOUBLE_MIN), Value("---")}, {Key(0.0), Value("aa")}, {Key(2.14), Value("bb")}, {Key(3.14), Value("---")}
    };

    auto expectedIt = expected.begin();
    for (auto elem: result)
    {
        EXPECT_EQ(elem.first.d, expectedIt->first.d);
        EXPECT_EQ(elem.second.str, expectedIt->second.str);
        ++expectedIt;
    }
    EXPECT_EQ(expected.size(), result.size());
}

