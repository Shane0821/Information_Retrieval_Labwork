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
        // 查询类型
        string querytype;
        while (querytype != "vector" && querytype != "bool") {
            cout << "请输入查询类型（bool/vector）：\n";
            cin >> querytype;
        }
        // cout << "请输入查询位置";

        cout << "请输入查询表达式（no spaces）：\n";
        cin >> s;

        querytype == "bool" ? myLexicon.boolQuery(s) : myLexicon.vectorQuery(s);

        string check;
        while (check != "y" && check != "n") {
            cout << "是否继续查询（y/n）？\n";
            cin >> check;
        }
        if (check == "n") break;
    }
    return 0;
}