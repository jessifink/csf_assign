#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>
#include "csapp.h"
#include "message.h"
#include "connection.h"
#include "client_util.h"

int main(int argc, char **argv) {
  if (argc != 5) {
    std::cerr << "Usage: ./receiver [server_address] [port] [username] [room]\n";
    return 1;
  }

  std::string server_hostname = argv[1];
  int server_port = std::stoi(argv[2]);
  std::string username = argv[3];
  std::string room_name = argv[4];

  Connection conn;
  conn.connect(server_hostname, server_port);
  if (!conn.is_open()) {
    //ERROR
  }
  Message msg;
  conn.receive(msg);
  std::string word; 
  while (std::cin >> word) {
    if (word.compare("/leave") == 0) {
      conn.send(Message(TAG_QUIT, username));
    } else if (word.compare("/quit")== 0) {
      conn.send(Message(TAG_LEAVE, username));
// quit the program
    } else if (word.substr(0,5).compare("./join") == 0) {
      conn.send(Message(TAG_JOIN, username));
    } else {
      std::cerr << "ERROR"; 
    }

  }

  // TODO: connect to server

  // TODO: send rlogin and join messages (expect a response from
  //       the server for each one)

  // TODO: loop waiting for messages from server
  //       (which should be tagged with TAG_DELIVERY)


  return 0;
}
