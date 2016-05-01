/* Copyright 2016 William Jagels */

#include "SetAssociative.hpp"

void SetAssociative::run() {
  for (auto cmd : trace_) {
    this->check_cache(cmd);
  }
}

void SetAssociative::check_cache(instr cmd) {
  uint32_t tag = compute_tag(cmd.second);
  bool result = check_tag(tag, cmd.first, true);
  if (result) {
    hits_++;
  } else {
    misses_++;
  }
}

bool SetAssociative::check_tag(uint32_t tag, bool type, bool first) {
  uint16_t set = compute_set(tag);
  bool result;
  if (type && no_write_)
    result = sets_.at(set).check_cache(tag, false);
  else
    result = sets_.at(set).check_cache(tag, true);
  if (first) {
    if (result) {
      if (prefetch_ && !on_miss_) check_tag(tag + 1, false, false);
    } else {
      if (prefetch_) check_tag(tag + 1, false, false);
    }
  }
  return result;
}

/*
 * Adapted from
 * http://stackoverflow.com/a/8012232/1666415
 */
unsigned createMask(uint32_t a, uint32_t b) {
  uint32_t r = 0;
  for (uint32_t i = a; i <= b; i++) r |= 1 << i;
  return r;
}

inline uint32_t SetAssociative::compute_tag(uint32_t addr) {
  return ((addr >> 5) & createMask(lg_set_, 27));
}

inline uint16_t SetAssociative::compute_set(uint32_t addr) {
  return (addr >> 5) & createMask(0, lg_set_ - 1);
}
