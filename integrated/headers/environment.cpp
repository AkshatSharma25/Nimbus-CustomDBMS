#include <bits/stdc++.h>
#include "environment.h"
using namespace std;

#define RED "\033[31m"
#define RESET "\033[0m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"

void environment::throughError(string error)
{
    cerr << RED << error << RESET << endl;
}
void environment::throughSuccess()
{
    cout << GREEN << "Execution successful!" << RESET << endl;
}
void environment::throughWarning(string warning)
{
    cout << YELLOW << warning << RESET << endl;
}

environment::environment()
{
    cout << GREEN << "Welcome to Nimbus" << RESET << endl;
}
string environment::takeInput()
{
    string input;
    // cout << YELLOW << "Nimbus>>" << RESET << " ";
    getline(cin, input);
    if(input=="exit"){
        return "exit";
    }
    if(input.empty()){
        return takeInput();
    }
    return input;
}