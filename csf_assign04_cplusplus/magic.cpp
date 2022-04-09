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
  Elf64_Ehdr *elf_base = (Elf64_Ehdr *) data;
  
  //print header summary
  std::cout << "Object file type: ";
  uint16_t file_type = elf_base->e_type;
  std:: cout << get_type_name(file_type) << "\n";

  std::cout << "Instruction set: ";
  uint16_t machine_name = elf_base->e_machine;
  std::cout << get_machine_name(machine_name) << "\n";

  std::cout << "Endianness: ";
  unsigned char endianness = elf_base->e_ident[EI_DATA];
  if (endianness == 1) {
    std::cout << "Little endian\n";
  } else if (endianness == 2) {
    std::cout << "Big endian\n";
  } else {
    std::cout << "error in endianness\n";
  }


  Elf64_Shdr * section_header = (Elf64_Shdr *) (elf_base + elf_base->e_shoff);
  Elf64_Shdr * sh_string_table = (Elf64_Shdr *) (elf_base + elf_base->e_shstrndx);
  //Elf64_Shdr * sh_string_table = section_header + elf_base->e_shstrndx;
  
  for (uint16_t i = 0; i < elf_base->e_shnum; i++) {
   std::cout << "Section header " << i << ": name=";
   //FIX THIS 
   //std::cout << sh_string_table[i].sh_name;
   //std::cout << (sh_string_table + sh_string_table->sh_offset + sh_string_table->sh_name);
   
   std::cout << ", type=";

   std::cout << ", offset=";

   std::cout << ", size=";

   std::cout << "\n";
   
   
  }
  
  //determine whether file is ELF file or not and print output
  //if ELF file, summarize and exit
  return 0;
}
