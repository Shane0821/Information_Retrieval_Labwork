#pragma once
#include <bits/stdc++.h>

#include "./ListNode.hpp"
#include "./PostingList.hpp"

using namespace std;

/**
 * encoding: GBK
 */

class InvertedIndex {
   public:
    // body 中字符索引
    unordered_map<string, PostingList*> dictBody;
    // title 中字符索引
    unordered_map<string, PostingList*> dictTitle;

    // 标题权重
    static double gTitle;
    // 内容权重
    static double gBody;
    // database 中文档总数
    static int n;

   public:
    /**
     * @param a PostingList 1
     * @param b PostingList 2
     * @return PostingList1 与 PostingList2 的交集
     */
    vector<ListNode> intersect2(vector<ListNode>& a, vector<ListNode>& b);

    /**
     * @param queries 查询 PostingList 集合
     * @return fileId 交集
     */
    vector<ListNode> intersectMulti(vector<vector<ListNode>>& queries);

    /**
     * @param a PostingList 1
     * @param b PostingList 2
     * @return PostingList1 与 PostingList2 的并集
     */
    vector<ListNode> union2(vector<ListNode>& a, vector<ListNode>& b);

    /**
     * @param queries 查询 PostingList 集合
     * @return fileId 并集
     */
    vector<ListNode> unionMulti(vector<vector<ListNode>>& queries);

    /**
     * @param a 关键字 1
     * @param b 关键字 2
     * @return fileId 差集
     */
    vector<ListNode> minus2(vector<ListNode> a, vector<ListNode> b);

   public:
    // 构造函数
    InvertedIndex() = default;
    // 析构函数
    ~InvertedIndex();
    // 加载数据
    void loadFromDataset();

    /**
     * @param a body 中查询得到的 fileId 集合
     * @param b title 中查询得到的 fileId 集合
     * @return 两个 fileId 集合的加权合并
     */
    vector<ListNode> zoneScore(const vector<ListNode>& vBody,
                               const vector<ListNode>& vTitle);

    /**
     * @param queries 查询字符集合
     * @param type 查询区域
     * @return fileId 交集
     */
    vector<ListNode> intersectMulti(vector<string>& queries,
                                    string type = "body");

    /**
     * @param queries 查询字符集合
     * @param type 查询区域
     * @return fileId 并集
     */
    vector<ListNode> unionMulti(vector<string>& queries, string type = "body");

    /**
     * @param a 查询字符 1
     * @param b 查询字符 2
     * @param type 查询区域
     * @return fileId 差集
     */
    vector<ListNode> minus2(const string a, const string b,
                            string type = "body");

    vector<ListNode> minus2(vector<ListNode> a, string b, string type = "body");
};
double InvertedIndex::gBody = 0.7;
double InvertedIndex::gTitle = 0.3;
int InvertedIndex::n = 265;

InvertedIndex::~InvertedIndex() {
    for (auto& p : dictBody) delete p.second;
}

void InvertedIndex::loadFromDataset() {
    for (int i = 1; i <= n; i++) {
        // generate file name
        stringstream ss;

        ss << "./dataset/" << i << ".txt";
        string filename = ss.str();

        // load data from file
        fstream fs(filename);
        // fs.imbue(loc);
        if (!fs) {
            cerr << "file could not open!\n";
            return;
        }
        string title;
        string author;
        string content;
        fs >> title >> author >> content;
        fs.close();

        // caculate freq of each char in body
        unordered_map<string, int> freqBody;
        for (int i = 0; i < content.size(); i += 2) {
            auto lex = content.substr(i, 2);
            if (lex == "。" || lex == "，" || lex == "：" || lex == "；" ||
                lex == "！" || lex == "？" || lex == "《" || lex == "》" ||
                lex == "”" || lex == "“" || lex == "\n" || lex == " ")
                continue;
            freqBody[lex]++;
        }
        // append to dictBody
        for (auto& pii : freqBody) {
            // 若索引中不存在这个关键字，则添加到索引中
            // 索引指向一个 PostingList
            if (!dictBody[pii.first]) dictBody[pii.first] = new PostingList;
            // PostingList 中插入节点
            dictBody[pii.first]->insert(ListNode(i, 0, pii.second));
        }

        // caculate freq of each char in title
        unordered_map<string, int> freqTitle;
        for (int i = 0; i < title.size(); i += 2) {
            auto lex = title.substr(i, 2);
            if (lex == "。" || lex == "，" || lex == "：" || lex == "；" ||
                lex == "！" || lex == "？" || lex == "《" || lex == "》" ||
                lex == "”" || lex == "“" || lex == "\n" || lex == " ")
                continue;
            freqTitle[lex]++;
        }
        // append to dictTitle
        for (auto& pii : freqTitle) {
            // 若索引中不存在这个关键字，则添加到索引中
            // 索引指向一个 PostingList
            if (!dictTitle[pii.first]) dictTitle[pii.first] = new PostingList;
            // PostingList 中插入节点
            dictTitle[pii.first]->insert(ListNode(i, 0, pii.second));
        }
    }
    cout << "Dataset loaded successfully!\n";

    // 初始化文件权重
    for (auto& p : dictBody) p.second->initWeight(n);
    for (auto& p : dictTitle) p.second->initWeight(n);
    cout << "Weight initialized.\n";
}

