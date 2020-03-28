#pragma once

#include <map>
#include <limits>
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
        // TODO
    }

    const V& operator[](const K& key ) const {
        return (--m_map.upper_bound(key))->second;
    }
};
