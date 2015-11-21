// Main program for Viterbi decoding or convolutional encoding.
//
// Author: Min Xu <xukmin@gmail.com>
// Date: 01/30/2015

#include "viterbi.h"

#include <cassert>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

// Whether to reverse polynomials. This is to support a different notation of
// generator polynomials, i.e. the one used in MATLAB. For example, polynomial
// 6 (0b110) becomes 3 (0b011) in MATLAB.
// See comments in viterbi.h for details.
static bool FLAGS_reverse_polynomials = false;

// Whether to perform encoding instead of decoding.
static bool FLAGS_encode = false;

void Usage(const std::string& exec) {
  std::cout
      << "Usage:\n"
      << "Show help message:\n"
      << "    " << exec << " --help\n\n"
      << "Read input from stdin:\n"
      << "    " << exec << " [--reverse_polynomials] [--encode]\n\n"
      << "    Input format is:\n"
      << "        <constraint> <polynomial>... <bits>\n\n"
      << "    Example input:\n"
      << "        3 7 5 0011100001100111111000101100111011\n\n"
      << "Read input from commandline arguments:\n"
      << "    " << exec << " [--reverse_polynomials] [--encode]"
      << " <constraint> <polynomial>... <bits>\n\n"
      << "Flags:\n"
      << "    --reverse_polynomials\n"
      << "        Reverse polynomials. E.g. 6 (=0b110) becomes 3 (=0b011).\n\n"
      << "    --encode\n"
      << "        Do encoding instead of decoding.\n\n"
      << "Examples:\n"
      << exec << " 3 7 5 0011100001100111111000101100111011\n"
      << exec << " 3 6 5 111011011100101011\n"
      << exec << " 7 91 121 1010110100001101001000011101\n"
      << exec << " 7 91 117 121 111100101110001011110101111111001011100111\n"
      << exec << " --reverse_polynomials 3 3 5 111011011100101011\n"
      << exec << " --encode 3 7 5 010111001010001\n"
      << exec << " --encode 3 6 5 1001101\n"
      << exec << " --encode --reverse_polynomials 3 3 5 1001101\n";
}

// Parses and sets command line flags (FLAGS_*).
// Returns all non-flag arguments, also excluding argv[0].
std::vector<std::string> ParseFlags(int argc, char** argv) {
  std::vector<std::string> args;
  for (int i = 1; i < argc; i++) {
    if (strcmp(argv[i], "--help") == 0) {
      Usage(argv[0]);
      exit(1);
    } else if (std::strcmp(argv[i], "--reverse_polynomials") == 0) {
      FLAGS_reverse_polynomials = true;
    } else if (std::strcmp(argv[i], "--encode") == 0) {
      FLAGS_encode = true;
    } else {
      args.push_back(argv[i]);
    }
  }
  return args;
}

int ParseInt(const std::string& s) {
  char* end;
  const int i = (int) std::strtol(s.c_str(), &end, 10);
  if (end - s.c_str() != s.size()) {
    std::cout << "Expected a number, found " << s << std::endl;
    exit(1);
  }
  return i;
}

void ViterbiMain(const std::vector<std::string>& args) {
  if (args.size() < 4) {
    std::cout << "Insufficient number of arguments." << std::endl;
    exit(1);
  }

  // Parse and validate constraint.
  const int constraint = ParseInt(args[0]);
  if (constraint <= 0) {
    std::cout << "Constraint should be greater than 0, found " << constraint
              << std::endl;
    exit(1);
  }

  // Parse and validate generator polynomials.
  std::vector<int> polynomials;
  for (int i = 1; i < args.size() - 1; i++) {
    const int polynomial = ParseInt(args[i]);
    if (polynomial <= 0) {
      std::cout << "Polynomial should be greater than 0, found " << polynomial
                << std::endl;
      exit(1);
    }
    if (polynomial >= (1 << constraint)) {
      std::cout << "Polynomial should be less than " << (1 << constraint)
                << ", found " << polynomial << std::endl;
      exit(1);
    }
    polynomials.push_back(polynomial);
  }
  if (FLAGS_reverse_polynomials) {
    for (int i = 0; i < polynomials.size(); i++) {
      polynomials[i] = ReverseBits(constraint, polynomials[i]);
    }
  }

  // Parse and validate bit sequence.
  const std::string& bits = args.back();
  for (int i = 0; i < bits.size(); i++) {
    if (bits[i] != '0' && bits[i] != '1') {
      std::cout << "Expected a binary sequence, found " << bits << std::endl;
      exit(1);
    }
  }

  ViterbiCodec codec(constraint, polynomials);

  if (FLAGS_encode) {
    std::cout << codec.Encode(bits) << std::endl;
  } else {
    std::cout << codec.Decode(bits) << std::endl;
  }
}

int main(int argc, char** argv) {
  std::vector<std::string> args = ParseFlags(argc, argv);

  if (args.empty()) {
    // No non-flag arguments are provided in commandline, Read input from stdin.
    std::string line;
    while (std::getline(std::cin, line)) {
      // Skip blank line and comment line (start with '#')
      if (line.empty() || line[0] == '#') {
        continue;
      }
      std::istringstream is(line);
      std::string arg;
      while (is >> arg) {
        args.push_back(arg);
      }
    }
  }

  ViterbiMain(args);
}
