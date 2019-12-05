//
//  main.cpp
//  do_file
//
//  Created by yangchienying on 2019/12/5.
//  Copyright © 2019 yangchienying. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

using namespace std;
int main(int argc, char *argv[]) {
    ofstream o("doC499_r");
    for(int i = 1; i <= 1; ++i) {
//        if(i == 13) continue;
        string file, dummy, location;
        stringstream ss;
        int size, out, level = 100;
//        if(i < 10) ss << "test" << i << ".aag";
//        else ss << "test" << i << ".aag";
        ss<<"C499_r.aag";
        file = ss.str();
        location = "tests.fraig/ISCAS85/" + file;
        ifstream in(location.c_str());
        in >> dummy >> size >> out >> out >> out;
        in.close();
        o << "cirr -r " << location << endl
        << "cirp" << endl
        << "cirp -n" << endl
        << "cirp -pi" << endl
        << "cirp -po" << endl
        << "cirp -fl" << endl;
        for(int j = 0; j <= size + out; ++j) {
            o << "cirg " << j << endl
            << "cirg " << j << " -fani " << level << endl;
//            << "cirg " << j << " -fano " << level << endl;
        }
        o << "cirw" << endl;
    }
    o << "q -f" << endl;
}
