#include <bits/stdc++.h>
#include "database.h"
#include <thread>
#include <filesystem>

using namespace std;
namespace fs = std::filesystem;

#define RED "\033[31m"
#define RESET "\033[0m"
#define GREEN "\033[32m"

void database::loadDatabase(string name, string metaDataFilePath)
{
    try
    {
        fstream metaDataFile("databases/" + name + "/" + metaDataFilePath);
        if (!metaDataFile)
        {
            string problem = "Error opening metadata file!";
            throw problem;
        }
        while (!metaDataFile.eof())
        {
            string line;
            getline(metaDataFile, line);
            if (line[0] == '0')
            {
                databaseName = line.substr(1, line.size() - 1);
            }
            else if (line[0] == '1')
            {
                line = line.substr(1, line.size() - 1);
                string temp = "";
                string tableName, tableMetaDataFile;
                for (int i = 0; i < line.size(); i++)
                {
                    if (line[i] == ',')
                    {
                        tableName = temp;
                        temp = "";
                    }
                    else
                    {
                        temp += line[i];
                    }
                }
                tableMetaDataFile = temp;
                table newTable = table(databaseName, tableMetaDataFile);
                tables.insert({tableName, {newTable, tableMetaDataFile}});
            }
        }
        metaDataFile.close();
    }
    catch (string &e)
    {
        cerr << e << endl;
    }
}

database::database()
{
    // empty default constructor
}

database::database(string name, string databaseMetaDataFilePath)
{
    loadDatabase(name, databaseMetaDataFilePath);
}

database::database(string name)
{
    databaseName = name;
    string metaDataFileName = "databases/" + name + "/" + name + ".MData.csv";
    string folderName = "databases/" + name;

    if (std::filesystem::create_directory(folderName))
    {
        // cout << "Folder created successfully: " << folderName << endl;
    }
    else
    {
        cout << "Failed to create folder or it already exists." << endl;
    }
    metaDataFileName = metaDataFileName;
    ofstream metaDataFile(metaDataFileName);
    if (!metaDataFile)
    {
        cerr << "Error opening metadata file!" << endl;
        return;
    }
    metaDataFile << "0" << name << endl;
    metaDataFile.close();
}

void database::printDatabaseMetaData()
{
    cout << "Database-Name: " << databaseName << endl
         << endl;
    for (auto i : tables)
    {
        cout << "Table-Name: " << i.first << endl;
        cout << "Table-Metadata-File: " << i.second.second << endl
             << endl;
    }
}

void database::selfDestruct()
{
    std::string folderName = "databases/" + databaseName;

    if (std::filesystem::remove_all(folderName))
    {
        std::cout << "Folder deleted successfully: " << folderName << std::endl;
    }
    else
    {
        std::cout << "Failed to delete folder or it does not exist." << std::endl;
    }
}

void database::createTable(string tableName, const vector<pair<string, string>> &inputs)
{
    string tableMetaDataFilePath = "databases/" + databaseName + "/" + tableName + "/" + tableName + ".TMData.csv";

    table newTable = table(tableName, databaseName, inputs);
    if (tables.find(tableName) != tables.end())
    {
        cout << RED << "Error: Table already exists" << RESET << endl;
        return;
    }
    tables.insert({tableName, {newTable, tableMetaDataFilePath}});
    string databaseMetaDataFilePath = ("databases/" + databaseName + "/" + databaseName + ".MData.csv");
    fstream databaseMetaDataFile(databaseMetaDataFilePath, std::ios::app);
    databaseMetaDataFile << "1" << tableName << "," << tableMetaDataFilePath << endl;
    databaseMetaDataFile.close();
}

