#ifndef _LINKED_LIST_HPP_
#define _LINKED_LIST_HPP_
#include <iostream>
#include <cstdlib>

namespace application {

	typedef struct Node {
		int value;
		struct Node * next;
	} LinkNode;

	class LinkedList {
	public:
		LinkedList();
		virtual ~LinkedList();

	public:
		void initLinkedList(unsigned int len);
		void destoryLinkedList();
		void destoryLinkedList(LinkNode* pNode);

		LinkNode* inspectRing();
		LinkNode* getHeadNode();
		LinkNode* getTailNode();

		int getLength();
		int getRingLength(LinkNode *pNode);
		int getLenA(LinkNode *pNode);
		LinkNode* ringStart(int lenA);
		LinkNode* forward(int len);

		bool isCrossed(LinkedList &list);
		LinkNode* getCrossedNode(LinkedList &list);
		void appendLink(LinkedList &list);
		void removeLink(LinkNode *cmnPtr);

		void reverse();
		void print();

	private:
		void initLinkedList(LinkNode ** ppLink, unsigned int len);
		void destoryLinkedList(LinkNode ** ppLink);
		void destoryLinkedList(LinkNode ** ppLink, LinkNode* pNode);

	private:
		LinkNode *pHead_ = nullptr;
	};

	void test_link_loop();
} /* namespace application */
#endif /* _LINKED_LIST_HPP_ */
