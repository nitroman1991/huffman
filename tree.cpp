#include "tree.hpp"

Tree::Tree(unsigned char let, unsigned long frequency) : l(nullptr), r(nullptr), f(nullptr), code(), letter(let), frequency(frequency)
{}

Tree::Tree(Tree *left, Tree *right, unsigned long frequency) : l(left), r(right), f(nullptr), code(), letter(-1), frequency(frequency)
{}

Tree::Tree(Tree *parent, unsigned char let) : l(nullptr), r(nullptr), f(parent), code(), letter(let), frequency(0)
{}

Tree::~Tree()
{
	l = nullptr;
	r = nullptr;
}

double Tree::freq() const
{
	return frequency;
}
