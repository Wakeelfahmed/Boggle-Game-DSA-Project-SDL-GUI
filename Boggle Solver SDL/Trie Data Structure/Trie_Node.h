#pragma once
const int ALPHABET_SIZE = 26;
class Trie_Node {
public:
	bool is_end_of_word;	//this tell about the end of a string
	Trie_Node* children[ALPHABET_SIZE];
	Trie_Node() {
		is_end_of_word = false;
		for (int i = 0; i < ALPHABET_SIZE; i++)
			children[i] = nullptr;
	}
};

