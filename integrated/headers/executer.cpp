#include <bits/stdc++.h>
#include "database.h"
#include "executer.h"
using namespace std;

#define RED "\033[31m"
#define RESET "\033[0m"
#define GREEN "\033[32m"

void executer::createDatabase(string databaseName, map<string, database> &dbs)
{
    try
    {
        auto ifDbExists = dbs.find(databaseName);
        if (ifDbExists == dbs.end())
        {
            database db(databaseName);
            dbs.insert({databaseName, db});
            cout << GREEN << "Database " << databaseName << " created successfully" << RESET << endl;
        }
        else
        {
            string problem = "Error creating database:Database already exists";
            throw problem;
        }
    }
    catch (string &e)
    {
        cerr << RED << e << RESET << endl;
    }
}

void executer::deleteDatabase(const string &databaseName, map<string, database> &dbs)
{
    try
    {
        auto ifDbExists = dbs.find(databaseName);
        if (ifDbExists != dbs.end())
        {
            dbs[databaseName].selfDestruct();
            dbs.erase(databaseName);
            cout << GREEN << "Database " << " deleted successfully" << RESET << endl;
        }
        else
        {
            string problem = "Error deleting database: Database does not exist";
            throw problem;
        }
    }
    catch (string &e)
    {
        cerr << RED << e << RESET << endl;
    }
}

void executer::executeDouble(const string &one, const string &two, const vector<pair<string, string>> &inputs, map<string, database> &dbs, const string &selectedDatabase)
{
    try
    {
        if (one == "db")
        {
            if (two == "createTable")
            {
                if (dbs.find(selectedDatabase) == dbs.end())
                {
                    string problem = "Error creating table: Database does not exist";
                    throw problem;
                }
                bool flag = false;
                string nameVal = "";
                for (auto i : inputs)
                {
                    if (i.first == "name")
                    {
                        nameVal = i.second;
                        flag = true;
                        break;
                    }
                }
                if (!flag)
                {
                    string problem = "Error creating table: 'name' is required";
                    throw problem;
                }
                if (!isalpha(nameVal[0]) || !all_of(nameVal.begin(), nameVal.end(), ::isalnum))
                {
                    string problem = "Error creating table: Invalid table name";
                    throw problem;
                }
                dbs[selectedDatabase].createTable(nameVal, inputs);
            }
            else if (two == "dropTable")
            {
                if (dbs.find(selectedDatabase) == dbs.end())
                {
                    string problem = "Error dropping table: Database does not exist";
                    throw problem;
                }
                bool flag = false;
                string nameVal = "";
                for (auto i : inputs)
                {
                    if (i.first == "name")
                    {
                        nameVal = i.second;
                        flag = true;
                        break;
                    }
                }
                if (!flag)
                {
                    string problem = "Error dropping table: 'name' is required";
                    throw problem;
                }
                dbs[selectedDatabase].dropTable(nameVal);
            }
            else if (two == "createIndex")
            {
                if (dbs.find(selectedDatabase) == dbs.end())
                {
                    string problem = "Error creating index: Database does not exist";
                    throw problem;
                }
                dbs[selectedDatabase].createIndex(inputs);
            }
            else
            {
                string problem = "Error:qqq Invalid middle keyword";
                throw problem;
            }
        }
        else
        {
            string problem = "Error: Invalid start keyword";
            throw problem;
        }
    }
    catch (string &e)
    {
        cerr << RED << e << RESET << endl;
    }
}

void executer::executeSingle(const string &one, const string &two, map<string, database> &dbs, string &selectedDatabase)
{
    try
    {
        if (one == "useDatabase")
        {
            if (dbs.find(two) == dbs.end())
            {
                string problem = "Error using database: Database does not exist";
                throw problem;
            }
            selectedDatabase = two;
            cout << GREEN << "Switched database\n"
                 << RESET;
        }
        else if (one == "createDatabase")
        {
            createDatabase(two, dbs);
            updateMainMetaDataADD(two);
        }
        else if (one == "dropDatabase")
        {
            deleteDatabase(two, dbs);
            updateMainMetaDataDELETE(two);
        }
        else
        {
            string error = "Error: Invalid start keyword";
            throw error;
        }
    }
    catch (string &e)
    {
        cerr << RED << e << endl
             << RESET;
    }
}

