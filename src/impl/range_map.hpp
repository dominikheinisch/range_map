#pragma once

#include <iterator>
#include <limits>
#include <map>
#include <utility>


template<typename K, typename V>
class range_map {

protected:
    std::map<K, V> m_map;

public:
    range_map(const V& val) {
        m_map.emplace_hint(m_map.end(), std::numeric_limits<K>::lowest(), val);
    }

    void add(const K& keyBegin, const K& keyEnd, const V& val) {
        if (!(keyBegin < keyEnd)) {
            return;
        }
        auto lowerBound = m_map.lower_bound(keyBegin);
        auto upperBound = m_map.upper_bound(keyEnd);
        auto stored = std::next(upperBound, -1)->second;
        bool isAdditionToBegin = m_map.begin() == lowerBound;
        bool isPrevEqualNew = std::next(lowerBound, -1)->second == val;
        m_map.erase(lowerBound, upperBound);
        if (isAdditionToBegin || !isPrevEqualNew) {
            m_map.emplace_hint(upperBound, keyBegin, val);
        }
        bool isStoredEqualNew = stored == val;
        if(!isStoredEqualNew) {
            m_map.emplace_hint(upperBound, keyEnd, stored);
        }
    }

    const V& operator[](const K& key ) const {
        return (--m_map.upper_bound(key))->second;
    }
};
