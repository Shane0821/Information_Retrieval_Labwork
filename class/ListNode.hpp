#pragma once

/**
 * encoding: utf-8
 */

class ListNode {
   public:
    // 文件编号
    int fileId;
    // 词频
    int freq;
    // 权重
    double weight;
    /**
     * 构造函数
     * @param fileId 文件编号
     * @param weight 权重
     * @param freq 词频
     */
    ListNode(const int& fileId, const double weight = 0, const int freq = 0);

    friend bool operator<(const ListNode& a, const ListNode& b);
};

ListNode::ListNode(const int& fileId, const double weight, const int freq) {
    this->fileId = fileId;
    this->freq = freq;
    this->weight = weight;
}

bool operator<(const ListNode& a, const ListNode& b) {
    if (a.weight == b.weight) return a.fileId < b.fileId;
    return a.weight > b.weight;
}