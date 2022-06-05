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
        while (querytype != "vector" && querytype != "bool" &&
               querytype != "language") {
            cout << "�������ѯģ�ͣ�bool/vector/language����\n";
            cin >> querytype;
        }
        // cout << "�������ѯλ��";

        cout << "�������ѯ���ʽ��no spaces����\n";
        cin >> s;

        if (querytype == "bool")
            myLexicon.boolQuery(s);
        else if (querytype == "vector")
            myLexicon.vectorQuery(s);
        else
            myLexicon.languageModel(s);

        string check;
        while (check != "y" && check != "n") {
            cout << "�Ƿ������ѯ��y/n����\n";
            cin >> check;
        }
        if (check == "n") break;
    }
    return 0;
}