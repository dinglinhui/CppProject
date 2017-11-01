#if 1
#include <iostream>
#include <vector>
#include <algorithm>
#include <cassert>

struct Node {
    int data;
    Node * next;
};

void initLink(Node ** link, unsigned int len) {
    Node *ptr = nullptr, *head = nullptr;

    ptr = new Node();
    ptr->data = 0;
    ptr->next = nullptr;
    (*link) = head = ptr;

    for (unsigned int i = 1; i < len; i++) {
        ptr = new Node();
        ptr->data = i;
        ptr->next = nullptr;
        head->next = ptr;
        head = ptr;
    }
}

void destoryLink(Node ** link) {
    if (*link == nullptr)
        return;
    Node *ptr = *link, *tmp = nullptr;
    do {
        tmp = ptr->next;
        ptr->next = nullptr;
        delete ptr, ptr = tmp;
    } while (ptr != nullptr);
}

void destoryCrossedLinks(std::vector<Node **> links) {

}

int getLength(Node *head) {
    unsigned int length = 0;
    if (head == nullptr)
        return length;

    do {
        length++;
        head = head->next;
    } while (head != nullptr);

    return length;
}

Node * getTailPtr(Node *head) {
    if (head == nullptr)
        return nullptr;

    Node *ptr = head;
    while (ptr->next != nullptr) {
        ptr = ptr->next;
    }
    return ptr;
}

void reverse(Node *&link) {
    Node *ptr = nullptr;
    Node *p = link, *q = link->next;
    p->next = nullptr;
    ptr = q->next;
    q->next = p;

    while (ptr != nullptr) {
        p = q;
        q = ptr;
        ptr = ptr->next;

        q->next = p;
    }
    link = q;
}

void print(const Node *link) {
    while (link->next != nullptr) {
        std::cout << link->data << "->";
        link = link->next;
    }
    std::cout << link->data << std::endl;
}

/*void mergeTwoLinks(Node *head1, Node *head2) {
 assert(head1 != nullptr && head2 != nullptr);

 }

 Node * merge(Node *p, Node *q) {
 if (p->data < q->data) {
 p->next = q;
 q->next = nullptr;
 } else {
 q->next = p;
 p->next = nullptr;
 }
 }*/

bool isCrossed(Node *head1, Node *head2) {
    if (head1 == nullptr || head2 == nullptr)
        return false;
    return getTailPtr(head1) == getTailPtr(head2) ? true : false;
}

Node * forward(Node *head, int len) {
    if (head == nullptr)
        return nullptr;

    if (len > getLength(head) || len < 0)
        return nullptr;

    do {
        head = head->next;
    } while (--len != 0);
    return head;
}

Node * getCrossedNode(Node *head1, Node *head2) {
    if (head1 == nullptr || head2 == nullptr)
        return nullptr;

    unsigned int len1 = getLength(head1);
    unsigned int len2 = getLength(head2);
    Node * ptr1 = head1;
    Node * ptr2 = head2;
    if (len1 > len2) {
        ptr1 = forward(ptr1, len1 - len2);
    } else {
        if (len2 > len1) {
            ptr2 = forward(ptr2, len2 - len1);
        }
    }

    while (ptr1 != ptr2) {
        ptr1 = ptr1->next;
        ptr2 = ptr2->next;
    }
    return ptr1;
}

void appendLink(Node ** link, Node ** cmnlink) {
    Node * linkTailPtr = getTailPtr(*link);
    linkTailPtr->next = *cmnlink;
}

unsigned int strlength(const char *destString) {
    assert(destString != nullptr);
    unsigned int len = 0;
    while (*destString++ != '\0')
        len++;
    return len;
}

char *strcopy(char *destString, unsigned int destLength, const char *srcString) {
    assert(destString != nullptr && srcString != nullptr);
    if (strlength(srcString) > destLength)
        return nullptr; // partly copy?

    char *dest = destString;
    while ((*destString++ = *srcString++) != '\0')
        ;
    return dest;
}

void test_link() {
    Node *head1 = nullptr;
    initLink(&head1, 10);
    print(head1);
    reverse(head1);
    print(head1);
    /*
     Node *head1 = nullptr, *head2 = nullptr, *cmnLink = nullptr;
     initLink(&head2, 12);
     initLink(&cmnLink, 6);
     appendLink(&head1, &cmnLink);
     appendLink(&head2, &cmnLink);

     if (isCrossed(head1, head2)) {
     std::vector<Node **> links;
     links.push_back(&head1);
     links.push_back(&head2);

     Node *cmnPtr = getCrossedNode(head1, head2);
     destoryCrossedLinks(links);

     } else {
     destoryLink(&head1);
     destoryLink(&head2);
     }

     const char *srcStr = "dinglinhui";
     char destStr[10];
     std::cout << strcopy(destStr, 10, srcStr) << std::endl;*/
}

#endif
