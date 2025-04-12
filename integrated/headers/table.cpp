#include <bits/stdc++.h>
#include <fstream>
#include "table.h"
#include <filesystem>
#include <thread>
#include <future>
#define RED "\033[31m"
#define RESET "\033[0m"

using namespace std;
namespace fs = std::filesystem;

map<string, string> table::interpretRead(ifstream &file, int sizeOfLine)
{

    vector<string> answer;
    map<string, string> dataVals;

    vector<char> buffer(sizeOfLine);
    file.read(buffer.data(), sizeOfLine);
    int offset = 0;
    for (auto i : data)
    {
        if (i.second.first == "char" && i.second.second == 1)
        {
            char temp;
            std::memcpy(&temp, buffer.data() + offset, sizeof(char));
            offset += sizeof(char);
            dataVals[i.first] = std::to_string(temp);
        }
        else if (i.second.first == "char")
        {
            char temp[i.second.second];
            std::memcpy(&temp, buffer.data() + offset, sizeof(char) * i.second.second);
            offset += sizeof(char) * i.second.second;
            dataVals[i.first] = string(temp);
        }
        else if (i.second.first == "bool")
        {
            bool temp;
            std::memcpy(&temp, buffer.data() + offset, sizeof(bool));
            offset += sizeof(bool);
            dataVals[i.first] = std::to_string(temp);
        }
        else if (i.second.first == "int")
        {
            int temp;
            std::memcpy(&temp, buffer.data() + offset, sizeof(int));
            offset += sizeof(int);
            dataVals[i.first] = std::to_string(temp);
        }
        else if (i.second.first == "double")
        {
            double temp;
            std::memcpy(&temp, buffer.data() + offset, sizeof(double));
            offset += sizeof(double);
            dataVals[i.first] = std::to_string(temp);
        }
    }
    return dataVals;
}

vector<string> table::interpretRead(ifstream &file)
{
    vector<string> answer;
    for (auto i : data)
    {
        const string colName = i.first;
        const string type = i.second.first;
        const size_t size = i.second.second;
        if (type == "char")
        {
            if (size == 1)
            {
                char value;
                file.read(reinterpret_cast<char *>(&value), sizeof(char));

                answer.push_back(string(1, value));
            }
            else
            {
                char value[size];
                file.read(reinterpret_cast<char *>(value), sizeof(char) * size);
                answer.push_back(value);
            }
        }
        else if (type == "int")
        {
            int value;
            file.read(reinterpret_cast<char *>(&value), sizeof(int));
            answer.push_back(to_string(value));
        }
        else if (type == "double")
        {
            double value;
            file.read(reinterpret_cast<char *>(&value), sizeof(double));
            answer.push_back(to_string(value));
        }
        else if (type == "bool")
        {
            bool value;
            file.read(reinterpret_cast<char *>(&value), sizeof(bool));
            answer.push_back(to_string(value));
        }
    }
    return answer;
}

bool table::interpretWrite(ofstream &file, vector<string> &values)
{
    for (int i = 0; i < data.size(); i++)
    {
        const string colName = data[i].first;
        const string type = data[i].second.first;
        const size_t size = data[i].second.second;
        if (type == "char")
        {
            vector<char> value(size, '\0');
            strncpy(value.data(), values[i].c_str(), min(size, values[i].length()));
            file.write(value.data(), size * sizeof(char));
        }
        else if (type == "int")
        {
            int value = stoi(values[i]);
            file.write(reinterpret_cast<char *>(&value), sizeof(int));
        }
        else if (type == "double")
        {
            double value = stod(values[i]);
            file.write(reinterpret_cast<char *>(&value), sizeof(double));
        }
        else if (type == "bool")
        {
            bool value = (values[i] == "1" || values[i] == "true");
            file.write(reinterpret_cast<char *>(&value), sizeof(bool));
        }
    }
    return true;
}

void table::getMetaData(const string databaseName, const string fileName)
{
    fstream metaDataFile(fileName);
    if (!metaDataFile)
    {
        cerr << "Error opening metadata file!" << endl;
        return;
    }

    string line;
    while (!metaDataFile.eof())
    {
        getline(metaDataFile, line);
        if (line.empty())
            continue;
        if (line[0] == '2')
        {
            numberOfRecords = stoi(line.substr(1, line.size() - 1));
        }
        else if (line[0] == '3')
        {

            string temp = line.substr(1, line.size() - 1);
            if (temp == "NULL")
            {
                isIndexed = false;
            }
            else
            {
                isIndexed = true;
                indexedColumn = temp;
                this->createIndex(databaseName, indexedColumn, 1);
            }
        }
        else if (line[0] == '1')
        {
            line = line.substr(1, line.size() - 1);
            if (line != "")
            {
                string columnName, columnType, size;
                int n = line.size();
                string temp = "";
                int ctr = 0;

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
                            columnName = temp;
                        }
                        else if (ctr == 1)
                        {
                            columnType = temp;
                        }
                        else
                        {
                            size = temp;
                        }
                        temp = "";
                        ctr++;
                    }
                }
                if (temp.size() > 0)
                    size = temp;
                data.push_back({columnName, {columnType, stoi(size)}});
            }
        }
        else
        {
            tableName = line.substr(1, line.size() - 1);
        }
    }
}

table::table()
{
}

table::table(string databaseName, string fileName)
{
    isIndexed = false;
    indexedColumn = "NULL";
    getMetaData(databaseName, fileName);
}

table::table(string tableName, string databaseName, vector<pair<string, string>> columns)
{

    isIndexed = false;
    indexedColumn = "NULL";
    string folderName = "databases/" + databaseName + "/" + tableName;
    if (std::filesystem::create_directory(folderName))
    {
    }
    else
    {
        cout << "Failed to create folder or it already exists." << endl;
    }
    string tableMetaDataFilePath = folderName + "/" + tableName + ".TMData.csv";

    ofstream tableMetaDataFile(tableMetaDataFilePath, std::ios::out);
    if (!tableMetaDataFile)
    {
        cerr << "Error opening metadata file!" << endl;
        return;
    }
    tableMetaDataFile << "0" << tableName << endl;
    for (auto i : columns)
    {
        if (i.first == "name")
            continue;

        string temp = "";
        int k;
        for (k = 0; k < i.second.size(); k++)
        {
            if (i.second[k] != '[')
                temp += i.second[k];
            else
                break;
        }
        int size = 1;
        string temp2 = "";
        k++;
        for (; k < i.second.size(); k++)
        {
            if (i.second[k] != ']')
                temp2 += i.second[k];
            else
                break;
        }
        if (temp2.size() > 0)
            size = stoi(temp2);

        tableMetaDataFile << "1" << i.first << "," << temp << "," << size << endl;
        data.push_back({i.first, {temp, size}});
    }
    this->tableName = tableName;
    numberOfRecords = 0;
    tableMetaDataFile << "2" << numberOfRecords << endl;
    tableMetaDataFile << "3" << "NULL" << endl;

    tableMetaDataFile.close();
}

void table::selfDestruct()
{
    cout << "Deleting table: " << tableName << endl;
}

void table::printMetaData()
{
    cout << "Table-Name: " << tableName << endl
         << endl;
    for (auto i : data)
    {
        cout << "Column-Name: " << i.first << endl;
        if (i.second.second == 1)
        {
            cout << "Column-Type: " << i.second.first << endl;
        }
        else
        {
            cout << "Column-Type: " << i.second.first << '[' << i.second.second << ']' << endl;
        }
        cout << endl;
    }
}

