/* Copyright 2016 William Jagels */

#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>
#include <string>

#include "main.hpp"
#include "cache.hpp"
#include "Direct.hpp"
#include "SetAssociative.hpp"
#include "FullyAssociative.hpp"

using std::string;

int main(int argc, char *argv[]) {
  if (argc != 3) {
    std::cerr << "Invalid argument size" << std::endl;
    return 1;
  }
  std::ofstream os;
  os.open(argv[2]);
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
  for (auto i : {1, 4, 16, 32}) {
    Direct d = Direct(v, 1024 * i);
    d.run();
    if (i != 1) os << " ";
    os << d.hits_ << "," << d.hits_ + d.misses_ << ";";
  }
  os << std::endl;
  for (auto i : {2, 4, 8, 16}) {
    SetAssociative sa = SetAssociative(v, i);
    sa.run();
    if (i != 2) os << " ";
    os << sa.hits_ << "," << sa.hits_ + sa.misses_ << ";";
  }
  os << std::endl;
  FullyAssociativeTrue fat(v);
  fat.run();
  os << fat.hits_ << "," << fat.hits_ + fat.misses_ << ";" << std::endl;
  FullyAssociative fa(v);
  fa.run();
  os << fa.hits_ << "," << fa.hits_ + fa.misses_ << ";" << std::endl;
  for (auto i : {2, 4, 8, 16}) {
    SetAssociative sa = SetAssociative(v, i, false, true, false);
    sa.run();
    if (i != 2) os << " ";
    os << sa.hits_ << "," << sa.hits_ + sa.misses_ << ";";
  }
  os << std::endl;
  for (auto i : {2, 4, 8, 16}) {
    SetAssociative sa = SetAssociative(v, i, true, false, false);
    sa.run();
    if (i != 2) os << " ";
    os << sa.hits_ << "," << sa.hits_ + sa.misses_ << ";";
  }
  os << std::endl;
  for (auto i : {2, 4, 8, 16}) {
    SetAssociative sa = SetAssociative(v, i, true, false, true);
    sa.run();
    if (i != 2) os << " ";
    os << sa.hits_ << "," << sa.hits_ + sa.misses_ << ";";
  }
  os << std::endl;
}
