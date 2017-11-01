#if 0
#include <stdio.h>
#include <stdlib.h>

typedef struct node {
    int value;
    struct node *next;
} LinkNode, *Linklist;

/// 创建链表(链表长度，环节点起始位置)
Linklist createList() {
    Linklist head = nullptr;
    LinkNode *preNode = head;
    LinkNode *FifthNode = nullptr;
    for (int i = 0; i < 6; i++) {
        LinkNode *tt = (LinkNode*) malloc(sizeof(LinkNode));
        tt->value = i;
        tt->next = nullptr;
        if (preNode == nullptr) {
            head = tt;
            preNode = head;
        } else {
            preNode->next = tt;
            preNode = tt;
        }

        if (i == 3)
            FifthNode = tt;
    }
    preNode->next = FifthNode;
    return head;
}

///判断链表是否有环
LinkNode* judgeRing(Linklist list) {
    LinkNode *fast = list;
    LinkNode *slow = list;

    if (list == nullptr)
        return nullptr;

    while (true) {
        if (slow->next != nullptr && fast->next != nullptr && fast->next->next != nullptr) {
            slow = slow->next;
            fast = fast->next->next;
        } else
            return nullptr;

        if (fast == slow)
            return fast;
    }
}

///获取链表环长
int getRingLength(LinkNode *ringMeetNode) {
    int RingLength = 0;
    LinkNode *fast = ringMeetNode;
    LinkNode *slow = ringMeetNode;
    for (;;) {
        fast = fast->next->next;
        slow = slow->next;
        RingLength++;
        if (fast == slow)
            break;
    }
    return RingLength;
}

///获取链表头到环连接点的长度
int getLenA(Linklist list, LinkNode *ringMeetNode) {
    int lenA = 0;
    LinkNode *fast = list;
    LinkNode *slow = ringMeetNode;
    for (;;) {
        fast = fast->next;
        slow = slow->next;
        lenA++;
        if (fast == slow)
            break;
    }
    return lenA;
}

///环起始点
///如果有环, 释放空空间时需要注意.
LinkNode* RingStart(Linklist list, int lenA) {
    if (!list || lenA <= 0) {
        return nullptr;
    }

    int i = 0;
    LinkNode* tmp = list;
    for (; i < lenA; ++i) {
        if (tmp != nullptr) {
            tmp = tmp->next;
        }
    }

    return (i == lenA) ? tmp : nullptr;
}

///释放空间
int freeMalloc(Linklist list, LinkNode* ringstart) {
    bool is_ringstart_free = false; //环起始点只能被释放空间一次
    LinkNode *nextnode = nullptr;

    while (list != nullptr) {
        nextnode = list->next;
        if (list == ringstart) { //如果是环起始点
            if (is_ringstart_free)
                break; //如果第二次遇到环起始点addr, 表示已经释放完成
            else
                is_ringstart_free = true; //记录已经释放一次
        }
        free(list);
        list = nextnode;
    }

    return 0;
}

int main() {
    Linklist list = nullptr;
    LinkNode *ringMeetNode = nullptr;
    LinkNode *ringStartNode = nullptr;

    int LenA = 0;
    int RingLength = 0;

    list = createList();
    ringMeetNode = judgeRing(list); //快慢指针相遇点

    if (ringMeetNode == nullptr)
        printf("No Ring\n");
    else {
        printf("Have Ring\n");
        RingLength = getRingLength(ringMeetNode);   //环长
        LenA = getLenA(list, ringMeetNode);

        printf("RingLength:%d\n", RingLength);
        printf("LenA:%d\n", LenA);
        printf("listLength=%d\n", RingLength + LenA);
    }

    ringStartNode = RingStart(list, LenA);  //获取环起始点
    freeMalloc(list, ringStartNode);  //释放环节点, 有环时需要注意. 采纳5楼建议
    return 0;
}

#endif
