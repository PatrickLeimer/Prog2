//
// Created by Patrick on 10/15/2024.
//

#include <iostream>
#include <fstream>
#include <string>
using namespace std;

string readFirstLine(string s) {
    ifstream file(s);
    string fLine;

    if(file.is_open()) {
        getline(file, fLine);
        file.close();
        return fLine;
    } else {
        cout << "Unable to open file" << s << endl;
    }

}

int readIntBin(string s) {
    int value = 0;
    ifstream file(s, ios::binary);

    if(file.is_open()) {
        file.read((char*)&value, sizeof(value));
        file.close();
        return value;
    } else {
        cout << "Error when opening file" << s << endl;
    }
}