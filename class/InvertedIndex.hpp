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

   private:
    /**
     * @param a PostingList 1
     * @param b PostingList 2
     * @return PostingList1 与 PostingList2 的交集
     */
    PostingList* intersect2(PostingList* a, PostingList* b);

    /**
     * @param queries 查询 PostingList 集合
     * @return fileId 交集
     */
    vector<ListNode> intersectMulti(vector<PostingList*>& queries);

    /**
     * @param a PostingList 1
     * @param b PostingList 2
     * @return PostingList1 与 PostingList2 的并集
     */
    PostingList* union2(PostingList* a, PostingList* b);

    /**
     * @param queries 查询 PostingList 集合
     * @return fileId 并集
     */
    vector<ListNode> unionMulti(vector<PostingList*>& queries);

    /**
     * @param a 关键字 1
     * @param b 关键字 2
     * @return fileId 差集
     */
    vector<ListNode> minus2(PostingList* a, PostingList* b);

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
    vector<ListNode> intersectMulti(vector<string> queries,
                                    string type = "body");

    /**
     * @param queries 查询字符集合
     * @param type 查询区域
     * @return fileId 并集
     */
    vector<ListNode> unionMulti(vector<string> queries, string type = "body");

    /**
     * @param a 查询字符 1
     * @param b 查询字符 2
     * @param type 查询区域
     * @return fileId 差集
     */
    vector<ListNode> minus2(const string a, const string b,
                            string type = "body");
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
            ans.push_back({vBody[i].fileId, gBody * vBody[i].weight});
            i++;
        } else if (i == n) {  // 仅在title 中出现
            ans.push_back({vTitle[j].fileId, gTitle * vTitle[j].weight});
            j++;
        } else if (vBody[i].fileId ==
                   vTitle[j].fileId) {  // title + body 中出现
            ans.push_back({vBody[i].fileId, gBody * vBody[i].weight +
                                                gTitle * vTitle[j].weight});
            i++;
            j++;
        } else if (vBody[i].fileId < vBody[j].fileId) {  // 仅在body 中出现
            ans.push_back({vBody[i].fileId, gBody * vBody[i].weight});
            i++;
        } else {  // 仅在title 中出现
            ans.push_back({vTitle[j].fileId, gTitle * vTitle[j].weight});
            j++;
        }
    }

    sort(ans.begin(), ans.end());
    return ans;
}

PostingList* InvertedIndex::intersect2(PostingList* a, PostingList* b) {
    auto va = a->vlist, vb = b->vlist;
    int pa = 0, pb = 0;

    PostingList* ans = new PostingList;
    // pa 和 pb 只要都不为空就可以取交集
    while (pa < va.size() && pb < vb.size()) {
        if (va[pa].fileId == vb[pb].fileId) {  // 文件 id 相同，加入交集
            ans->insert(ListNode(va[pa].fileId, va[pa].weight + vb[pb].weight,
                                 va[pa].freq + vb[pb].freq));
            pa++, pb++;
        } else if (va[pa].fileId <
                   vb[pb].fileId) {  // 文件 id 不同，不加入交集，pa 移动
            pa++;
        } else {  // 文件 id 不同，不加入交集，pb 移动
            pb++;
        }
    }
    return ans;
}

vector<ListNode> InvertedIndex::intersectMulti(vector<PostingList*>& queries) {
    if (!queries.size()) return {};
    // 有一个空则交集为空
    for (auto& p : queries)
        if (!p) return {};
    // 按长度从小到大排序
    auto cmp = [&](const PostingList* a, const PostingList* b) -> bool {
        return a->cntFile < b->cntFile;
    };
    sort(queries.begin(), queries.end(), cmp);

    PostingList* resultList = queries[0];
    // 排序后每次合并的一定是最短的两个
    for (int i = 1; i < queries.size(); i++) {
        auto tmp = resultList;
        resultList = intersect2(resultList, queries[i]);
        if (tmp != queries[0]) delete tmp;
    }

    resultList->sort();
    auto ans = resultList->vlist;
    if (resultList != queries[0]) delete resultList;
    return ans;
}

vector<ListNode> InvertedIndex::intersectMulti(vector<string> queries,
                                               string type) {
    if (type != "body" && type != "title") return {};
    vector<PostingList*> vPL;
    for (auto& c : queries) {
        // 空集要加入intersect
        if (type == "body") vPL.push_back(dictBody[c]);
        if (type == "title") vPL.push_back(dictTitle[c]);
    }
    return intersectMulti(vPL);
}

