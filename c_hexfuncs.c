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
long string_len (const char s[]) {
  long count = 0; 
  char c;
  while (c != '\0') {
    c = s[count];
    count++;
  }
  return count;
}

void hex_write_string(const char s[]) {
  char str[string_len(s)];
  int i = 0;
  //check if printable if true adds to string to be writen out 
  while (s[i] != '\0') {
    str[i] = hex_to_printable(s[i]);
    i++;
  }
  str[i] = '\0';
  
  write(1, str, string_len(str));
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
  if (byteval <  31  || byteval >  126  ) {
    return '.';
  }
  return byteval;
}

