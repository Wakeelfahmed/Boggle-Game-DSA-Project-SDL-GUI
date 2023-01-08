#include "Trie_Tree.h"
#include<iostream>
#include<fstream>
using namespace std;
Trie_Tree::Trie_Tree() { root = new Trie_Node();	Number_of_Words_in_Tree = 0; }
int Trie_Tree::get_Number_of_Words_in_Tree() const { return Number_of_Words_in_Tree; }
void Trie_Tree::Insert(const string& word) {
	Trie_Node* current = root;
	for (char c : word) {
		int index = c - 'a';
		//cout << index << " ";
		if (current->children[index] == nullptr)
			current->children[index] = new Trie_Node();

		current = current->children[index];
	}
	Number_of_Words_in_Tree++;
	current->is_end_of_word = true;
}
bool Trie_Tree::Search(const string& word) const {
	Trie_Node* current = root;
	for (char c : word) {
		int index = c - 'a';
		if (current->children[index] == nullptr)
			return false;

		current = current->children[index];
	}
	return current->is_end_of_word;
}
bool Trie_Tree::StartsWith(const string& prefix) const {
	Trie_Node* current = root;
	for (char c : prefix) {
		int index = c - 'a';
		if (current->children[index] == nullptr)
			return false;

		current = current->children[index];
	}
	return true;
}
void Trie_Tree::Display() const { Display(root, ""); }
void Trie_Tree::Display(Trie_Node* node, string word) const {
	if (node->is_end_of_word) {
		cout << word << endl;
	}
	for (int i = 0; i < ALPHABET_SIZE; i++) {
		if (node->children[i] != nullptr) {
			char c = 'a' + i;
			Display(node->children[i], word + c);
		}
	}
}
void Trie_Tree::Write_SORTED_To_File_fr_Trie_Tree() const { Write_SORTED_To_File_fr_Trie_Tree(root, ""); }
ofstream file("SORTED Words Dictionary.txt");
void Trie_Tree::Write_SORTED_To_File_fr_Trie_Tree(Trie_Node* node, string word) const {
	string temp;	int Number_of_W_Written = 0;
	if (!file) {
		cout << "Unable to Write to File(Words Dictionary.txt) !!\n";
	}
	if (node->is_end_of_word) {
		file << word << endl;
	}
	for (int i = 0; i < ALPHABET_SIZE; i++) {
		if (node->children[i] != nullptr) {
			char c = 'a' + i;
			Write_SORTED_To_File_fr_Trie_Tree(node->children[i], word + c);
		}
	}
	cout << "DONE WRITING\n";
}