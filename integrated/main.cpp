
#include<bits/stdc++.h>
#include"headers/nimbus.h"
#include <locale>
#define RED "\033[31m"
#define RESET "\033[0m"
#define GREEN "\033[32m" 


using namespace std;



int main()
{
    // std::locale::global(std::locale("C.UTF-8"));
    try{
        nimbus NIMBUS;
        NIMBUS.start();
    }
    catch (const std::exception &e)
    {
        cerr << RED << e.what() << RESET << endl;
    }
    catch (string &e)
    {
        cerr << RED << e << RESET << endl;
    }
    catch (...)
    {
        cerr << RED << "Unknown error occurred" << RESET << endl;
    }

    return 0;
}