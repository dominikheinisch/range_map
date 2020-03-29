#pragma once

#include <iterator>
#include <limits>
#include <map>
#include <utility>


template<typename K, typename V>
class range_map {
    using const_iterator = typename std::map<K,V>::const_iterator;

protected:
    std::map<K, V> m_map;

public:
    range_map(const V& val) {
        m_map.emplace_hint(m_map.end(), std::numeric_limits<K>::lowest(), val);
    }

    std::pair<const_iterator, bool> add(const K& keyBegin, const K& keyEnd, const V& val) {
        if (!(keyBegin < keyEnd)) {
            return std::make_pair(m_map.end(), false);
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
        const auto resultIt = std::next(upperBound, -1);
        bool isStoredEqualNew = stored == val;
        if(!isStoredEqualNew) {
            m_map.emplace_hint(upperBound, keyEnd, stored);
        }
        return std::make_pair(resultIt, true);
    }

    const V& operator[](const K& key ) const {
        return (--m_map.upper_bound(key))->second;
    }
};