bool table::insert(string databaseName, vector<string> inputs)
{
    if (inputs.size() != data.size())
    {
        cout << RED << "Error: Number of columns in the table and the number of values to insert do not match" << RESET << endl;
        return false;
    }
    for (int i = 0; i < data.size(); i++)
    {
        const string colName = data[i].first;
        const string type = data[i].second.first;
        const size_t size = data[i].second.second;

        if (type == "char")
        {
            vector<char> value(size, '\0');
            try
            {
                strncpy(value.data(), inputs[i].c_str(), min(size, inputs[i].length()));
            }
            catch (const invalid_argument &e)
            {
                cout << RED << "Error: Invalid value for column " << colName << RESET << endl;
                return false;
            }
        }
        else if (type == "int")
        {
            try
            {
                int value = stoi(inputs[i]);
            }
            catch (const invalid_argument &e)
            {
                cout << RED << "Error: Invalid value for column " << colName << RESET << endl;
                return false;
            }
        }
        else if (type == "double")
        {
            try
            {
                double value = stod(inputs[i]);
            }
            catch (const invalid_argument &e)
            {
                cout << RED << "Error: Invalid value for column " << colName << RESET << endl;
                return false;
            }
        }
        else if (type == "bool")
        {
            try
            {
                bool value = (inputs[i] == "1" || inputs[i] == "true");
            }
            catch (const invalid_argument &e)
            {
                cout << RED << "Error: Invalid value for column " << colName << RESET << endl;
                return false;
            }
        }
    }
    int fileNumber = 1;

    fileNumber = numberOfRecords / 1000 + 1;

    string tableDataFilePath = "databases/" + databaseName + "/" + tableName + "/" + tableName + to_string(fileNumber) + ".bin";
    ofstream tableDataFile(tableDataFilePath, ios::app);
    if (!tableDataFile)
    {
        cout << RED << "Error: Failed to open table data file" << RESET << endl;
        return false;
    }

    interpretWrite(tableDataFile, inputs);
    tableDataFile.close();
    ++numberOfRecords;
    updateRowNumber(databaseName);
    return true;
}

bool table::deleteX(string databaseName, int i, string mp)
{
    try
    {
        try
        {
            ifstream tableDataFile("databases/" + databaseName + "/" + tableName + "/" + tableName + to_string(i) + ".bin", ios::in | ios::binary);

            if (!tableDataFile)
            {
                throw runtime_error("Failed to open table data file: " + tableName + to_string(i) + ".bin");
            }
            try
            {
                vector<map<string, string>> answer;

                map<string, pair<string, int>> cols;

                for (int i = 0; i < data.size(); i++)
                {
                    string temp = "char";
                    if (data[i].second.first == "char" && data[i].second.second > 1)
                    {
                        temp = "string";
                        cols.insert({data[i].first, {temp, i}});
                    }
                    else
                    {
                        temp = data[i].second.first;
                        cols.insert({data[i].first, {temp, i}});
                    }
                }
                int ctr = 0;
                vector<pair<string, string>> cond;
                vector<string> between;
                string temp = "";
                string value = "";
                string condition = "";
                bool check = true;
                string expression = "";
                for (int i = 0; i < mp.size(); i++)
                {
                    if (mp[i] == '&' || mp[i] == '|')
                    {
                        cond.push_back({temp, value});
                        between.push_back(condition);
                        expression += mp[i];
                        temp = "";
                        value = "";
                        condition = "";
                        check = true;
                    }
                    else if (mp[i] == '=' && mp[i + 1] == '=')
                    {
                        condition = "==";
                        i++;
                        check = false;
                    }
                    else if (mp[i] == '>' && mp[i + 1] == '=')
                    {
                        condition = ">=";
                        i++;
                        check = false;
                    }
                    else if (mp[i] == '<' && mp[i + 1] == '=')
                    {
                        condition = "<=";
                        i++;
                        check = false;
                    }
                    else if (mp[i] == '!' && mp[i + 1] == '=')
                    {
                        condition = "!=";
                        i++;
                        check = false;
                    }
                    else if (mp[i] == '<')
                    {
                        condition = "<";
                        i++;
                        check = false;
                    }
                    else if (mp[i] == '>')
                    {
                        condition = ">";
                        i++;
                        check = false;
                    }
                    else if (check && mp[i] != '{' && mp[i] != '}')
                    {
                        temp += mp[i];
                    }
                    else if (mp[i] != '{' && mp[i] != '}')
                    {
                        value += mp[i];
                    }
                    else
                    {
                        if (mp[i] == '}')
                        {
                            if (temp != "" && condition != "" && value != "")
                            {
                                cond.push_back({temp, value});
                                between.push_back(condition);
                            }
                            temp = "";
                            value = "";
                            condition = "";
                            check = true;
                        }
                        expression += mp[i];
                    }
                }

                int sizeOfLine = getTotalBytes(this->data);
                int rowNumber = 0;

                while (true)
                {
                    rowNumber++;

                    map<string, string> b = interpretRead(tableDataFile, sizeOfLine);
                    vector<bool> checks(cond.size());

                    for (int i = 0; i < cond.size(); i++)
                    {

                        if (cols[cond[i].first].first == "int")
                        {
                            int one = stoi(b[cond[i].first]);
                            int two = stoi(cond[i].second);

                            if (between[i] == "==")
                            {
                                if (one == two)
                                {
                                    checks[i] = true;
                                }
                            }
                            else if (between[i] == "!=")
                            {
                                if (one != two)
                                {
                                    checks[i] = true;
                                }
                            }
                            else if (between[i] == "<=")
                            {
                                if (one <= two)
                                {
                                    checks[i] = true;
                                }
                            }
                            else if (between[i] == "<")
                            {
                                if (one < two)
                                {
                                    checks[i] = true;
                                }
                            }
                            else if (between[i] == ">=")
                            {
                                if (one >= two)
                                {
                                    checks[i] = true;
                                }
                            }
                            else if (between[i] == ">")
                            {
                                if (one > two)
                                {
                                    checks[i] = true;
                                }
                            }
                        }
                        else if (cols[cond[i].first].first == "double")
                        {
                            double one = stod(b[cond[i].first]);
                            double two = stod(cond[i].second);
                            if (between[i] == "==")
                            {
                                if (one == two)
                                {
                                    checks[i] = true;
                                }
                            }
                            else if (between[i] == "!=")
                            {
                                if (one != two)
                                {
                                    checks[i] = true;
                                }
                            }
                            else if (between[i] == "<=")
                            {
                                if (one <= two)
                                {
                                    checks[i] = true;
                                }
                            }
                            else if (between[i] == "<")
                            {
                                if (one < two)
                                {
                                    checks[i] = true;
                                }
                            }
                            else if (between[i] == ">=")
                            {
                                if (one >= two)
                                {
                                    checks[i] = true;
                                }
                            }
                            else if (between[i] == ">")
                            {
                                if (one > two)
                                {
                                    checks[i] = true;
                                }
                            }
                        }
                        else if (cols[cond[i].first].first == "char")
                        {
                            char one = b[cond[i].first][0];
                            char two = cond[i].second[0];
                            if (between[i] == "==")
                            {
                                if (one == two)
                                {
                                    checks[i] = true;
                                }
                            }
                            else if (between[i] == "!=")
                            {
                                if (one != two)
                                {
                                    checks[i] = true;
                                }
                            }
                            else if (between[i] == "<=")
                            {
                                if (one <= two)
                                {
                                    checks[i] = true;
                                }
                            }
                            else if (between[i] == "<")
                            {
                                if (one < two)
                                {
                                    checks[i] = true;
                                }
                            }
                            else if (between[i] == ">=")
                            {
                                if (one >= two)
                                {
                                    checks[i] = true;
                                }
                            }
                            else if (between[i] == ">")
                            {
                                if (one > two)
                                {
                                    checks[i] = true;
                                }
                            }
                        }
                        else if (cols[cond[i].first].first == "string")
                        {
                            string one = (b[cond[i].first]);
                            string two = (cond[i].second);
                            if (between[i] == "==")
                            {
                                if (one == two)
                                {
                                    checks[i] = true;
                                }
                            }
                            else if (between[i] == "!=")
                            {
                                if (one != two)
                                {
                                    checks[i] = true;
                                }
                            }
                            else if (between[i] == "<=")
                            {
                                if (one <= two)
                                {
                                    checks[i] = true;
                                }
                            }
                            else if (between[i] == "<")
                            {
                                if (one < two)
                                {
                                    checks[i] = true;
                                }
                            }
                            else if (between[i] == ">=")
                            {
                                if (one >= two)
                                {
                                    checks[i] = true;
                                }
                            }
                            else if (between[i] == ">")
                            {
                                if (one > two)
                                {
                                    checks[i] = true;
                                }
                            }
                        }
                        else if (cols[cond[i].first].first == "bool")
                        {
                            int one = stoi(b[cond[i].first]);
                            int two = stoi(cond[i].second);
                            if (between[i] == "==")
                            {
                                if (one == two)
                                {
                                    checks[i] = true;
                                }
                            }
                            else if (between[i] == "!=")
                            {
                                if (one != two)
                                {
                                    checks[i] = true;
                                }
                            }
                            else if (between[i] == "<=")
                            {
                                if (one <= two)
                                {
                                    checks[i] = true;
                                }
                            }
                            else if (between[i] == "<")
                            {
                                if (one < two)
                                {
                                    checks[i] = true;
                                }
                            }
                            else if (between[i] == ">=")
                            {
                                if (one >= two)
                                {
                                    checks[i] = true;
                                }
                            }
                            else if (between[i] == ">")
                            {
                                if (one > two)
                                {
                                    checks[i] = true;
                                }
                            }
                        }
                    }
                    if (checks.size() == 1)
                    {
                        if (checks[0] == true)
                        {
                            continue;
                            cout << '$' << endl;
                            for (auto i : b)
                            {
                                cout << i.first << ' ' << i.second << endl;
                            }
                        }
                        else
                        {
                            answer.push_back(b);
                        }
                    }
                    else
                    {

                        bool check = evaluateExpr(expression, checks);
                        if (!check)
                        {
                            answer.push_back(b);
                        }
                    }
                    if (tableDataFile.eof())
                        break;
                }

                try
                {
                    string tableDataFilePath = "databases/" + databaseName + "/" + tableName + "/" + tableName + to_string(i) + ".bin";
                    ofstream tableDataFile(tableDataFilePath, ios::out | ios::binary);
                    if (!tableDataFile)
                    {
                        cout << RED << "Error: Failed to open table data file" << RESET << endl;
                        return false;
                    }
                    int k = answer.size();
                    k--;
                    for (auto i : answer)
                    {
                        if (k == 0)
                            break;
                        k--;
                        vector<string> toWrite;
                        for (auto k : data)
                        {
                            toWrite.push_back(i[k.first]);
                        }
                        interpretWrite(tableDataFile, toWrite);
                    }
                    tableDataFile.close();
                }
                catch (const exception &e)
                {
                    cerr << RED << "Error processing data at line " << __LINE__ << ": " << e.what() << RESET << endl;
                }
            }
            catch (const exception &e)
            {
                cerr << RED << "Error processing data at line " << __LINE__ << ": " << e.what() << RESET << endl;
            }

            tableDataFile.close();
        }
        catch (const exception &e)
        {
            cerr << RED << "Error processing file at line " << __LINE__ << ": " << e.what() << RESET << endl;
            return false;
        }

        return true;
    }
    catch (const exception &e)
    {
        cerr << RED << "Fatal error in search function at line " << __LINE__ << ": " << e.what() << RESET << endl;
        return false;
    }
}

