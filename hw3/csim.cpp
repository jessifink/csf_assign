#include <stdlib.h>
#include <iostream>
#include <string>
#include <set>
#include <vector>
#include "block.h"
using namespace std;


struct Slot {
    unsigned tag;
    bool valid;
    unsigned loadTime;
    unsigned accessTime;
    bool dirty;
};
struct Set {
    vector <Slot> blocks;
};
struct Cache {
    vector <Set> sets;
    int numSets;
    int numBlocks;
    int numBites;
    int slotsPerSet;
    bool wa;
    bool wt;
    bool isLru;
    int loadHits = 0;
    int loadMisses = 0;
    int storeHits = 0;
    int storeMisses = 0;
    int totalLoads = 0;
    int totalStores = 0;
    int totalCycles = 0;
    int currentTime;
};


//valid if there is data at that spot in cache
//load time increases every load or store
//accesstime - hits 

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
bool powerOfTwo(int block) {
    if (block == 0)
        return 0;
    while (block != 1)
    {
        if (block % 2 != 0)
            return 0;
        block /= 2; 
    }
    return 1;
}

string hexToBinary(char c) {
    switch (toupper(c)) {
        case '0': return "0000";
        case '1': return "0001";
        case '2': return "0010";
        case '3': return "0011";
        case '4': return "0100";
        case '5': return "0101";
        case '6': return "0110";
        case '7': return "0111";
        case '8': return "1000";
        case '9': return "1001";
        case 'A': return "1010";
        case 'B': return "1011";
        case 'C': return "1100";
        case 'D': return "1101";
        case 'E': return "1110";
        default: return "1111";
    }
}
bool checkTag(string readOrWrite, string tag, Chache cache) {
    // iterate through all of the cache check to see if the tag matches anything already in cache 
    //if isLRU load: then move to front 
    //store create new block with dirty bit set to true, and move to front and delete old block
}

void handleStore(string tag, bool found, Cache cache) {
    totalStores++;
    
}



int main(int argc, char *argv[]) {
    int sets = 0;
    int blocks = 0;
    int index;
    int bytes = 0;
    bool wa = false;
    bool wt = false;
    bool isLru = false;
    string file_name = "";
    unsigned int address;
    char lors;
    if (argc != 8) { //check for valid number of arguments
        std::cerr << "error\n"; 
        return 1;
    }
    sets = std::stoi(argv[1]);
    blocks = std::stoi(argv[2]);
    if (blocks < 4) {
        std::cerr <<  "error\n";
        return 1;
    }
    if (!powerOfTwo(blocks) || !powerOfTwo(sets)) {
        std::cerr <<  "error\n";
        return 1;
    }
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
    int tagBits = 32 - (offsetBits + indexBits);
    //address = address >> offsetBits;
    index = address % sets;
    Cache *cache = new Cache();
    string line;
    while (getline(cin, line)) { 
        string readOrWrite = line.substr(0, 1);
        string memAddress = line.substr(4, 8);
        for (int i = 0; i < 4; i++) {
            memAddress += hexToBinary(memAddress[i]);
        }
        memAddress = std::stoi(memAddress) >> offsetBits;
        string tag = memAddress.substr(0,tagBits);
        string index = memAddress.substr(tagBits, indexBits);
        bool found = checkTag(readOrWrite,tag, cache); 
        handleStore(tag, found,cache);

        
    }
    return 0;
}