void database::dropTable(string tableName)
{
    tables[tableName].first.selfDestruct();
    tables.erase(tableName);
    string tableMetaDataFilePath = "databases/" + databaseName + "/" + tableName + "/" + tableName + ".TMData.csv";
    fs::remove_all("databases/" + databaseName + "/" + tableName);
    // cout<<tableMetaDataFilePath<<endl;
    // cout<<"databases/"+databaseName+"/"+tableName<<endl;
    ifstream databaseMetaDataFile(("databases/" + databaseName + "/" + databaseName + ".MData.csv").c_str(), std::ios::in);
    if (!databaseMetaDataFile)
    {
        cerr << RED << "Error opening metadata file!" << RESET << endl;
        return;
    }
    string line;
    vector<string> newContent;
    getline(databaseMetaDataFile, line);
    newContent.push_back(line);
    while (!databaseMetaDataFile.eof())
    {
        // getline(databaseMetaDataFile, line);
        getline(databaseMetaDataFile, line);
        if(line=="") break;
        // cout<<line<<endl;
        string tableNameX, tableMetaDataFile;
        string temp = "";
        line = line.substr(1, line.size() - 1);
        int ctr = 0;
        int n = line.size();
        for (int i = 0; i < n; i++)
        {
            if (line[i] != ',')
            {
                temp += line[i];
            }
            else
            {
                if (ctr == 0)
                {
                    tableNameX = temp;
                }
                else
                {
                    tableMetaDataFile = temp;
                }
                temp = "";
                ctr++;
            }
        }
        if (temp.size() > 0)
            tableMetaDataFile = temp;
        // cout << tableNameX << ' ' << tableName << endl;
        if (tableNameX != tableName)
        {
            newContent.push_back("1" + line);
        }
    }
    databaseMetaDataFile.close();

    ofstream newDatabaseMetaDataFile(("databases/" + databaseName + "/" + databaseName + ".MData.csv").c_str(), std::ios::out);
    for (auto i : newContent)
    {
        // cout << i << endl;
        newDatabaseMetaDataFile << i << endl;
    }
    newDatabaseMetaDataFile.close();
    cout << GREEN << "Table deletion Successful!" << RESET << endl;
}

void database::insertIntoTable(vector<string> inputs)
{
    if (inputs.size() == 0)
    {
        cout << RED << "Error: No values to insert" << RESET << endl;
        return;
    }
    string nameString = inputs[0];
    for (int i = 0; i < nameString.size(); i++)
    {
        if (nameString[i] == ':')
        {
            nameString = nameString.substr(i + 1, nameString.size() - i - 1);
            break;
        }
    }
    if (tables.find(nameString) == tables.end())
    {
        cout << RED << "Error: Table does not exist" << RESET << endl;
        return;
    }
    inputs.erase(inputs.begin());
    tables[nameString].first.insert(databaseName, inputs);
}

void database::deleteFromTable(string inputs)
{
    if (inputs.size() == 0)
    {
        cout << RED << "Error: No values to insert" << RESET << endl;
        return;
    }
    string temp = "";
    string name = "";
    string valueString = "";
    int sizeOfInputs = inputs.size();
    for (int i = 0; i < sizeOfInputs; i++)
    {
        if (inputs[i] == ',')
        {
            temp = inputs.substr(0, i);
            inputs = inputs.substr(i + 1, sizeOfInputs - i - 2);
            break;
        }
    }

    for (int i = 0; i < temp.size(); i++)
    {
        if (temp[i] == ':')
        {
            valueString = temp.substr(i + 1, temp.size() - i - 1);
            name = temp.substr(0, i - 1);

            break;
        }
    }

    // cout<<name<<' '<<valueString<<' '<<inputs<<endl;
    // string nameString = inputs[0].first;
    if (name != "(nam")
    {
        cout << RED << "Error: Invalid column name" << RESET << endl;
        return;
    }
    // string valueString = inputs[0].second;
    if (valueString == "")
    {
        cout << RED << "Error: Invalid name value" << RESET << endl;
        return;
    }

    if (tables.find(valueString) == tables.end())
    {
        cout << RED << "Error: Table does not exist" << RESET << endl;
        return;
    }
    tables[valueString].first.parallelDelete(databaseName, inputs);
    // cout << GREEN << "Data searched successfully!" << RESET << endl;
}

void database::updateTable(string inputs)
{
    try
    {
        if (inputs.size() == 0)
        {
            cout << RED << "Error: No values to insert" << RESET << endl;
            return;
        }
        string temp = "";
        string name = "";
        string valueString = "";
        int sizeOfInputs = inputs.size();
        for (int i = 0; i < sizeOfInputs; i++)
        {
            if (inputs[i] == ',')
            {
                temp = inputs.substr(0, i);
                inputs = inputs.substr(i + 1, sizeOfInputs - i - 2);
                break;
            }
        }

        for (int i = 0; i < temp.size(); i++)
        {
            if (temp[i] == ':')
            {
                valueString = temp.substr(i + 1, temp.size() - i - 1);
                name = temp.substr(0, i - 1);

                break;
            }
        }

        // cout<<name<<' '<<valueString<<' '<<inputs<<endl;
        // string nameString = inputs[0].first;
        if (name != "(nam")
        {
            cout << RED << "Error: Invalid column name" << RESET << endl;
            return;
        }
        // string valueString = inputs[0].second;
        if (valueString == "")
        {
            cout << RED << "Error: Invalid name value" << RESET << endl;
            return;
        }

        if (tables.find(valueString) == tables.end())
        {
            cout << RED << "Error: Table does not exist" << RESET << endl;
            return;
        }

        // if (inputs.size() == 0)
        // {
        //     tables[valueString].first.search(databaseName);
        //     cout << GREEN << "Data searched successfully!" << RESET << endl;
        //     return;
        // }
        // if(inputs.size()==1){
        //     if(inputs[0].first==tables[valueString].first.indexedColumn){
        //         cout<<inputs[0].first<<' '<<inputs[0].second<<endl;
        //         tables[valueString].first.indexedSearch(databaseName, inputs[0].first,inputs[0].second);
        //         cout << GREEN << "Data searched successfully!" << RESET << endl;
        //         return;
        //     }
        // }

        // Call the update function in the corresponding table
        tables[valueString].first.parallelUpdate(databaseName, inputs);

        // cout << GREEN << "Update successful in table '" << valueString << "'\n"
            //  << RESET;
    }
    catch (string &error)
    {
        cerr << RED << error << RESET << endl;
    }
}

