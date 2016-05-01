/* Copyright 2016 William Jagels */

#pragma once

#include <vector>
#include <utility>

#include "cache.hpp"

class FullyAssociative : public Cache {
 public:
  explicit FullyAssociative(const std::vector<instr>& trace)
      : Cache(trace), cache_((16 * 1024) / 32, hot_addr(false, 0)) {
    initialize();
  }
  void run() override;
  static inline uint32_t compute_tag(uint32_t);

 protected:
  typedef std::pair<bool, uint32_t> hot_addr;
  void check_cache(uint32_t);
  void access(uint32_t);
  void initialize();
  void heat_up(uint32_t);
  uint32_t find_victim();
  void replace(uint32_t);
  std::vector<hot_addr> cache_;
};

class FullyAssociativeTrue : public Cache {
 public:
  explicit FullyAssociativeTrue(const std::vector<instr>& trace)
      : Cache(trace), cache_((16 * 1024) / 32) {}
  void run() override;

 protected:
  Set cache_;
};
