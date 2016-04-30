/* Copyright 2016 William Jagels */

#ifndef CACHE_HPP_
#define CACHE_HPP_

#include <cstdint>
#include <vector>
#include <utility>

#include "main.hpp"

class Cache {
 public:
  explicit Cache(const std::vector<instr>& trace)
      : hits_(0), misses_(0), trace_(trace) {}
  virtual void run() {}
  int32_t hits_;
  int32_t misses_;

 protected:
  const std::vector<instr>& trace_;
};

class Direct : public Cache {
 public:
  Direct(const std::vector<instr>& trace, int32_t size)
      : Cache(trace),
        cache_size_(size),
        cache_capacity_(size / 32),
        cache_(cache_capacity_, instr(false, 0x0)) {
    std::cout << cache_capacity_ << " " << cache_size_ << std::endl;
  }
  void run() override;

 protected:
  void check_cache(instr);
  int32_t cache_size_;
  int32_t cache_capacity_;
  std::vector<instr> cache_;
};
#endif  // CACHE_HPP_
