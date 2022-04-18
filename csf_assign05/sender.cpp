#include <iostream>
#include <string>
#include <sstream>
#include <stdexcept>
#include "csapp.h"
#include "message.h"
#include "connection.h"
#include "client_util.h"

int main(int argc, char **argv) {
  if (argc != 4) {
    std::cerr << "Usage: ./sender [server_address] [port] [username]\n";
    return 1;
  }

  std::string server_hostname;
  int server_port;
  std::string username;

  server_hostname = argv[1];
  server_port = std::stoi(argv[2]);
  username = argv[3];
  int fd = 2;
  //int file_desc = open_clientfd(server_hostname, server_port); 
  Connection conn;
  conn.connect(server_hostname, server_port);
  if (!conn.is_open()) {
    //ERROR
  }
  conn.send(Message(TAG_SLOGIN, username));
  
  /* - read in the line then check if it is join message or leave  or quit rlogin
join
sendall
leave
ok
err 
*/



  // TODO: connect to server

  // TODO: send slogin message

  // TODO: loop reading commands from user, sending messages to
  //       server as appropriate

  return 0;
}
