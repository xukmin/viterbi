// Unit test for ViterbiCodec.
//
// Author: Min Xu <xukmin@gmail.com>
// Date: 01/30/2015

#include "viterbi.h"

#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <string>
#include <vector>

void TestViterbiDecoding(const ViterbiCodec& codec,
                         const std::string& received,
                         const std::string& message) {
  const std::string decoded = codec.Decode(received);

  std::cout << std::string(60, '=') << std::endl
            << codec << std::endl
            << "received = " << received << std::endl
            << "decoded  = " << decoded << std::endl;

  assert(decoded == message);

  std::cout << std::endl;
}

void TestViterbiDecodingSamples() {
  {
    std::vector<int> polynomials;
    polynomials.push_back(7);
    polynomials.push_back(5);

    ViterbiCodec codec(3, polynomials);

    TestViterbiDecoding(codec,
                        "0011100001100111111000101100111011",
                        "010111001010001");

    // Inject 1 error bit.
    TestViterbiDecoding(codec,
                        "0011100001100111110000101100111011",
                        "010111001010001");
  }

  {
    std::vector<int> polynomials;
    polynomials.push_back(7);
    polynomials.push_back(6);

    ViterbiCodec codec(3, polynomials);

    TestViterbiDecoding(codec, "1011010100110000", "101100");
  }

  {
    std::vector<int> polynomials;
    polynomials.push_back(6);
    polynomials.push_back(5);

    ViterbiCodec codec(3, polynomials);

    TestViterbiDecoding(codec, "011011011101101011", "1001101");

    // Inject 2 error bits.
    TestViterbiDecoding(codec, "111011011100101011", "1001101");
  }

  {
    std::vector<int> polynomials;
    polynomials.push_back(91);
    polynomials.push_back(117);
    polynomials.push_back(121);

    ViterbiCodec codec(7, polynomials);

    TestViterbiDecoding(codec,
                        "111100101110001011110101111111001011100111",
                        "10110111");

    // Inject 4 error bits.
    TestViterbiDecoding(codec,
                        "100100101110001011110101110111001011100110",
                        "10110111");
  }
}

// Test the given ViterbiCodec by randomly generating 10 input sequences of
// length 8, 16, 32 respectively, encode and decode them, then test if the
// decoded string is the same as the original input.
void TestViterbiCodecAutomatic(const ViterbiCodec& codec) {
  std::cout << std::string(60, '=') << std::endl
            << codec << std::endl << std::endl;
  for (int num_bits = 8; num_bits <= 32; num_bits <<= 1) {
    for (int i = 0; i < 10; i++) {
      std::string message;
      for (int j = 0; j < num_bits; j++) {
        message += (std::rand() & 1) + '0';
      }

      std::string encoded = codec.Encode(message);
      std::string decoded = codec.Decode(encoded);
      std::cout << "encoded = " << encoded << std::endl
                << "decoded = " << decoded << std::endl << std::endl;
      assert(decoded == message);
    }
  }
}

int main(int argc, char** argv) {
  TestViterbiDecodingSamples();

  std::srand(std::time(NULL));

  {
    std::vector<int> polynomials;
    polynomials.push_back(7);
    polynomials.push_back(5);

    ViterbiCodec codec(3, polynomials);

    TestViterbiCodecAutomatic(codec);
  }

  {
    std::vector<int> polynomials;
    polynomials.push_back(6);
    polynomials.push_back(5);

    ViterbiCodec codec(3, polynomials);

    TestViterbiCodecAutomatic(codec);
  }

  {
    // Voyager
    std::vector<int> polynomials;
    polynomials.push_back(109);
    polynomials.push_back(79);

    ViterbiCodec codec(7, polynomials);

    TestViterbiCodecAutomatic(codec);
  }

  {
    // LTE
    std::vector<int> polynomials;
    polynomials.push_back(91);
    polynomials.push_back(117);
    polynomials.push_back(121);

    ViterbiCodec codec(7, polynomials);

    TestViterbiCodecAutomatic(codec);
  }

  {
    // CDMA 2000
    std::vector<int> polynomials;
    polynomials.push_back(501);
    polynomials.push_back(441);
    polynomials.push_back(331);
    polynomials.push_back(315);

    ViterbiCodec codec(9, polynomials);

    TestViterbiCodecAutomatic(codec);
  }

  {
    // Cassini / Mars Pathfinder
    std::vector<int> polynomials;
    polynomials.push_back(15);
    polynomials.push_back(17817);
    polynomials.push_back(20133);
    polynomials.push_back(23879);
    polynomials.push_back(30451);
    polynomials.push_back(32439);
    polynomials.push_back(26975);

    ViterbiCodec codec(15, polynomials);

    TestViterbiCodecAutomatic(codec);
  }

  std::cout << "PASS" << std::endl;
}