bool table::updateRowNumber(string databaseName)
{
    ifstream tableDataFile("databases/" + databaseName + "/" + tableName + "/" + tableName + ".TMData.csv", ios::in);
    if (!tableDataFile)
    {
        cout << "Error: Failed to open table data file" << endl;
        return false;
    }
    int rowNumber = 0;
    vector<string> answer;
    while (!tableDataFile.eof())
    {
        string line;
        getline(tableDataFile, line);
        answer.push_back(line);
    }
    tableDataFile.close();
    remove(("databases/" + databaseName + "/" + tableName + "/" + tableName + ".TMData.csv").c_str());
    ofstream tableDataFile2("databases/" + databaseName + "/" + tableName + "/" + tableName + ".TMData.csv", ios::out);
    if (!tableDataFile2)
    {
        cout << "Error: Failed to open table data file" << endl;
        return false;
    }
    for (auto i : answer)
    {
        if (i[0] == '2')
        {
            tableDataFile2 << "2" << numberOfRecords << endl;
        }
        else if (i[0] == '1' || i[0] == '0')
        {
            tableDataFile2 << i << endl;
        }
        else if (i[0] == '3')
        {
            tableDataFile2 << "3" << indexedColumn << endl;
        }
        else
            continue;
    }
    tableDataFile.close();
    return true;
}

