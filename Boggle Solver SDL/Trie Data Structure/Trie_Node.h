#pragma once
const int ALPHABET_SIZE = 26;

class Trie_Node {
public:
	bool is_end_of_word{ 0 };
	Trie_Node* children[ALPHABET_SIZE]{ 0 };
	bool is_registerd{ 0 };
	//Uniform Initialization hence no need of construtor
	Trie_Node* get_child(int index) const { return children[index]; }
	bool get_is_end_of_word() const { return is_end_of_word; }
	bool get_is_registerd() const { return is_registerd; }
	~Trie_Node() {
		// deleting the left and right pointers
		//delete left;
		//delete right;
		// printing node data
		//cout << "Deleted node with data " << this->children[] << endl;
	}
};

