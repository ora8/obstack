 PHONY: all

cbuild:
	cc -std=c23 -g obstack.c main.c -o cobstack

cppbuild:
	c++ -std=c++26 -g obstack.cpp main.cpp -o cppobstack

all: cbuild cppbuild