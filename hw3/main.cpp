#include <stdlib.h>
#include <iostream>
#include <string>

using namespace std;

int main(int argc, char *argv[]) {
    int sets = 0;
    int blocks = 0;
    int bytes = 0;
    bool wa = false;
    bool wt = false;
    bool isLru = false;
    string file_name = "";


    if (argc != 8) { //check for valid number of arguments
        return 1;
    }

    sets = (int) argv[1];
    blocks = (int) argv[2];
    bytes = (int) argv[3];

    string input = argv[4];
    string cmp = "write-allocate";
    if (input == cmp) {
        wa == true;
    }

    input = argv[5];
    string cmp = "write-through";
    if (input == cmp) {
        wt == true;
    }

    input = argv[6];
    string cmp = "lru";
    if (input == cmp) {
        isLru = true;
    }

    input = argv[7];
    file_name = input;

    return 0;
}