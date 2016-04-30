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
    std::cout << d.hits_ << "," << d.hits_ + d.misses_ << std::endl;
  }
}
