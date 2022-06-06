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
        res.set_header("Access-Control-Allow-Origin", "*");
        res.set_header("Cache-Control", "no-cache");

        string expression = req.get_param_value("expression");
        string mod = req.get_param_value("mod");
        string position = req.get_param_value("position");

        Json::Value ret;

        if (mod == "0") {
            for (auto [score, id] : myLexicon.boolQuery(expression)) {
                if (id <= 0) {
                    ret["error"] = (id == 0 ? "请以 and 或 or 开头！"
                                    : id == -1 ? "布尔运算符后应有字符！"
                                               : "not 后面只能跟一个字符！");
                    res.set_content(ret.toStyledString(), "text/plain");
                    return;
                }
                Json::Value new_item;

                new_item["title"] = myLexicon.docTitle[id];
                new_item["content"] = myLexicon.docContent[id];
                new_item["score"] = score;
                new_item["id"] = id;

                ret.append(new_item);
            }
        } else if (mod == "1") {
            for (auto [score, id] : myLexicon.vectorQuery(expression)) {
                Json::Value new_item;

                new_item["title"] = myLexicon.docTitle[id];
                new_item["content"] = myLexicon.docContent[id];
                new_item["score"] = score;
                new_item["id"] = id;

                ret.append(new_item);
            }
        } else {
            for (auto [score, id] : myLexicon.languageModel(expression)) {
                Json::Value new_item;

                new_item["title"] = myLexicon.docTitle[id];
                new_item["content"] = myLexicon.docContent[id];
                new_item["score"] = score;
                new_item["id"] = id;

                ret.append(new_item);
            }
        }
        res.set_content(ret.toStyledString(), "text/plain");
    });
    svr.listen("localhost", 8081);
    return 0;
}