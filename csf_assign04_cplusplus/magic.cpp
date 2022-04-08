#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cstdarg>
#include <cerrno>
#include <cstdint>
#include <elf.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/unistd.h>
#include <fcntl.h>
#include <iostream>
#include "elf_names.h"


int main(int argc, char **argv) {

  
  //open the specified file as command-line argument
  if (argc != 2) {
    std::cout << "Error: invalid number of arguments\n";
    return 1;
  }
  
  char *  filename = argv[1];

  //use mmap to map data to memory
  int fd = open(filename, O_RDONLY);
  struct stat statbuf;
  int rc = fstat(fd, &statbuf);
  size_t file_size;
  if (rc != 0) {
    // error
  } else {
    file_size = statbuf.st_size;
    // ...
  }
  void *data = mmap(NULL, file_size, PROT_READ, MAP_PRIVATE, fd, 0);
 
  //determine whether file is ELF file or not and print output
  //if ELF file, summarize and exit
  return 0;
}
