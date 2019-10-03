/****************************************************************************
 FileName     [ p2Main.cpp ]
 PackageName  [ p2 ]
 Synopsis     [ Define main() function ]
 Author       [ Chien-Ying Yang ]
 Copyright    [ Copyleft(c) 2016-present DVLab, GIEE, NTU, Taiwan ]
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
bool AllisNum(string str);

int main()
{
    Json json;

    // Read in the csv file. Do NOT change this part of code.
    string jsonFile;
    cout << "Please enter the file name: ";
    // cin >> jsonFile;
    getline(cin,jsonFile);
    if (json.read(jsonFile))
    {
        cout << "File \"" << jsonFile << "\" was read in successfully." << endl;
    }
    else
    {
        cerr << "Failed to read in file \"" << jsonFile << "\"!" << endl;
        exit(-1); // jsonFile does not exist.
    }

    // TODO read and execute commands
    while (1)
    {
        cout << "Enter command: ";
        string input;
        getline(cin,input);
        int first = input.assign(input, 0, input.length()).find_first_not_of(" ");
        string x;
        x = input.assign(input, first, input.length()-first);
        
        vector<string> u;
        string::size_type pos1, pos2;
        string c = " ";
        pos2 = x.find(c);
        pos1 = 0;
        while(string::npos != pos2){
            u.push_back(x.substr(pos1, pos2-pos1));
            pos1 = pos2 + c.size();
            pos2 = x.find(c, pos1);
        }
        if(pos1 != x.length())
            u.push_back(x.substr(pos1));

        bool nothing;
        vector<string>::size_type nothingi = 1;
        for(nothingi = 1; nothingi != u.size(); ++nothingi){
            if(!u[nothingi].empty()){
                nothing = false;
                break;
            }
            else 
                nothing = true;
        }
        if (u[0] == "PRINT"){
            if(u.size()==1)
                json.print();
            else{
                if(nothing)
                    json.print();
                else{
                    // int first = x.assign(x, 6, x.length()-5).find_first_not_of(" ");
                    // cout << "Error: Extra argument \""<<x.substr(first, x.length())<<"\"!!"<<endl;
                }
            }
        }
        else if (u[0] == "ADD"){
            if(u.size()==1) {
                // cout<<"Missing argument!!"<<endl;
            }
            else{
                if(nothing) {
                    // cout<<"Missing argument!!"<<endl;
                }
                else{
                    int first = x.assign(x, 4, x.length()-3).find_first_not_of(" ");
                    //cout << "Error: Extra argument \""<<x.substr(first, x.length())<<"\"!!"<<endl;
                    string str = x.substr(first, x.length());
                    // cout<<"str:"<<str<<endl;
                    int strfirst = str.find_first_of(" ");
                    string key = str.substr(0,strfirst);
                    // cout<<"key:"<<key<<endl;
                    string str2 = str.substr(strfirst+1,str.length());
                    //cout<<"str2:"<<str2<<endl;
                    int str2first = str2.find_first_not_of(" ");
                    string value = str2.assign(str2, str2first, str2.length()-str2first);
                    string value_ = value;
                    // cout<<"value:"<<value<<endl;
                    if(key==value){
                        //cout<<"Error: Missing argument after \""<<key<<"\"!!"<<endl;
                    }
                    else{
                        if(AllisNum(value)){
                            stringstream iss (value);
                            int number;
                            iss >> number;
                            json.add(key,number);
                            // cout<<"can add"<<endl;
                        }
                        else{
                            if (value[0]=='-'){
                                if(AllisNum(value_.assign(value_,1,value_.length()-1))){
                                    istringstream iss (value);
                                    int number;
                                    iss >> number;
                                    json.add(key,number);
                                    // cout<<"can add"<<endl;
                                }
                                else {
                                    // cout<<"Error: Illegal argument \""<<value<<"\"!!"<<endl;
                                }
                            }
                            else {
                                //cout<<"Error: Illegal argument \""<<value<<"\"!!"<<endl;
                            }
                        }
                    }

                    
                }
            }
        }
        else if (u[0] == "SUM" ){
            if(u.size()==1)
                json.sum();
            else{
                if(nothing)
                    json.sum();
                else{
                    // int first = x.assign(x, 4, x.length()-3).find_first_not_of(" ");
                    // cout << "Error: Extra argument \""<<x.substr(first, x.length())<<"\"!!"<<endl;
                }
            }
        }
        else if (u[0] == "AVE" ){
            if(u.size()==1)
                json.ave();
            else{
                if(nothing)
                    json.ave();
                else{
                    // int first = x.assign(x, 4, x.length()-3).find_first_not_of(" ");
                    // cout << "Error: Extra argument \""<<x.substr(first, x.length())<<"\"!!"<<endl;
                }
            }
        }
        else if (u[0] == "MAX"){
            if(u.size()==1)
                json.max();
            else{
                if(nothing)
                    json.max();
                else{
                    // int first = x.assign(x, 4, x.length()-3).find_first_not_of(" ");
                    // cout << "Error: Extra argument \""<<x.substr(first, x.length())<<"\"!!"<<endl;
                }
            }
        }
        else if (u[0]== "MIN" ){
            if(u.size()==1)
                json.min();
            else{
                if(nothing)
                    json.min();
                else{
                    // int first = x.assign(x, 4, x.length()-3).find_first_not_of(" ");
                    // cout << "Error: Extra argument \""<<x.substr(first, x.length())<<"\"!!"<<endl;
                }
            }
        }
        else if (u[0] == "EXIT")
            break;
        else{
            // cout << "Error: unknown command: \""<< u[0] << "\"" << endl;
        }
    }
}

bool AllisNum(string str)  {  
    vector<string>::size_type i;
    for ( i= 0; i < str.size(); i++){
        int tmp = (int)str[i];
        if (tmp >= 48 && tmp <= 57)
            continue;
        else
            return false;
    } 
    return true;
}
