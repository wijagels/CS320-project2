/* Copyright 2016 William Jagels */
#include <cstdint>

#include "Direct.hpp"

void Direct::run() {
  for (auto cmd : trace_) {
    check_cache(cmd);
  }
}

void Direct::check_cache(instr cmd) {
  uint32_t tag = cmd.second >> 5;
  auto &entry = cache_.at(tag % cache_capacity_);
  if (tag == entry) {
    hits_++;
  } else {
    misses_++;
    entry = tag;
  }
}
