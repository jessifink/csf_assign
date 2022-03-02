// C implementation of hexdump main function

#include "hexfuncs.h"  // this is the only header file which may be included!


int main(void) { 
  char input[16] = {' '};
  char output[16] = {' '};
  char offset[16] = {' '};
  int count = 0;
  unsigned int length = 0;
  int done = 0;
  while (!done) {
    input[0] = '\0';
    length = hex_read(input);
    input[length] = '\0';
    if (length <= 0) {
      done = 1;
    } else {
      hex_format_offset(count,output);
      hex_write_string(output);
      hex_write_string(":");
      for (int i = 0; i < length; i++ ) {    
        hex_write_string(" ");
        hex_format_byte_as_hex(input[i],output);
        hex_write_string(output);
      }
      int l = length;
      while (l < 16) {
        write(1, "   ", 3);
        l++;  
      }
      hex_write_string("  ");
      count += 16;
      for (int i = 0; i < length; i++) {
        char cbuf[2];
        cbuf[0] = (char) hex_to_printable(input[i]);
        cbuf[1] = '\0';
        hex_write_string(cbuf);
      }
      write(1,"\n", 1);
    }
  }
  
  return 0;
}
