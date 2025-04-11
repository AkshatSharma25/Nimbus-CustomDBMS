#include <bits/stdc++.h>
#include "returnObject.h"
using namespace std;

// flag:
// 0:executeSingle
// 1:executeDouble
// 2:executeInsert
//-1:NULL

returnObject::returnObject(string one, string two, vector<pair<string, string>> inputs)
{
    this->one = one;
    this->two = two;
    this->inputs = inputs;
    this->flag = 1;
}


returnObject::returnObject()
{
    this->flag = -1;
}



returnObject::returnObject(string one, string two)
{
    this->flag = 0;
    this->one = one;
    this->two = two;
}

returnObject::returnObject(string one,string two,vector<string>inputs2,bool andFunc){
    this->flag=4;
    this->one=one;
    this->andFunc = true;
    this->two=two;
    this->inputs2=inputs2;
}


returnObject::returnObject(string one,string two,vector<string>inputs2){
    this->flag=2;
    this->one=one;
    this->two=two;
    this->inputs2=inputs2;
}



// mine
returnObject::returnObject(string one, string two, string tableName, 
    vector<pair<string, string>> whereClauses, 
    vector<pair<string, string>> setClauses) {
    this->one = one;
    this->two = two;
    this->tableName = tableName;
    this->whereClauses = whereClauses;
    this->setClauses = setClauses;
    this->flag = 3;  // New flag for update queries
}

returnObject::returnObject(string one){
    this->one=one;
    this->flag=6;
}


returnObject::returnObject(string one,string two,string findInputs){
    this->one=one;
    this->two=two;
    this->findInputs=findInputs;
    this->flag=5;
}