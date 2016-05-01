/* Copyright 2016 William Jagels */

#include <algorithm>

#include "cache.hpp"

bool Set::check_cache(uint32_t tag) {
  for (auto& e : cache_) {
    if (e.second == tag) {
      e.first = ++global_counter_;
      return true;
    }
  }
  this->replace(tag);
  return false;
}

void Set::replace(uint32_t tag) {
  auto min = std::min_element(
      cache_.begin(), cache_.end(),
      [](lruaddr a, lruaddr b) -> bool { return a.first < b.first; });
  // C++11 4 lyfe
  min->first = ++global_counter_;
  min->second = tag;
}
