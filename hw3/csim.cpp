#include <stdlib.h>
#include <iostream>
#include <string>
#include <set>
#include <vector>
//#include "block.h"
using namespace std;


struct Slot {
    unsigned int tag;
    bool valid = false; //valid means "empty" if valid, slot holds information. if invalid, no info
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
    int numBytes;
    int slotsPerSet;
    bool wa;
    bool wt = false;
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
void print(int loads, int stores, int loadHits, int loadMiss, int storeHit, int storeMiss, int totalCycles) {
    cout << "Total loads: " << loads << "\n";
    cout << "Total stores: " << stores << "\n";
    cout << "Load hits: " << loadHits << "\n";
    cout << "Load misses: " << loadMiss << "\n";
    cout << "Store hits: " << storeHit << "\n";
    cout << "Store misses: " << storeMiss << "\n";
    cout << "Total cycles: " << totalCycles << "\n";
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


bool found(Cache * c, unsigned int tag, unsigned int index, bool wt, bool isLru) {
    if (c->sets.empty()) {
        return false;
    }
    vector<Slot>::iterator it;
    for (it = c->sets.at(index).blocks.begin(); it != c->sets.at(index).blocks.end(); ++it) {
        if ((*it).tag == tag) {
            if (isLru) {
                //takes the slot with the correct tag and moves it to front 
                c->sets.at(index).blocks.insert(c->sets.at(index).blocks.begin(), *it);
                //erases the old slot
                c->sets.at(index).blocks.erase(it);
            }
            if (wt == true) {
                (*it).dirty = true;
            }
            return true;
        }
    }
    return false;
}

bool found(Cache * c, unsigned int tag, unsigned int index, bool isLru) {
    if (c->sets.empty()) {
        return false;
    }
    vector<Slot>::iterator it;
    for (it = c->sets.at(index).blocks.begin(); it != c->sets.at(index).blocks.end(); ++it) {
        if ((*it).tag == tag) {
            if (isLru) {
                //takes the slot with the correct tag and moves it to front 
                c->sets.at(index).blocks.insert(c->sets.at(index).blocks.begin(), *it);
                //erases the old slot
                c->sets.at(index).blocks.erase(it);
            }
            return true;
        }
    }
    return false;
}


int main(int argc, char *argv[]) {
    int sets = 0;
    int blocks = 0;
    int bytes = 0;
    bool hit = false;
    bool isLru = false; 
    unsigned int address;
    string readOrWrite;
    Cache *cache = new Cache();

    if (argc != 7) { //check for valid number of arguments
        std::cerr << "error: invalid number of arguments\n"; 
        return 1;
    }
    string setString = argv[1];
    string blockString = argv[2];
    sets = std::stoi(setString);
    blocks = std::stoi(blockString);
    if (blocks < 4) {
        std::cerr <<  "error: invalid block size\n";
        return 1;
    }
    if (!powerOfTwo(blocks) || !powerOfTwo(sets)) {
        std::cerr <<  "error: not power of two\n";
        return 1;
    }
    string byteString = argv[3];
    bytes = std::stoi(byteString);
    string input = argv[4];
    if (input.compare("write-allocate") == 0) {
        cache->wa = true;
    }
    input = argv[5];
    if (input.compare("write-through") == 0) {
        cache->wt = true;
    }
    input = argv[6];
    if (input.compare("lru") == 0) {
        cache->isLru = true;
    }

    int offsetBits = bitCount(blocks);
    int indexBits = bitCount(sets);
    int tagBits = 32 - (offsetBits + indexBits);
    //address = address >> offsetBits;
    
    
    string line;
    //long address;
    
    while (cin >> readOrWrite) { 
        //cin >> readOrWrite;
        cin >> std::hex >> address;
        //string memAddressString;
        //cin >> memAddressString;
        string extra;
        cin >> extra;


        // for (int i = 0; i < 4; i++) {
        //     memAddressString += hexToBinary(memAddressString[i]);
        // }
        //string readOrWrite = line.substr(0, 1);
        //string memAddress = line.substr(4, 8);
        //int memAddress = std::stoi(memAddressString) >> offsetBits;
        //long memAddress = address >> offsetBits;
        //string memAddressString = to_string(memAddress);
       // string tag = memAddressString.substr(0,tagBits);
       long offset = ((1 << offsetBits) - 1) & address;
       long i = ((1 << indexBits) - 1) & (address >> offsetBits);
       long t = ((1 << tagBits) - 1) & (address >> (32 - tagBits));

       // unsigned int t = stoi(tag);
       // string index = memAddressString.substr(tagBits, indexBits);
        //int i = stoi(index);
        vector<Slot>::iterator it;
        
        if (readOrWrite.compare("l") == 0) {
            (cache->totalLoads)++;
            //vector<slot> slots = cache->sets.at(index);
            if (cache->sets.at(i).blocks.empty()) { //check if cache at index is empty
                hit = false;
                Slot * s = new Slot();
                    s->dirty = false;
                    s->tag = t;
                    s->valid = true;
                    //add to beginning of set blocks
                    Slot old = cache->sets.at(i).blocks.back();
                    cache->sets.at(i).blocks.pop_back();
                    cache->sets.at(i).blocks.insert(cache->sets.at(i).blocks.begin(), *s);
            } else {
                if (found(cache, t, i, isLru)) {
                    hit = true;
                    (cache->loadHits)++;
                    (cache->totalCycles)++;
                }
            }
            
        hit = false;
        (cache->loadMisses)++; 
        cache->totalCycles += cache->numBytes / 4 * 100;
        //rearrange cache
        }
        if (readOrWrite.compare("s") == 0) {  
            (cache->totalStores)++;
//cache->sets.at(i).blocks.empty() ||
            if (!(found(cache, t, i, isLru))) {
                //hit = false;
                (cache->storeMisses)++;
                if (cache->wt == false) {
                    cache->totalCycles += 100;
                } 
                if (cache->wa == true) {
                    Slot * s = new Slot();
                    s->dirty = true;
                    s->tag = t;
                    s->valid = true;
                    //add to beginning of set blocks
                    struct Slot old; 
                    if (!cache->sets.empty()) {
                        old = cache->sets.at(i).blocks.back();
                        cache->sets.at(i).blocks.pop_back();
                    }
                    cache->sets.at(i).blocks.insert(cache->sets.at(i).blocks.begin(), *s);

                    //cache->sets.at(stoi(index)).blocks.erase(cache->sets.at(stoi(index)).blocks.end());
                    //remove last
                    //add to front 
                    (cache->totalCycles) += 100 * cache->numBytes / 4;
                    (cache->totalCycles)++;
                    if (cache->wt && old.dirty) {
                        (cache->totalCycles) += 100 * cache->numBytes / 4;
                    } else if (!cache->wt){
                        (cache->totalCycles) += 0; 
                    }
                }
            } else if (found(cache, t, i, cache->wt)) { 
                (cache->storeHits)++;
                if (cache->wt) {
                    (cache->totalCycles) += 100;
                }

            }
        }    
    }
    print(cache->totalLoads, cache->totalStores, cache->loadHits, cache->loadMisses, cache->storeHits, cache->storeMisses, cache->totalCycles);
        
    return 0;
}