#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <bits/stdc++.h>

#include "./class/InvertedIndex.hpp"
#include "./jsoncpp/json.h"
#include "httplib.h"
/*
编译命令
g++ main.cpp -o main -lwsock32 -lws2_32
*/

signed main() {
    InvertedIndex myLexicon;
    myLexicon.loadFromDataset();

    using namespace httplib;
    Server svr;

    svr.Get("/", [&](const Request &req, Response &res) {
        string expression = req.get_param_value("expression");
        string mod = req.get_param_value("mod");
        string position = req.get_param_value("position");
        
        Json::Value ret;

        ret["response"]["x"] = 1;
        
        if (mod == "0") {
            // puts("m0");
        } else if (mod == "1") {
            // myLexicon.vectorQuery(expression);
        } else {
            // myLexicon.languageModel(expression);
        }
        res.set_content(ret.toStyledString(), "text/plain");
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