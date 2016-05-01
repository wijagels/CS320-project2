/* Copyright 2016 William Jagels */

#ifndef CACHE_HPP_
#define CACHE_HPP_

#include <cstdint>
#include <cmath>
#include <vector>
#include <utility>

#include "main.hpp"

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

class Set {
 public:
  explicit Set(int32_t size)
      : cache_capacity_(size),
        global_counter_(0),
        cache_(cache_capacity_, lruaddr(0, 0)) {}
  bool check_cache(uint32_t);
  void replace(uint32_t);

 protected:
  typedef std::pair<uint64_t, uint32_t> lruaddr;
  uint32_t cache_capacity_;
  uint64_t global_counter_;
  std::vector<lruaddr> cache_;
};

class SetAssociative : public Cache {
 public:
  SetAssociative(const std::vector<instr>& trace, uint32_t assoc)
      : Cache(trace),
        set_count_((cache_size_ / 32) / assoc),
        sets_(set_count_, Set(assoc)),
        lg_set_(log2(set_count_)) {}
  void run() override;

 protected:
  void check_cache(instr);
  inline uint32_t compute_tag(uint32_t);
  inline uint16_t compute_set(uint32_t);
  const uint32_t cache_size_ = 16 * 1024;
  uint16_t set_count_;
  std::vector<Set> sets_;
  uint16_t lg_set_;
};

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

#endif  // CACHE_HPP_
