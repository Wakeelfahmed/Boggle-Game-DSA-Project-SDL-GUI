#include "Trie_Tree.h"
#include<iostream>
#include<fstream>
using namespace std;
Trie_Tree::Trie_Tree() { root = new Trie_Node();	Number_of_Words_in_Tree = 0; }
int Trie_Tree::get_Number_of_Words_in_Tree() const { return Number_of_Words_in_Tree; }
void Trie_Tree::Insert(const string word) {
	Trie_Node* current = root;
	for (char c : word) {
		int index = c - 'a';		//cout << index << " ";
		if (current->children[index] == nullptr)
			current->children[index] = new Trie_Node();

		current = current->children[index];
	}
	Number_of_Words_in_Tree++;
	current->is_end_of_word = true;
}
bool Trie_Tree::Search(const string word) const {
	Trie_Node* current = root;
	for (char c : word) {
		int index = c - 'a';
		if (current->children[index] == nullptr)
			return false;

		current = current->children[index];
	}
	return current->is_end_of_word;
}
bool Trie_Tree::StartsWith(const string prefix) const {
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
	if (node->is_end_of_word)
		cout << word << endl;

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
	//cout << "DONE WRITING\n";
}
Trie_Node* Trie_Tree::get_Tree_Root() const { return root; }
Trie_Node* Trie_Tree::get_Parent(Trie_Node* Child, string Current_Word) const {
	Trie_Node* p = root;
	int index;
	for (int i = 0; i < Current_Word.size() - 1; i++)
	{
		index = tolower((Current_Word[i])) - 'a';
		//cout << Current_Word[i] << endl;
		//cout << index << endl;
		if (!(index >= 0 && index <= 25))//kind of exception handling
		{
			cout << "Warning\n";
			exit(0);
		}
		if (i == Current_Word.size() - 1) {
			//cout << "returning p: " << p->children[index];
			return p;
		}
		if (p->children[index] == nullptr)
			return NULL;
		p = p->children[index];
	}
	return p;
}
void Trie_Tree::Display_Registered_Word(Trie_Node* node, string word, string &All_Words) {
	if (node->is_end_of_word && node->is_registerd) {
		All_Words += word;
		All_Words += " ";
	}
	for (int i = 0; i < ALPHABET_SIZE; i++) {
		if (node->children[i] != nullptr) {
			char c = 'a' + i;
			Display_Registered_Word(node->children[i], word + c, All_Words);
		}
	}
}
void Trie_Tree::Reset_Registered_Words(Trie_Node* node, string word) const {
	if (node->is_end_of_word && node->is_registerd == 1)
		node->is_registerd = 0;
	
	for (int i = 0; i < ALPHABET_SIZE; i++) {
		if (node->children[i] != nullptr) {
			char c = 'a' + i;
			Reset_Registered_Words(node->children[i], word + c);
		}
	}
}
int max1 = 0;
int Trie_Tree::get_longest_string(Trie_Node* node, string word) const {
	if (node->is_end_of_word) {
		if (word.size() > max1) {
			cout << "Longest WOrd" << word << endl;
			max1 = word.size();
		}
	}
	for (int i = 0; i < ALPHABET_SIZE; i++) {
		if (node->children[i] != nullptr) {
			char c = 'a' + i;
			get_longest_string(node->children[i], word + c);
		}
	}
	return max1;
}