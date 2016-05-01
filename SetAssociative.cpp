/* Copyright 2016 William Jagels */

#include "cache.hpp"

void SetAssociative::run() {
  for (auto cmd : trace_) {
    this->check_cache(cmd);
  }
  // for (auto set : sets_) std::cout << set.global_counter_ << std::endl;
}

void SetAssociative::check_cache(instr cmd) {
  uint32_t tag = compute_tag(cmd.second);
  uint16_t set = compute_set(cmd.second);
  bool result = sets_.at(set).check_cache(tag);
  if (result)
    hits_++;
  else
    misses_++;
}

unsigned createMask(unsigned a, unsigned b) {
  unsigned r = 0;
  for (unsigned i = a; i <= b; i++) r |= 1 << i;
  return r;
}

inline uint32_t SetAssociative::compute_tag(uint32_t addr) {
  return (addr >> 5) & createMask(lg_set_ + 1, 27);
}

inline uint16_t SetAssociative::compute_set(uint32_t addr) {
  return (addr >> (5 + lg_set_)) % set_count_;
}