bool table::update(string databaseName, int j, string mp)
{
    try
    {
        try
        {
            ifstream tableDataFile("databases/" + databaseName + "/" + tableName + "/" + tableName + to_string(j) + ".bin", ios::in | ios::binary);

            if (!tableDataFile)
            {
                throw runtime_error("Failed to open table data file: " + tableName + to_string(j) + ".bin");
            }
            try
            {
                vector<map<string, string>> answer;
                map<string, pair<string, int>> cols;
                vector<map<string, string>> updateData;
                map<string, string> toUpdate;
                for (int i = 0; i < data.size(); i++)
                {
                    string temp = "char";
                    if (data[i].second.first == "char" && data[i].second.second > 1)
                    {
                        temp = "string";
                        cols.insert({data[i].first, {temp, i}});
                    }
                    else
                    {
                        temp = data[i].second.first;
                        cols.insert({data[i].first, {temp, i}});
                    }
                }
                int ctr = 0;
                vector<pair<string, string>> cond;
                vector<string> between;
                string temp = "";
                string value = "";
                string condition = "";
                bool check = true;
                string expression = "";
                int i = 0;
                for (; i < mp.size(); i++)
                {
                    if (mp[i] == '$')
                    {
                        break;
                    }
                    if (mp[i] == '&' || mp[i] == '|')
                    {
                        cond.push_back({temp, value});
                        between.push_back(condition);
                        expression += mp[i];
                        temp = "";
                        value = "";
                        condition = "";
                        check = true;
                    }
                    else if (mp[i] == '=' && mp[i + 1] == '=')
                    {
                        condition = "==";
                        i++;
                        check = false;
                    }
                    else if (mp[i] == '>' && mp[i + 1] == '=')
                    {
                        condition = ">=";
                        i++;
                        check = false;
                    }
                    else if (mp[i] == '<' && mp[i + 1] == '=')
                    {
                        condition = "<=";
                        i++;
                        check = false;
                    }
                    else if (mp[i] == '>' && mp[i + 1] == '=')
                    {
                        condition = ">=";
                        i++;
                        check = false;
                    }
                    else if (mp[i] == '<')
                    {
                        condition = "<";
                        i++;
                        check = false;
                    }
                    else if (mp[i] == '>')
                    {
                        condition = ">";
                        i++;
                        check = false;
                    }
                    else if (check && mp[i] != '{' && mp[i] != '}')
                    {
                        temp += mp[i];
                    }
                    else if (mp[i] != '{' && mp[i] != '}')
                    {
                        value += mp[i];
                    }
                    else
                    {
                        if (mp[i] == '}')
                        {
                            if (temp != "" && condition != "" && value != "")
                            {
                                cond.push_back({temp, value});
                                between.push_back(condition);
                            }
                            temp = "";
                            value = "";
                            condition = "";
                            check = true;
                        }
                        expression += mp[i];
                    }
                }

                i += 3;

                string temp2 = "";
                string value2 = "";
                bool t = 1;
                for (; i < mp.size(); i++)
                {
                    if (mp[i] == '}')
                        break;
                    if (mp[i] == ',')
                    {
                        toUpdate.insert({temp2, value2});
                        temp2 = "";
                        value2 = "";
                        t = 1;
                    }
                    else if (mp[i] == ':')
                    {
                        t = 0;
                    }
                    else if (t)
                    {
                        temp2 += mp[i];
                    }
                    else
                    {
                        value2 += mp[i];
                    }
                }
                if (temp2.size() > 0 && value2.size() > 0)
                {
                    toUpdate.insert({temp2, value2});
                }

                int sizeOfLine = getTotalBytes(this->data);
                int rowNumber = 0;
                while (!tableDataFile.eof())
                {
                    rowNumber++;
                    map<string, string> b = interpretRead(tableDataFile, sizeOfLine);
                    vector<bool> checks(cond.size());

                    for (int i = 0; i < cond.size(); i++)
                    {

                        if (cols[cond[i].first].first == "int")
                        {
                            int one = stoi(b[cond[i].first]);
                            int two = stoi(cond[i].second);

                            if (between[i] == "==")
                            {
                                if (one == two)
                                {
                                    checks[i] = true;
                                }
                            }
                            else if (between[i] == "!=")
                            {
                                if (one != two)
                                {
                                    checks[i] = true;
                                }
                            }
                            else if (between[i] == "<=")
                            {
                                if (one <= two)
                                {
                                    checks[i] = true;
                                }
                            }
                            else if (between[i] == "<")
                            {
                                if (one < two)
                                {
                                    checks[i] = true;
                                }
                            }
                            else if (between[i] == ">=")
                            {
                                if (one >= two)
                                {
                                    checks[i] = true;
                                }
                            }
                            else if (between[i] == ">")
                            {
                                if (one > two)
                                {
                                    checks[i] = true;
                                }
                            }
                        }
                        else if (cols[cond[i].first].first == "double")
                        {
                            double one = stod(b[cond[i].first]);
                            double two = stod(cond[i].second);
                            if (between[i] == "==")
                            {
                                if (one == two)
                                {
                                    checks[i] = true;
                                }
                            }
                            else if (between[i] == "!=")
                            {
                                if (one != two)
                                {
                                    checks[i] = true;
                                }
                            }
                            else if (between[i] == "<=")
                            {
                                if (one <= two)
                                {
                                    checks[i] = true;
                                }
                            }
                            else if (between[i] == "<")
                            {
                                if (one < two)
                                {
                                    checks[i] = true;
                                }
                            }
                            else if (between[i] == ">=")
                            {
                                if (one >= two)
                                {
                                    checks[i] = true;
                                }
                            }
                            else if (between[i] == ">")
                            {
                                if (one > two)
                                {
                                    checks[i] = true;
                                }
                            }
                        }
                        else if (cols[cond[i].first].first == "char")
                        {
                            char one = stoi(b[cond[i].first]);
                            char two = cond[i].second[0];

                            if (between[i] == "==")
                            {
                                if (one == two)
                                {
                                    checks[i] = true;
                                }
                            }
                            else if (between[i] == "!=")
                            {
                                if (one != two)
                                {
                                    checks[i] = true;
                                }
                            }
                            else if (between[i] == "<=")
                            {
                                if (one <= two)
                                {
                                    checks[i] = true;
                                }
                            }
                            else if (between[i] == "<")
                            {
                                if (one < two)
                                {
                                    checks[i] = true;
                                }
                            }
                            else if (between[i] == ">=")
                            {
                                if (one >= two)
                                {
                                    checks[i] = true;
                                }
                            }
                            else if (between[i] == ">")
                            {
                                if (one > two)
                                {
                                    checks[i] = true;
                                }
                            }
                        }
                        else if (cols[cond[i].first].first == "string")
                        {
                            string one = (b[cond[i].first]);
                            string two = (cond[i].second);
                            if (between[i] == "==")
                            {
                                if (one == two)
                                {
                                    checks[i] = true;
                                }
                            }
                            else if (between[i] == "!=")
                            {
                                if (one != two)
                                {
                                    checks[i] = true;
                                }
                            }
                            else if (between[i] == "<=")
                            {
                                if (one <= two)
                                {
                                    checks[i] = true;
                                }
                            }
                            else if (between[i] == "<")
                            {
                                if (one < two)
                                {
                                    checks[i] = true;
                                }
                            }
                            else if (between[i] == ">=")
                            {
                                if (one >= two)
                                {
                                    checks[i] = true;
                                }
                            }
                            else if (between[i] == ">")
                            {
                                if (one > two)
                                {
                                    checks[i] = true;
                                }
                            }
                        }
                        else if (cols[cond[i].first].first == "bool")
                        {
                            int one = stoi(b[cond[i].first]);
                            int two = stoi(cond[i].second);
                            if (between[i] == "==")
                            {
                                if (one == two)
                                {
                                    checks[i] = true;
                                }
                            }
                            else if (between[i] == "!=")
                            {
                                if (one != two)
                                {
                                    checks[i] = true;
                                }
                            }
                            else if (between[i] == "<=")
                            {
                                if (one <= two)
                                {
                                    checks[i] = true;
                                }
                            }
                            else if (between[i] == "<")
                            {
                                if (one < two)
                                {
                                    checks[i] = true;
                                }
                            }
                            else if (between[i] == ">=")
                            {
                                if (one >= two)
                                {
                                    checks[i] = true;
                                }
                            }
                            else if (between[i] == ">")
                            {
                                if (one > two)
                                {
                                    checks[i] = true;
                                }
                            }
                        }
                    }
                    if (checks.size() == 1)
                    {
                        if (checks[0] == true)
                        {
                            updateData.push_back(b);
                            continue;
                        }
                        else
                        {
                            answer.push_back(b);
                        }
                    }
                    else
                    {

                        bool check = evaluateExpr(expression, checks);
                        if (check)
                        {
                            updateData.push_back(b);
                        }
                        else
                        {

                            answer.push_back(b);
                        }
                    }
                }
                try
                {
                    string tableDataFilePath = "databases/" + databaseName + "/" + tableName + "/" + tableName + to_string(j) + ".bin";
                    ofstream tableDataFile(tableDataFilePath, ios::out | ios::binary);
                    if (!tableDataFile)
                    {
                        cout << RED << "Error: Failed to open table data file" << RESET << endl;
                        return false;
                    }
                    // cout<<'$'<<answer.size()<<endl;
                    int k = answer.size();
                    k--;
                    for (auto i : answer)
                    {
                        if (k == 0)
                            break;
                        k--;
                        vector<string> toWrite;
                        for (auto k : data)
                        {
                            toWrite.push_back(i[k.first]);
                        }
                        interpretWrite(tableDataFile, toWrite);
                    }
                    // cout<<'#'<<updateData.size()<<endl;
                    for (auto &i : updateData)
                    {

                        for (auto k : toUpdate)
                        {
                            i[k.first] = k.second;
                        }

                        vector<string> toWrite;
                        for (auto k : data)
                        {
                            toWrite.push_back(i[k.first]);
                        }
                        interpretWrite(tableDataFile, toWrite);
                    }
                }
                catch (const exception &e)
                {
                    cerr << RED << "Error processing data at line " << __LINE__ << ": " << e.what() << RESET << endl;
                }
            }
            catch (const exception &e)
            {
                cerr << RED << "Error processing data at line " << __LINE__ << ": " << e.what() << RESET << endl;
            }

            tableDataFile.close();
        }
        catch (const exception &e)
        {
            cerr << RED << "Error processing file at line " << __LINE__ << ": " << e.what() << RESET << endl;
            return false;
        }

        return true;
    }
    catch (const exception &e)
    {
        cerr << RED << "Fatal error in search function at line " << __LINE__ << ": " << e.what() << RESET << endl;
        return false;
    }
}

