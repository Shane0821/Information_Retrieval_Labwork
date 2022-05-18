#pragma once
#include <bits/stdc++.h>

#include "./ListNode.hpp"
using namespace std;

/**
 * encoding: utf-8
 */

class PostingList {
   public:
    // 存储链表节点
    vector<ListNode> vlist;
    // 文件总数
    int cntFile;
    // 总词频
    int totalFreq;
    //
    double idf;

   public:
    // 构造函数
    PostingList();
    // 析构函数
    ~PostingList();
    /**
     * 在链表尾部插入一个节点
     * @param x 待插入的节点的指针
     */
    void insert(const ListNode x);

    /**
     * @return 和 list 等价的 vector
     */
    vector<pair<int, double>> toVector();

    // 初始化所有节点权重，注意若插入新节点需要重新初始化
    void initWeight(const int& n);
    // 对 vlist 排序
    void sort();
};

PostingList::PostingList() {
    cntFile = totalFreq = 0;
    idf = 0;
}

PostingList::~PostingList() {}

void PostingList::insert(const ListNode x) {
    vlist.push_back(x);
    // 累加文件数
    cntFile++;
    // 累加词频
    totalFreq += x.freq;
}

void PostingList::initWeight(const int& n) {
    idf = log10(1.0 * n / cntFile);
    for (auto& node : vlist) node.weight = idf * node.freq;
}

void PostingList::sort() { std::sort(vlist.begin(), vlist.end()); }