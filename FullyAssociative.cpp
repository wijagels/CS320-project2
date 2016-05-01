/* Copyright 2016 William Jagels */

#include "cache.hpp"

void FullyAssociative::run() {
  for (auto cmd : trace_) {
    this->check_cache(compute_tag(cmd.second));
  }
}

void FullyAssociative::check_cache(uint32_t tag) {
  for (size_t i = 0; i < cache_.size(); i++) {
    if (cache_.at(i).second == tag) {
      access(i);
      hits_++;
      return;
    }
  }
  replace(tag);
  misses_++;
}

void FullyAssociative::access(uint32_t loc) { heat_up(loc); }

void FullyAssociative::initialize() {
  std::cout << cache_.size() << std::endl;
  for (size_t i = 0; i < cache_.size(); i++) {
    replace(0);
  }
}

void FullyAssociative::heat_up(uint32_t loc) {
  uint32_t begin = 0;
  uint32_t end = cache_.size() - 1;
  uint32_t cur = (begin + end) / 2;
  for (; begin + 1 != end; cur = (begin + end) / 2) {
    if (cur < loc) {  // Go right
      begin = cur;
      cache_.at(cur).first = true;
    } else {  // Go left
      end = cur;
      cache_.at(cur).first = false;
    }
  }
  // At destination
  if (cur < loc) {
    cache_.at(cur).first = true;
  } else {
    cache_.at(cur).first = false;
  }
}

uint32_t FullyAssociative::find_victim() {
  uint32_t begin = 0;
  uint32_t end = cache_.size() - 1;
  uint32_t cur = (begin + end) / 2;
  for (; cur != begin && cur != end; cur = (begin + end) / 2) {
    if (cache_.at(cur).first) {  // Tighten back
      end = cur;
    } else {  // Tighten front
      begin = cur;
    }
  }
  // Now at a leaf node
  if (cache_.at(cur).first) {
    return cur;
  } else {
    return cur + 1;
  }
}

void FullyAssociative::replace(uint32_t tag) {
  uint32_t victim = find_victim();
  // std::cout << "Victim way #" << victim << std::endl;
  cache_.at(victim).second = tag;
  access(victim);
}

inline uint32_t FullyAssociative::compute_tag(uint32_t addr) {
  return addr >> 5;
}

void FullyAssociativeTrue::run() {
  for (auto cmd : trace_) {
    bool res = cache_.check_cache(FullyAssociative::compute_tag(cmd.second));
    if (res) {
      hits_++;
    } else {
      misses_++;
    }
  }
}