bool table::parallelUpdate(string databaseName, string inputs)
{
    try
    {
        if (inputs.size() < 6)
        {
            cout << RED << "invalid condtion\n"
                 << RESET << endl;
            return false;
        }
        if (inputs == "{}")
        {
            throw runtime_error("no condition provided");
            return false;
        }
        string c = "", u = "";
        bool k = false;
        if (inputs[0] != '{' || inputs[inputs.size() - 1] != '}')
        {
            cerr << RED << "Error in parsing condition: " << inputs << RESET << endl;
            return false;
        }

        for (int i = 0; i < inputs.size(); i++)
        {
            if (!k && inputs[i] != '$')
            {

                c += inputs[i];
            }
            else if (inputs[i] != '$')
            {
                u += inputs[i];
            }
            else
            {
                k = true;
                i++;
            }
        }

        bool parseCheck = parseCondition(c);
        if (!parseCheck)
        {
            cerr << RED << "Error in parsing condition: " << inputs << RESET << endl;
            return false;
        }
        parseCheck=parseCondition(u,true);
        if (!parseCheck)
        {
            cerr << RED << "Error in parsing condition: " << inputs << RESET << endl;
            return false;
        }
        int numberOfFiles = numberOfRecords / 1000 + 1;

        std::vector<std::future<bool>> futures;

        int maxThreads = 2;

        for (int i = 1; i <= numberOfFiles; i++)
        {
            if (futures.size() >= maxThreads)
            {

                for (auto &f : futures)
                    f.get();
                futures.clear();
            }

            futures.push_back(std::async(std::launch::async, [this, databaseName, i, inputs]()
                                         { return this->update(databaseName, i, inputs); }));
        }

        for (auto &f : futures)
            f.get();

        return true;
    }
    catch (const std::exception &e)
    {
        std::cerr << RED << "Error : " << e.what() << RESET << std::endl;
        return false;
    }
}

bool table::parallelSearch(string databaseName, string inputs)
{
    try
    {
        if (inputs.size() < 2)
        {
            cout << RED << "ERROR in parsing condition: " << inputs << RESET << endl;
            return false;
        }
        if (inputs == "{}")
        {
            this->search(databaseName);
            return true;
        }
        if (inputs[0] != '{' || inputs[inputs.size() - 1] != '}')
        {
            cerr << RED << "Error in parsing condition: " << inputs << RESET << endl;
            return false;
        }
        bool parseCheck = parseCondition(inputs);
        if (!parseCheck)
        {
            cerr << RED << "Error in parsing condition: " << inputs << RESET << endl;
            return false;
        }
        int numberOfFiles = numberOfRecords / 1000 + 1;

        std::vector<std::future<bool>> futures;

        int maxThreads = 2;

        for (int i = 1; i <= numberOfFiles; i++)
        {
            if (futures.size() >= maxThreads)
            {

                for (auto &f : futures)
                    f.get();
                futures.clear();
            }

            futures.push_back(std::async(std::launch::async, [this, databaseName, i, inputs]()
                                         { return this->search(databaseName, i, inputs); }));
        }

        for (auto &f : futures)
            f.get();

        return true;
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error in parallelSearch: " << e.what() << std::endl;
        return false;
    }
}

bool table::parallelDelete(string databaseName, string inputs)
{
    try
    {
        if (inputs == "{}")
        {
            throw runtime_error("no condition provided");
            return false;
        }
        if (inputs[0] != '{' || inputs[inputs.size() - 1] != '}')
        {
            cerr << RED << "Error in parsing condition: " << inputs << RESET << endl;
            return false;
        }
        bool parseCheck = parseCondition(inputs);
        if (!parseCheck)
        {
            cerr << RED << "Error in parsing condition: " << inputs << RESET << endl;
            return false;
        }
        int numberOfFiles = numberOfRecords / 1000 + 1;

        std::vector<std::future<bool>> futures;

        int maxThreads = 2;

        for (int i = 1; i <= numberOfFiles; i++)
        {
            if (futures.size() >= maxThreads)
            {

                for (auto &f : futures)
                    f.get();
                futures.clear();
            }

            futures.push_back(std::async(std::launch::async, [this, databaseName, i, inputs]()
                                         { return this->deleteX(databaseName, i, inputs); }));
        }

        for (auto &f : futures)
            f.get();

        return true;
    }
    catch (const std::exception &e)
    {
        std::cerr << RED << "Error : " << e.what() << RESET << std::endl;
        return false;
    }
}

