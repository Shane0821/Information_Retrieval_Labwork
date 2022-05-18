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

   private:
    /**
     * @param a PostingList 1
     * @param b PostingList 2
     * @return PostingList1 �� PostingList2 �Ľ���
     */
    PostingList* intersect2(PostingList* a, PostingList* b);

    /**
     * @param queries ��ѯ PostingList ����
     * @return fileId ����
     */
    vector<ListNode> intersectMulti(vector<PostingList*>& queries);

    /**
     * @param a PostingList 1
     * @param b PostingList 2
     * @return PostingList1 �� PostingList2 �Ĳ���
     */
    PostingList* union2(PostingList* a, PostingList* b);

    /**
     * @param queries ��ѯ PostingList ����
     * @return fileId ����
     */
    vector<ListNode> unionMulti(vector<PostingList*>& queries);

    /**
     * @param a �ؼ��� 1
     * @param b �ؼ��� 2
     * @return fileId �
     */
    vector<ListNode> minus2(PostingList* a, PostingList* b);

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
    vector<ListNode> intersectMulti(vector<string> queries,
                                    string type = "body");

    /**
     * @param queries ��ѯ�ַ�����
     * @param type ��ѯ����
     * @return fileId ����
     */
    vector<ListNode> unionMulti(vector<string> queries, string type = "body");

    /**
     * @param a ��ѯ�ַ� 1
     * @param b ��ѯ�ַ� 2
     * @param type ��ѯ����
     * @return fileId �
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
            ans.push_back({vBody[i].fileId, gBody * vBody[i].weight});
            i++;
        } else if (i == n) {  // ����title �г���
            ans.push_back({vTitle[j].fileId, gTitle * vTitle[j].weight});
            j++;
        } else if (vBody[i].fileId ==
                   vTitle[j].fileId) {  // title + body �г���
            ans.push_back({vBody[i].fileId, gBody * vBody[i].weight +
                                                gTitle * vTitle[j].weight});
            i++;
            j++;
        } else if (vBody[i].fileId < vBody[j].fileId) {  // ����body �г���
            ans.push_back({vBody[i].fileId, gBody * vBody[i].weight});
            i++;
        } else {  // ����title �г���
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
    // pa �� pb ֻҪ����Ϊ�վͿ���ȡ����
    while (pa < va.size() && pb < vb.size()) {
        if (va[pa].fileId == vb[pb].fileId) {  // �ļ� id ��ͬ�����뽻��
            ans->insert(ListNode(va[pa].fileId, va[pa].weight + vb[pb].weight,
                                 va[pa].freq + vb[pb].freq));
            pa++, pb++;
        } else if (va[pa].fileId <
                   vb[pb].fileId) {  // �ļ� id ��ͬ�������뽻����pa �ƶ�
            pa++;
        } else {  // �ļ� id ��ͬ�������뽻����pb �ƶ�
            pb++;
        }
    }
    return ans;
}

vector<ListNode> InvertedIndex::intersectMulti(vector<PostingList*>& queries) {
    if (!queries.size()) return {};
    // ��һ�����򽻼�Ϊ��
    for (auto& p : queries)
        if (!p) return {};
    // �����ȴ�С��������
    auto cmp = [&](const PostingList* a, const PostingList* b) -> bool {
        return a->cntFile < b->cntFile;
    };
    sort(queries.begin(), queries.end(), cmp);

    PostingList* resultList = queries[0];
    // �����ÿ�κϲ���һ������̵�����
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
        // �ռ�Ҫ����intersect
        if (type == "body") vPL.push_back(dictBody[c]);
        if (type == "title") vPL.push_back(dictTitle[c]);
    }
    return intersectMulti(vPL);
}

PostingList* InvertedIndex::union2(PostingList* a, PostingList* b) {
    auto va = a->vlist, vb = b->vlist;
    int pa = 0, pb = 0;

    PostingList* ans = new PostingList;
    // pa �� pb ֻҪ��һ����Ϊ�վͿ���ȡ��
    while (pa < va.size() || pb < vb.size()) {
        if (pa == va.size()) {
            ans->insert(ListNode(vb[pb].fileId, vb[pb].weight, vb[pb].freq));
            pb++;
        } else if (pb == vb.size()) {
            ans->insert(ListNode(va[pa].fileId, va[pa].weight, va[pa].freq));
            pa++;
        } else if (va[pa].fileId ==
                   vb[pb].fileId) {  // �ļ� id ��ͬ��ȡ 1 �����벢��
            ans->insert(ListNode(va[pa].fileId, va[pa].weight + vb[pb].weight,
                                 va[pa].freq + vb[pb].freq));
            pa++, pb++;
        } else if (va[pa].fileId <
                   vb[pb].fileId) {  // pa ���ļ� id С�����벢��
            ans->insert(ListNode(va[pa].fileId, va[pa].weight, va[pa].freq));
            pa++;
        } else {  // pb ���ļ� id С�����벢��
            ans->insert(ListNode(vb[pb].fileId, vb[pb].weight, vb[pb].freq));
            pb++;
        }
    }
    return ans;
}

vector<ListNode> InvertedIndex::unionMulti(vector<PostingList*>& queries) {
    if (!queries.size()) return {};
#define piip tuple<int, int, PostingList*>
    // ���ȣ��Ƿ���ԭ�����ڵ� List��List ָ��
    priority_queue<piip, vector<piip>, greater<piip>> q;
#undef piip

    for (auto& p : queries) q.push({p->cntFile, 1, p});
    while (q.size() >= 2) {
        // ȡ��̵������ϲ�
        auto tp1 = q.top();
        q.pop();
        auto tp2 = q.top();
        q.pop();

        auto merged = union2(get<2>(tp1), get<2>(tp2));
        q.push({merged->cntFile, 0, merged});

        // ����ԭ�����ڵ�ɾ������ֹ�ڴ�й©
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
    // pa ��Ϊ�ղ�����
    while (pa < va.size()) {
        if (pb == vb.size()) {
            resultList->insert(
                ListNode(va[pa].fileId, va[pa].weight, va[pa].freq));
            pa++;
        } else if (va[pa].fileId ==
                   vb[pb].fileId) {  // �ļ� id ��ͬ�����ڲ��
            pa++, pb++;
        } else if (va[pa].fileId <
                   vb[pb].fileId) {  // pa ���ļ� id С������
            resultList->insert(
                ListNode(va[pa].fileId, va[pa].weight, va[pa].freq));
            pa++;
        } else {  // pb ���ļ� id С����Ӱ��������
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