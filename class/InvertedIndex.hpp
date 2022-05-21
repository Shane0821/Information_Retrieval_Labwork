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
    // body ���ַ�����
    unordered_map<string, PostingList*> dictBody;
    // title ���ַ�����
    unordered_map<string, PostingList*> dictTitle;

    // ����Ȩ��
    static double gTitle;
    // ����Ȩ��
    static double gBody;
    // database ���ĵ�����
    static int n;

   public:
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

   public:
    // ���캯��
    InvertedIndex() = default;
    // ��������
    ~InvertedIndex();
    // ��������
    void loadFromDataset();

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
            if (lex == "��" || lex == "��" || lex == "��" || lex == "��" ||
                lex == "��" || lex == "��" || lex == "��" || lex == "��" ||
                lex == "��" || lex == "��" || lex == "\n" || lex == " ")
                continue;
            freqBody[lex]++;
        }
        // append to dictBody
        for (auto& pii : freqBody) {
            // �������в���������ؼ��֣�����ӵ�������
            // ����ָ��һ�� PostingList
            if (!dictBody[pii.first]) dictBody[pii.first] = new PostingList;
            // PostingList �в���ڵ�
            dictBody[pii.first]->insert(ListNode(i, 0, pii.second));
        }

        // caculate freq of each char in title
        unordered_map<string, int> freqTitle;
        for (int i = 0; i < title.size(); i += 2) {
            auto lex = title.substr(i, 2);
            if (lex == "��" || lex == "��" || lex == "��" || lex == "��" ||
                lex == "��" || lex == "��" || lex == "��" || lex == "��" ||
                lex == "��" || lex == "��" || lex == "\n" || lex == " ")
                continue;
            freqTitle[lex]++;
        }
        // append to dictTitle
        for (auto& pii : freqTitle) {
            // �������в���������ؼ��֣�����ӵ�������
            // ����ָ��һ�� PostingList
            if (!dictTitle[pii.first]) dictTitle[pii.first] = new PostingList;
            // PostingList �в���ڵ�
            dictTitle[pii.first]->insert(ListNode(i, 0, pii.second));
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
            ans.push_back(ListNode(vBody[i].fileId, gBody * vBody[i].weight));
            i++;
        } else if (i == n) {  // ����title �г���
            ans.push_back(
                ListNode(vTitle[j].fileId, gTitle * vTitle[j].weight));
            j++;
        } else if (vBody[i].fileId ==
                   vTitle[j].fileId) {  // title + body �г���
            ans.push_back(
                ListNode(vBody[i].fileId,
                         gBody * vBody[i].weight + gTitle * vTitle[j].weight));
            i++;
            j++;
        } else if (vBody[i].fileId < vBody[j].fileId) {  // ����body �г���
            ans.push_back(ListNode(vBody[i].fileId, gBody * vBody[i].weight));
            i++;
        } else {  // ����title �г���
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
    // pa �� pb ֻҪ����Ϊ�վͿ���ȡ����
    while (pa < a.size() && pb < b.size()) {
        if (a[pa].fileId == b[pb].fileId) {  // �ļ� id ��ͬ�����뽻��
            ans.push_back(ListNode(a[pa].fileId, a[pa].weight + b[pb].weight,
                                   a[pa].freq + b[pb].freq));
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
            ans.push_back(ListNode(b[pb].fileId, b[pb].weight, b[pb].freq));
            pb++;
        } else if (pb == b.size()) {
            ans.push_back(ListNode(a[pa].fileId, a[pa].weight, a[pa].freq));
            pa++;
        } else if (a[pa].fileId ==
                   b[pb].fileId) {  // �ļ� id ��ͬ��ȡ 1 �����벢��
            ans.push_back(ListNode(a[pa].fileId, a[pa].weight + b[pb].weight,
                                   a[pa].freq + b[pb].freq));
            pa++, pb++;
        } else if (a[pa].fileId < b[pb].fileId) {  // pa ���ļ� id С�����벢��
            ans.push_back(ListNode(a[pa].fileId, a[pa].weight, a[pa].freq));
            pa++;
        } else {  // pb ���ļ� id С�����벢��
            ans.push_back(ListNode(b[pb].fileId, b[pb].weight, b[pb].freq));
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
                ListNode(a[pa].fileId, a[pa].weight, a[pa].freq));
            pa++;
        } else if (a[pa].fileId == b[pb].fileId) {  // �ļ� id ��ͬ�����ڲ��
            pa++, pb++;
        } else if (a[pa].fileId < b[pb].fileId) {  // pa ���ļ� id С������
            resultList.push_back(
                ListNode(a[pa].fileId, a[pa].weight, a[pa].freq));
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