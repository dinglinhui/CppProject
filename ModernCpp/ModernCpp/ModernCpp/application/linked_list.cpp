#include "linked_list.hpp"
#include <random>

namespace application {

LinkedList::LinkedList() {

}

LinkedList::~LinkedList() {
}

void LinkedList::initLinkedList(unsigned int len) {
	initLinkedList(&pHead_, len);
}

void LinkedList::initLinkedList(LinkNode** ppLink, unsigned int len) {
	std::default_random_engine e;
	std::uniform_int_distribution<unsigned> u(0, 100); //随机数分布对象

	LinkNode *pPreNode = *ppLink;
	for (unsigned int i = 0; i < len; i++) {
		LinkNode *pNode = new LinkNode();
		pNode->value = u(e);
		pNode->next = nullptr;

		if (pPreNode == nullptr) {
			pHead_ = pNode;
			pPreNode = pHead_;
		} else {
			pPreNode->next = pNode;
			pPreNode = pNode;
		}
	}
	pPreNode->next = nullptr;
}

void LinkedList::destoryLinkedList() {
	LinkNode* pNode = inspectRing();
	if (!pNode) {
		/*		LinkNode *pNextNode = nullptr;
		 while (pHead_ != nullptr) {
		 pNextNode = pHead_->next;

		 delete pHead_, pHead_ = pNextNode;
		 }*/
		destoryLinkedList(&pHead_);
	} else {
		destoryLinkedList(&pHead_, pNode);
	}
}

void LinkedList::destoryLinkedList(LinkNode* pNode) {
	destoryLinkedList(&pHead_, pNode);
}

void LinkedList::destoryLinkedList(Node ** ppLink) {
	LinkNode *pNextNode = nullptr;
	while (*ppLink != nullptr) {
		pNextNode = (*ppLink)->next;

		delete *ppLink, *ppLink = pNextNode;
	}
}

void LinkedList::destoryLinkedList(Node** ppLink, LinkNode* pNode) {
	bool is_ringstart_free = false; //环起始点只能被释放空间一次
	LinkNode *pNextNode = nullptr;

	while (*ppLink != nullptr) {
		pNextNode = (*ppLink)->next;
		if (*ppLink == pNode) { //如果是环起始点
			if (is_ringstart_free)
				break; //如果第二次遇到环起始点addr, 表示已经释放完成
			else
				is_ringstart_free = true; //记录已经释放一次
		}
		delete *ppLink;
		*ppLink = pNextNode;
	}
}

///判断链表是否有环
LinkNode* LinkedList::inspectRing() {
	if (pHead_ == nullptr)
		return nullptr;

	LinkNode *pFast = pHead_, *pSlow = pHead_;
	while (true) {
		if (pSlow->next != nullptr && pFast->next != nullptr
				&& pFast->next->next != nullptr) {
			pSlow = pSlow->next;
			pFast = pFast->next->next;
		} else {
			return nullptr;
		}

		if (pFast == pSlow) {
			return pFast;
		}
	}
}

LinkNode* LinkedList::getHeadNode() {
	return pHead_;
}

LinkNode* LinkedList::getTailNode() {
	if (pHead_ == nullptr)
		return nullptr;

	LinkNode *pNode = pHead_;
	while (pNode->next != nullptr) {
		pNode = pNode->next;
	}
	return pNode;
}

int LinkedList::getLength() {
	int len = 0;
	LinkNode *pNextNode = pHead_;
	while (pNextNode != nullptr) {
		len++;
		pNextNode = pNextNode->next;
	}
	return len;
}

///获取链表环长
int LinkedList::getRingLength(LinkNode *pNode) {
	int len = 0;
	LinkNode *pFast = pNode, *pSlow = pNode;
	while (true) {
		pFast = pFast->next->next;
		pSlow = pSlow->next;
		len++;

		if (pFast == pSlow) {
			break;
		}
	}
	return len;
}

///获取链表头到环连接点的长度
int LinkedList::getLenA(LinkNode *pNode) {
	int lenA = 0;
	LinkNode *pFast = pHead_;
	LinkNode *pSlow = pNode;
	while (true) {
		pFast = pFast->next;
		pSlow = pSlow->next;
		lenA++;

		if (pFast == pSlow) {
			break;
		}
	}
	return lenA;
}

///环起始点
///如果有环, 释放空空间时需要注意.
LinkNode* LinkedList::ringStart(int lenA) {
	if (!pHead_ || lenA <= 0) {
		return nullptr;
	}

	int i = 0;
	LinkNode* tmp = pHead_;
	for (; i < lenA; ++i) {
		if (tmp != nullptr) {
			tmp = tmp->next;
		}
	}

	return (i == lenA) ? tmp : nullptr;
}

LinkNode* LinkedList::forward(int len) {
	if (pHead_ == nullptr)
		return nullptr;

	if (len > getLength() || len < 0)
		return nullptr;

	LinkNode *ptr = pHead_;
	do {
		ptr = ptr->next;
	} while (--len != 0);
	return ptr;
}

bool LinkedList::isCrossed(LinkedList &list) {
	return this->getTailNode() == list.getTailNode() ? true : false;
}

LinkNode* LinkedList::getCrossedNode(LinkedList &list) {
	unsigned int len1 = this->getLength();
	unsigned int len2 = list.getLength();
	LinkNode * ptr1 = pHead_;
	LinkNode * ptr2 = list.getHeadNode();
	if (len1 > len2) {
		ptr1 = this->forward(len1 - len2);
	} else {
		if (len2 > len1) {
			ptr2 = list.forward(len2 - len1);
		}
	}

	while (ptr1 != ptr2) {
		ptr1 = ptr1->next;
		ptr2 = ptr2->next;
	}
	return ptr1;
}

void LinkedList::appendLink(LinkedList &list) {
	LinkNode * pTailPtr = this->getTailNode();
	pTailPtr->next = list.getHeadNode();
}

void LinkedList::removeLink(LinkNode *cmnPtr) {
	LinkNode *p = pHead_, *q = nullptr;
	while (p != cmnPtr) {
		q = p;
		p = p->next;
	}
	q->next = nullptr;
}

void LinkedList::reverse() {
	LinkNode *ptr = nullptr;
	LinkNode *p = pHead_, *q = pHead_->next;

	p->next = nullptr;
	ptr = q->next;
	q->next = p;

	while (ptr != nullptr) {
		p = q;
		q = ptr;
		ptr = ptr->next;

		q->next = p;
	}
	pHead_ = q;
}

void LinkedList::print() {
	LinkNode *p = pHead_;
	while (p->next != nullptr) {
		std::cout << p->value << "->";
		p = p->next;
	}
	std::cout << p->value << std::endl;
}

void test_link_loop() {

	{
		int nLenA = 0;
		int nRingLength = 0;

		LinkedList list;
		list.initLinkedList(12);
		list.print();

		list.reverse();
		list.print();

		LinkNode * pRingStartNode = nullptr;
		LinkNode * pRingNode = list.inspectRing();
		if (pRingNode != nullptr) {
			nRingLength = list.getRingLength(pRingNode);
			nLenA = list.getLenA(pRingNode);
			pRingStartNode = list.ringStart(nLenA);
		}

		list.destoryLinkedList(pRingNode);
	}

	{
		LinkedList list1, list2, cmnList;
		list1.initLinkedList(12);
		list2.initLinkedList(10);
		cmnList.initLinkedList(5);
		list1.print();
		list2.print();
		cmnList.print();

		list1.appendLink(cmnList);
		list2.appendLink(cmnList);
		list1.print();
		list2.print();
		if (list1.isCrossed(list2)) {
			LinkNode *cmnPtr = list1.getCrossedNode(list2);
			list1.removeLink(cmnPtr);
			list1.print();
			list2.print();
		}
		list1.destoryLinkedList();
		list2.destoryLinkedList();
	}
}

} /* namespace application */
