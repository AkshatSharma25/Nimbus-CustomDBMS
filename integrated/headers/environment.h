#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H
#include<bits/stdc++.h>
using namespace std;
class environment{
    private:
        void throughError(string error);
        void throughSuccess();
        void throughWarning(string warning);
    public:
        environment();
        string takeInput();
};

#endif
