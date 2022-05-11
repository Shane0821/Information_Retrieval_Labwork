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
    vector<pair<int, double>> intersectMulti(vector<PostingList*>& queries);

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
    vector<pair<int, double>> unionMulti(vector<PostingList*>& queries);

    /**
     * @param a �ؼ��� 1
     * @param b �ؼ��� 2
     * @return fileId �
     */
    vector<pair<int, double>> minus2(PostingList* a, PostingList* b);

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
    vector<pair<int, double>> zoneScore(
        const vector<pair<int, double>>& vBody,
        const vector<pair<int, double>>& vTitle);

    /**
     * @param queries ��ѯ�ַ�����
     * @param type ��ѯ����
     * @return fileId ����
     */
    vector<pair<int, double>> intersectMulti(vector<string> queries,
                                             string type = "body");

    /**
     * @param queries ��ѯ�ַ�����
     * @param type ��ѯ����
     * @return fileId ����
     */
    vector<pair<int, double>> unionMulti(vector<string> queries,
                                         string type = "body");

    /**
     * @param a ��ѯ�ַ� 1
     * @param b ��ѯ�ַ� 2
     * @param type ��ѯ����
     * @return fileId �
     */
    vector<pair<int, double>> minus2(const string a, const string b,
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
            dictBody[pii.first]->insert(new ListNode(i, 0, pii.second));
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
            dictTitle[pii.first]->insert(new ListNode(i, 0, pii.second));
        }
    }
    cout << "Dataset loaded successfully!\n";

    // ��ʼ���ļ�Ȩ��
    for (auto& p : dictBody) p.second->initWeight(n);
    for (auto& p : dictTitle) p.second->initWeight(n);
    cout << "Weight initialized.\n";
}
vector<pair<int, double>> InvertedIndex::zoneScore(
    const vector<pair<int, double>>& vBody,
    const vector<pair<int, double>>& vTitle) {
    int n = vBody.size(), m = vTitle.size();
    int i = 0, j = 0;
    vector<pair<int, double>> ans;
    while (i < n || j < m) {
        if (j == m) {  // ����body �г���
            ans.push_back({vBody[i].first, gBody});
            i++;
        } else if (i == n) {  // ����title �г���
            ans.push_back({vTitle[j].first, gTitle});
            j++;
        } else if (vBody[i].first == vTitle[j].first) {  // title + body �г���
            ans.push_back({vBody[i].first, gBody + gTitle});
            i++;
            j++;
        } else if (vBody[i].first < vBody[j].first) {  // ����body �г���
            ans.push_back({vBody[i].first, gBody});
            i++;
        } else {  // ����title �г���
            ans.push_back({vTitle[j].first, gTitle});
            j++;
        }
    }

    // ��Ȩֵ�Ӵ�С����
    auto cmp = [&](const pair<int, double>& a,
                   const pair<int, double>& b) -> bool {
        if (a.second == b.second) return a.first < b.first;
        return a.second > b.second;
    };

    sort(ans.begin(), ans.end(), cmp);
    return ans;
}

PostingList* InvertedIndex::intersect2(PostingList* a, PostingList* b) {
    ListNode* pa = a ? a->head : nullptr;
    ListNode* pb = b ? b->head : nullptr;

    PostingList* ans = new PostingList;
    // pa �� pb ֻҪ����Ϊ�վͿ���ȡ����
    while (pa && pb) {
        if (pa->fileId == pb->fileId) {  // �ļ� id ��ͬ�����뽻��
            ans->insert(new ListNode(pa->fileId, pa->weight + pb->weight,
                                     pa->freq + pb->freq));
            pa = pa->nxt;
            pb = pb->nxt;
        } else if (pa->fileId <
                   pb->fileId) {  // �ļ� id ��ͬ�������뽻����pa �ƶ�
            pa = pa->nxt;
        } else {  // �ļ� id ��ͬ�������뽻����pb �ƶ�
            pb = pb->nxt;
        }
    }
    return ans;
}

vector<pair<int, double>> InvertedIndex::intersectMulti(
    vector<PostingList*>& queries) {
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
    auto ans = resultList->toVector();
    if (resultList != queries[0]) delete resultList;
    return ans;
}

vector<pair<int, double>> InvertedIndex::intersectMulti(vector<string> queries,
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
    auto pa = a->head;
    auto pb = b->head;

    PostingList* ans = new PostingList;
    // pa �� pb ֻҪ��һ����Ϊ�վͿ���ȡ��
    while (pa || pb) {
        if (!pa) {
            ans->insert(new ListNode(pb->fileId, pb->weight, pb->freq));
            pb = pb->nxt;
        } else if (!pb) {
            ans->insert(new ListNode(pa->fileId, pa->weight, pa->freq));
            pa = pa->nxt;
        } else if (pa->fileId == pb->fileId) {  // �ļ� id ��ͬ��ȡ 1 �����벢��
            ans->insert(new ListNode(pa->fileId, pa->weight + pb->weight,
                                     pa->freq + pb->freq));
            pa = pa->nxt;
            pb = pb->nxt;
        } else if (pa->fileId < pb->fileId) {  // pa ���ļ� id С�����벢��
            ans->insert(new ListNode(pa->fileId, pa->weight, pa->freq));
            pa = pa->nxt;
        } else {  // pb ���ļ� id С�����벢��
            ans->insert(new ListNode(pb->fileId, pb->weight, pb->freq));
            pb = pb->nxt;
        }
    }
    return ans;
}

vector<pair<int, double>> InvertedIndex::unionMulti(
    vector<PostingList*>& queries) {
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
    auto ans = resultList->toVector();
    if (!get<1>(q.top())) delete resultList;
    return ans;
}

vector<pair<int, double>> InvertedIndex::unionMulti(vector<string> queries,
                                                    string type) {
    if (type != "body" && type != "title") return {};
    vector<PostingList*> vPL;
    for (auto& c : queries) {
        if (type == "body" && dictBody[c]) vPL.push_back(dictBody[c]);
        if (type == "title" && dictTitle[c]) vPL.push_back(dictTitle[c]);
    }
    return unionMulti(vPL);
}

vector<pair<int, double>> InvertedIndex::minus2(PostingList* a,
                                                PostingList* b) {
    ListNode* pa = a ? a->head : nullptr;
    ListNode* pb = b ? b->head : nullptr;

    PostingList* resultList = new PostingList;
    // pa ��Ϊ�ղ�����
    while (pa) {
        if (!pb) {
            resultList->insert(new ListNode(pa->fileId, pa->weight, pa->freq));
            pa = pa->nxt;
        } else if (pa->fileId == pb->fileId) {  // �ļ� id ��ͬ�����ڲ��
            pa = pa->nxt;
            pb = pb->nxt;
        } else if (pa->fileId < pb->fileId) {  // pa ���ļ� id С������
            resultList->insert(new ListNode(pa->fileId, pa->weight, pa->freq));
            pa = pa->nxt;
        } else {  // pb ���ļ� id С����Ӱ��������
            pb = pb->nxt;
        }
    }

    auto ans = resultList->toVector();
    delete resultList;
    return ans;
}

vector<pair<int, double>> InvertedIndex::minus2(const string a, const string b,
                                                string type) {
    if (type != "body" && type != "title") return {};
    return type == "body" ? minus2(dictBody[a], dictBody[b])
                          : minus2(dictTitle[a], dictTitle[b]);
}