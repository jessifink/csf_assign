// C implementation of hexdump functions

#include <unistd.h>  // this is the only system header file you may include!
#include "hexfuncs.h"


unsigned hex_read(char data_buf[]) {
  //declare array of null pointers
  for (int i = 0; i < 16; i++) {
    data_buf[i] = '\0';
  }
  //read in information
  return read(STDIN_FILENO, data_buf, 16);
}

//strlen function
int string_len (const char s[]) {
 int count = 0;
  while (*s != '\0') {
    s++;
    count++;
  }
  if (count == 0) {
    count = 16;
  }
  return count;
}

void hex_write_string(const char s[]) {
  unsigned int l = string_len(s);
  char str[l];
  for (int i = 0; i < l; i++) {
    str[i] = hex_to_printable(s[i]);
  }
  //str[l] = '\0';
  write(1, str, l);

}



void hex_format_offset(unsigned offset, char sbuf[]) {
  char* p = sbuf;
  //go through all 36 bits and find correct hex digit representation
  for (int i = 28; i >= 0; i -= 4) {
    *p++ = "0123456789abcdef"[(offset >> i) & 0xF];
  }
  *p = '\0';
}


void hex_format_byte_as_hex(unsigned char byteval, char sbuf[]) {
  char *p = sbuf;
  //looks leftmost four bits are and convert to hex digit  
  *p++ = "0123456789abcdef"[(byteval >> 4) & 0xF];
  //looks at rightmost four bits are by and with 15 b/c 00001111
  *p++ = "0123456789abcdef"[byteval & 0xF];
  *p = '\0';
} 

char hex_to_printable(unsigned char byteval) {
  //outside ascii printable range
  if (byteval <=  31  || byteval >=  126  ) {
    return '.';
  }
  return byteval;
}



