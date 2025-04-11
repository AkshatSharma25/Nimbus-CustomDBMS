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
        if(two=="" && (one=="showDatabases" || one=="showTables")){
            return returnObject(one);
        }
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
        bool isBracketValid=isBracketStructureValid(three);
        if(!isBracketValid){
            const string error = "Syntax Error: invalid bracket structure";
            throw error;
        }
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
        
        else if(two=="find" || two=="delete"||two=="update"){
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
    catch(const exception&e){
        cerr << RED << e.what() << RESET << endl;
        return returnObject();
    }
    catch(...){
        cerr << RED << "Unknown error occurred" << RESET << endl;
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
    startKeywords.insert("showDatabases");
    startKeywords.insert("showTables");
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
        if(userQuery=="showDatabases" || userQuery=="showTables"){
            return checkSyntax(userQuery,"");
        }
        string first="", second="", third = "";
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


bool parser:: isBracketStructureValid(const std::string& input) {
    std::stack<char> stk;
    bool inside = false;
    bool outerOpened = false;
    bool outerClosed = false;

    for (size_t i = 0; i < input.size(); ++i) {
        char ch = input[i];

        // 1. Disallow [ or ]
        if (ch == '[' || ch == ']') {
            throw std::runtime_error("Invalid character '[' or ']' at position " + std::to_string(i));
        }

        // 2. Opening brackets
        if (ch == '(' || ch == '{') {
            if (stk.empty()) {
                // New outermost bracket
                if (outerOpened) {
                    throw std::runtime_error("Multiple outermost '()' pairs detected at position " + std::to_string(i));
                }
                if (ch != '(') {
                    throw std::runtime_error("Outermost bracket must be '(' at position " + std::to_string(i));
                }
                outerOpened = true;
                inside = true;
            } else if (ch == '(') {
                // Nested parentheses not allowed
                throw std::runtime_error("Nested '()' not allowed at position " + std::to_string(i));
            }
            stk.push(ch);
        }

        // 3. Closing brackets
        else if (ch == ')' || ch == '}') {
            if (stk.empty()) {
                throw std::runtime_error("Unmatched closing bracket at position " + std::to_string(i));
            }

            char top = stk.top();
            if ((ch == ')' && top != '(') || (ch == '}' && top != '{')) {
                throw std::runtime_error("Mismatched bracket at position " + std::to_string(i));
            }

            stk.pop();

            // Outermost closed
            if (stk.empty()) {
                inside = false;
                outerClosed = true;
            }
        }

        // 4. Non-bracket characters
        else {
            if (!inside) {
                throw std::runtime_error("Character outside outermost '()' at position " + std::to_string(i));
            }
        }

        // 5. Extra content after outermost '()' is closed
        if (outerClosed && i + 1 < input.size()) {
            throw std::runtime_error("Extra characters after outermost '()' closed at position " + std::to_string(i + 1));
        }
    }

    // Final checks
    if (!stk.empty()) {
        throw std::runtime_error("Unmatched opening bracket at end of input.");
    }
    if (!outerOpened || !outerClosed) {
        throw std::runtime_error("Missing or incomplete outermost '()' pair.");
    }

    return true;
}


