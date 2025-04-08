#ifndef RETURN_OBJECT_H
#define RETURN_OBJECT_H
#include<bits/stdc++.h>
using namespace std;

class returnObject{
    public:
    int flag;
    string one,two, tableName;
    bool andFunc;
    vector<pair<string,string>>inputs, whereClauses, setClauses;
    vector<string>inputs2;
    returnObject(string one,string two,vector<pair<string,string>>inputs);
    returnObject(string one,string two,vector<string>inputs2);
    returnObject();
    returnObject(string one,string two);
    // mine
    returnObject(string one, string two, string tableName, vector<pair<string, string>> whereClauses, 
        vector<pair<string, string>> setClauses);
    returnObject(string one, string two, vector<string> inputs2, bool andFunc);
};



#endif  