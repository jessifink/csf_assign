// C implementation of hexdump main function

#include "hexfuncs.h"  // this is the only header file which may be included!
int main(void) { 
  char input[16] = {' '};
  char output[16] = {' '};
  int count = 0;
  int length = hex_read(input); 
  while (length <= 16 && length > 0) {
    hex_format_offset(count,output);
    hex_write_string(output);
    hex_write_string(": ");
    for (int i = 0; i < length; i++ ) {    
      hex_format_byte_as_hex(input[i],output);
      hex_write_string(output);
      hex_write_string(" ");
    }
    
    int l = length;
    while (l < 16) {
      write(1, "    ", 3);
      l++;  
    }
    count += 16;
    hex_write_string(input); 
    length = hex_read(input);
    write(1,"\n", 1);
  }
  
  return 0;
}