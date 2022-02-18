// C implementation of hexdump main function

#include "hexfuncs.h"  // this is the only header file which may be included!

int main() { 
  char * input; 
  int count = 0;
  int length = hex_read(input);
  printf("%d \n", length);
  while (length > 0) {
    hex_format_offset(count,input);
    hex_write_string(input);
    write(1,": ", 1); 
    for (int i = 0; i < length; i++ ) {
      hex_to_printable(input[i]);
      hex_format_byte_as_hex(input[i],input);
      write(1,"  ", 1); 
    }
    count += 16;
    hex_write_string(input);
    write(1,"\n", 1); 
  }
}
