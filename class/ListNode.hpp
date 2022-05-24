#pragma once

/**
 * encoding: utf-8
 */

class ListNode {
   public:
    // 文件编号
    int fileId;
    // 文档总长度
    int fileLen;
    // 词频
    int tf;
    // tf-idf
    double tf_idf;
    // 归一化权重
    double wf;
    /**
     * 构造函数
     * @param fileId 文件编号
     * @param tf_idf 权重
     * @param tf 词频
     * @param wf 归一化权重
     */
    ListNode(const int& fileId, const double tf_idf = 0, const int tf = 0, const int wf = 0, const int fileLen = 0);

    friend bool operator<(const ListNode& a, const ListNode& b);
};

ListNode::ListNode(const int& fileId, const double tf_idf, const int tf, const int wf, const int fileLen) {
    this->fileId = fileId;
    this->tf = tf;
    this->tf_idf = tf_idf;
    this->wf = wf;
    this->fileLen = fileLen;
}

bool operator<(const ListNode& a, const ListNode& b) {
    if (a.tf_idf == b.tf_idf) return a.fileId < b.fileId;
    return a.tf_idf > b.tf_idf;
}