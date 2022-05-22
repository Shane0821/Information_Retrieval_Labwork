#pragma once
#include <bits/stdc++.h>

#include "./ListNode.hpp"
#include "./PostingList.hpp"

using namespace std;

/**
 * encoding: GBK
 */
const static int n = 265;

class InvertedIndex {
   public:
    // body ���ַ�����
    unordered_map<string, PostingList*> dictBody;
    // title ���ַ�����
    unordered_map<string, PostingList*> dictTitle;

    // ����Ȩ��
    static double gTitle;
    // ����Ȩ��
    static double gBody;
    // database ���ĵ�����

    // �����ļ����ݵĳ���
    int contentLength[n + 1];

   private:
    /**
     * @param a PostingList 1
     * @param b PostingList 2
     * @return PostingList1 �� PostingList2 �Ľ���
     */
    vector<ListNode> intersect2(vector<ListNode>& a, vector<ListNode>& b);

    /**
     * @param queries ��ѯ PostingList ����
     * @return fileId ����
     */
    vector<ListNode> intersectMulti(vector<vector<ListNode>>& queries);

    /**
     * @param a PostingList 1
     * @param b PostingList 2
     * @return PostingList1 �� PostingList2 �Ĳ���
     */
    vector<ListNode> union2(vector<ListNode>& a, vector<ListNode>& b);

    /**
     * @param queries ��ѯ PostingList ����
     * @return fileId ����
     */
    vector<ListNode> unionMulti(vector<vector<ListNode>>& queries);

    /**
     * @param a �ؼ��� 1
     * @param b �ؼ��� 2
     * @return fileId �
     */
    vector<ListNode> minus2(vector<ListNode> a, vector<ListNode> b);

    /**
     * @param a body �в�ѯ�õ��� fileId ����
     * @param b title �в�ѯ�õ��� fileId ����
     * @return ���� fileId ���ϵļ�Ȩ�ϲ�
     */
    vector<ListNode> zoneScore(const vector<ListNode>& vBody,
                               const vector<ListNode>& vTitle);

    /**
     * @param queries ��ѯ�ַ�����
     * @param type ��ѯ����
     * @return fileId ����
     */
    vector<ListNode> intersectMulti(vector<string>& queries,
                                    string type = "body");

    /**
     * @param queries ��ѯ�ַ�����
     * @param type ��ѯ����
     * @return fileId ����
     */
    vector<ListNode> unionMulti(vector<string>& queries, string type = "body");

    /**
     * @param a ��ѯ�ַ� 1
     * @param b ��ѯ�ַ� 2
     * @param type ��ѯ����
     * @return fileId �
     */
    vector<ListNode> minus2(const string a, const string b,
                            string type = "body");

    vector<ListNode> minus2(vector<ListNode> a, string b, string type = "body");

   public:
    // ���캯��
    InvertedIndex() = default;
    // ��������
    ~InvertedIndex();
    // ��������
    void loadFromDataset();

    // ������ѯ
    void vectorQuery(string s);
    // ������ѯ
    void boolQuery(string s);
};
double InvertedIndex::gBody = 0.7;
double InvertedIndex::gTitle = 0.3;

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

        // caculate freq of each char in body
        unordered_map<string, int> freqBody;
        int maxFreqBody = 0;
        for (int i = 0; i < content.size(); i += 2) {
            auto lex = content.substr(i, 2);
            if (lex == "��" || lex == "��" || lex == "��" || lex == "��" ||
                lex == "��" || lex == "��" || lex == "��" || lex == "��" ||
                lex == "��" || lex == "��" || lex == "\n" || lex == " ")
                continue;
            freqBody[lex]++;
            maxFreqBody = max(maxFreqBody, freqBody[lex]);
            contentLength[d]++;
        }
        // append to dictBody
        for (auto& pii : freqBody) {
            // �������в���������ؼ��֣�����ӵ�������
            // ����ָ��һ�� PostingList
            if (!dictBody[pii.first]) dictBody[pii.first] = new PostingList;
            // PostingList �в���ڵ�
            dictBody[pii.first]->insert(
                ListNode(d, 0, pii.second, 1.0 * pii.second / maxFreqBody));
        }

        // caculate freq of each char in title
        unordered_map<string, int> freqTitle;
        int maxFreqTitle = 0;
        for (int i = 0; i < title.size(); i += 2) {
            auto lex = title.substr(i, 2);
            if (lex == "��" || lex == "��" || lex == "��" || lex == "��" ||
                lex == "��" || lex == "��" || lex == "��" || lex == "��" ||
                lex == "��" || lex == "��" || lex == "\n" || lex == " ")
                continue;
            freqTitle[lex]++;
            maxFreqTitle = max(maxFreqTitle, freqTitle[lex]);
        }
        // append to dictTitle
        for (auto& pii : freqTitle) {
            // �������в���������ؼ��֣�����ӵ�������
            // ����ָ��һ�� PostingList
            if (!dictTitle[pii.first]) dictTitle[pii.first] = new PostingList;
            // PostingList �в���ڵ�
            dictTitle[pii.first]->insert(
                ListNode(d, 0, pii.second, 1.0 * pii.second / maxFreqTitle));
        }
    }
    cout << "Dataset loaded successfully!\n";

    // ��ʼ���ļ�Ȩ��
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
        if (j == m) {  // ����body �г���
            ans.push_back(ListNode(vBody[i].fileId, gBody * vBody[i].tf_idf));
            i++;
        } else if (i == n) {  // ����title �г���
            ans.push_back(
                ListNode(vTitle[j].fileId, gTitle * vTitle[j].tf_idf));
            j++;
        } else if (vBody[i].fileId ==
                   vTitle[j].fileId) {  // title + body �г���
            ans.push_back(
                ListNode(vBody[i].fileId,
                         gBody * vBody[i].tf_idf + gTitle * vTitle[j].tf_idf));
            i++;
            j++;
        } else if (vBody[i].fileId < vBody[j].fileId) {  // ����body �г���
            ans.push_back(ListNode(vBody[i].fileId, gBody * vBody[i].tf_idf));
            i++;
        } else {  // ����title �г���
            ans.push_back(
                ListNode(vTitle[j].fileId, gTitle * vTitle[j].tf_idf));
            j++;
        }
    }
    return ans;
}

