#pragma once
#include <bits/stdc++.h>

#include "./ListNode.hpp"
#include "./PostingList.hpp"

using namespace std;

// 文档总数
const static int n = 265;

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
    // 各个文件内容的长度
    int contentLength[n + 1];
    // cf 的和
    static int cs;

   private:
    /**
     * @param a 文档向量1
     * @param b 文档向量2
     * @return 文档向量1与文档向量2的交集
     */
    vector<ListNode> intersect2(vector<ListNode>& a, vector<ListNode>& b);

    /**
     * @param queries 查询文档向量集合
     * @return 文档向量交集
     */
    vector<ListNode> intersectMulti(vector<vector<ListNode>>& queries);

    /**
     * @param a 文档向量1
     * @param b 文档向量2
     * @return 文档向量1与文档向量2的并集
     */
    vector<ListNode> union2(vector<ListNode>& a, vector<ListNode>& b);

    /**
     * @param queries 查询文档向量集合
     * @return 文档向量并集
     */
    vector<ListNode> unionMulti(vector<vector<ListNode>>& queries);

    /**
     * @param a 文档向量1
     * @param b 文档向量2
     * @return 文档向量差集
     */
    vector<ListNode> minus2(vector<ListNode> a, vector<ListNode> b);

    /**
     * @param a body 中查询得到的文档向量
     * @param b title 中查询得到的文档向量
     * @param type type=0 表示不带权，type=1 表示带权
     * @return 两个文档向量的加权合并
     */
    vector<ListNode> zoneScore(const vector<ListNode>& vBody,
                               const vector<ListNode>& vTitle, bool type = 0);

    /**
     * @param queries 查询字符集合
     * @param type 查询区域
     * @return 文档向量交集
     */
    vector<ListNode> intersectMulti(vector<string>& queries,
                                    string type = "body");

    /**
     * @param queries 查询字符集合
     * @param type 查询区域
     * @return 文档向量并集
     */
    vector<ListNode> unionMulti(vector<string>& queries, string type = "body");

    /**
     * @param a 查询字符1
     * @param b 查询字符2
     * @param type 查询区域
     * @return 文档向量差集
     */
    vector<ListNode> minus2(const string a, const string b,
                            string type = "body");

    /**
     * @param a 文档向量
     * @param b 查询字符
     * @param type 查询区域
     * @return 文档向量差集
     */
    vector<ListNode> minus2(vector<ListNode> a, string b, string type = "body");

    /**
     * @param query 查询涉及的 PostingList
     * @param K 需要返回的文档数
     * @return 满足要求的文档得分和编号的集合
     */
    vector<pair<double, int>> fastCosineScore(vector<PostingList*> query,
                                              int K);

    /**
     * @param query 查询涉及的 PostingList
     * @param K 需要返回的文档数
     * @return 满足要求的文档得分和编号的集合
     */
    vector<pair<double, int>> heuristicTopK(vector<PostingList*> query, int K);

   public:
    // 构造函数
    InvertedIndex() = default;
    // 析构函数
    ~InvertedIndex();
    // 加载数据
    void loadFromDataset();

    // 向量查询
    void vectorQuery(string s);
    // 布尔查询
    void boolQuery(string s);
    // 语言模型
    void languageModel(string s);
};
double InvertedIndex::gBody = 0.7;
double InvertedIndex::gTitle = 0.3;
int InvertedIndex::cs = 0;

InvertedIndex::~InvertedIndex() {
    for (auto& p : dictBody) delete p.second;
}

void InvertedIndex::loadFromDataset() {
    for (int d = 1; d <= n; d++) {
        contentLength[d] = 0;
        // generate file name
        stringstream ss;

        ss << "./dataset/" << d << ".txt";
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

        // caculate tf of each char in body
        unordered_map<string, int> freqBody;
        int maxFreqBody = 0;
        for (int i = 0; i < content.size(); i += 3) {
            auto lex = content.substr(i, 3);
            if (lex == "。" || lex == "，" || lex == "：" || lex == "；" ||
                lex == "！" || lex == "？" || lex == "《" || lex == "》" ||
                lex == "”" || lex == "“" || lex == "\n" || lex == " ")
                continue;
            freqBody[lex]++;
            maxFreqBody = max(maxFreqBody, freqBody[lex]);
            contentLength[d]++;
        }
        // append to dictBody
        for (auto& pii : freqBody) {
            // 若索引中不存在这个关键字，则添加到索引中
            // 索引指向一个 PostingList
            if (!dictBody[pii.first]) dictBody[pii.first] = new PostingList;
            // PostingList 中插入节点
            dictBody[pii.first]->insert(ListNode(d, 0, pii.second,
                                                 1.0 * pii.second / maxFreqBody,

                                                 contentLength[d]));
        }
        cs += contentLength[d];

        // caculate tf of each char in title
        unordered_map<string, int> freqTitle;
        for (int i = 0; i < title.size(); i += 3) {
            auto lex = title.substr(i, 3);
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
            dictTitle[pii.first]->insert(ListNode(d, 0, pii.second));
        }
    }
    cout << "Dataset loaded successfully!\n";

    // 初始化文件权重
    for (auto& p : dictBody) p.second->initWeight(n), p.second->sorList2();
    for (auto& p : dictTitle) p.second->initWeight(n);
    cout << "Weight initialized.\n";
}

