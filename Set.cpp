/* Copyright 2016 William Jagels */

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
  auto& min = cache_.front();
  for (auto& e : cache_) {
    if (e.first < min.first) {
      min = e;
      if (min.first == 0) break;
    }
  }
  min.first = ++global_counter_;
  min.second = tag;
}
