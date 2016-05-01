/* Copyright 2016 William Jagels */

#pragma once

#include <vector>
#include <cmath>

#include "main.hpp"
#include "cache.hpp"

class SetAssociative : public Cache {
 public:
  SetAssociative(const std::vector<instr>& trace, uint32_t assoc)
      : Cache(trace),
        set_count_((cache_size_ / 32) / assoc),
        sets_(set_count_, Set(assoc)),
        lg_set_(log2(set_count_)),
        prefetch_(false),
        no_write_(false),
        on_miss_(false) {}
  SetAssociative(const std::vector<instr>& trace, uint32_t assoc, bool prefetch,
                 bool no_write, bool on_miss)
      : Cache(trace),
        set_count_((cache_size_ / 32) / assoc),
        sets_(set_count_, Set(assoc)),
        lg_set_(log2(set_count_)),
        prefetch_(prefetch),
        no_write_(no_write),
        on_miss_(on_miss) {}
  void run() override;

 protected:
  void check_cache(instr);
  bool check_addr(uint32_t, bool, bool);
  inline uint32_t compute_tag(uint32_t);
  inline uint16_t compute_set(uint32_t);
  const uint32_t cache_size_ = 16 * 1024;
  uint16_t set_count_;
  std::vector<Set> sets_;
  uint16_t lg_set_;
  bool prefetch_;
  bool no_write_;
  bool on_miss_;
};
