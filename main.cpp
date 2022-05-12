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
        string type, s, pos;

        while (type != "and" && type != "or" && type != "andnot") {
            cout << "请输入查询类型(and / or / andnot)：\n";
            cin >> type;
        }

        if (type == "and") {
            cout << "请输入查询关键字：\n";
            cin.ignore();
            getline(cin, s);
            vector<string> query;
            for (int i = 0; i < s.size(); i += 2)
                query.push_back(s.substr(i, 2));

            while (pos != "title" && pos != "body" && pos != "both") {
                cout << "请输入查询位置(title / body / both)：\n";
                cin >> pos;
            }

            if (pos == "title") {
                auto v = myLexicon.intersectMulti(query, pos);
                cout << "满足查询条件的文档 id：\n";
                if (v.size())
                    for (auto& [id, score] : v)
                        cout << id << ": " << score << endl;
                else
                    cout << "null";
                cout << endl;
            } else if (pos == "body") {
                auto v = myLexicon.intersectMulti(query, pos);
                cout << "满足查询条件的文档 id：\n";
                if (v.size())
                    for (auto& [id, score] : v)
                        cout << id << ": " << score << endl;
                else
                    cout << "null";
                cout << endl;
            } else if (pos == "both") {
                auto v1 = myLexicon.intersectMulti(query, "body");
                auto v2 = myLexicon.intersectMulti(query, "title");
                auto v3 = myLexicon.zoneScore(v1, v2);
                cout << "满足查询条件的文档 id & score：\n";
                if (v3.size())
                    for (auto& [id, score] : v3)
                        cout << id << ": " << score << endl;
                else
                    cout << "null\n";
            }
        } else if (type == "or") {
            cout << "请输入查询关键字：\n";
            cin.ignore();
            getline(cin, s);
            vector<string> query;
            for (int i = 0; i < s.size(); i += 2)
                query.push_back(s.substr(i, 2));

            while (pos != "title" && pos != "body" && pos != "both") {
                cout << "请输入查询位置(title / body / both)：\n";
                cin >> pos;
            }

            if (pos == "title") {
                auto v = myLexicon.unionMulti(query, pos);
                cout << "满足查询条件的文档 id：\n";
                if (v.size())
                    for (auto& [id, score] : v)
                        cout << id << ": " << score << endl;
                else
                    cout << "null";
                cout << endl;
            } else if (pos == "body") {
                auto v = myLexicon.unionMulti(query, pos);
                cout << "满足查询条件的文档 id：\n";
                if (v.size())
                    for (auto& [id, score] : v)
                        cout << id << ": " << score << endl;
                else
                    cout << "null";
                cout << endl;
            } else if (pos == "both") {
                auto v1 = myLexicon.unionMulti(query, "body");
                auto v2 = myLexicon.unionMulti(query, "title");
                auto v3 = myLexicon.zoneScore(v1, v2);
                cout << "满足查询条件的文档 id & score：\n";
                if (v3.size())
                    for (auto& [id, score] : v3)
                        cout << id << ": " << score << endl;
                else
                    cout << "null\n";
            }
        } else if (type == "andnot") {
            string a, b;
            cout << "请输入关键字1和关键字2：\n";
            cout << "关键字1: ";
            cin >> a;
            cout << "关键字2: ";
            cin >> b;

            while (pos != "title" && pos != "body") {
                cout << "请输入查询位置(title / body)：\n";
                cin >> pos;
            }
            auto v = myLexicon.minus2(a, b, pos);

            cout << "满足查询条件的文档 id：\n";
            if (v.size())
                for (auto& [id, score] : v) cout << id << ": " << score << endl;
            else
                cout << "null";
            cout << endl;
        }

        string opt;
        while (opt != "yes" && opt != "no") {
            cout << "继续查询(yes / no)？\n";
            cin >> opt;
        }
        if (opt == "no") break;
    }
    return 0;
}