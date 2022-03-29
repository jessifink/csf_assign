//jfinke14
//nbalaji2
#include <stdlib.h>
#include <iostream>
#include <string>
#include <set>
#include <vector>
using namespace std;
//PROBLEMS: how to make the found access the right index -- currently getting index of two when the set count is only 2
//how to do time acess when does that get incremented and how do u use it?
//when do you evict? -- how do we evict based on -- how to determine what is least recently used based on counter
//if it calcluates that it is index 2 does that mean it is
/// 
struct Slot {
  Slot (int, bool, bool);
  Slot() : tag(0), valid(false), dirty(false){}
    unsigned int tag;
    bool valid = false; //valid means "empty" if valid, slot holds information. if invalid, no info
  unsigned loadTime; 
    unsigned accessTime;
    bool dirty;
  unsigned setNumber;
};
struct Set {
    vector <Slot> blocks;
};
struct Cache {
  vector <Set> sets; //each set composed of blocks
    int numSets; 
    int numBlocks;
    int numBytes;
    int slotsPerSet;
    bool wa; //write-allocate
    bool wt = false; //write through
    bool isLru; //LRU or FIFO
   Cache(int,int,int, bool, bool, bool);
  Cache() : numSets(0), numBlocks(0), numBytes(0), wa(false), wt (false), isLru(false){}
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
    
    if (block == 1) {
        return 1;
    }
    while (block != 1)
    {
        if (block % 2 != 0)
            return 0;
        block /= 2; 
    }
    return 1;
}


bool found(Cache & c, unsigned int tag, unsigned int index) {
    if (c.sets.empty()) { //if no elements in cache
        return false;
    }
    vector<Slot>::iterator it; //to check every element of cache
    //    for (it = c.sets.at(index).blocks.begin(); it != c.sets.at(index).blocks.end(); ++it) {
    for (size_t i = 0; i < c.sets.at(index).blocks.size(); i++) {
      if (((c.sets.at(index).blocks.at(i)).tag == tag) && ((c.sets.at(index).blocks.at(i)).valid == true)) {
            return true;
        }
    }
    return false;
}

