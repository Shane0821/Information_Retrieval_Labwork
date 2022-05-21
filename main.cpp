#include <bits/stdc++.h>

#include "./class/InvertedIndex.hpp"

using namespace std;

/**
 * encoding: GBK
 */

signed main() {
    InvertedIndex myLexicon;
    // myLexicon.loadFromDataset();

    while (1) {
        string s;
        // 查询类型

        // cout << "请输入查询位置";

        cout << "请输入查询表达式 (no spaces)：\n";
        cin >> s;

        string lastopt = "";
        vector<ListNode> ansBody;
        vector<string> query;
        for (int i = 0, round = 0; i <= s.size(); i += 2) {
            string curopt = s.substr(i, 2);
            if (i == 0 && (curopt != "an" && curopt != "or")) {
                cout << "请以 and 或 or 开头！\n";
                break;
            }
            if (curopt == "an" || curopt == "or" || curopt == "no" ||
                i == s.size()) {
                if (lastopt != "" && !query.size()) {
                    cout << "两运算符之间应该有字符！\n";
                    break;
                }
                if (round != 0)
                    if (lastopt == "an") {
                        auto vec = myLexicon.intersectMulti(query);
                        if (round == 1)
                            ansBody = vec;
                        else
                            ansBody = myLexicon.intersect2(ansBody, vec);
                    } else if (lastopt == "or") {
                        auto vec = myLexicon.unionMulti(query);
                        if (round == 1)
                            ansBody = vec;
                        else
                            ansBody = myLexicon.union2(ansBody, vec);
                    } else if (lastopt == "no") {
                        if (query.size() > 1) {
                            cout << "not 后面只能跟一个字符！\n";
                            break;
                        }
                        ansBody = myLexicon.minus2(ansBody, query[0]);
                    }

                lastopt = curopt;
                query.clear();
                if (curopt == "an" || curopt == "no") i++;
                round++;
            } else {
                query.push_back(curopt);
            }
        }

        string check;
        while (check != "y" && check != "n") {
            cout << "是否继续查询（y/n）？\n";
            cin >> check;
        }
        if (check == "n") break;
    }
    return 0;
}