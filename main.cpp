#include <bits/stdc++.h>

#include "./class/InvertedIndex.hpp"

using namespace std;

/**
 * encoding: GBK
 */

signed main() {
    InvertedIndex myLexicon;
    myLexicon.loadFromDataset();

    while (1) {
        string s;
        // ��ѯ����
        string querytype;
        while (querytype != "vector" && querytype != "bool") {
            cout << "�������ѯ���ͣ�bool/vector����\n";
            cin >> querytype;
        }
        // cout << "�������ѯλ��";

        cout << "�������ѯ���ʽ��no spaces����\n";
        cin >> s;

        querytype == "bool" ? myLexicon.boolQuery(s) : myLexicon.vectorQuery(s);

        string check;
        while (check != "y" && check != "n") {
            cout << "�Ƿ������ѯ��y/n����\n";
            cin >> check;
        }
        if (check == "n") break;
    }
    return 0;
}