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


  //store filename
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
   if (data == (void*)-1) { 
    std::cerr << "ERROR: mapped to inaccessible memory region" << std::endl;
    return 4;
  }
  Elf64_Ehdr *elf_base = (Elf64_Ehdr*) data;

  //check if ELF File
  if (elf_base->e_ident[0] != 127 ||
      elf_base->e_ident[1] != 69 ||
      elf_base->e_ident[2] != 76 ||
      elf_base->e_ident[3] != 70) {  
    std::cout << "Not an ELF file" << std::endl;
    return 0;
  }
  
  //print header summary
  std::cout << "Object file type: " << get_type_name(elf_base->e_type) << std::endl;
  std::cout << "Instruction set: " << get_machine_name(elf_base->e_machine) << std::endl;
  std::cout << "Endianness: ";
  unsigned char endianness = elf_base->e_ident[EI_DATA];
  if (endianness == 1) {
    std::cout << "Little endian\n";
  } else if (endianness == 2) {
    std::cout << "Big endian\n";
  } 



  Elf64_Shdr * section_header = (Elf64_Shdr *) (data + elf_base->e_shoff);
  Elf64_Shdr * sh_string_table = (Elf64_Shdr *) (section_header + elf_base->e_shstrndx);
  unsigned char* str_tab_data;
  Elf64_Sym* sym_table;
  int sym_counter;


  //Print Section summary
  for (uint16_t i = 0; i < elf_base->e_shnum; i++) {
    std::cout << "Section header " << i << ": name=";
    std::cout << (char*) (data + sh_string_table->sh_offset + section_header[i].sh_name);
   
    std::cout << ", type=";
    printf("%lx", section_header[i].sh_type);

    std::cout << ", offset=";
    printf("%lx", section_header[i].sh_offset);

    std::cout << ", size=";
    printf("%lx", section_header[i].sh_size);
    std::cout << "\n";
    std::string name = (char*) (data + sh_string_table->sh_offset + section_header[i].sh_name);
    
    //store strtab and symtab
    std::string strtab = ".strtab";
    std::string symtab = ".symtab";
    if (symtab.compare(name) == 0) {
      sym_counter = section_header[i].sh_size / section_header[i].sh_entsize;
      sym_table = (Elf64_Sym*) ((unsigned char*) data + section_header[i].sh_offset); 
    } else if (strtab.compare(name) == 0) {
      str_tab_data = (unsigned char*) data + section_header[i].sh_offset;
    }
  }

  //print symbol summary
  for (int j = 0; j < sym_counter; j++) {
    std::cout << "Symbol " << j; 
    if (sym_table->st_name != 0) {
      std::cout << ": name=";
      std::cout <<  (unsigned char*) ( sym_table->st_name + str_tab_data);
    } else {
      std::cout << ": name=";
    }
  
    std::cout << ", size=";
    printf("%lx", sym_table->st_size);
    std::cout << ", info=";
    printf("%lx", (long unsigned int) sym_table->st_info);
    std::cout << ", other=";
    printf("%lx", (long unsigned int) sym_table->st_other);
    std::cout << "\n";
    sym_table++;
  }

  //return
  return 0;
}