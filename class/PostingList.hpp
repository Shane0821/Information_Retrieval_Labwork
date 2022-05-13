#pragma once
#include <bits/stdc++.h>

#include "./ListNode.hpp"
using namespace std;

/**
 * encoding: utf-8
 */

class PostingList {
   public:
    // 链表头
    ListNode* head;
    // 链表尾
    ListNode* tail;
    // 文件总数
    int cntFile;
    // 总词频
    int totalFreq;

   public:
    // 构造函数
    PostingList();
    // 析构函数
    ~PostingList();
    /**
     * 在链表尾部插入一个节点
     * @param x 待插入的节点的指针
     */
    void insert(ListNode* x);

    /**
     * @return 和 list 等价的 vector
     */
    vector<pair<int, double>> toVector();

    // 初始化所有节点权重，注意若插入新节点需要重新初始化
    void initWeight(const int& n);
};

PostingList::PostingList() {
    head = tail = nullptr;
    cntFile = totalFreq = 0;
}

PostingList::~PostingList() {
    // 从前往后遍历删除所有节点
    while (head) {
        auto nxt = head->nxt;
        delete head;
        head = nxt;
    }
}

void PostingList::insert(ListNode* x) {
    if (!head) {
        head = tail = x;
    } else {
        tail->setNext(x);
        tail = x;
    }
    // 累加文件数
    cntFile++;
    // 累加词频
    totalFreq += x->freq;
}

vector<pair<int, double>> PostingList::toVector() {
    auto p = head;
    vector<pair<int, double>> ans;
    while (p) {
        ans.push_back({p->fileId, p->weight});
        p = p->nxt;
    }

    auto cmp = [&](const pair<int, double>& a,
                   const pair<int, double>& b) -> bool {
        if (a.second == b.second) return a.first < b.first;
        return a.second > b.second;
    };

    sort(ans.begin(), ans.end(), cmp);
    return ans;
}

void PostingList::initWeight(const int& n) {
    double idf = log10(1.0 * n / cntFile);
    auto p = head;
    while (p) {
        p->weight = idf * p->freq;
        p = p->nxt;
    }
}