vector<ListNode> InvertedIndex::zoneScore(const vector<ListNode>& vBody,
                                          const vector<ListNode>& vTitle,
                                          bool type) {
    int n = vBody.size(), m = vTitle.size();
    int i = 0, j = 0;
    vector<ListNode> ans;
    while (i < n || j < m) {
        if (j == m) {  // 仅在body 中出现
            ans.push_back(ListNode(vBody[i].fileId,
                                   gBody * (type ? vBody[i].tf_idf : 1)));
            i++;
        } else if (i == n) {  // 仅在title 中出现
            ans.push_back(ListNode(vTitle[j].fileId,
                                   gTitle * (type ? vTitle[j].tf_idf : 1)));
            j++;
        } else if (vBody[i].fileId ==
                   vTitle[j].fileId) {  // title + body 中出现
            ans.push_back(ListNode(vBody[i].fileId,
                                   gBody * (type ? vBody[i].tf_idf : 1) +
                                       gTitle * (type ? vTitle[j].tf_idf : 1)));
            i++;
            j++;
        } else if (vBody[i].fileId < vBody[j].fileId) {  // 仅在body 中出现
            ans.push_back(ListNode(vBody[i].fileId,
                                   gBody * (type ? vBody[i].tf_idf : 1)));
            i++;
        } else {  // 仅在title 中出现
            ans.push_back(ListNode(vTitle[j].fileId,
                                   gTitle * (type ? vTitle[j].tf_idf : 1)));
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
            ans.push_back(ListNode(a[pa].fileId, a[pa].tf_idf + b[pb].tf_idf));
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
            ans.push_back(ListNode(b[pb].fileId, b[pb].tf_idf));
            pb++;
        } else if (pb == b.size()) {
            ans.push_back(ListNode(a[pa].fileId, a[pa].tf_idf));
            pa++;
        } else if (a[pa].fileId ==
                   b[pb].fileId) {  // 文件 id 相同，取 1 个加入并集
            ans.push_back(ListNode(a[pa].fileId, a[pa].tf_idf + b[pb].tf_idf));
            pa++, pb++;
        } else if (a[pa].fileId < b[pb].fileId) {  // pa 的文件 id 小，加入并集
            ans.push_back(ListNode(a[pa].fileId, a[pa].tf_idf));
            pa++;
        } else {  // pb 的文件 id 小，加入并集
            ans.push_back(ListNode(b[pb].fileId, b[pb].tf_idf));
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
                ListNode(a[pa].fileId, a[pa].tf_idf, a[pa].tf));
            pa++;
        } else if (a[pa].fileId == b[pb].fileId) {  // 文件 id 相同，不在差集中
            pa++, pb++;
        } else if (a[pa].fileId < b[pb].fileId) {  // pa 的文件 id 小，加入差集
            resultList.push_back(
                ListNode(a[pa].fileId, a[pa].tf_idf, a[pa].tf));
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

vector<pair<double, int>> InvertedIndex::fastCosineScore(
    vector<PostingList*> query, int K) {
    vector<double> score(n + 1);
    for (auto& pList : query) {
        for (auto& node : pList->vlist) {
            score[node.fileId] += node.wf;
        }
    }
    for (int i = 1; i <= n; i++) {
        assert(contentLength[i] != 0);
        score[i] /= contentLength[i];
    }

    priority_queue<pair<double, int>> q;
    for (int i = 1; i <= n; i++) q.push({score[i], -i});

    vector<pair<double, int>> res;
    for (int i = 1; i <= K; i++) {
        auto tp = q.top();
        q.pop();
        res.push_back({tp.first, -tp.second});
    }

    return res;
}

vector<pair<double, int>> InvertedIndex::heuristicTopK(
    vector<PostingList*> query, int K) {
    auto cmp = [&](const PostingList* a, const PostingList* b) -> bool {
        return a->idf > b->idf;
    };
    sort(query.begin(), query.end(), cmp);

    vector<double> score(n + 1);
    const double eps = 0.5;
    int cnt = 0;
    for (auto& pList : query) {
        for (auto& node : pList->vlist2) {
            if (node.tf_idf < eps) break;
            cnt++;
            score[node.fileId] += node.tf_idf;
        }
    }
    cout << cnt << endl;

    for (int i = 1; i <= n; i++) {
        assert(contentLength[i] != 0);
        score[i] /= contentLength[i];
    }

    priority_queue<pair<double, int>> q;
    for (int i = 1; i <= n; i++) q.push({score[i], -i});

    vector<pair<double, int>> res;
    for (int i = 1; i <= K; i++) {
        auto tp = q.top();
        q.pop();
        res.push_back({tp.first, -tp.second});
    }

    return res;
}

void InvertedIndex::vectorQuery(string s) {
    vector<PostingList*> query;
    for (int i = 0; i < s.size(); i += 3) {
        auto tmp = s.substr(i, 3);
        if (dictBody[tmp]) query.push_back(dictBody[tmp]);
    }

    const int K = 20;
    // auto res = fastCosineScore(query, K);
    auto res = heuristicTopK(query, K);

    cout << "查询结果：\n";
    for (auto& p : res) cout << p.second << ": " << p.first << endl;
}

void InvertedIndex::boolQuery(string s) {
    string lastopt = "";
    vector<ListNode> ansBody;
    vector<ListNode> ansTitle;
    vector<string> query;
    for (int i = 0, round = 0; i <= s.size(); i += 3) {
        string curopt = s.substr(i, 3);
        if (curopt == " ") {
            cout << "不要有空格！\n";
            break;
        }
        // 开头为 and 或 or
        if (i == 0 && (curopt != "an" && curopt != "or")) {
            cout << "请以 and 或 or 开头！\n";
            break;
        }
        // 碰到运算符，或者结尾，把之前的字符进行运算加入答案
        if (curopt == "an" || curopt == "or" || curopt == "no" ||
            i == s.size()) {
            if (lastopt != "" && !query.size()) {
                cout << "两运算符之间应该有字符！\n";
                break;
            }
            if (round != 0)
                if (lastopt == "an") {
                    auto vec = intersectMulti(query);
                    auto vec2 = intersectMulti(query, "title");
                    if (round == 1) {
                        ansBody = vec;
                        ansTitle = vec2;
                    } else {
                        ansBody = intersect2(ansBody, vec);
                        ansTitle = intersect2(ansTitle, vec2);
                    }
                } else if (lastopt == "or") {
                    auto vec = unionMulti(query);
                    auto vec2 = unionMulti(query, "title");
                    if (round == 1) {
                        ansBody = vec;
                        ansTitle = vec2;
                    } else {
                        ansBody = union2(ansBody, vec);
                        ansTitle = union2(ansTitle, vec2);
                    }
                } else if (lastopt == "no") {
                    if (query.size() > 1) {
                        cout << "not 后面只能跟一个字符！\n";
                        break;
                    }
                    ansBody = minus2(ansBody, query[0]);
                    ansTitle = minus2(ansTitle, query[0], "title");
                }

            lastopt = curopt;
            query.clear();
            if (curopt == "an" || curopt == "no") i++;
            round++;
        } else {
            query.push_back(curopt);
        }
    }

    string qtype = "";
    cout << "请输入查询位置（body/title/both）: \n";
    while (qtype != "title" && qtype != "body" && qtype != "both") {
        cin >> qtype;
    }

    cout << "查询结果：\n";
    if (qtype == "title") {
        if (!ansTitle.size()) {
            cout << "null\n";
        } else {
            sort(ansTitle.begin(), ansTitle.end());
            for (auto& node : ansTitle)
                cout << node.fileId << ": " << node.tf_idf << endl;
        }

    } else if (qtype == "body") {
        if (!ansBody.size()) {
            cout << "null\n";
        } else {
            sort(ansBody.begin(), ansBody.end());
            for (auto& node : ansBody)
                cout << node.fileId << ": " << node.tf_idf << endl;
        }

    } else {
        auto res = zoneScore(ansBody, ansTitle, 0);
        if (!res.size()) {
            cout << "null\n";
        } else {
            sort(res.begin(), res.end());
            for (auto& node : res)
                cout << node.fileId << ": " << node.tf_idf << endl;
        }
    }
    cout << endl;
}

void InvertedIndex::languageModel(string s) {
    const static double lambda = 0.5;
    // 计算权重
    vector<double> score(n + 1);
    for (int d = 1; d <= n; d++) {
        double tmp = 1;
        for (int i = 0; i < s.size(); i += 3) {
            string cur = s.substr(i, 3);
            if (cur == "\n" || cur == " ") continue;
            assert(contentLength[d] != 0);
            assert(cs != 0);
            tmp *= lambda * dictBody[cur]->getFreqOfDoc(d) / contentLength[d] +
                   (1.0 - lambda) * dictBody[cur]->totalFreq / cs;
        }
        score[d] = tmp;
    }

    // 取 topK
    const static int K = 20;
    priority_queue<pair<double, int>> q;
    for (int i = 1; i <= n; i++) q.push({score[i], -i});

    vector<pair<double, int>> res;
    for (int i = 1; i <= K; i++) {
        auto tp = q.top();
        q.pop();
        res.push_back({tp.first, -tp.second});
    }

    cout << "查询结果：\n";
    for (auto& p : res) cout << p.second << ": " << p.first << endl;
}