vector<ListNode> InvertedIndex::intersect2(vector<ListNode>& a,
                                           vector<ListNode>& b) {
    int pa = 0, pb = 0;

    vector<ListNode> ans;
    // pa �� pb ֻҪ����Ϊ�վͿ���ȡ����
    while (pa < a.size() && pb < b.size()) {
        if (a[pa].fileId == b[pb].fileId) {  // �ļ� id ��ͬ�����뽻��
            ans.push_back(ListNode(a[pa].fileId, a[pa].tf_idf + b[pb].tf_idf));
            pa++, pb++;
        } else if (a[pa].fileId <
                   b[pb].fileId) {  // �ļ� id ��ͬ�������뽻����pa �ƶ�
            pa++;
        } else {  // �ļ� id ��ͬ�������뽻����pb �ƶ�
            pb++;
        }
    }
    return ans;
}

vector<ListNode> InvertedIndex::intersectMulti(
    vector<vector<ListNode>>& queries) {
    for (auto& p : queries)
        if (!p.size()) return {};
    // �����ȴ�С��������
    auto cmp = [&](const vector<ListNode>& a,
                   const vector<ListNode>& b) -> bool {
        return a.size() < b.size();
    };
    sort(queries.begin(), queries.end(), cmp);

    vector<ListNode> resultList = queries[0];
    // �����ÿ�κϲ���һ������̵�����
    for (int i = 1; i < queries.size(); i++)
        resultList = intersect2(resultList, queries[i]);

    return resultList;
}

vector<ListNode> InvertedIndex::intersectMulti(vector<string>& queries,
                                               string type) {
    if (type != "body" && type != "title") return {};
    vector<vector<ListNode>> vec;
    for (auto& c : queries) {
        // ��һ��Ϊ������Ϊ��
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
    // pa �� pb ֻҪ��һ����Ϊ�վͿ���ȡ��
    while (pa < a.size() || pb < b.size()) {
        if (pa == a.size()) {
            ans.push_back(ListNode(b[pb].fileId, b[pb].tf_idf));
            pb++;
        } else if (pb == b.size()) {
            ans.push_back(ListNode(a[pa].fileId, a[pa].tf_idf));
            pa++;
        } else if (a[pa].fileId ==
                   b[pb].fileId) {  // �ļ� id ��ͬ��ȡ 1 �����벢��
            ans.push_back(ListNode(a[pa].fileId, a[pa].tf_idf + b[pb].tf_idf));
            pa++, pb++;
        } else if (a[pa].fileId < b[pb].fileId) {  // pa ���ļ� id С�����벢��
            ans.push_back(ListNode(a[pa].fileId, a[pa].tf_idf));
            pa++;
        } else {  // pb ���ļ� id С�����벢��
            ans.push_back(ListNode(b[pb].fileId, b[pb].tf_idf));
            pb++;
        }
    }
    return ans;
}

vector<ListNode> InvertedIndex::unionMulti(vector<vector<ListNode>>& queries) {
    if (!queries.size()) return {};
#define piip tuple<int, int, vector<ListNode>>
    // ���ȣ��Ƿ���ԭ�����ڵ� List��List ָ��
    priority_queue<piip, vector<piip>, greater<piip>> q;
#undef piip

    for (auto& p : queries) q.push({p.size(), 1, p});
    while (q.size() >= 2) {
        // ȡ��̵������ϲ�
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
    // pa ��Ϊ�ղ�����
    while (pa < a.size()) {
        if (pb == b.size()) {
            resultList.push_back(
                ListNode(a[pa].fileId, a[pa].tf_idf, a[pa].freq));
            pa++;
        } else if (a[pa].fileId == b[pb].fileId) {  // �ļ� id ��ͬ�����ڲ��
            pa++, pb++;
        } else if (a[pa].fileId < b[pb].fileId) {  // pa ���ļ� id С������
            resultList.push_back(
                ListNode(a[pa].fileId, a[pa].tf_idf, a[pa].freq));
            pa++;
        } else {  // pb ���ļ� id С����Ӱ��������
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

void InvertedIndex::vectorQuery(string s) {}

void InvertedIndex::boolQuery(string s) {
    string lastopt = "";
    vector<ListNode> ansBody;
    vector<ListNode> ansTitle;
    vector<string> query;
    for (int i = 0, round = 0; i <= s.size(); i += 2) {
        string curopt = s.substr(i, 2);
        if (curopt == " ") {
            cout << "��Ҫ�пո�\n";
            break;
        }
        // ��ͷΪ and �� or
        if (i == 0 && (curopt != "an" && curopt != "or")) {
            cout << "���� and �� or ��ͷ��\n";
            break;
        }
        // ��������������߽�β����֮ǰ���ַ�������������
        if (curopt == "an" || curopt == "or" || curopt == "no" ||
            i == s.size()) {
            if (lastopt != "" && !query.size()) {
                cout << "�������֮��Ӧ�����ַ���\n";
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
                        cout << "not ����ֻ�ܸ�һ���ַ���\n";
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

    for (auto& node : ansBody) cout << node.fileId << " ";
    cout << endl;
}