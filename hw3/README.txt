Jessi Finkelstein - jfinke14
Nandita Balaji - nbalaji2
We worked together on every element of the project doing paired programing.


We set up the experiment by using the same cache the same size and then going through the different situations including write-allocate write through and write allocate write back. We also did each step using both LRU and fifo. The worst of all was the direct map as the cycles were consistently the highest. The best according to cycles though not very cost effective were the assosiative cache method. The best in that section was the write allocate write through when using fifo. 




1. Direct Map
2. Associative Cache
3. Set-Associative Cache

Direct Map:
./csim 64 1 16 write-allocate write-through lru < gcc.trace
Total loads: 318197
Total stores: 197486
Load hits: 260084
Load misses: 58113
Store hits: 173877
Store misses: 23609
Total cycles: 133683370

./csim 64 1 16 write-allocate write-through fifo < gcc.trace
Total loads: 318197
Total stores: 197486
Load hits: 260084
Load misses: 58113
Store hits: 173877
Store misses: 23609
Total cycles: 133683370

./csim 64 1 16 write-allocate write-through lru < swim.trace
Total loads: 220668
Total stores: 82525
Load hits: 185119
Load misses: 35549
Store hits: 66100
Store misses: 16425
Total cycles: 89137744

./csim 64 1 16 write-allocate write-through fifo  < swim.trace
Total loads: 220668
Total stores: 82525
Load hits: 185119
Load misses: 35549
Store hits: 66100
Store misses: 16425
Total cycles: 89137744

./csim 64 1 16 write-allocate write-back lru < swim.trace
Total loads: 220668
Total stores: 82525
Load hits: 185119
Load misses: 35549
Store hits: 66100
Store misses: 16425
Total cycles: 80885244

./csim 64 1 16 write-allocate write-back lru < gcc.trace
Total loads: 318197
Total stores: 197486
Load hits: 260084
Load misses: 58113
Store hits: 173877
Store misses: 23609
Total cycles: 113934770

./csim 64 1 16 write-allocate write-back fifo  < gcc.trace
Total loads: 318197
Total stores: 197486
Load hits: 260084
Load misses: 58113
Store hits: 173877
Store misses: 23609
Total cycles: 113934770

Associative Cache:

./csim 1 4 16 write-allocate write-through lru < gcc.trace
Total loads: 318197
Total stores: 197486
Load hits: 120364
Load misses: 197833
Store hits: 101401
Store misses: 96085
Total cycles: 106646050

./csim 1 4 16 write-allocate write-through fifo < gcc.trace
Total loads: 318197
Total stores: 197486
Load hits: 118260
Load misses: 199937
Store hits: 100341
Store misses: 97145
Total cycles: 106226346

./csim 1 4 16 write-allocate write-through lru < swim.trace
Total loads: 220668
Total stores: 82525
Load hits: 64398
Load misses: 156270
Store hits: 39571
Store misses: 42954
Total cycles: 51340223

./csim 1 4 16 write-allocate write-back lru < swim.trace
Total loads: 220668
Total stores: 82525
Load hits: 64398
Load misses: 156270
Store hits: 39571
Store misses: 42954
Total cycles: 43087723

./csim 1 4 16 write-allocate write-back fifo < swim.trace
Total loads: 220668
Total stores: 82525
Load hits: 62710
Load misses: 157958
Store hits: 39287
Store misses: 43238
Total cycles: 42524435

./csim 1 4 16 write-allocate write-back lru < gcc.trace
Total loads: 318197
Total stores: 197486
Load hits: 120364
Load misses: 197833
Store hits: 101401
Store misses: 96085
Total cycles: 86897450

Set-Associative Cache:
./csim 64  4 16 write-allocate write-through lru < gcc.trace
Total loads: 318197
Total stores: 197486
Load hits: 312099
Load misses: 6098
Store hits: 187526
Store misses: 9960
Total cycles: 149081785
./csim 64  4 16 write-allocate write-through lru < swim.trace
Total loads: 220668
Total stores: 82525
Load hits: 214923
Load misses: 5745
Store hits: 71353
Store misses: 11172
Total cycles: 98987948

./csim 64  4 16 write-allocate write-back lru < swim.trace
Total loads: 220668
Total stores: 82525
Load hits: 214923
Load misses: 5745
Store hits: 71353
Store misses: 11172
Total cycles: 90735448
