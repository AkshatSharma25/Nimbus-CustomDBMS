#ifndef NIMBUS_H
#define NIMBUS_H
#include "database.h"
#include "environment.h"
#include "executer.h"
#include "parser.h"
#include <chrono>
class nimbus{
    private:
        map<string,database> dbs;
        string selectedDatabase;
        environment ENVIRONMENT;
        executer EXECUTER;
        parser PARSER;
        void loadMetadata();

    public:
        nimbus();
        void start();
};

#endif
