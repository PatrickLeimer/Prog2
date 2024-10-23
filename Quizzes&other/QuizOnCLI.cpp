// Write the int main() function that is able to take in
// command line arguments, and print the first argument

#include <iostream>
using namespace std;

int main(int argc, char* argv[]) {
    cout << (argc - 1) << " : " << argv[1] << endl;
    return 0;
}

// #Create the default build rule below
//
// all: myprogram.out
//
// myprogram.out: main.cpp
//     g++ -o myprogram.out main.cpp
//
// run:
//     ./myprogram.out I Love Programming 2
//
// clean:
//     rm -f myprogram.out