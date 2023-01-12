#include"Node.h"
#include<Windows.h>

#include<iostream>
using namespace std;
Node::Node() : next(0) {}
Node::Node(COORD Data) : Data(Data) ,next(0) {}