//bool found(Cache * c, unsigned int tag, unsigned int index, bool isLru) {
//  if (c->sets.empty()) {
//      return false;
//  }
//  vector<Slot>::iterator it;
//  for (it = c->sets.at(index).blocks.begin(); it != c->sets.at(index).blocks.end(); ++it) {
//      if ((*it).tag == tag) {
/*          if (isLru) {
                //takes the slot with the correct tag and moves it to front 
                c->sets.at(index).blocks.insert(c->sets.at(index).blocks.end(), *it);
                //erases the old slot
                c->sets.at(index).blocks.erase(it);
            }
            return true;
        }
    }
    return false;
}
*/
Cache::Cache(int numSets,int numBlocks,int  numBytes,bool  wa,bool  wt,bool  isLru) : numSets{numSets}, numBlocks{numBlocks}, numBytes{numBytes}, wa{wa}, wt{wt}, isLru{isLru} {}
Slot::Slot (int tag, bool valid , bool dirty) : tag{tag}, valid{valid}, dirty{dirty} {} 
int main(int argc, char *argv[]) {
    int sets = 0;
    int blocks = 0;
    int bytes = 0;
    bool hit = false;
    bool isLruOrFifo = false;
    bool writeAllocate = false;
    bool writeThrough = false;
    unsigned int address;
    string readOrWrite;

    if (argc != 7) { //check for valid number of arguments
        std::cerr << "error: invalid number of arguments\n"; 
        return 1;
    }
    string setString = argv[1];
    string blockString = argv[2];
    sets = std::stoi(setString);
    blocks = std::stoi(blockString);

    //to check if arguments are valid
    if (!powerOfTwo(blocks) || !powerOfTwo(sets)) {
        std::cerr <<  "error: not power of two\n";
        return 1;
    }
    string byteString = argv[3];
    bytes = std::stoi(byteString);
    
    if (bytes < 4) {
        std::cerr <<  "error: invalid block size\n";
        return 1;
    }

    //storing argument values
    string input = argv[4];
    if (input.compare("write-allocate") == 0) {
	writeAllocate = true;
    }
    input = argv[5];
    if (input.compare("write-through") == 0) {
	 writeThrough = true;
    }
    input = argv[6];
    if (input.compare("lru") == 0) {
	isLruOrFifo = true;
    }

    int offsetBits = bitCount(blocks);
    int indexBits = bitCount(sets);
    int tagBits = 32 - (offsetBits + indexBits);

    int numLines = 0;
    int countSlotPerSets = 0; 
    Cache cache(sets,blocks, bytes, writeAllocate, writeThrough, isLruOrFifo);
    cache.sets.resize(sets);
    vector<Set>:: iterator i;
    for (i = cache.sets.begin(); i != cache.sets.end(); ++i) {
      (*i).blocks.resize(blocks);
    }

    //evict when the number of blocks exceed the amount possible for one set -- everytime cache miss and have to add in new block you check 
    //when access a block set time to 0 increment the rest of the blocks by 1 (load or write = acess) evict the block with highest number LRU for both difference between LRU and fifo - in LRU if cache hit set time to 0 and the rest of the blocks get incremented by 1 
    //Fifo -- evict the one with the highest time 
    //evict is just using erase function
    //read in from trace file
    while (cin >> readOrWrite) {  
        cin >> std::hex >> address;
        string extra;
        cin >> extra;

        //store offset, index, and tag
        long offset = ((1 << offsetBits) - 1) & address;
        long i =
	  //take the first number given (set count) log 2 of that number has to be a power of 2 -- ex: 16 4 bits those 4 bits will be combined bacislay in binary to be 0 -15 
	  // ((1 << indexBits) - 1) & (address >> offsetBits);
        long t = ((1 << tagBits) - 1) & (address >> (32 - tagBits));

        vector<Slot>::iterator it;
        //initialize to empty vectors/set
	//vector<Set> * cacheSets = new vector<Set>;
	//cache->sets = *cacheSets;
	//Set * s = new Set();
	//cache->sets.push_back(*s);
	//std::vector<Slot> * block = new vector<Slot>;
	//	cache->sets.at(i).blocks = *block;
        
        if (readOrWrite.compare("l") == 0) { //load
            (cache.totalLoads)++;
	    Slot slot (t, true, true);

            if (numLines == 0) {
                //since cache is currently empty, add into cache
                hit = false;
                (cache.sets.at(i).blocks).push_back(slot);
		countSlotPerSets++;
                (cache.loadMisses)++; 
            } else if (!(found(cache, t, i))) { //if not found in cache, add to cache
                hit = false;
                (cache.sets.at(i).blocks).push_back(slot);
		countSlotPerSets++;
                (cache.loadMisses)++; 

                if (countSlotPerSets == blocks ) { //if space is full, evict
		  //(cache->sets.at(i).blocks).erase(*((cache->sets.at(i).blocks).begin()));
		  //  (cache.sets.at(i).blocks).erase((cache.sets.at(i).blocks).begin());
                }

            } else { //if it is found in cache
                if (found(cache, t, i)) {
                hit = true;
                (cache.loadHits)++;
                (cache.totalCycles)++;
                } 
            }
            
    
        cache.totalCycles += cache.numBytes / 4 * 100;
        
        }

        if (readOrWrite.compare("s") == 0) {  
            (cache.totalStores)++;

            if (!(found(cache, t, i))) { //if not found
                (cache.storeMisses)++;
                if (cache.wt == false) {
                    cache.totalCycles += 100;
                } 
                if (cache.wa == true) {
		    Slot old (t, true, true);
		    Slot s (t, true, true);
		    // if (!cache.sets.empty()) { //if not empty, evict
		      //    old = cache.sets.at(i).blocks.front();
		      // cache.sets.at(i).blocks.erase(cache.sets.at(i).blocks.begin()); //erase oldest element
		      // }
                    cache.sets.at(i).blocks.push_back(s); //add to cache

                    (cache.totalCycles) += 100 * cache.numBytes / 4; //calculate cycles
                    (cache.totalCycles)++;

                    if (cache.wt && old.dirty) {
                        (cache.totalCycles) += 100 * cache.numBytes / 4;
                    } else if (!cache.wt){
                        (cache.totalCycles) += 0; 
                    }
                }
            } else if (found(cache, t, i)) {  //if found & already in cache
                (cache.storeHits)++;
                if (cache.wt) {
                    (cache.totalCycles) += 100;
                }

            }
        }
    numLines++; //to track number of lines read from trace file
    cache.numBytes = bytes; //update numBytes
    }

    //print output
    print(cache.totalLoads, cache.totalStores, cache.loadHits, cache.loadMisses, cache.storeHits, cache.storeMisses, cache.totalCycles);
        
    return 0;
}
