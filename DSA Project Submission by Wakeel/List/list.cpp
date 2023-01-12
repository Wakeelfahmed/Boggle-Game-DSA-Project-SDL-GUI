#include"list.h"
#include<iostream>

using namespace std;
list::list() : head(NULL) { }
Node* list::get_head() const { return head; }
bool list::isEmpty() const {
	if (head == NULL)
		return true;
	return false;
}
void list::insert_beg(COORD v) {
	Node* NewNode = new Node(v);
	if (isEmpty()) {
		head = NewNode;
		NewNode->next = head;
	}
	else {
		NewNode->next = head;
		Node* p = head;
		while (p->next != head)
			p = p->next;
		head = NewNode;
		p->next = head;  //last Node point to head
	}
}
void list::insert_specfic_Position(int Position, COORD newvalue) {
	int position_count = 1;
	if (Position > Number_of_Nodes()) {
		cout << "List has less nodes than the place, Node CAN't be inserted!!\n";
		return;
	}
	Node* NewNode = new Node(newvalue);		//NewNode->Data = newvalue;
	Node* p = head;
	while (p != head)
	{
		if (Position == 1) {
			insert_beg(newvalue);
			return;
		}
		else if (Position == Number_of_Nodes()) {
			insert_end(newvalue);
			return;
		}
		else if (position_count == Position - 1 && Position != 0) {
			NewNode->next = p->next;
			p->next = NewNode;
			return;
		}
		p = p->next;		//move to next node
		position_count++;
	}
}
void list::insert_after(COORD oldvalue, COORD newvalue) {
	Node* p = head;
	if (isEmpty())
		return;
	do
	{
		if (p->get_Data().X == oldvalue.X && p->get_Data().Y == oldvalue.Y)
		{
			Node* NewNode = new Node(newvalue);
			NewNode->next = p->next;
			p->next = NewNode;
			return;
		}
		p = p->next;
	} while (p != head);
}
void list::insert_end(COORD value) {
	Node* NewNode = new Node(value);
	if (isEmpty()) {
		head = NewNode;
		NewNode->next = head;
	}
	else {
		Node* p = head;
		do
		{
			p = p->next;
		} while (p->next != head);
		p->next = NewNode;
		NewNode->next = head;
		return;
	}
}
void list::Display_list() const {
	if (isEmpty())
	{
		cout << "list is empty!!" << endl;
		return;
	}
	Node* p = head;
	cout << "List: \t";
	do
	{
		//cout << p->Data << "\t";
		p = p->next;
	} while (p != head);
	cout << endl;
}
int Search_in_List(COORD Value_to_search, list& list) {
	Node* p = list.get_head();
	int Position_counter = 1;
	do {
		if (p->get_Data().X == Value_to_search.X && p->get_Data().Y == Value_to_search.Y)
			return Position_counter;
		p = p->next;
		Position_counter++;
	} while (p != list.get_head());
	return 0; // no record found
}
void list::delete_Node(COORD Value_to_delete) {
	if (isEmpty()) {
		cout << "List is Empty\n"; return;
	}
	else if (!Search_in_List(Value_to_delete, *this)) {
		cout << "Not found in list\n"; return;
	}
	Node* p = head;
	Node* Temp;
	if (Value_to_delete.X == head->get_Data().X && Value_to_delete.Y == head->get_Data().Y) {
		Temp = p;
		if (head->next == head) {
			delete Temp; head = NULL; return;
		}
		else
		{
			do
			{
				p = p->next;
			} while (p->next != head);
			p->next = head->next;
			head = head->next;
			delete Temp;
			return;
		}
	}
	else
	{
		p = head;
		do
		{
			if (p->next->get_Data().X == Value_to_delete.X && p->next->get_Data().Y == Value_to_delete.Y)
			{
				Temp = p->next;
				p->next = p->next->next;
				delete Temp;
				return;
			}
			p = p->next;
		} while (p->next != head);
		cout << Number_of_Nodes() << endl;
	}
}
list list::concatenate(const list& list2) {
	list Result = *this;
	Node* p = get_head();	Node* loop2 = list2.get_head();
	while (p->next != get_head())
		p = p->next;
	while (loop2->next != list2.get_head())
		loop2 = loop2->next;
	loop2->next = head;	//connect last node of list2 to head of list1 making circular
	p->next = list2.get_head();//connect last node of list1 to head of list2.
	cout << "Concatenated list is:\n";
	Result.Display_list();
	return Result;
}
int list::Number_of_Nodes() const {
	if (isEmpty())
	{
		//cout << "list is empty!! \n there are no nodes." << endl;
		return 0;
	}
	int Counter = 0;
	Node* p = head;
	do
	{
		Counter++;
		p = p->next;
	} while (p != head);
	return Counter;
}
list::~list() {
	//cout << "Entering ~\n";
	if (isEmpty())
		return;
	Node* p = head;
	Node* q = head->next;
	do {
		//cout << "Deleteing:" << p->Data << endl;
		delete p;
		p = q;
		if (p != head)
			q = q->next;
		if (p == head) {
			//cout << "Returing ~\n";
			//return;
		}
	} while (p != head);
	//cout << "Leaving ~\n";
}
list::list(list& list) {
	head = list.head;
}