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

        // cout << "�������ѯλ��";

        cout << "�������ѯ���ʽ (no spaces)��\n";
        cin >> s;

        myLexicon.boolQuery(s);

        string check;
        while (check != "y" && check != "n") {
            cout << "�Ƿ������ѯ��y/n����\n";
            cin >> check;
        }
        if (check == "n") break;
    }
    return 0;
}