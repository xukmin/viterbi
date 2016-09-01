# Makefile for viterbi codec.
#
# Created by: Min Xu <mxu@scu.edu> or <xukmin@gmail.com>
# Date: 01/30/2015

CXX = g++

BINS = viterbi_main viterbi_test
SRCS = viterbi.cpp viterbi_main.cpp viterbi_test.cpp

all: $(BINS)

clean:
	$(RM) *.o $(BINS)

test: viterbi_test
	./viterbi_test

viterbi.o: viterbi.cpp viterbi.h
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $<

viterbi_main.o: viterbi_main.cpp viterbi.h
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $<

viterbi_main: viterbi_main.o viterbi.o
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $^ -o $@

viterbi_test.o: viterbi_test.cpp viterbi.h
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $<

viterbi_test: viterbi_test.o viterbi.o
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $^ -o $@

.PHONY: all clean test

