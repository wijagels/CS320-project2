/* Copyright 2016 William Jagels */

#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>
#include <string>

#include "main.hpp"
#include "cache.hpp"

using std::string;

int main(int argc, char *argv[]) {
  if (argc != 3) {
    std::cerr << "Invalid argument size" << std::endl;
    return 1;
  }
  uint64_t addr;
  string behavior, line;
  std::ifstream infile(argv[1]);
  std::vector<instr> v;
  v.reserve(MAX_TRACE_LENGTH);  // Eliminate vector resizing
  while (std::getline(infile, line)) {
    std::stringstream s(line);
    s >> std::hex >> behavior >> addr;
    v.push_back(instr(behavior == "S", addr));
  }
  std::cout << v.size() << std::endl;
  for (auto i : {1, 4, 16, 32}) {
    Direct d = Direct(v, 1024 * i);
    d.run();
    if (i != 1) std::cout << " ";
    std::cout << d.hits_ << "," << d.hits_ + d.misses_ << ";";
  }
  std::cout << std::endl;
  for (auto i : {2, 4, 8, 16}) {
    SetAssociative sa = SetAssociative(v, i);
    sa.run();
    if (i != 2) std::cout << " ";
    std::cout << sa.hits_ << "," << sa.hits_ + sa.misses_ << ";";
  }
  std::cout << std::endl;
  SetAssociative sa(v, 1);
  sa.run();
  std::cout << sa.hits_ << "," << sa.hits_ + sa.misses_ << ";" << std::endl;
  FullyAssociative fa(v);
  fa.run();
  std::cout << fa.hits_ << "," << fa.hits_ + fa.misses_ << ";" << std::endl;
}
