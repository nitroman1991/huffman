#pragma once
#include <stdio.h>
#include <cstdlib>
#include <fstream>

#include "tree.hpp"

#define CHAR(x) (char)(x + 48)
#define INT(x) ((int)x - 48)
#define PRINT 0

int first_phase(string const &filename, map_counter &store_result);

char construct_symbol(std::queue<char> &q);

void generate_output(string const &in_name, string const &out_name, string const &header, map_tree &codes);

int compress(string const &in, string const &out);

int uncompress(string const &in, string const &out);

char search_letter(Tree *head, char const **ptr_to_prefix_code);

Tree *construct_tree(map_counter &frequencies, map_tree &refs);

void recursive_code_building(Tree *head);

void header_building(string &header, Tree *head);

Tree *rebuild_tree_from_file(string::iterator beg, string::iterator end);
