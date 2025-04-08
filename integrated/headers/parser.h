#ifndef PARSER_H
#define PARSER_H
#include<bits/stdc++.h>
#include"returnObject.h"
using namespace std;

class parser{
    private:
    vector<string> commandKeywords;
    string query;
    unordered_set<string> middleKeywords;
    unordered_set<string> startKeywords;
    returnObject checkSyntax(const string&one,const string&two);
    returnObject checkSyntax(const string&one,const string&two,string three);
    public:
    parser();
    returnObject parse(string userQuery);
    void printQuery();
};
#endif