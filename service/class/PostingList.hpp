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
    // 按节点 tf 降序排列
    vector<ListNode> vlist2;
    // 文件总数
    int cntFile;
    // 总词频
    int totalFreq;
    // 逆文档频率
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

    // 对 vlist2 按 tf 从高到低排序
    void sorList2();

    // 找到编号为 d 的文档的词频，若该文档不存在返回 0
    int getFreqOfDoc(int id);
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
    totalFreq += x.tf;
}

void PostingList::initWeight(const int& n) {
    idf = log2(1.0 * n / cntFile);
    for (auto& node : vlist) node.tf_idf = idf * node.tf;
}

void PostingList::sorList2() {
    vlist2 = vlist;

    auto cmp = [&](const ListNode& a, const ListNode& b) -> bool {
        return a.tf > b.tf;
        // return a.fileLen > b.fileLen;
    };

    std::sort(vlist2.begin(), vlist2.end(), cmp);
}
// void PostingList::sort() { std::sort(vlist.begin(), vlist.end()); }

int PostingList::getFreqOfDoc(int id) {
    // 根据 id 二分查找
    int l = 0, r = (int)vlist.size() - 1;
    while (l <= r) {
        int mid = l + r >> 1;
        if (vlist[mid].fileId == id) return vlist[mid].tf;
        if (vlist[mid].fileId < id)
            l = mid + 1;
        else
            r = mid - 1;
    }
    return 0;
}