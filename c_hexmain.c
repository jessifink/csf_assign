// C implementation of hexdump main function

#include "hexfuncs.h"  // this is the only header file which may be included!
#include "stdio.h"

int main(void) { 
  char input[100]; 
  char output[100];
  int count = 0;
  int length = hex_read(input);
  while (length > 0) {
    hex_format_offset(count,output);
    hex_write_string(output);
    write(1,": ", 1); 
    for (int i = 0; i < length;  i++ ) {
      printf("%c", input[i]);
      //hex_to_printable(input[i]);
      hex_format_byte_as_hex(input[i],output);
      hex_write_string(output);
      write(1,"  ", 1); 
    }
    count += 16;
    hex_write_string(output);
    write(1,"\n", 1); 
    length = hex_read(input);
  }
  return 0;
}