vector<ListNode> InvertedIndex::zoneScore(const vector<ListNode>& vBody,
                                          const vector<ListNode>& vTitle) {
    int n = vBody.size(), m = vTitle.size();
    int i = 0, j = 0;
    vector<ListNode> ans;
    while (i < n || j < m) {
        if (j == m) {  // 仅在body 中出现
            ans.push_back(ListNode(vBody[i].fileId, gBody * vBody[i].weight));
            i++;
        } else if (i == n) {  // 仅在title 中出现
            ans.push_back(
                ListNode(vTitle[j].fileId, gTitle * vTitle[j].weight));
            j++;
        } else if (vBody[i].fileId ==
                   vTitle[j].fileId) {  // title + body 中出现
            ans.push_back(
                ListNode(vBody[i].fileId,
                         gBody * vBody[i].weight + gTitle * vTitle[j].weight));
            i++;
            j++;
        } else if (vBody[i].fileId < vBody[j].fileId) {  // 仅在body 中出现
            ans.push_back(ListNode(vBody[i].fileId, gBody * vBody[i].weight));
            i++;
        } else {  // 仅在title 中出现
            ans.push_back(
                ListNode(vTitle[j].fileId, gTitle * vTitle[j].weight));
            j++;
        }
    }
    return ans;
}

vector<ListNode> InvertedIndex::intersect2(vector<ListNode>& a,
                                           vector<ListNode>& b) {
    int pa = 0, pb = 0;

    vector<ListNode> ans;
    // pa 和 pb 只要都不为空就可以取交集
    while (pa < a.size() && pb < b.size()) {
        if (a[pa].fileId == b[pb].fileId) {  // 文件 id 相同，加入交集
            ans.push_back(ListNode(a[pa].fileId, a[pa].weight + b[pb].weight,
                                   a[pa].freq + b[pb].freq));
            pa++, pb++;
        } else if (a[pa].fileId <
                   b[pb].fileId) {  // 文件 id 不同，不加入交集，pa 移动
            pa++;
        } else {  // 文件 id 不同，不加入交集，pb 移动
            pb++;
        }
    }
    return ans;
}

vector<ListNode> InvertedIndex::intersectMulti(
    vector<vector<ListNode>>& queries) {
    for (auto& p : queries)
        if (!p.size()) return {};
    // 按长度从小到大排序
    auto cmp = [&](const vector<ListNode>& a,
                   const vector<ListNode>& b) -> bool {
        return a.size() < b.size();
    };
    sort(queries.begin(), queries.end(), cmp);

    vector<ListNode> resultList = queries[0];
    // 排序后每次合并的一定是最短的两个
    for (int i = 1; i < queries.size(); i++)
        resultList = intersect2(resultList, queries[i]);

    return resultList;
}

vector<ListNode> InvertedIndex::intersectMulti(vector<string>& queries,
                                               string type) {
    if (type != "body" && type != "title") return {};
    vector<vector<ListNode>> vec;
    for (auto& c : queries) {
        // 有一个为空则结果为空
        if (type == "body") {
            if (!dictBody[c]) return {};
            vec.push_back(dictBody[c]->vlist);
        }
        if (type == "title") {
            if (!dictTitle[c]) return {};
            vec.push_back(dictTitle[c]->vlist);
        }
    }
    return intersectMulti(vec);
}

