#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <bits/stdc++.h>

#include "./class/InvertedIndex.hpp"
#include "httplib.h"
/**
 * encoding: GBK
 */

signed main() {
    InvertedIndex myLexicon;
    myLexicon.loadFromDataset();

    using namespace httplib;
    Server svr;

    svr.Get("/", [](const Request &req, Response &res) {
        if (req.has_param("expression")) {
            auto val = req.get_param_value("expression");
            cout << val.size() << endl;
            for (int i = 0; i < val.size(); i += 3) {
                cout << val.substr(i, 3) << endl;
            }
        }
        if (req.has_param("mod")) {
            auto val = req.get_param_value("mod");
            cout << val << endl;
        }
        if (req.has_param("position")) {
            auto val = req.get_param_value("position");
            cout << val << endl;
        }

        res.set_content("Hello World!", "text/plain");
    });
    svr.listen("localhost", 8081);

    // while (1) {
    //     string s;
    //     // 查询类型
    //     string querytype;
    //     while (querytype != "vector" && querytype != "bool" &&
    //            querytype != "language") {
    //         cout << "请输入查询模型（bool/vector/language）：\n";
    //         cin >> querytype;
    //     }
    //     // cout << "请输入查询位置";

    //     cout << "请输入查询表达式（no spaces）：\n";
    //     cin >> s;

    //     if (querytype == "bool")
    //         myLexicon.boolQuery(s);
    //     else if (querytype == "vector")
    //         myLexicon.vectorQuery(s);
    //     else
    //         myLexicon.languageModel(s);

    //     string check;
    //     while (check != "y" && check != "n") {
    //         cout << "是否继续查询（y/n）？\n";
    //         cin >> check;
    //     }
    //     if (check == "n") break;
    // }
    return 0;
}

/*
g++ main.cpp -o main -lwsock32 -lws2_32
main
*/