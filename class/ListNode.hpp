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
    // tf-idf
    double tf_idf;
    // 归一化权重
    double wf;
    /**
     * 构造函数
     * @param fileId 文件编号
     * @param tf_idf 权重
     * @param freq 词频
     */
    ListNode(const int& fileId, const double tf_idf = 0, const int freq = 0, const int wf = 0);

    friend bool operator<(const ListNode& a, const ListNode& b);
};

ListNode::ListNode(const int& fileId, const double tf_idf, const int freq, const int wf) {
    this->fileId = fileId;
    this->freq = freq;
    this->tf_idf = tf_idf;
    this->wf = wf;
}

bool operator<(const ListNode& a, const ListNode& b) {
    if (a.tf_idf == b.tf_idf) return a.fileId < b.fileId;
    return a.tf_idf > b.tf_idf;
}