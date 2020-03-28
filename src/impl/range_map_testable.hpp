#pragma once

#include "range_map.hpp"


template<typename K, typename V>
class range_map_testable : public range_map<K,V> {

public:
    range_map_testable( V const& val) : range_map<K,V> (val) {}

    const std::map<K,V>& getRawMap() {
        return range_map<K,V>::m_map;
    }
};
