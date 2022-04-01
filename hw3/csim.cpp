//jfinke14
//nbalaji2
#include <stdlib.h>
#include <iostream>
#include <string>
#include <set>
#include <sstream>
//#include <stdio>
#include <vector>
using namespace std;
//PROBLEMS: how to make the found access the right index -- currently getting index of two when the set count is only 2
//how to do time acess when does that get incremented and how do u use it?
//when do you evict? -- how do we evict based on -- how to determine what is least recently used based on counter
//if it calcluates that it is index 2 does that mean it is
/// 

struct Slot {
  Slot (uint32_t, bool, bool);
  Slot() : tag(0), valid(false), dirty(false){}
  uint32_t tag;
  bool valid = false; //valid means "empty" if valid, slot holds information. if invalid, no info                                                                    
  unsigned int loadTime;
  unsigned accessTime;
  bool dirty;
  unsigned setNumber;
};
struct Set {
  unsigned int  numBlocksFilled = 0;
  vector <Slot> blocks;
  vector<Slot>::iterator recent;
};
struct Cache {
  Cache(uint32_t,uint32_t,uint32_t, bool, bool, bool);                                                                                                                                                                   
  Cache() : numSets(0), numBlocks(0), numBytes(0), wa(false), wt (false), isLru(false){}
  vector <Set> sets; //each set composed of blocks                                                                                 int numSets;
  uint32_t numBlocks;
  uint32_t numSets;
  int numBytes;
  int slotsPerSet;
  bool wa; //write-allocate                                                                                                                                          
  bool wt = false; //write through                                                                                                                                   
  bool isLru; //LRU or FIFO                                                                                                                                          
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
void print(int loads, int stores, int loadHits, int loadMiss, int storeHit, int storeMiss, int totalCycles) {
  cout << "Total loads: " << loads << "\n";
  cout << "Total stores: " << stores << "\n";
  cout << "Load hits: " << loadHits << "\n";
  cout << "Load misses: " << loadMiss << "\n";
  cout << "Store hits: " << storeHit << "\n";
  cout << "Store misses: " << storeMiss << "\n";
  cout << "Total cycles: " << totalCycles << "\n";
}                   
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
unsigned int bitCount(unsigned int num) {
  unsigned  int count = 0;
  while (num != 1) {
    count++;
    num = num >> 1;
  }
  return count;
  //  return log2(num);                                                                                                                                                
}
bool found(Cache & c, unsigned int tag, unsigned int index) {
  if (c.sets.empty()) { //if no elements in cache                                                                                                                    
      return false;
  }                                                                      
  for (size_t i = 0; i < c.numBlocks /*c.sets.at(index).blocks.size()*/; i++) {
    if ((c.sets.at(index).blocks.at(i).tag == tag) && (c.sets.at(index).blocks.at(i).valid == true)) {
          return true;
      }
  }
  return false;
}

unsigned int findIndex(Cache & c, unsigned int tag, unsigned int index) {
  int foundIndex = -1;
  int tracker = 0;
  for (size_t i = 0; i < c.numBlocks /*c.sets.at(index).blocks.size()*/; i++) {
    if ((c.sets.at(index).blocks.at(i).tag == tag) && (c.sets.at(index).blocks.at(i).valid == true)) {
      foundIndex = tracker;
    }
  tracker++;
  }
  return foundIndex;
} 

Cache::Cache(uint32_t numSets,uint32_t numBlocks,uint32_t  numBytes,bool  wa,bool  wt,bool  isLru) : numSets{numSets}, numBlocks{numBlocks}, numBytes{numBytes}, wa{wa}, wt{wt}, isLr\
u{isLru} {}
Slot::Slot (uint32_t tag, bool valid , bool dirty) : tag{tag}, valid{valid}, dirty{dirty} {}
  int main(int argc, char *argv[]) {
    uint32_t sets = 0;
    uint32_t blocks = 0;
    uint32_t bytes = 0;
    bool hit = false;
    bool isLruOrFifo = false;
    bool writeAllocate = false;
    bool writeThrough = false;
    string  address;
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
    if ( !writeAllocate && !writeThrough) {
      std::cerr <<  "error: invalid parameters\n";
        return 1;
    }
    int offsetBits = bitCount(bytes);
    int indexBits = bitCount(sets);
    int tagBits = 32 - offsetBits - indexBits;

    int numLines = 0;
    int countSlotPerSets = 0;
    Cache cache(sets,blocks, bytes, writeAllocate, writeThrough, isLruOrFifo);
    cache.sets.resize(sets);
    vector<Set>:: iterator i;
    for (i = cache.sets.begin(); i != cache.sets.end(); ++i) {
      (*i).blocks.resize(blocks);
    }
    string fullLine;
    while (cin >> readOrWrite) {
      unsigned int address;
      cin >> std::hex >> address;
      uint32_t i = (address << tagBits) >> (tagBits + offsetBits);
      uint32_t t = ((1 << tagBits) - 1) & (address >> (32 - tagBits));                                                                         
      string extra;
      cin >> extra;
      if ((tagBits + offsetBits) == 32) {
          i = 0;
      }
      if (readOrWrite.compare("l") == 0) { //load                                                                                                                     
        vector<Slot>::iterator iterator;
        for (iterator = cache.sets.at(i).blocks.begin(); iterator != cache.sets.at(i).blocks.end(); ++iterator) {
          if ((*iterator).valid == true) {
            (*iterator).loadTime++;
          }
        }                                                                                                       
        (cache.totalLoads)++;
        Slot slot (t, true, false);
        slot.loadTime = 0;
        if (numLines == 0) {                                                                                                      
          cache.sets.at(i).blocks.at(0) = slot;
          //numLines++;
          (cache.sets.at(i).numBlocksFilled)++;
          (cache.loadMisses)++;
        } else if (!(found(cache, t, i))) { //if not found in cache, add to cache                                                                                  
          (cache.loadMisses)++;//hit = false;                                                                                                                      
                //check if numBLocks excceeds num blocks per set                                                                                                      
          if (blocks == (cache.sets.at(i).numBlocksFilled)) {
                  // if (isLruOrFifo == true) {                                                                                                                       
            vector<Slot>::iterator it;
            vector<Slot>::iterator max;
            int maxIndex = 0;
            int index = 0;
            for (it = cache.sets.at(i).blocks.begin(); it != cache.sets.at(i).blocks.end(); it++) {
                unsigned int maxLoadTime = 0;
                      // int numIndex = findIndex(cache, t, i);                                                                                                       
                if (cache.sets.at(i).blocks.at(index).loadTime > maxLoadTime) {
                    maxIndex = index;
                    maxLoadTime = cache.sets.at(i).blocks.at(index).loadTime;
                }
                    index++;
            }
            cache.sets.at(i).blocks.at(maxIndex) = slot;
                    //cache.sets.at(i).blocks.erase(max);                                                                                                             
          } else if (cache.sets.at(i).numBlocksFilled < blocks) {
                cache.sets.at(i).blocks.at(cache.sets.at(i).numBlocksFilled) = slot;                                                                                 
		(cache.sets.at(i).numBlocksFilled)++;
	  }
                //(cache.loadMisses)++;                                                                                                                               
	  //  (cache.sets.at(i).numBlocksFilled)++;
        } else { //if it is found in cache                                                                                                                         
          if (found(cache, t, i)) {                                                                                                         
            (cache.loadHits)++;
            (cache.totalCycles)++;
            if (isLruOrFifo) {
                vector<Slot>::iterator iterator;
                for (iterator = cache.sets.at(i).blocks.begin(); iterator != cache.sets.at(i).blocks.end(); iterator++) {
                    (*iterator).loadTime++;
                }
                  //if LRU, updated most recently accessed element                                                                                                    
                  //ERROR IS HERE USE FINDINDEX METHOD                                                                                                                
                int numIndex = findIndex(cache, t, i);
                  // int numIndex       = (cache.sets.at(i).numBlocksFilled) - 2;                                                                                     
                cache.sets.at(i).blocks.at(numIndex).loadTime = 0;
            }

          }
          cache.totalCycles += cache.numBytes / 4 * 100;
        }
      }	
      if (readOrWrite.compare("s") == 0) {  
        vector<Slot>::iterator storeIt;
        for (storeIt = cache.sets.at(i).blocks.begin(); storeIt != cache.sets.at(i).blocks.end(); ++storeIt) {
          if ((*storeIt).valid == true) {
	          (*storeIt).loadTime++;
	        }
        }
	  //(cache.sets.at(i).blocks.at(i).loadTime) = 0;
	      (cache.totalStores)++;
	      Slot slot (t, true, false);
	      slot.loadTime = 0;
        if (!(found(cache, t, i))) { //if not found
          (cache.storeMisses)++;
          if (cache.wa == false) {
            cache.totalCycles += 100;
          } else if (cache.wa == true) {
		//check if numBLocks excceeds num blocks per set                                                                 
            if (blocks == (cache.sets.at(i).numBlocksFilled)) {
		          vector<Slot>::iterator it;
              vector<Slot>::iterator max;
              int maxIndex = 0;
              int index = 0;
		        for (it = cache.sets.at(i).blocks.begin(); it != cache.sets.at(i).blocks.end(); it++) {
              unsigned int maxLoadTime = 0;
                      // int numIndex = findIndex(cache, t, i);                                                                                       
              if (cache.sets.at(i).blocks.at(index).loadTime > maxLoadTime) {
                maxIndex = index;
                maxLoadTime = cache.sets.at(i).blocks.at(index).loadTime;
              }
              index++;
            }
              cache.sets.at(i).blocks.at(maxIndex) = slot;
                    //cache.sets.at(i).blocks.erase(max);                                                                         
            } else if (cache.sets.at(i).numBlocksFilled < blocks) {
              cache.sets.at(i).blocks.at(cache.sets.at(i).numBlocksFilled) = slot;
		          (cache.sets.at(i).numBlocksFilled)++;          
		}
            (cache.totalCycles) += 100 * cache.numBytes / 4; //calculate cycles
            (cache.totalCycles)++;
		       if (cache.wt == true) {
		          cache.totalCycles += 100;
		       }
		      }
	     } else if (found(cache, t, i)) {//if found & already in cache
	        (cache.storeHits)++;
		      (cache.totalCycles)++;
        if (cache.wt) {
          (cache.totalCycles) += 100;
        } else {
		      int numIndex = findIndex(cache, t, i);
		  //write only to cache and mark block as dirty
		      cache.sets.at(i).blocks.at(numIndex).dirty = true;
	    	}
        vector<Slot>::iterator iterator;
        for (iterator = cache.sets.at(i).blocks.begin(); iterator != cache.sets.at(i).blocks.end(); ++iterator) {
          (*iterator).loadTime++;
        }
		                    //if LRU, updated most recently accessed element                                                                  
                  //(cache.sets.at(i).blocks.at(cache.sets.at(i).blocks.at(cache.sets.at(i).numBlocksFilled - 1).loadTime) = 0;
		    if (isLruOrFifo) {
		      unsigned int numIndex = findIndex(cache, t, i);
          cache.sets.at(i).blocks.at(numIndex).loadTime = 0;
		  }
 	  }				
	  }
    numLines++; //to track number of lines read from trace file
    //cache.numBytes = bytes; //update numBytes
	
     	}
    

    //print output
    print(cache.totalLoads, cache.totalStores, cache.loadHits, cache.loadMisses, cache.storeHits, cache.storeMisses, cache.totalCycles);
        
    return 0;

}

 
