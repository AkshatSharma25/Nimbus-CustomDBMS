#include <bits/stdc++.h>
#include "parser.h"
#include "environment.h"
#include "executer.h"
#include "database.h"
#include "nimbus.h"
#include <chrono>
using namespace std;
using namespace chrono;

void nimbus::loadMetadata()
{
    fstream metaDataFile("NimbusMetaData.csv");
    
    if (!metaDataFile)
    {
        string problem = "Error loading metadata: Metadata file not found";
        throw problem;
    }

    // 1databaseName,databaseMetaDataFile
    // 0defaultDatabase
    while (!metaDataFile.eof())
    {
        string line;
        getline(metaDataFile, line);
        if (line[0] == '0')
        {
            // selectedDatabase = line.substr(1, line.size() - 1);
        }
        else if (line[0] == '1')
        {
            line = line.substr(1, line.size() - 1);
            string temp = "";
            string databaseName, databaseMetaDataFile;
            for (int i = 0; i < line.size(); i++)
            {
                if (line[i] == ',')
                {
                    databaseName = temp;
                    temp = "";
                }
                else
                {
                    temp += line[i];
                }
            }
            databaseMetaDataFile = temp;
            dbs[databaseName] = database(databaseName, databaseMetaDataFile);
        }
    }
}

nimbus::nimbus()
{
    selectedDatabase = "test";
    loadMetadata();
}

void nimbus::start()
{
    // if (!login())
    // {
    //     cout << "Login failed. Exiting..." << endl;
    //     return;
    // }
    // cout << "Welcome to Nimbus!" << endl;
    while (true)
    {
        try
        {
            string input = ENVIRONMENT.takeInput();
            auto start = high_resolution_clock::now();
            if (input == "exit")
            {
                break;
            }
            returnObject p = PARSER.parse(input);
            if (p.flag == 2)
            {
                EXECUTER.executeInsert(p.one, p.two, p.inputs2, dbs, selectedDatabase);
            }
            else if (p.flag == 0)
            {
                EXECUTER.executeSingle(p.one, p.two, dbs, selectedDatabase);
            }
            else if (p.flag == 5)
            {
                EXECUTER.executeFindDeleteUpdate(p.one, p.two, p.findInputs, dbs, selectedDatabase);
            }
            else if (p.flag == 6)
            {
                if (p.one == "showDatabases")
                {
                    EXECUTER.showDatabases(dbs);
                }
                else if (p.one == "showTables")
                {
                    EXECUTER.showTables(dbs, selectedDatabase);
                }
                else
                {
                    cout << "Invalid command" << endl;
                }
            }
            else
            {
                EXECUTER.executeDouble(p.one, p.two, p.inputs, dbs, selectedDatabase);
            }
            auto stop = high_resolution_clock::now();
            auto duration = duration_cast<microseconds>(stop - start);
            // cout << "processed in: " << duration.count() / 1000 << " milli-seconds" << endl;
        }
        catch (const exception &e)
        {
            cerr << "Error: " << e.what() << endl;
        }
        catch (string &e)
        {
            cerr << "Error: " << e << endl;
        }
        catch (...)
        {
            cerr << "Unknown error occurred" << endl;
        }
    }
}

bool nimbus::login()
{
    string username, password;
    cout << "Enter username: ";
    cin >> username;
    cout << "Enter password: ";
    cin >> password;

    if (username == "admin" && password == "12345")
    {
        cout << "Login successful!" << endl;
        return true;
    }
    else
    {
        cout << "Invalid credentials. Please try again." << endl;
        return false;
    }
}
