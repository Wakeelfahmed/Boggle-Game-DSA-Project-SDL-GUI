#pragma once
#include<Windows.h>
class Node
{
	COORD Data{};
public:
	Node* next;
	Node();
	Node(COORD Data);
	bool operator==(Node* list2) {
		if (list2->Data.X == Data.X && list2->Data.Y == Data.Y)
			return 1;
	}
	COORD get_Data() const { return Data; }
};