
#include "engine.hpp"

int first_phase(string const &filename, map_counter &store_result)
{
	char tmp_char;
	unsigned long sum = 0; 
	std::fstream input(filename.c_str(), std::ios::in);
	while(input.get(tmp_char))
	{
		store_result[tmp_char].val += 1;
		++sum;
	}
	input.close();
	cout << "Input file has the size of " << sum << "." << endl;
	if(PRINT)
		cout << "first phase completed..\n";
	
	if(sum == 0)
		return 1;
	return 0;
}

char construct_symbol(std::queue<char> &q)
{
	int next_sym = 0;
	int razr = 128;
	while (razr > 0 && !q.empty())
	{
		next_sym += razr * (q.front() - 48); //atoi: '0' -> 0, '1' -> 1
		razr >>= 1;
		q.pop();
	}
	return (char)next_sym;
}

void generate_output(string const &in_name, string const &out_name, string const &header, map_tree &codes)
{
	std::fstream output(out_name.c_str(), std::ios::out);
	output.write(header.c_str(), header.size());
	output << ";";
	cout << "The size of header is " << header.size() << "." << endl;	
	if(header.empty())
	{
		output.close();
		return;
	}

	unsigned long sum = 0;
	char tmp_char;
	std::fstream input(in_name.c_str(), std::ios::in);
	std::queue<char> q;
	while(input.get(tmp_char))
	{
		for(string::reverse_iterator it = (codes[tmp_char] -> code).rbegin(); it != (codes[tmp_char] -> code).rend(); ++it)
		{
			q.push(*it);
			if(q.size() >= 8)
			{
				output.put(construct_symbol(q));
				++sum;
			}
		}
	}
	
	int tail_size = q.size();
	output.put(construct_symbol(q));
	output.put(CHAR((8 - tail_size)));
	cout << "The size of data is " << sum + 2 << "." << endl;

	input.close();
	output.close();
	if(PRINT)
		cout << "Output generated..\n";
	return;
}

int compress(string const &in, string const &out)
{
	map_counter frequencies_of_symbols;
	map_tree references_to_leafs;

	//first stage
	first_phase(in.c_str(), frequencies_of_symbols); //counting the statistics of the file
	Tree *head = construct_tree(frequencies_of_symbols, references_to_leafs); //..
	frequencies_of_symbols.clear();

	//second stage
	string header;
	header_building(header, head);
	if(PRINT)
		cout << "Header built..\n";
	recursive_code_building(head); //remove tree in memory and retrieve coding strings
	if(PRINT)	
		cout << "Recursive code built..\n";
	generate_output(in, out, header, references_to_leafs);
	return 0;
}

int uncompress(string const &in, string const &out)
{
	string header;
	std::fstream input(in.c_str(), std::ios::in);
	std::fstream output(out.c_str(), std::ios::out);
	istream_iterator<char> ii(input);
	
	while (*ii != ';')
		header.push_back(*(ii++));
	cout << "The size of header is " << header.size() << "." << endl;	
	if(header.empty())
	{
		output.close();
		return 1;
	}
	Tree *head = rebuild_tree_from_file(header.begin(), header.end());
	if(PRINT)
		cout << "Tree rebuilded from file..\n";
	header.clear();

	char tmp_char;
	unsigned long sum = 0;
	while(input.get(tmp_char))
	{
		std::bitset<8*sizeof(char)> bits(tmp_char);
		header += bits.to_string();
		++sum;
	}
	input.close();
	cout << "The size of data is " << sum << "." << endl;
	
	int tail = INT(tmp_char);
	assert(tail >= 0 && tail <= 8);
	header.resize(header.size() - tail - 8*sizeof(char));

	char const *bin_data = header.c_str();
	sum = 0;
	do
	{
		output.put(search_letter(head, &bin_data));
		++sum;	
	}while(*bin_data != 0);
	cout << "Out file has the size of " << sum << "." << endl;

	output.close();
	if(PRINT)
		cout << "Uncompressed!\n";
	return 0;
}

char search_letter(Tree *head, char const **ptr_to_prefix_code)
{
	while(head -> l != nullptr && head -> r != nullptr)
	{
		if(**ptr_to_prefix_code == '0')
			head = head -> l;
		else if (**ptr_to_prefix_code == '1')
			head = head -> r;
		++(*ptr_to_prefix_code);
	}
	return head -> letter;
}

Tree *construct_tree(map_counter &frequencies, map_tree &refs)
{
	std::priority_queue<Tree*, std::vector<Tree *>, compare> que;
	for(auto &it : frequencies)
	{
		refs[it.first] = new Tree(it.first, it.second.val);
		que.push(refs[it.first]);
	}

	while(que.size() > 1)
	{
		Tree *less = que.top();
		que.pop();
		less -> code = '0';

		Tree *more = que.top();
		que.pop();
		more -> code = '1';
		
		Tree *tmp = new Tree(less, more, less -> frequency + more -> frequency); 
		less -> f = more -> f = tmp;
		
		que.push(tmp);
	}
	if(PRINT)
		cout << "Tree constructed..";
	if(que.empty())
		return nullptr;
	return que.top();
}

void recursive_code_building(Tree *head)
{
	if(head == nullptr) return;
	if(head -> f != nullptr)
	{
		head -> code += head -> f -> code;
		head -> f = nullptr;
	}
	if(head -> l != nullptr)
		recursive_code_building(head -> l);
	if(head -> r != nullptr)
		recursive_code_building(head -> r);
	if(head -> l != nullptr && head -> r != nullptr)
		delete head;
}

void header_building(string &header, Tree *head)
{
	if(head == nullptr) return;
	if(head -> l != nullptr && head -> r != nullptr)
	{
		header += '{';
		header_building(header, head -> l);
		header += ':';
		header_building(header, head -> r);
		header += '}';
	}
	else
		header += std::to_string((unsigned char)(head -> letter));
	
}

inline unsigned char stoi(string::iterator big, string::iterator little)
{
	unsigned char sum = 0;
	int div = 1;
	while(big != little)
	{
		sum += INT(*(little-1)) * div;
		div *= 10;
		--little;
	}
	return sum;
}

Tree *rebuild_tree_from_file(string::iterator beg, string::iterator end)
{
	if(*beg != '{' && *(end - 1) != '}')
		return new Tree(nullptr, (unsigned char)stoi(beg, end));
	else
	{
		--end;
		++beg;
		string::iterator border = beg;
		int left_brackets = 0;
		int right_brackets = 0;
		do
		{
			if(*border == '{') ++left_brackets;
			if(*border == '}') ++right_brackets;
			++border;
		}while(left_brackets > right_brackets || (left_brackets == 0 && *border != ':'));
		assert(*border == ':');
		
		Tree *left = rebuild_tree_from_file(beg, border);
		left -> code = '0';

		Tree *right = rebuild_tree_from_file(border + 1, end);
		right -> code = '1';

		Tree *now = new Tree(left, right, 0);
		left -> f = right -> f = now;
		
		return now;
	}
}