bool table::search(string databaseName, int i, string mp)
{
    try
    {
        try
        {
            ifstream tableDataFile("databases/" + databaseName + "/" + tableName + "/" + tableName + to_string(i) + ".bin", ios::in | ios::binary);

            if (!tableDataFile)
            {
                throw runtime_error("Failed to open table data file: " + tableName + to_string(i) + ".bin");
            }
            try
            {
                vector<vector<string>> answer;
                map<string, pair<string, int>> cols;
                for (int i = 0; i < data.size(); i++)
                {
                    string temp = "char";
                    if (data[i].second.first == "char" && data[i].second.second > 1)
                    {
                        temp = "string";
                        cols.insert({data[i].first, {temp, i}});
                    }
                    else
                    {
                        temp = data[i].second.first;
                        cols.insert({data[i].first, {temp, i}});
                    }
                }
                int ctr = 0;
                vector<pair<string, string>> cond;
                vector<string> between;
                string temp = "";
                string value = "";
                string condition = "";
                bool check = true;
                string expression = "";
                for (int i = 0; i < mp.size(); i++)
                {
                    if (mp[i] == '&' || mp[i] == '|')
                    {
                        cond.push_back({temp, value});
                        between.push_back(condition);
                        expression += mp[i];
                        temp = "";
                        value = "";
                        condition = "";
                        check = true;
                    }
                    else if (mp[i] == '=' && mp[i + 1] == '=')
                    {
                        condition = "==";
                        i++;
                        check = false;
                    }
                    else if (mp[i] == '>' && mp[i + 1] == '=')
                    {
                        condition = ">=";
                        i++;
                        check = false;
                    }
                    else if (mp[i] == '<' && mp[i + 1] == '=')
                    {
                        condition = "<=";
                        i++;
                        check = false;
                    }
                    else if (mp[i] == '<')
                    {
                        condition = "<";
                        i++;
                        check = false;
                    }
                    else if (mp[i] == '>')
                    {
                        condition = ">";
                        i++;
                        check = false;
                    }
                    else if (check && mp[i] != '{' && mp[i] != '}')
                    {
                        temp += mp[i];
                    }
                    else if (mp[i] != '{' && mp[i] != '}')
                    {
                        value += mp[i];
                    }
                    else
                    {
                        if (mp[i] == '}')
                        {
                            if (temp != "" && condition != "" && value != "")
                            {
                                cond.push_back({temp, value});
                                between.push_back(condition);
                            }
                            temp = "";
                            value = "";
                            condition = "";
                            check = true;
                        }
                        expression += mp[i];
                    }
                }
                // cout << expression << endl;
                // for (auto i : cond)
                // {
                //     cout << i.first << ' ' << i.second << endl;
                // }
                // cout << endl;
                // for (auto i : cols)
                // {
                //     cout << i.first << ' ';
                // }
                // cout << endl;
                int sizeOfLine = getTotalBytes(this->data);
                int rowNumber = 0;
                while (!tableDataFile.eof())
                {
                    rowNumber++;

                    map<string, string> b = interpretRead(tableDataFile, sizeOfLine);
                    vector<bool> checks(cond.size());

                    for (int i = 0; i < cond.size(); i++)
                    {

                        if (cols[cond[i].first].first == "int")
                        {
                            int one = stoi(b[cond[i].first]);
                            int two = stoi(cond[i].second);

                            if (between[i] == "==")
                            {
                                if (one == two)
                                {
                                    checks[i] = true;
                                }
                            }
                            else if (between[i] == "!=")
                            {
                                if (one != two)
                                {
                                    checks[i] = true;
                                }
                            }
                            else if (between[i] == "<=")
                            {
                                if (one <= two)
                                {
                                    checks[i] = true;
                                }
                            }
                            else if (between[i] == "<")
                            {
                                if (one < two)
                                {
                                    checks[i] = true;
                                }
                            }
                            else if (between[i] == ">=")
                            {
                                if (one >= two)
                                {
                                    checks[i] = true;
                                }
                            }
                            else if (between[i] == ">")
                            {
                                if (one > two)
                                {
                                    checks[i] = true;
                                }
                            }
                        }
                        else if (cols[cond[i].first].first == "double")
                        {
                            double one = stod(b[cond[i].first]);
                            double two = stod(cond[i].second);
                            if (between[i] == "==")
                            {
                                if (one == two)
                                {
                                    checks[i] = true;
                                }
                            }
                            else if (between[i] == "!=")
                            {
                                if (one != two)
                                {
                                    checks[i] = true;
                                }
                            }
                            else if (between[i] == "<=")
                            {
                                if (one <= two)
                                {
                                    checks[i] = true;
                                }
                            }
                            else if (between[i] == "<")
                            {
                                if (one < two)
                                {
                                    checks[i] = true;
                                }
                            }
                            else if (between[i] == ">=")
                            {
                                if (one >= two)
                                {
                                    checks[i] = true;
                                }
                            }
                            else if (between[i] == ">")
                            {
                                if (one > two)
                                {
                                    checks[i] = true;
                                }
                            }
                        }
                        else if (cols[cond[i].first].first == "char")
                        {
                            char one = stoi(b[cond[i].first]);
                            char two = cond[i].second[0];

                            if (between[i] == "==")
                            {
                                if (one == two)
                                {
                                    checks[i] = true;
                                }
                            }
                            else if (between[i] == "!=")
                            {
                                if (one != two)
                                {
                                    checks[i] = true;
                                }
                            }
                            else if (between[i] == "<=")
                            {
                                if (one <= two)
                                {
                                    checks[i] = true;
                                }
                            }
                            else if (between[i] == "<")
                            {
                                if (one < two)
                                {
                                    checks[i] = true;
                                }
                            }
                            else if (between[i] == ">=")
                            {
                                if (one >= two)
                                {
                                    checks[i] = true;
                                }
                            }
                            else if (between[i] == ">")
                            {
                                if (one > two)
                                {
                                    checks[i] = true;
                                }
                            }
                        }
                        else if (cols[cond[i].first].first == "string")
                        {
                            string one = (b[cond[i].first]);
                            string two = (cond[i].second);
                            if (between[i] == "==")
                            {
                                if (one == two)
                                {
                                    checks[i] = true;
                                }
                            }
                            else if (between[i] == "!=")
                            {
                                if (one != two)
                                {
                                    checks[i] = true;
                                }
                            }
                            else if (between[i] == "<=")
                            {
                                if (one <= two)
                                {
                                    checks[i] = true;
                                }
                            }
                            else if (between[i] == "<")
                            {
                                if (one < two)
                                {
                                    checks[i] = true;
                                }
                            }
                            else if (between[i] == ">=")
                            {
                                if (one >= two)
                                {
                                    checks[i] = true;
                                }
                            }
                            else if (between[i] == ">")
                            {
                                if (one > two)
                                {
                                    checks[i] = true;
                                }
                            }
                        }
                        else if (cols[cond[i].first].first == "bool")
                        {
                            int one = stoi(b[cond[i].first]);
                            int two = stoi(cond[i].second);
                            if (between[i] == "==")
                            {
                                if (one == two)
                                {
                                    checks[i] = true;
                                }
                            }
                            else if (between[i] == "!=")
                            {
                                if (one != two)
                                {
                                    checks[i] = true;
                                }
                            }
                            else if (between[i] == "<=")
                            {
                                if (one <= two)
                                {
                                    checks[i] = true;
                                }
                            }
                            else if (between[i] == "<")
                            {
                                if (one < two)
                                {
                                    checks[i] = true;
                                }
                            }
                            else if (between[i] == ">=")
                            {
                                if (one >= two)
                                {
                                    checks[i] = true;
                                }
                            }
                            else if (between[i] == ">")
                            {
                                if (one > two)
                                {
                                    checks[i] = true;
                                }
                            }
                        }
                    }
                    if (checks.size() == 1)
                    {
                        if (checks[0] == true)
                        {
                            vector<string> temp;
                            for (auto i : b)
                            {
                                temp.push_back(i.second);
                            }
                            answer.push_back(temp);
                            continue;
                        }
                    }
                    else
                    {

                        bool check = evaluateExpr(expression, checks);
                        if (check)
                        {
                            vector<string> temp;
                            for (auto i : b)
                            {
                                temp.push_back(i.second);
                            }
                            answer.push_back(temp);
                        }
                    }
                }
                for (auto i : answer)
                {
                    for (auto j : i)
                    {
                        cout << j << ' ';
                    }
                    cout << endl;
                }
            }
            catch (const exception &e)
            {
                cerr << RED << "Error processing data at line " << __LINE__ << ": " << e.what() << RESET << endl;
            }

            tableDataFile.close();
        }
        catch (const exception &e)
        {
            cerr << RED << "Error processing file at line " << __LINE__ << ": " << e.what() << RESET << endl;
            return false;
        }

        return true;
    }
    catch (const exception &e)
    {
        cerr << RED << "Fatal error in search function at line " << __LINE__ << ": " << e.what() << RESET << endl;
        return false;
    }
}

bool table::search(string databaseName)
{
    try
    {

        int numberOfFiles = numberOfRecords / 1000 + 1;
        for (int i = 1; i <= numberOfFiles; i++)
        {
            try
            {

                ifstream tableDataFile("databases/" + databaseName + "/" + tableName + "/" + tableName + to_string(i) + ".bin", ios::in | ios::binary);

                if (!tableDataFile)
                {
                    throw runtime_error("Failed to open table data file: " + tableName + to_string(i) + ".bin");
                }

                vector<vector<string>> answer;
                try
                {
                    while (!tableDataFile.eof())
                    {
                        vector<string> check = interpretRead(tableDataFile);
                        if (!check.empty())
                        {
                            answer.push_back(check);
                        }
                    }
                }
                catch (const exception &e)
                {
                    cerr << RED << "Error reading file at line " << __LINE__ << ": " << e.what() << RESET << endl;
                    tableDataFile.close();
                    continue;
                }

                tableDataFile.close();
                if (answer.empty())
                {
                    continue;
                }
                for (auto i : data)
                {
                    cout << i.first << ' ';
                }
                cout << endl;
                for (size_t i = 0; i < answer.size() - 1; i++)
                {
                    for (size_t j = 0; j < answer[i].size(); j++)
                    {
                        cout << answer[i][j] << ' ';
                    }
                    cout << endl;
                }

                tableDataFile.close();
            }
            catch (const exception &e)
            {
                cerr << RED << "Error processing file at line " << __LINE__ << ": " << e.what() << RESET << endl;
                continue;
            }
        }
        return true;
    }
    catch (const exception &e)
    {
        cerr << RED << "Fatal error in search function at line " << __LINE__ << ": " << e.what() << RESET << endl;
        return false;
    }
}

