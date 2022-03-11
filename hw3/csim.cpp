#include <stdlib.h>
#include <iostream>
#include <string>

using namespace std;

int bitCount(int num) {
    int count = 0;
    while (num != 0) {
        count++;
        num = num >> 1;
    }
    return count;
}
//direct map - split middle adress into three parts index points to particular set in cache block is the contetent need to compare tag for every set number / index use tag/ first aprt of memory adress to compare to the tag part in the set if match stores content you want hit if matches 
//ignore offset

//allocate 
//block size - 2^8 -- 8 bits for offset 
//num bits for index depends on number of sets/ slots -- index   number of sets is 2^10 10 bits for index
//whats left is the tag bits 

//direct map 1 block inside 1 set 
//if memory adress is in cache storing writing to cache reading from cache read hit loading 
//write through modify cache and memory the same adress 
//write back - modify content in cache but not memory 
//gets 
//same inex different tag different adress 
//same index new tag number eveicts the adress -- new adress has same index but different tag which would cause you to change the cache
// before that happens move the past index and adress to main memory --write back 
//write miss - same index different tag -- write allocate bring content from main memory to cache same index different tag 
// no write allocate - dont care about cache if cache doesnt match directly modify main memory 




int main(int argc, char *argv[]) {
    int sets = 0;
    int blocks = 0;
    int bytes = 0;
    bool wa = false;
    bool wt = false;
    bool isLru = false;
    string file_name = "";
    char lors;
    if (argc != 8) { //check for valid number of arguments
        std::cerr << "error\n"; 
        return 1;
    }
    sets = std::stoi(argv[1]);
    blocks = std::stoi(argv[2]);
    bytes = std::stoi(argv[3]);

    string input = argv[4];
    string cmp = "write-allocate";
    if (input == cmp) {
        wa == true;
    }
    input = argv[5];
    cmp = "write-through";
    if (input == cmp) {
        wt == true;
    }
    input = argv[6];
    cmp = "lru";
    if (input == cmp) {
        isLru = true;
    }
    input = argv[7];
    file_name = input;
    int offsetBits = bitCount(blocks);
    int indexBits = bitCount(sets);
    return 0;
}