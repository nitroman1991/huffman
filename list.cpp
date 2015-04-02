#include <iostream>
#include <string>
#include <unistd.h>

#include "tree.hpp"
#include "engine.hpp"


int main(int argc, char *argv[])
{
	string in;
	string out;
	bool compress_or_not = false;
	int opt = 0;
	while ((opt = getopt(argc, argv, "hcuf:o:")) != -1)
	{
		switch (opt) {
			case 'c': compress_or_not = true; break;
			case 'u': compress_or_not = false; break;
			case 'f': in = string(optarg); break;
			case 'o': out = string(optarg); break;
		}
	}
	if(compress_or_not)
		compress(in, out);
	else
		uncompress(in, out);
	return 0;
}
