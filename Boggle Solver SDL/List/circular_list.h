#pragma once
#include"Node.h"
class circular_list
{
private:
	Node* head;
public:
	circular_list();
	Node* get_head() const;
	void insert_beg(COORD v);
	void insert_end(COORD value);
	void insert_specfic_Position(int place, COORD value);
	void insert_after(COORD oldvalue, COORD newvalue);
	void delete_Node(COORD Value_to_delete);
	void Display_list() const;
	bool isEmpty() const;
	int Number_of_Nodes()const;
	~circular_list();
	circular_list concatenate(const circular_list& list2);
	circular_list(circular_list& list);	//Copy constructor
	Node* get_Node_by_Pos(int index) {
		int i = 1;
		if (head == NULL)
			return NULL;
		Node* p = head;
		do {
			if (i == index)
				return p;
			i++;
			p = p->next;
		} while (p != head);
	}
};