bool table::createIndex(string databaseName, string columnName, bool init)
{
    if (init == 0)
    {

        if (isIndexed)
        {

            cout << isIndexed << endl;
            cout << "Index already exists" << endl;
            return false;
        }
        else
        {
            string folderName = "databases/" + databaseName + "/" + tableName;
            string tableMetaDataFilePath = folderName + "/" + tableName + ".TMData.csv";
            ifstream tableMetaDataFile(tableMetaDataFilePath, ios::in);
            if (!tableMetaDataFile)
            {
                throw runtime_error("Failed to open table meta data file: " + tableName + ".TMData.csv");
            }
            vector<string> temp;
            while (!tableMetaDataFile.eof())
            {
                string line;
                getline(tableMetaDataFile, line);
                temp.push_back(line);
            }
            tableMetaDataFile.close();
            ofstream tableMetaDataFile2(tableMetaDataFilePath, ios::out);
            if (!tableMetaDataFile2)
            {
                throw runtime_error("Failed to open table meta data file: " + tableName + ".TMData.csv");
            }
            for (auto i : temp)
            {
                if (i[0] == '3')
                {
                    tableMetaDataFile2 << "3" << columnName << endl;
                }
                else
                {
                    tableMetaDataFile2 << i << endl;
                }
            }
            tableMetaDataFile2.close();

            indexedColumn = columnName;
            isIndexed = true;
        }
    }
    vector<vector<string>> answer;
    int numberOfFiles = numberOfRecords / 1000 + 1;
    for (int i = 1; i <= numberOfFiles; i++)
    {
        try
        {
            ifstream tableDataFile("databases/" + databaseName + "/" + tableName + "/" + tableName + to_string(i) + ".bin", ios::in | ios::binary);

            if (!tableDataFile)
            {
                throw runtime_error("Failed to open table data file: " + tableName + to_string(i) + ".bin");
            }

            try
            {
                while (!tableDataFile.eof())
                {
                    vector<string> check = interpretRead(tableDataFile);
                    if (!check.empty())
                    {
                        answer.push_back(check);
                    }
                }
            }
            catch (const exception &e)
            {
                cerr << RED << "Error reading file at line " << __LINE__ << ": " << e.what() << RESET << endl;
                tableDataFile.close();
                continue;
            }

            tableDataFile.close();
            if (answer.empty())
            {
                continue;
            }

            tableDataFile.close();
        }
        catch (const exception &e)
        {
            cerr << RED << "Error processing file at line " << __LINE__ << ": " << e.what() << RESET << endl;
            continue;
        }
    }

    int p = 0;
    for (int i = 0; i < data.size(); i++)
    {
        if (data[i].first == columnName)
        {
            p = i;
            break;
        }
        p++;
    }
    string indexType = data[p].second.first;
    if (data[p].second.second > 1)
    {
        indexType = "string";
    }
    if (indexType == "string")
    {
        indexString = map<string, pair<int, int>>();
        for (size_t k = 0; k < answer.size() - 1; k++)
        {

            indexString[answer[k][p]] = {k / 1000 + 1, k % 1000 + 1};
        }
    }
    else if (indexType == "int")
    {
        indexInt = map<int, pair<int, int>>();
        for (size_t i = 0; i < answer.size() - 1; i++)
        {

            indexInt[stoi(answer[i][p])] = {i / 1000 + 1, i % 1000 + 1};
        }
    }
    else if (indexType == "double")
    {
        indexDouble = map<double, pair<int, int>>();
        for (size_t i = 0; i < answer.size() - 1; i++)
        {

            indexDouble[stod(answer[i][p])] = {i / 1000 + 1, i % 1000 + 1};
        }
    }
    else if (indexType == "char")
    {
        indexChar = map<char, pair<int, int>>();
        for (size_t i = 0; i < answer.size() - 1; i++)
        {

            indexChar[answer[i][p][0]] = {i / 1000 + 1, i % 1000 + 1};
        }
    }

    if (!init)
    {

        cout << "Index created successfully" << endl;
    }

    return true;
}

bool table::indexedSearch(string databaseName, string columnName, string value)
{
    string type = "";
    string op = value.substr(0, 2);

    for (const auto &col : data)
    {
        if (col.first == columnName)
        {
            type = (col.second.second > 1) ? "string" : col.second.first;
            break;
        }
    }

    value = value.substr(3);

    unordered_map<string, string> opMap = {{"eq", "="}, {"gt", ">"}, {"lt", "<"}, {"ge", ">="}, {"le", "<="}, {"nt", "!="}};
    if (opMap.find(op) == opMap.end())
    {
        cout << RED << "Error: Invalid operator" << RESET << endl;
        return false;
    }
    op = opMap[op];

    vector<pair<int, int>> results;

    if (type == "string")
    {
        if (op == "=")
        {
            auto it = indexString.find(value);
            if (it != indexString.end())
            {
                results.push_back(it->second);
            }
        }
        else
        {
            for (auto it = (op == ">" || op == ">=") ? indexString.lower_bound(value) : indexString.begin();
                 it != indexString.end() && (op == "<" || op == "<=" || it->first >= value);
                 ++it)
            {
                if ((op == "<" && it->first >= value) || (op == ">" && it->first <= value))
                    break;
                results.push_back(it->second);
            }
        }
    }
    else if (type == "int")
    {
        int intValue = stoi(value);
        if (op == "=")
        {
            auto it = indexInt.find(intValue);
            if (it != indexInt.end())
                results.push_back(it->second);
        }
        else if (op == ">=")
        {
            for (auto it = indexInt.lower_bound(intValue); it != indexInt.end(); ++it)
                results.push_back(it->second);
        }
        else if (op == ">")
        {
            for (auto it = indexInt.upper_bound(intValue); it != indexInt.end(); ++it)
                results.push_back(it->second);
        }
        else if (op == "<=")
        {
            for (auto it = indexInt.begin(); it != indexInt.upper_bound(intValue); ++it)
                results.push_back(it->second);
        }
        else if (op == "<")
        {
            for (auto it = indexInt.begin(); it != indexInt.lower_bound(intValue); ++it)
                results.push_back(it->second);
        }
    }
    else if (type == "double")
    {
        double doubleValue = stod(value);
        if (op == "=")
        {
            auto it = indexDouble.find(doubleValue);
            if (it != indexDouble.end())
                results.push_back(it->second);
        }
        else if (op == ">=")
        {
            for (auto it = indexDouble.lower_bound(doubleValue); it != indexDouble.end(); ++it)
                results.push_back(it->second);
        }
        else if (op == ">")
        {
            for (auto it = indexDouble.upper_bound(doubleValue); it != indexDouble.end(); ++it)
                results.push_back(it->second);
        }
        else if (op == "<=")
        {
            for (auto it = indexDouble.begin(); it != indexDouble.upper_bound(doubleValue); ++it)
                results.push_back(it->second);
        }
        else if (op == "<")
        {
            for (auto it = indexDouble.begin(); it != indexDouble.lower_bound(doubleValue); ++it)
                results.push_back(it->second);
        }
    }
    else if (type == "char")
    {
        char charValue = value[0];
        if (op == "=")
        {
            auto it = indexChar.find(charValue);
            if (it != indexChar.end())
                results.push_back(it->second);
        }
        else if (op == ">=")
        {
            for (auto it = indexChar.lower_bound(charValue); it != indexChar.end(); ++it)
                results.push_back(it->second);
        }
        else if (op == ">")
        {
            for (auto it = indexChar.upper_bound(charValue); it != indexChar.end(); ++it)
                results.push_back(it->second);
        }
        else if (op == "<=")
        {
            for (auto it = indexChar.begin(); it != indexChar.upper_bound(charValue); ++it)
                results.push_back(it->second);
        }
        else if (op == "<")
        {
            for (auto it = indexChar.begin(); it != indexChar.lower_bound(charValue); ++it)
                results.push_back(it->second);
        }
    }
    else
    {
        cout << RED << "Error: Invalid column type" << RESET << endl;
        return false;
    }

    if (results.empty())
    {
        cout << RED << "Error: Value not found" << RESET << endl;
        return false;
    }

    for (const auto &[fileNumber, lineNumber] : results)
    {
        string fileName = "databases/" + databaseName + '/' + tableName + '/' + tableName + to_string(fileNumber) + ".bin";
        ifstream file(fileName, ios::binary);
        if (!file)
        {
            cout << RED << "Error: Unable to open file " << fileName << RESET << endl;
            continue;
        }

        int pos = (lineNumber - 1) * getTotalBytes(data);
        if (pos < 0)
            pos++;
        file.seekg(pos, ios::beg);

        vector<string> record;
        for (const auto &col : data)
        {
            const string &colType = col.second.first;
            size_t size = col.second.second;
            if (colType == "char")
            {
                if (size == 1)
                {
                    char val;
                    file.read(reinterpret_cast<char *>(&val), sizeof(char));
                    record.push_back(string(1, val));
                }
                else
                {
                    char val[size];
                    file.read(reinterpret_cast<char *>(val), sizeof(char) * size);
                    record.push_back(val);
                }
            }
            else if (colType == "int")
            {
                int val;
                file.read(reinterpret_cast<char *>(&val), sizeof(int));
                record.push_back(to_string(val));
            }
            else if (colType == "double")
            {
                double val;
                file.read(reinterpret_cast<char *>(&val), sizeof(double));
                record.push_back(to_string(val));
            }
            else if (colType == "bool")
            {
                bool val;
                file.read(reinterpret_cast<char *>(&val), sizeof(bool));
                record.push_back(to_string(val));
            }
        }

        for (const string &field : record)
        {
            cout << field << " ";
        }
        cout << endl;

        file.close();
    }

    return true;
}

