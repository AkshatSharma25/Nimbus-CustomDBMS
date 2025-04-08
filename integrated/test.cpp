vector<string> table::interpretRead(ifstream &file, map<string, pair<string, string>> &inputs)
{

    // eq: equal to
    // nt: not equal to
    // gt: greater than
    // lt: less than
    // ge: greater than or equal to
    // le: less than or equal to

    if (file.eof())
    {
        return {};
    }
    bool check = false;
    bool checkColName = false;

    vector<string> answer;
    for (int j = 0; j < data.size(); j++)
    {
        auto i = data[j];
        const string colName = i.first;
        const string type = i.second.first;
        const size_t size = i.second.second;

        string op = inputs[colName].first;
        string toCompare = inputs[colName].second;
        if (type == "char")
        {
            if (size == 1)
            {
                char value;
                file.read(reinterpret_cast<char *>(&value), sizeof(char));
                answer.push_back(string(1, value));
                if (inputs.find(colName) == inputs.end())
                {
                    continue;
                }
                checkColName = true;
                if (op == "eq")
                {
                    if (value == (toCompare[0]))
                    {
                        check = true;
                    }
                }
                else if (op == "nt")
                {
                    if (value != (toCompare[0]))
                    {
                        check = true;
                    }
                }
                else if (op == "gt")
                {
                    if (value > (toCompare[0]))
                    {
                        check = true;
                    }
                }
                else if (op == "lt")
                {
                    if (value < (toCompare[0]))
                    {
                        check = true;
                    }
                }
                else if (op == "ge")
                {
                    if (value >= (toCompare[0]))
                    {
                        check = true;
                    }
                }
                else if (op == "le")
                {
                    if (value <= (toCompare[0]))
                    {
                        check = true;
                    }
                }
            }
            else
            {

                char value[size];
                file.read(reinterpret_cast<char *>(value), sizeof(char) * size);
                answer.push_back(string(value));
                if (inputs.find(colName) == inputs.end())
                {
                    continue;
                }
                checkColName = true;
                if (op == "eq")
                {
                    if (strcmp(value, toCompare.c_str()) == 0)
                    {
                        check = true;
                    }
                }
                else if (op == "nt")
                {
                    if (strcmp(value, toCompare.c_str()) != 0)
                    {
                        check = true;
                    }
                }
                else if (op == "gt")
                {
                    if (strcmp(value, toCompare.c_str()) > 0)
                    {
                        check = true;
                    }
                }
                else if (op == "lt")
                {
                    if (strcmp(value, toCompare.c_str()) < 0)
                    {
                        check = true;
                    }
                }
                else if (op == "ge")
                {
                    if (strcmp(value, toCompare.c_str()) >= 0)
                    {
                        check = true;
                    }
                }
                else if (op == "le")
                {
                    if (strcmp(value, toCompare.c_str()) <= 0)
                    {
                        check = true;
                    }
                }
            }
        }
        else if (type == "int")
        {
            int value;
            file.read(reinterpret_cast<char *>(&value), size * sizeof(int));
            answer.push_back(to_string(value));
            if (inputs.find(colName) == inputs.end())
            {
                continue;
            }
            checkColName = true;
            if (op == "eq")
            {
                if (value == stoi(toCompare))
                {
                    check = true;
                }
            }
            else if (op == "nt")
            {
                if (value != stoi(toCompare))
                {
                    check = true;
                }
            }
            else if (op == "gt")
            {
                if (value > stoi(toCompare))
                {
                    check = true;
                }
            }
            else if (op == "lt")
            {
                if (value < stoi(toCompare))
                {
                    check = true;
                }
            }
            else if (op == "ge")
            {
                if (value >= stoi(toCompare))
                {
                    check = true;
                }
            }
            else if (op == "le")
            {
                if (value <= stoi(toCompare))
                {
                    check = true;
                }
            }
        }
        else if (type == "double")
        {
            double value;
            file.read(reinterpret_cast<char *>(&value), size * sizeof(double));
            answer.push_back(to_string(value));
            if (inputs.find(colName) == inputs.end())
            {
                continue;
            }
            checkColName = true;
            if (op == "eq")
            {
                if (value == stod(toCompare))
                {
                    check = true;
                }
            }
            else if (op == "nt")
            {
                if (value != stod(toCompare))
                {
                    check = true;
                }
            }
            else if (op == "gt")
            {
                if (value > stod(toCompare))
                {
                    check = true;
                }
            }
            else if (op == "lt")
            {
                if (value < stod(toCompare))
                {
                    check = true;
                }
            }
            else if (op == "ge")
            {
                if (value >= stod(toCompare))
                {
                    check = true;
                }
            }
            else if (op == "le")
            {
                if (value <= stod(toCompare))
                {
                    check = true;
                }
            }
        }
        else if (type == "bool")
        {
            bool value;
            file.read(reinterpret_cast<char *>(&value), size * sizeof(bool));
            answer.push_back(to_string(value));
            if (inputs.find(colName) == inputs.end())
            {
                continue;
            }
            checkColName = true;
            if (op == "eq")
            {
                if (value == stoi(toCompare))
                {
                    check = true;
                }
            }
            else if (op == "nt")
            {
                if (value != stoi(toCompare))
                {
                    check = true;
                }
            }
            else if (op == "gt")
            {
                if (value > stoi(toCompare))
                {
                    check = true;
                }
            }
            else if (op == "lt")
            {
                if (value < stoi(toCompare))
                {
                    check = true;
                }
            }
            else if (op == "ge")
            {
                if (value >= stoi(toCompare))
                {
                    check = true;
                }
            }
            else if (op == "le")
            {
                if (value <= stoi(toCompare))
                {
                    check = true;
                }
            }
        }
    }
    if (check)
    {
        return answer;
    }
    else
        return {};
}