PostingList* InvertedIndex::union2(PostingList* a, PostingList* b) {
    auto va = a->vlist, vb = b->vlist;
    int pa = 0, pb = 0;

    PostingList* ans = new PostingList;
    // pa 和 pb 只要有一个不为空就可以取并
    while (pa < va.size() || pb < vb.size()) {
        if (pa == va.size()) {
            ans->insert(ListNode(vb[pb].fileId, vb[pb].weight, vb[pb].freq));
            pb++;
        } else if (pb == vb.size()) {
            ans->insert(ListNode(va[pa].fileId, va[pa].weight, va[pa].freq));
            pa++;
        } else if (va[pa].fileId ==
                   vb[pb].fileId) {  // 文件 id 相同，取 1 个加入并集
            ans->insert(ListNode(va[pa].fileId, va[pa].weight + vb[pb].weight,
                                 va[pa].freq + vb[pb].freq));
            pa++, pb++;
        } else if (va[pa].fileId <
                   vb[pb].fileId) {  // pa 的文件 id 小，加入并集
            ans->insert(ListNode(va[pa].fileId, va[pa].weight, va[pa].freq));
            pa++;
        } else {  // pb 的文件 id 小，加入并集
            ans->insert(ListNode(vb[pb].fileId, vb[pb].weight, vb[pb].freq));
            pb++;
        }
    }
    return ans;
}

vector<ListNode> InvertedIndex::unionMulti(vector<PostingList*>& queries) {
    if (!queries.size()) return {};
#define piip tuple<int, int, PostingList*>
    // 长度，是否是原本存在的 List，List 指针
    priority_queue<piip, vector<piip>, greater<piip>> q;
#undef piip

    for (auto& p : queries) q.push({p->cntFile, 1, p});
    while (q.size() >= 2) {
        // 取最短的两个合并
        auto tp1 = q.top();
        q.pop();
        auto tp2 = q.top();
        q.pop();

        auto merged = union2(get<2>(tp1), get<2>(tp2));
        q.push({merged->cntFile, 0, merged});

        // 不是原本存在的删除，防止内存泄漏
        if (!get<1>(tp1)) delete get<2>(tp1);
        if (!get<1>(tp2)) delete get<2>(tp2);
    }
    auto resultList = get<2>(q.top());

    resultList->sort();
    auto ans = resultList->vlist;
    if (!get<1>(q.top())) delete resultList;
    return ans;
}

vector<ListNode> InvertedIndex::unionMulti(vector<string> queries,
                                           string type) {
    if (type != "body" && type != "title") return {};
    vector<PostingList*> vPL;
    for (auto& c : queries) {
        if (type == "body" && dictBody[c]) vPL.push_back(dictBody[c]);
        if (type == "title" && dictTitle[c]) vPL.push_back(dictTitle[c]);
    }
    return unionMulti(vPL);
}

vector<ListNode> InvertedIndex::minus2(PostingList* a, PostingList* b) {
    if (!a) return {};
    if (!b) return a->vlist;

    auto va = a->vlist, vb = b->vlist;
    int pa = 0, pb = 0;

    PostingList* resultList = new PostingList;
    // pa 不为空才做差
    while (pa < va.size()) {
        if (pb == vb.size()) {
            resultList->insert(
                ListNode(va[pa].fileId, va[pa].weight, va[pa].freq));
            pa++;
        } else if (va[pa].fileId ==
                   vb[pb].fileId) {  // 文件 id 相同，不在差集中
            pa++, pb++;
        } else if (va[pa].fileId <
                   vb[pb].fileId) {  // pa 的文件 id 小，加入差集
            resultList->insert(
                ListNode(va[pa].fileId, va[pa].weight, va[pa].freq));
            pa++;
        } else {  // pb 的文件 id 小，不影响差集，跳过
            pb++;
        }
    }

    resultList->sort();
    auto ans = resultList->vlist;
    delete resultList;
    return ans;
}

vector<ListNode> InvertedIndex::minus2(const string a, const string b,
                                       string type) {
    if (type != "body" && type != "title") return {};
    return type == "body" ? minus2(dictBody[a], dictBody[b])
                          : minus2(dictTitle[a], dictTitle[b]);
}