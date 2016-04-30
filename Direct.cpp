/* Copyright 2016 William Jagels */
#include <cstdint>

#include "cache.hpp"

void Direct::run() {
  for (auto cmd : trace_) {
    check_cache(cmd);
    // std::cout << cmd.second << std::endl;
  }
}

void Direct::check_cache(instr cmd) {
  uint64_t tag = cmd.second >> 5;
  auto &entry = cache_.at(tag % cache_capacity_);
  uint64_t cur = entry.second >> 5;
  if (cmd.first) {  // Is store
    if (tag == cur) {
      hits_++;
      entry.first = true;
    } else {
      misses_++;
      entry.second = cmd.second;
      entry.first = true;
    }
  } else {
    if (tag == cur) {
      if (entry.first) {  // Cache hit
        hits_++;
      } else {
        misses_++;
        entry.first = true;
      }
    } else {  // Cache miss
      misses_++;
      entry.second = cmd.second;
      entry.first = true;
    }
  }
}
