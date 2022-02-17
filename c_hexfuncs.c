// C implementation of hexdump functions

#include <unistd.h>  // this is the only system header file you may include!
#include "hexfuncs.h"


unsigned hex_read(char data_buf[]) {
  return read(STDIN_FILENO, data_buf, 16) -1 ;
}

void hex_write_string(const char s[]) {
    write(1, s, 16); 
}

void hex_format_offset(unsigned offset, char sbuf[]) {
  //repeat steps every remainder you have is a digit in hex numbers keep track how many times have to divide 
  //once we do that we know how much we need to pad 
  if (offset < 16) {
    offset << 16    
  }
}

void hex_format_byte_as_hex(unsigned char byteval, char sbuf[]) {
  //TODO
}

char hex_to_printable(unsigned char byteval) {
  return 'a';
}

// TODO: add function implementations here
