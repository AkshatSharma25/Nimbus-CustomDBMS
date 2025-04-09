#include <bits/stdc++.h>
#include "returnObject.h"
#include "parser.h"
using namespace std;
#define RED "\033[31m"
#define RESET "\033[0m"
#define GREEN "\033[32m"

returnObject parser::checkSyntax(const string &one, const string &two)
{
    try
    {
        if (startKeywords.find(one) == startKeywords.end())
        {
            const string error = "Syntax Error: invalid start keyword";
            throw error;
        }
        if (!all_of(two.begin(), two.end(), ::isalnum))
        {
            const string error = "Syntax Error: invalid database name";
            throw error;
        }
        else
        {
            returnObject p(one, two);
            return p;
        }
    }
    catch (string &e)
    {
        cerr << RED << e << RESET << endl;
        return returnObject();
    }
}


returnObject parser::checkSyntax(const string &one, const string &two, string three)
{
    try
    {
        if (startKeywords.find(one) == startKeywords.end())
        {
            const string error = "Syntax Error: invalid start keyword";
            throw error;
        }
        else if (middleKeywords.find(two) == middleKeywords.end())
        {
            const string error = "Syntax Error: invalid middle keyword";
            throw error;
        }
        else if (two == "insert")
        {
            cout<<'x'<<endl;
            three=three.substr(1,three.size()-2);
            vector<string> inputs2;
            string temp = "";
            int ctr = 0;
            for (int i = 0; i < three.size(); i++)
            {
                if (three[i] == ',')
                {
                    if (temp == "")
                    {
                        const string error = "Syntax Error: invalid input format";
                        throw error;
                    }
                    inputs2.push_back(temp);
                    temp = "";
                    ctr++;
                }
                else
                {
                    temp += three[i];
                }
            }
            if (temp != "")
            {
                inputs2.push_back(temp);
            }
            returnObject p(one, two, inputs2);
            return p;
        }
        // mine
        else if (two == "update") {
            // Extract parameters from three
            size_t namePos = three.find("name:");
            size_t setPos = three.find("set:(");
            
            if (namePos == string::npos || setPos == string::npos) {
                throw string("Syntax Error: missing required fields in update query");
            }

            // Extract table name
            size_t nameEnd = three.find(',', namePos);
            string tableName = three.substr(namePos + 5, nameEnd - (namePos + 5));

            // Extract where condition (before set)
            string whereCondition = three.substr(nameEnd + 1, setPos - (nameEnd + 2));

            // Extract set values
            string setValues = three.substr(setPos + 5, three.size() - (setPos + 6));

            // Parse WHERE conditions (e.g., "stName:eq~RXDnmEsUJm")
            vector<pair<string, string>> whereClauses;
            size_t colonPos = whereCondition.find(':');
            size_t tildePos = whereCondition.find('~');

            if (colonPos != string::npos && tildePos != string::npos) {
                string column = whereCondition.substr(0, colonPos);
                string condition = whereCondition.substr(colonPos + 1, tildePos - colonPos - 1);
                string value = whereCondition.substr(tildePos + 1);
                whereClauses.push_back({column + ":" + condition, value});
            } else {
                throw string("Syntax Error: invalid WHERE clause");
            }

            // Parse SET values (e.g., "stName=akdjsl,gender=f")
            vector<pair<string, string>> setClauses;
            stringstream ss(setValues);
            string setPair;
            while (getline(ss, setPair, ',')) {
                size_t eqPos = setPair.find('=');
                if (eqPos == string::npos) {
                    throw string("Syntax Error: invalid SET clause");
                }
                string column = setPair.substr(0, eqPos);
                string value = setPair.substr(eqPos + 1);
                setClauses.push_back({column, value});
            }

            returnObject p(one, two, tableName, whereClauses, setClauses);
            return p;
        }
        else if(two=="find" || two=="delete"){
            returnObject p(one, two, three);
            return p;
        }
        else
        {
            bool isAndQuery = false;
            vector<pair<string, string>> inputs;
            int n = three.size();
            string columnName = "", what = "";
            string temp = "";

            if (three[0] != '(' && !three[n - 1] != ')')
            {
                const string error = "Syntax Error: invalid input format";
                throw error;
            }

            if (!three[0] != '(' && three[n - 1] != ')')
            {
                const string error = "Syntax Error: invalid input format";
                throw error;
            }
            for (int i = 1; i < n - 1; i++)
            {
                if (three[i] == ':')
                {
                    if (temp == "")
                    {
                        const string error = "Syntax Error: invalid input format";
                        throw error;
                    }
                    columnName = temp;
                    temp = "";
                }
                else if (three[i] == ',')
                {
                    if(temp=="&&"){
                        isAndQuery = true;
                        temp = "";
                        continue;
                    }
                    if (temp == "")
                    {
                        const string error = "Syntax Error: invalid input format";
                        throw error;
                    }
                    what = temp;
                    inputs.push_back({columnName, what});
                    temp = "";
                }
                else
                {
                    temp += three[i];
                }
            }
            if (temp != "")
            {
                inputs.push_back({columnName, temp});
            }
            
            returnObject p(one, two, inputs);
            return p;
        }
    
    }
    catch (string &e)
    {
        cerr << RED << e << RESET << endl;
        return returnObject();
    }
}




parser::parser()
{
    // current database in initialized to test

    middleKeywords.insert("find");
    middleKeywords.insert("findAll");
    middleKeywords.insert("createTable");
    middleKeywords.insert("dropTable");
    middleKeywords.insert("insert");
    middleKeywords.insert("update");
    middleKeywords.insert("delete");
    middleKeywords.insert("createIndex");

    startKeywords.insert("useDatabase");
    startKeywords.insert("createDatabase");
    startKeywords.insert("dropDatabase");
    startKeywords.insert("db");
}

returnObject parser::parse(string userQuery)
{
    try
    {
        if (userQuery == "")
        {
            return returnObject();
        }
        query = userQuery;
        string temp = "";
        int ctr = 0;
        string first, second, third = "";
        for (int i = 0; i < query.length(); i++)
        {
            if(query[i]=='.' && i>0 && query[i-1]>='0' && query[i-1]<='9'){
                temp+='.';
                continue;
            }
            else if(query[i]=='.' && i==0){
                const string error = "Syntax Error: invalid input format";
                throw error;
            }
            else if (query[i] == '.')
            {
                if (ctr == 0)
                {
                    first = temp;
                }
                else if (ctr == 1)
                {
                    second = temp;
                }
                else
                {
                    third = temp;
                }
                temp = "";
                ctr++;
            }
            else
            {
                temp += query[i];
            }
        }
        if (temp != "" && ctr == 1)
            second = temp;
        else if (temp != "" && ctr == 2)
            third = temp;
        if (third == "")
            return checkSyntax(first, second);
        else
            return checkSyntax(first, second, third);
    }
    catch (string &e)
    {
        cerr << RED << e << RESET << endl;
        return returnObject();
    }
}

void parser::printQuery()
{
    for (auto i : commandKeywords)
    {
        cout << i << ' ';
    }
    cout << endl;
}
