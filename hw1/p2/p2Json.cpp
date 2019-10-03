/****************************************************************************
 FileName     [ p2Json.cpp ]
 PackageName  [ p2 ]
 Synopsis     [ Define member functions of class Json and JsonElem ]
 Author       [ Chien-Ying Yang ]
 Copyright    [ Copyleft(c) 2018-present DVLab, GIEE, NTU, Taiwan ]
 ****************************************************************************/
#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <algorithm> 
#include <fstream>
#include <sstream>
#include "p2Json.h"

using namespace std;

// Implement member functions of class Row and Table here
bool
Json::read(const string& jsonFile)
{
    ifstream infile;
    infile.open("./" + jsonFile);
    if (infile.fail()) return false;
    stringstream strStream;
    strStream << infile.rdbuf();
    string jsonstring = strStream.str();
        
    // remove specific symbol
    jsonstring.erase(remove(jsonstring.begin(), jsonstring.end(), ' '), jsonstring.end());
    jsonstring.erase(remove(jsonstring.begin(), jsonstring.end(), '\"'), jsonstring.end());
    jsonstring.erase(remove(jsonstring.begin(), jsonstring.end(), '{'), jsonstring.end());
    jsonstring.erase(remove(jsonstring.begin(), jsonstring.end(), '}'), jsonstring.end());
    jsonstring.erase(remove(jsonstring.begin(), jsonstring.end(), '\n'), jsonstring.end());
    jsonstring.erase(remove(jsonstring.begin(), jsonstring.end(), '\t'), jsonstring.end());
    
    // split jsonstring with ,
    vector<string> u;
    string::size_type pos1, pos2;
    string c = ",";
    pos2 = jsonstring.find(c);
    pos1 = 0;
    while(string::npos != pos2)
    {
        u.push_back(jsonstring.substr(pos1, pos2-pos1));
        pos1 = pos2 + c.size();
        pos2 = jsonstring.find(c, pos1);
    }
    if(pos1 != jsonstring.length())
        u.push_back(jsonstring.substr(pos1));
    
    // for(vector<string>::size_type i = 0; i != u.size(); ++i)
    //     cout << "u["<<i<<"]" << u[i] << endl;
    
    // split key and value with :
    for(vector<string>::size_type i = 0; i != u.size(); ++i){
        vector<string> v;
        string::size_type pos1_, pos2_;
        string c_ = ":";
        pos2_ = u[i].find(c_);
        pos1_ = 0;
        while(string::npos != pos2_)
        {
            v.push_back(u[i].substr(pos1_, pos2_-pos1_));
            pos1_ = pos2_ + c_.size();
            pos2_ = u[i].find(c_, pos1_);
        }
        if(pos1_ != u[i].length())
            v.push_back(u[i].substr(pos1_));
        
        // cout<<"v[0]"<<v[0]<<endl;
        // cout<<"v[1]"<<v[1]<<endl;
        
        // convert string v[1] into int
        istringstream iss (v[1]);
        int number;
        iss >> number;
        
        JsonElem jsonelem(v[0],number);
        addelem(jsonelem);
    }
    
    return true; // TODO
}

void
Json::print(){
    if(!_obj.empty()){
        cout<<"{\n";
        for(vector<string>::size_type i = 0; i != _obj.size()-1; ++i)
        cout<<"  "<<_obj[i]<<","<<endl;
        cout<<"  "<<_obj.back()<<endl;
        cout<<"}"<<endl;
    }
    else{
        cout<<"{"<<endl<<"}"<<endl;
    }
}

void
Json::add(string _key,int _value){
    JsonElem jsonelem(_key,_value);
    addelem(jsonelem);
}

void
Json::addelem(JsonElem jsonelem){
    _obj.push_back(jsonelem);
}

void
Json::max(){
    if(!_obj.empty()){
        int Max = _obj[0].getValue();
        vector<string>::size_type i;
        int j;
        for( i = 1 ; i != _obj.size(); ++i)
        {
            if(Max<_obj[i].getValue()){
                Max = _obj[i].getValue(); 
                j=i;
            }
        }
        cout<<"The maximum element is: { "<<_obj[j]<<" }."<<endl;
    }
    else{
        // cout<<"Error: No element found!!"<<endl;
    }
}

void
Json::min(){
    if(!_obj.empty()){
        int Min = _obj[0].getValue();
        vector<string>::size_type i;
        int j;
        for( i = 1 ; i != _obj.size(); ++i)
        {
            if(Min>_obj[i].getValue()){
                Min = _obj[i].getValue(); 
                j=i;
            }
        }
        cout<<"The minimum element is: { "<<_obj[j]<<" }."<<endl;
    }
    else{
        // cout<<"Error: No element found!!"<<endl;
    }
}

void
Json::sum(){
    if(!_obj.empty()){
        vector<string>::size_type i;
        int SUM = 0;
        for( i = 0 ; i != _obj.size(); ++i){
            SUM += _obj[i].getValue();
        }
        cout<<"The summation of the values is: "<<SUM<<"."<<endl;
    }
    else{
        // cout<<"Error: No element found!!"<<endl;
    }
}

void
Json::ave(){
    if(!_obj.empty()){
        vector<string>::size_type i;
        int SUM = 0;
        float AVE = 0;
        for( i = 0 ; i != _obj.size(); ++i){
            SUM += _obj[i].getValue();
        }
        AVE = (float)((float)SUM/(float)_obj.size());
        cout<<"The average of the values is: "<<fixed<<setprecision(1)<<AVE<<"."<<endl;
    }
    else{
        // cout<<"Error: No element found!!"<<endl;
    }
}

int
JsonElem::getValue(){
    return _value;
}

ostream&
operator << (ostream& os, const JsonElem& j)
{
    return (os << "\"" << j._key << "\" : " << j._value);
}