executer::executer()
{
}

void executer::updateMainMetaDataADD(const string &databaseName)
{
    fstream metaDataFile("NimbusMetaData.csv", std::ios::app);
    if (!metaDataFile)
    {
        string problem = "Error updating metadata: Metadata file not found";
        throw problem;
    }
    string line = "1" + databaseName + "," + databaseName + ".MData.csv";
    metaDataFile << line << endl;
    metaDataFile.close();
}

void executer::updateMainMetaDataDELETE(const string &databaseNameX)
{
    fstream metaDataFile("NimbusMetaData.csv");
    if (!metaDataFile)
    {
        string problem = "Error updating metadata: Metadata file not found";
        throw problem;
    }
    vector<string> writeBack;
    string line;
    getline(metaDataFile, line);
    writeBack.push_back(line);
    while (metaDataFile.eof())
    {
        getline(metaDataFile, line);
        if (line[0] == '1')
        {
            string temp = "";
            line = line.substr(1, line.size() - 1);
            string databaseName, databaseMetaDataFilePath;
            for (int i = 0; i < line.size(); i++)
            {
                if (line[i] == ',')
                {
                    databaseName = temp;
                    break;
                }
                else
                {
                    temp += line[i];
                }
            }
            databaseMetaDataFilePath = temp;
            if (databaseName == databaseNameX)
            {
                continue;
            }
            else
            {
                writeBack.push_back("1" + line);
            }
        }
        else
        {
            writeBack.push_back(line);
        }
    }
    metaDataFile.close();
    metaDataFile.open("NimbusMetaData.csv", std::ios::out);
    for (int i = 0; i < writeBack.size(); i++)
    {
        metaDataFile << writeBack[i] << endl;
    }
    metaDataFile.close();
}

void executer::executeInsert(const string &one, const string &two, const vector<string> &inputs2, map<string, database> &dbs, string &selectedDatabase)
{
    try
    {
        if (one == "db")
        {
            if (two == "insert")
            {
                if (dbs.find(selectedDatabase) == dbs.end())
                {
                    string problem = "Error inserting data: Database does not exist";
                    throw problem;
                }
                dbs[selectedDatabase].insertIntoTable(inputs2);
            }
            else if (two == "update")
            {
                if (dbs.find(selectedDatabase) == dbs.end())
                {
                    throw string("Error updating data: Database does not exist");
                }

                if (inputs2.size() < 3)
                {
                    throw string("Error updating data: Insufficient parameters. Required: table_name, column_name, new_value, condition.");
                }

                string tableName = inputs2[0];
                string columnName = inputs2[1];
                string newValue = inputs2[2];
                string condition = (inputs2.size() > 3) ? inputs2[3] : "";

                dbs[selectedDatabase].updateTable(tableName, columnName, newValue, condition);

                cout << GREEN << "Records updated successfully in " << tableName << RESET << endl;
            }
            else
            {
                string problem = "Error:ppp Invalid middle keyword";
                throw problem;
            }
        }
        else
        {
            string problem = "Error: Invalid start keyword";
            throw problem;
        }
    }
    catch (string &e)
    {
        cerr << RED << e << RESET << endl;
    }
}

void executer::executeUpdate(const string &one, const string &two, map<string, database> &dbs, string &selectedDatabase, const string &tableName, vector<pair<string, string>> &whereClause, vector<pair<string, string>> &setClause)
{
}

void executer::executeFindDelete(string &one, string &two, string &findInputs, map<string, database> &dbs, string &selectedDatabase)
{
    if (two == "find")
    {
        if (dbs.find(selectedDatabase) == dbs.end())
        {
            string problem = "Error searching data: Database does not exist";
            throw problem;
        }
        // cout<<selectedDatabase<<' '<<findInputs<<endl;
        dbs[selectedDatabase].searchInTable(findInputs);
    }
    else
    {
        if (dbs.find(selectedDatabase) == dbs.end())
        {
            string problem = "Error searching data: Database does not exist";
            throw problem;
        }

        dbs[selectedDatabase].deleteFromTable(findInputs);
    }
}


