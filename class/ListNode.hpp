#pragma once

/**
 * encoding: utf-8
 */

class ListNode {
   public:
    // 下一个节点
    ListNode* nxt;
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

    /**
     * 设置当前节点的下一个节点
     * @param x nxt 节点指针
     */
    void setNext(ListNode* x);
};

ListNode::ListNode(const int& fileId, const double weight, const int freq) {
    this->fileId = fileId;
    this->freq = freq;
    this->weight = weight;
    nxt = nullptr;
}

void ListNode::setNext(ListNode* x) { this->nxt = x; }