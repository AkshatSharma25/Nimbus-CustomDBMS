#ifndef DATABASE_H
#define DATABASE_H
#include <bits/stdc++.h>
#include "table.h"
#include<thread>
using namespace std;

class database
{
private:
    map<string, pair<table, string>> tables;
    string databaseName;
    void loadDatabase(string name,string metaDataFilePath);

public:
    database();
    database(string name, string databaseMetaDataFilePath);
    database(string name);
    void printDatabaseMetaData();
    void selfDestruct();
    void createTable(string tableName, const vector<pair<string, string>> &inputs);
    void dropTable(string tableName);
    void insertIntoTable(vector<string> inputs);
    void deleteFromTable(string inputs);
    void updateTable(string inputs);
    void searchInTable(string inputs);
    void createIndex(vector<pair<string, string>> inputs);
    void showTables();
};

#endif
