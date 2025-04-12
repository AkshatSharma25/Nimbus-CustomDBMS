#ifndef TABLE_H
#define TABLE_H
#include<thread>
#include<bits/stdc++.h>
#include<fstream>
using namespace std;
class table{
    private:
        vector<pair<string,pair<string,int>>>data;

        int numberOfRecords;

        string tableName;

        void getMetaData(const string databaseName,const string fileName);

        map<string,string> interpretRead(ifstream&file,int sizeOfLine);

        bool interpretWrite(ofstream&file,vector<string>&values);

        vector<string> interpretRead(ifstream&file);

        bool updateRowNumber(string databaseName);

        map<string,pair<int,int>>indexString;
        map<int,pair<int,int>>indexInt;
        map<double,pair<int,int>>indexDouble;
        map<char,pair<int,int>>indexChar;
        map<int,pair<string,string>>updateBuffer;

        bool evaluateExpr(const string& expr, const vector<bool>& between);
        int getTotalBytes(const vector<pair<string, pair<string, int>>> &data);
        bool deleteX(string databaseName,int i,string inputs);
        bool update(string databaseName, int i, string inputs);
        bool search(string databaseName,int i,string  mp);
        bool search(string databaseName);
        bool parseCondition(string inputs,bool colon=false);
    public:
        bool isIndexed;
        string indexedColumn;
        table();
        table(string databaseName,string fileName);
        table(string tableName,string databaseName,vector<pair<string,string>>columns);
        void selfDestruct();
        void printMetaData();  
        bool insert(string databaseName,vector<string>inputs);      
        bool parallelSearch(string databaseName,string inputs);
        bool parallelUpdate(string databaseName,string inputs);
        bool parallelDelete(string databaseName,string inputs);
        bool createIndex(string databaseName,string columnName,bool init);
        bool indexedSearch(string databaseName,string columnName,string value);
};


#endif