vector<ListNode> InvertedIndex::union2(vector<ListNode>& a,
                                       vector<ListNode>& b) {
    int pa = 0, pb = 0;

    vector<ListNode> ans;
    // pa 和 pb 只要有一个不为空就可以取并
    while (pa < a.size() || pb < b.size()) {
        if (pa == a.size()) {
            ans.push_back(ListNode(b[pb].fileId, b[pb].weight, b[pb].freq));
            pb++;
        } else if (pb == b.size()) {
            ans.push_back(ListNode(a[pa].fileId, a[pa].weight, a[pa].freq));
            pa++;
        } else if (a[pa].fileId ==
                   b[pb].fileId) {  // 文件 id 相同，取 1 个加入并集
            ans.push_back(ListNode(a[pa].fileId, a[pa].weight + b[pb].weight,
                                   a[pa].freq + b[pb].freq));
            pa++, pb++;
        } else if (a[pa].fileId < b[pb].fileId) {  // pa 的文件 id 小，加入并集
            ans.push_back(ListNode(a[pa].fileId, a[pa].weight, a[pa].freq));
            pa++;
        } else {  // pb 的文件 id 小，加入并集
            ans.push_back(ListNode(b[pb].fileId, b[pb].weight, b[pb].freq));
            pb++;
        }
    }
    return ans;
}

vector<ListNode> InvertedIndex::unionMulti(vector<vector<ListNode>>& queries) {
    if (!queries.size()) return {};
#define piip tuple<int, int, vector<ListNode>>
    // 长度，是否是原本存在的 List，List 指针
    priority_queue<piip, vector<piip>, greater<piip>> q;
#undef piip

    for (auto& p : queries) q.push({p.size(), 1, p});
    while (q.size() >= 2) {
        // 取最短的两个合并
        auto tp1 = q.top();
        q.pop();
        auto tp2 = q.top();
        q.pop();

        auto merged = union2(get<2>(tp1), get<2>(tp2));
        q.push({merged.size(), 0, merged});
    }
    auto resultList = get<2>(q.top());

    return resultList;
}

vector<ListNode> InvertedIndex::unionMulti(vector<string>& queries,
                                           string type) {
    if (type != "body" && type != "title") return {};
    vector<vector<ListNode>> vec;
    for (auto& c : queries) {
        if (type == "body" && dictBody[c]) vec.push_back(dictBody[c]->vlist);
        if (type == "title" && dictTitle[c]) vec.push_back(dictTitle[c]->vlist);
    }
    return unionMulti(vec);
}

vector<ListNode> InvertedIndex::minus2(vector<ListNode> a, vector<ListNode> b) {
    if (!a.size()) return {};
    if (!b.size()) return a;

    int pa = 0, pb = 0;

    vector<ListNode> resultList;
    // pa 不为空才做差
    while (pa < a.size()) {
        if (pb == b.size()) {
            resultList.push_back(
                ListNode(a[pa].fileId, a[pa].weight, a[pa].freq));
            pa++;
        } else if (a[pa].fileId == b[pb].fileId) {  // 文件 id 相同，不在差集中
            pa++, pb++;
        } else if (a[pa].fileId < b[pb].fileId) {  // pa 的文件 id 小，加入差集
            resultList.push_back(
                ListNode(a[pa].fileId, a[pa].weight, a[pa].freq));
            pa++;
        } else {  // pb 的文件 id 小，不影响差集，跳过
            pb++;
        }
    }

    return resultList;
}

vector<ListNode> InvertedIndex::minus2(const string a, const string b,
                                       string type) {
    if (type != "body" && type != "title") return {};
    return type == "body"
               ? minus2(dictBody[a] ? dictBody[a]->vlist : vector<ListNode>(),
                        dictBody[b] ? dictBody[b]->vlist : vector<ListNode>())
               : minus2(
                     dictTitle[a] ? dictTitle[a]->vlist : vector<ListNode>(),
                     dictTitle[b] ? dictTitle[b]->vlist : vector<ListNode>());
}

vector<ListNode> InvertedIndex::minus2(vector<ListNode> a, string b,
                                       string type) {
    if (type != "body" && type != "title") return {};
    return type == "body" ? minus2(a, dictBody[b] ? dictBody[b]->vlist
                                                  : vector<ListNode>())
                          : minus2(a, dictTitle[b] ? dictTitle[b]->vlist
                                                   : vector<ListNode>());
}