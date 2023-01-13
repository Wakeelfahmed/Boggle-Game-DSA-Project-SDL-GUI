#pragma once
#include"Trie_Node.h"
using namespace std;
#include<string>
class Trie_Tree {
public:
	Trie_Node* root;
	int Number_of_Words_in_Tree;//Words of Words in Tree
	Trie_Tree();
	int get_Number_of_Words_in_Tree() const;
	void Insert(const string word);
	bool Search(const string word) const;
	bool StartsWith(const string prefix) const;
	void Display(Trie_Node* node, string word) const;
	void Write_SORTED_To_File_fr_Trie_Tree() const;
	void Write_SORTED_To_File_fr_Trie_Tree(Trie_Node* node, string word) const;
	void Display() const;
	Trie_Node* get_Tree_Root() const;
	Trie_Node* get_Parent(Trie_Node* Child, string Current_Word) const;
	void Display_Registered_Word(Trie_Node* node, string word, string& All_Words, int string_length);
	void Reset_Registered_Words(Trie_Node* node, string word) const;
	void get_smallest_longest_string(Trie_Node* node, string word, int& smallest, int& longest) const;
};
