// C implementation of hexdump functions

#include <unistd.h>  // this is the only system header file you may include!
#include "hexfuncs.h"
#include <stdio.h>


unsigned hex_read(char data_buf[]) {
  unsigned length = read(stdin, data_buf, 16);
  // if (length == 0) {
  //   // return error;
  // }
  return length;
}

void hex_write_string(const char s[]) {
    int length = write(1, s, strlen(s));
    // if (length == 0) {
    //   return;
    // }
}

int strlen (const char s[]) {
  int count = 0; 
  char c;
  while (c != '\0') {
    c = s[count];
    count++;
  }
  return count;
}
//strelen fucntion -- feed in pointer increment until hits null pointer 

void hex_format_offset(unsigned offset, char sbuf[]) {
  char* p = sbuf;
  for (int i = 28; i >= 0; i -= 4) {
    *p++ = "0123456789abcdef"[(offset >> i) & 0xF];
  }
  *p = '\0';

}


void hex_format_byte_as_hex(unsigned char byteval, char sbuf[]) {
  char *p = sbuf;
  *p++ = "0123456789abcdef"[(byteval >> 4) & 0xF];
  *p++ = "0123456789abcdef"[byteval & 0xF];
  *p = '\0';
} 

char hex_to_printable(unsigned char byteval) {
  if (byteval <  31  || byteval >  126  ) {
    return '.';
  }
  return byteval;
}

