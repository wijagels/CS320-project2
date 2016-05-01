/* Copyright 2016 William Jagels */

#pragma once

#include <vector>

#include "cache.hpp"

class Direct : public Cache {
 public:
  Direct(const std::vector<instr>& trace, uint32_t size)
      : Cache(trace),
        cache_size_(size),
        cache_capacity_(size / 32),
        cache_(cache_capacity_, -1) {}
  void run() override;

 protected:
  void check_cache(instr);
  uint32_t cache_size_;
  uint32_t cache_capacity_;
  std::vector<uint32_t> cache_;
};