void database::createIndex(vector<pair<string, string>> inputs)
{
    if (inputs.size() == 0)
    {
        cout << RED << "Error: No values to insert" << RESET << endl;
        return;
    }
    string nameString = inputs[0].first;
    if (nameString != "name")
    {
        cout << RED << "Error: Invalid column name" << RESET << endl;
        return;
    }
    string valueString = inputs[0].second;
    if (valueString == "")
    {
        cout << RED << "Error: Invalid name value" << RESET << endl;
        return;
    }

    if (tables.find(valueString) == tables.end())
    {
        cout << RED << "Error: Table does not exist" << RESET << endl;
        return;
    }
    inputs.erase(inputs.begin());
    if (inputs.size() == 0)
    {
        cout << "RED" << "invalid query: no column name provided" << RESET << endl;
        return;
    }
    if (inputs[0].first == "col")
    {
        tables[valueString].first.createIndex(databaseName, inputs[0].second, 0);
        cout << GREEN << "Index created successfully!" << RESET << endl;
        return;
    }
    cout << "RED" << "invalid query: no column name provided" << RESET << endl;
    return;
}

void database::searchInTable(string inputs)
{
    if (inputs.size() == 0)
    {
        cout << RED << "Error: No values to insert" << RESET << endl;
        return;
    }
    string temp = "";
    string name = "";
    string valueString = "";
    int sizeOfInputs = inputs.size();
    for (int i = 0; i < sizeOfInputs; i++)
    {
        if (inputs[i] == ',')
        {
            temp = inputs.substr(0, i);
            inputs = inputs.substr(i + 1, sizeOfInputs - i - 2);
            break;
        }
    }

    for (int i = 0; i < temp.size(); i++)
    {
        if (temp[i] == ':')
        {
            valueString = temp.substr(i + 1, temp.size() - i - 1);
            name = temp.substr(0, i - 1);

            break;
        }
    }

    // cout<<name<<' '<<valueString<<' '<<inputs<<endl;
    // string nameString = inputs[0].first;
    if (name != "(nam")
    {
        cout << RED << "Error: Invalid column name" << RESET << endl;
        return;
    }
    // string valueString = inputs[0].second;
    if (valueString == "")
    {
        cout << RED << "Error: Invalid name value" << RESET << endl;
        return;
    }

    if (tables.find(valueString) == tables.end())
    {
        cout << RED << "Error: Table does not exist" << RESET << endl;
        return;
    }

    // if (inputs.size() == 0)
    // {
    //     tables[valueString].first.search(databaseName);
    //     cout << GREEN << "Data searched successfully!" << RESET << endl;
    //     return;
    // }
    // if(inputs.size()==1){
    //     if(inputs[0].first==tables[valueString].first.indexedColumn){
    //         cout<<inputs[0].first<<' '<<inputs[0].second<<endl;
    //         tables[valueString].first.indexedSearch(databaseName, inputs[0].first,inputs[0].second);
    //         cout << GREEN << "Data searched successfully!" << RESET << endl;
    //         return;
    //     }
    // }
    tables[valueString].first.parallelSearch(databaseName, inputs);
    // cout << GREEN << "Data searched successfully!" << RESET << endl;
}

void database::showTables()
{
    try
    {
        cout << "Database Name: " << databaseName << endl;
        cout << "Tables in the database: " << endl;
        for (auto i : tables)
        {
            cout << i.first << endl;
        }
        // cout<<endl;
        cout << "Total number of tables: " << tables.size() << endl;
        // cout << endl;
    }
    catch (exception &e)
    {
        cout << "error at line " << __LINE__ << " " << e.what() << endl;
    }
}




