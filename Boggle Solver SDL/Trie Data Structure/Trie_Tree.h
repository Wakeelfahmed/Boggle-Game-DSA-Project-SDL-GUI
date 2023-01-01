#pragma once
#include"Trie_Node.h"
using namespace std;
#include<string>
class Trie_Tree {
private:
	Trie_Node* root;
	int Number_of_Words_in_Tree;
public:
	Trie_Tree();
	int get_Number_of_Words_in_Tree() const;
	void Insert(const string& word);
	bool Search(const string& word) const;
	bool StartsWith(const string& prefix) const;
	void Display(Trie_Node* node, string word) const;
	void Display() const;
};
