#pragma once
const int ALPHABET_SIZE = 26;

class Trie_Node {
public:
	bool is_end_of_word{ 0 };
	Trie_Node* children[ALPHABET_SIZE]{ 0 };
	bool is_registerd{ 0 };

	Trie_Node* get_child(int index) const { return children[index]; }	//Uniform Initialization
	bool get_is_end_of_word() const { return is_end_of_word; }
	bool get_is_registerd() const { return is_registerd; }

};

