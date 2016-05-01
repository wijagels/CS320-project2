/* Copyright 2016 William Jagels */

#pragma once

#include <cstdint>
#include <vector>
#include <utility>

#include "main.hpp"

/*
 * Generic superclass for a caching algorithm
 */
class Cache {
 public:
  explicit Cache(const std::vector<instr>& trace)
      : hits_(0), misses_(0), trace_(trace) {}
  virtual void run() {}
  uint64_t hits_;
  uint64_t misses_;

 protected:
  const std::vector<instr>& trace_;
};

/*
 * A fixed-capacity cache with O(n) lookup
 * Replacement algorithm is LRU
 */
class Set {
 public:
  explicit Set(int32_t size)
      : cache_capacity_(size),
        global_counter_(0),
        cache_(cache_capacity_, lruaddr(0, 0)) {}
  bool check_cache(uint32_t tag) { return check_cache(tag, true); }
  bool check_cache(uint32_t, bool);
  void replace(uint32_t);

 protected:
  typedef std::pair<uint64_t, uint32_t> lruaddr;
  uint32_t cache_capacity_;
  uint64_t global_counter_;
  std::vector<lruaddr> cache_;
};