int table::getTotalBytes(const vector<pair<string, pair<string, int>>> &data)
{
    int totalBytes = 0;

    for (const auto &entry : data)
    {
        string dataType = entry.second.first;
        int count = entry.second.second;

        if (dataType == "int")
        {
            totalBytes += 4 * count;
        }
        else if (dataType == "char")
        {
            totalBytes += 1 * count;
        }
        else if (dataType == "double")
        {
            totalBytes += 8 * count;
        }
        else if (dataType == "bool")
        {
            totalBytes += 1 * count;
        }
        else
        {
            cout << "Unknown data type: " << dataType << endl;
        }
    }

    return totalBytes;
}

bool table::evaluateExpr(const string &expr, const vector<bool> &between)
{
    stack<bool> valueStack;
    int index = 0;

    for (size_t i = 0; i < expr.size(); ++i)
    {
        char ch = expr[i];

        if (ch == '{')
        {
            if (i + 2 < expr.size() && expr[i + 2] == '}')
            {
                char op = expr[i + 1];

                if (index + 1 >= between.size())
                {
                    cerr << "Error: Not enough operands in 'between' for operator " << op << "\n";
                    return false;
                }

                bool a = between[index++];
                bool b = between[index++];

                bool result = (op == '&') ? (a & b) : (op == '|') ? (a | b)
                                                                  : false;

                valueStack.push(result);

                i += 2;
            }
            else
            {
                cerr << "Error: Invalid expression format near position " << i << "\n";
                return false;
            }
        }
    }

    if (!valueStack.empty())
    {
        return valueStack.top();
    }

    cerr << "Error: No result on value stack\n";
    return false;
}

bool table::parseCondition(string mp, bool colon)
{
    map<string, string> columns;
    // cout<<"i was called\n";
    for (int i = 0; i < data.size(); i++)
    {
        string temp = "char";
        if (data[i].second.first == "char" && data[i].second.second > 1)
        {
            temp = "string";
            columns.insert({data[i].first, temp});
        }
        else
        {
            temp = data[i].second.first;
            columns.insert({data[i].first, temp});
        }
    }
    vector<pair<string, string>> cond;
    vector<string> between;
    string temp = "";
    string value = "";
    string condition = "";
    bool check = true;
    string expression = "";
    if (!colon)
    {

        for (int i = 0; i < mp.size(); i++)
        {
            if (mp[i] == '&' || mp[i] == '|')
            {
                if (temp == "" || value == "")
                {
                    cout << RED << "Error: Invalid condition" << RESET << endl;
                    return false;
                }
                cond.push_back({temp, value});
                between.push_back(condition);
                expression += mp[i];
                temp = "";
                value = "";
                condition = "";
                check = true;
            }
            else if (mp[i] == '=' && mp[i + 1] == '=')
            {
                condition = "==";
                i++;
                check = false;
            }
            else if (mp[i] == '>' && mp[i + 1] == '=')
            {
                condition = ">=";
                i++;
                check = false;
            }
            else if (mp[i] == '<' && mp[i + 1] == '=')
            {
                condition = "<=";
                i++;
                check = false;
            }
            else if (mp[i] == '!' && mp[i + 1] == '=')
            {
                condition = "!=";
                i++;
                check = false;
            }
            else if (mp[i] == '<')
            {
                condition = "<";
                i++;
                check = false;
            }
            else if (mp[i] == '>')
            {
                condition = ">";
                i++;
                check = false;
            }
            else if (check && mp[i] != '{' && mp[i] != '}')
            {
                temp += mp[i];
            }
            else if (mp[i] != '{' && mp[i] != '}')
            {
                value += mp[i];
            }
            else
            {
                if (mp[i] == '}')
                {
                    if (temp == "" || value == "")
                    {
                        cout << RED << "Error: Invalid condition" << RESET << endl;
                        return false;
                    }
                    if (temp != "" && condition != "" && value != "")
                    {

                        cond.push_back({temp, value});
                        between.push_back(condition);
                    }
                    temp = "";
                    value = "";
                    condition = "";
                    check = true;
                }
                expression += mp[i];
            }
        }
    }
    else{
        for (int i = 0; i < mp.size(); i++)
        {
            if (mp[i] == '&' || mp[i] == '|')
            {
                if (temp == "" || value == "")
                {
                    cout << RED << "Error: Invalid condition" << RESET << endl;
                    return false;
                }
                cond.push_back({temp, value});
                between.push_back(condition);
                expression += mp[i];
                temp = "";
                value = "";
                condition = "";
                check = true;
            }
            else if(mp[i]==':'){
                check=false;
            }
            else if (check && mp[i] != '{' && mp[i] != '}')
            {
                temp += mp[i];
            }
            else if (mp[i] != '{' && mp[i] != '}')
            {
                value += mp[i];
            }
            else
            {
                if (mp[i] == '}')
                {
                    if (temp == "" || value == "")
                    {
                        cout << RED << "Error: Invalid condition" << RESET << endl;
                        return false;
                    }
                    if (temp != "" && condition != "" && value != "")
                    {

                        cond.push_back({temp, value});
                        between.push_back(condition);
                    }
                    temp = "";
                    value = "";
                    condition = "";
                    check = true;
                }
                expression += mp[i];
            }
        }
    }
    // for(auto i:cond){
    //     cout<<i.first<<' '<<i.second<<endl;
    // }
    for (auto i : cond)
    {
        if (columns.find(i.first) == columns.end())
        {
            cout << RED << "Error: Column " << i.first << " not found" << RESET << endl;
            return false;
        }
        if (i.second == "")
        {
            cout << RED << "Error: Invalid value " << i.second << " for column " << i.first << RESET << endl;
            return false;
        }
        if (columns[i.first] == "int")
        {
            try
            {
                int valueInt = stoi(i.second);
                if (valueInt < 0)
                {
                    cout << RED << "Error: Invalid value " << i.second << " for column " << i.first << RESET << endl;
                    return false;
                }
                if (i.second.size() > 10)
                {
                    cout << RED << "Error: Invalid value " << i.second << " for column " << i.first << RESET << endl;
                    return false;
                }
            }
            catch (const exception &e)
            {
                cout << RED << "Error: Invalid value " << i.second << " for column " << i.first << RESET << endl;
                return false;
            }
        }
        else if (columns[i.first] == "double")
        {
            try
            {
                int valueInt = stod(i.second);
                if (valueInt < 0)
                {
                    cout << RED << "Error: Invalid value " << i.second << " for column " << i.first << RESET << endl;
                    return false;
                }
                if (i.second.size() > 10)
                {
                    cout << RED << "Error: Invalid value " << i.second << " for column " << i.first << RESET << endl;
                    return false;
                }
            }
            catch (const exception &e)
            {
                cout << RED << "Error: Invalid value " << i.second << " for column " << i.first << RESET << endl;
                return false;
            }
        }
        else if (columns[i.first] == "char")
        {
            if (i.second.size() != 1)
            {
                cout << RED << "Error: Invalid value " << i.second << " for column " << i.first << RESET << endl;
                return false;
            }
        }
        else if (columns[i.first] == "string")
        {
            // do nothing
        }
    }
    return true;
}

