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
    std::cerr << "Error: invalid number of arguments\n";
    return 1;
  }


  char *  filename = argv[1];

  //use mmap to map data to memory
  int fd = open(filename, O_RDONLY);
  if (fd < 0) {  // If cannot open file
    std::cerr << "ERROR: Cannot open file" << std::endl;
    return 2;  
  }
  struct stat statbuf;
  int rc = fstat(fd, &statbuf);
  size_t file_size;
  if (rc != 0) {
    std::cerr << "Error: fstat failed\n";
    return 2;
  } else {
    file_size = statbuf.st_size;
  }

  void *data = mmap(NULL, file_size, PROT_READ, MAP_PRIVATE, fd, 0);
  Elf64_Ehdr *elf_base = (Elf64_Ehdr *) data;
    if (elf_base->e_ident[0] != 127 ||
      elf_base->e_ident[1] != 69 ||
      elf_base->e_ident[2] != 76 ||
      elf_base->e_ident[3] != 70) {  
    std::cout << "Not an ELF file" << std::endl;
    return 0;
  }
  
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


  Elf64_Shdr * section_header = (Elf64_Shdr *) (data + elf_base->e_shoff);
  Elf64_Shdr * sh_string_table = (Elf64_Shdr *) (section_header + elf_base->e_shstrndx);
  //Elf64_Shdr * sh_string_table = section_header + elf_base->e_shstrndx;
  
  for (uint16_t i = 0; i < elf_base->e_shnum; i++) {
   std::cout << "Section header " << i << ": name=";
   std::cout << (char*) (data + sh_string_table->sh_offset + section_header[i].sh_name);
   
   std::cout << ", type=";
   std::cout << section_header[i].sh_type;

   std::cout << ", offset=";
   printf("%lx", section_header[i].sh_offset);

   std::cout << ", size=";
   printf("%lx", section_header[i].sh_size);
   std::cout << "\n";
   std::string name = (char*) (data + sh_string_table->sh_offset + section_header[i].sh_name);
   std::string symtab = ".symtab";
   if (symtab.compare(name) == 0) {
    int sym_counter = section_header->sh_size / section_header->sh_entsize;
    Elf64_Sym* sym_table = (Elf64_Sym*) ((unsigned char*) data + section_header->sh_offset); 
    for (int j = 0; j < sym_counter; j++) {
      std::cout << "Symbol " << i; 
      if (sym_table->st_name != 0) {
        std::cout << ": name=";
        std::cout <<  (char*) (data + sym_table->st_name + section_header[i].sh_offset);
      } else {
         std::cout << ": name= ";
      }
      std::cout << ", size=";
      printf("%lx", sym_table->st_size);
      std::cout << ", info=";
      printf("%lx", sym_table->st_info);
      std::cout << ", other=";
      printf("%lx", sym_table->st_other);
    }
  }
  }

  
  //determine whether file is ELF file or not and print output
  //if ELF file, summarize and exit
  return 0;
}
