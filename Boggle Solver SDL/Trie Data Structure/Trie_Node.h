#pragma once
const int ALPHABET_SIZE = 26;
class Trie_Node {
public:
	bool is_end_of_word{0};//Marks the end of a string
	Trie_Node* children[ALPHABET_SIZE]{0};	//Uniform Initialization
	bool is_registerd{0};
	//Trie_Node() {
	//	is_end_of_word = false;
	//	for (int i = 0; i < ALPHABET_SIZE; i++)
	//		children[i] = nullptr;
	//	is_registerd = 0;
	//}
};

