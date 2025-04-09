#ifndef EXECUTER_H
#define EXECUTER_H
#include <bits/stdc++.h>
#include "database.h"

using namespace std;

class executer
{
private:
public:
    executer();
    void createDatabase(string databaseName, map<string, database> &dbs);
    void deleteDatabase(const string &databaseName, map<string, database> &dbs);
    void updateMainMetaDataADD(const string &databaseName);
    void updateMainMetaDataDELETE(const string &databaseNameX);
    void executeDouble(const string &one, const string &two, const vector<pair<string, string>> &inputs, map<string, database> &dbs, const string &selectedDatabase);
    void executeSingle(const string &one, const string &two, map<string, database> &dbs, string &selectedDatabase);
    void executeInsert(const string &one, const string &two, const vector<string> &inputs2, map<string, database> &dbs, string &selectedDatabase);
    void executeUpdate(const string &one, const string &two,map<string, database> &dbs, string &selectedDatabase, const string &tableName, vector<pair<string, string>> &whereClause, vector<pair<string, string>> &setClause);
    void executeFindDelete(string &one,string &two,string &findInputs,map<string, database> &dbs, string &selectedDatabase);
};

#endif