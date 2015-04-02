#pragma once

#include <functional>
#include <cstdlib>
#include <assert.h>
#include <map>
#include <queue>
#include <iterator>
#include <bitset>

#include <iostream>

using std::string;
using std::ifstream;
using std::ofstream;
using std::istream_iterator;
using std::cout;
using std::cin;
using std::endl;

struct Tree;
struct long_counter;

typedef std::map<const unsigned char, long_counter> map_counter;
typedef std::map<const unsigned char, Tree *> map_tree;

struct Tree
{
private:
	Tree *l;
	Tree *r;
	Tree *f; //father
	string code; //left or rigth? 0 or 1?
	unsigned char letter;
	double frequency;


	friend struct compare;

public:
	Tree(unsigned char let, unsigned long frequency);
	Tree(Tree *parent, unsigned char let);
	Tree(Tree *left, Tree *right, unsigned long frequency);

	friend char search_letter(Tree *head, char const **ptr_to_prefix_code);
	friend Tree *construct_tree(map_counter &frequencies, map_tree &refs);
	friend void recursive_code_building(Tree *head);
	friend void header_building(string &header, Tree *head);
	friend void generate_output(string const &in_name, string const &out_name, string const &header, map_tree &codes);
	friend Tree *rebuild_tree_from_file(string::iterator beg, string::iterator end);
	double freq() const;

	~Tree();
};


struct compare : std::binary_function<Tree*, Tree*, bool>
{
	bool operator() (Tree const *a, Tree const *b) const
	{
		return b -> frequency < a -> frequency; //inverted order - to pop a less element
	}
};


struct long_counter
{
	unsigned long val;
	long_counter() : val(0)
	{}
};
