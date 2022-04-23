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
  //int file_desc = open_clientfd(server_hostname, server_port); 
  Connection conn;
  conn.connect(server_hostname, server_port);
  if (!conn.is_open()) {
    std::cerr << "Connection Failed\n";
    return 1;
    //ERROR
  }

  conn.send(Message(TAG_SLOGIN, username));
  Message ok_msg;
  conn.receive(ok_msg);
  if (!conn.receive(ok_msg)) {
    if (conn.get_last_result() == Connection::INVALID_MSG) {
      std::cerr << "Invalid Message";
      return 1;
    } else if (conn.get_last_result() == Connection::EOF_OR_ERROR) {
      std::cerr << "Invalid Message";
      return 1;
    }
}

  while (1) { 
   //getline()
   //if call fails break out of function
   //line = fgets(line, 4000, stdin)
   std::string str;
   getline(std::cin, str);
   std::string payload;
   std::string command;
   std::stringstream ss;
   ss << str;
   ss >> command >> payload;
    if (command.compare("/join") == 0) {
      conn.send(Message(TAG_JOIN, payload));
      conn.receive(ok_msg);
      if (ok_msg.tag == TAG_ERR) {
        std::cerr << "Join Error:" << payload << "\n";
      }
    } else if (command.compare("/leave") == 0) {
      conn.send(Message(TAG_LEAVE, payload));
      conn.receive(ok_msg);
      if (ok_msg.tag == TAG_ERR) {
        std::cerr << "Other Error: " << payload << "\n";
      }
    } else if (command.compare("/quit") == 0) {
      conn.send(Message(TAG_QUIT, payload));
      conn.receive(ok_msg);
      if (ok_msg.tag == TAG_ERR) {
        std::cerr << "Other Error: " << payload << "\n";
      }
      //close connection
      conn.close();
      return 1;
    } else { //(command.compare("/sendall") == 0) {
      conn.send(Message(TAG_SENDALL, command));
      conn.receive(ok_msg);
      if (ok_msg.tag == TAG_ERR) {
        std::cerr << "Other Error: " << payload << "\n";
      }
    }
  }

  conn.close();

  //send rlogin (TAG_RLOGIN)
  //first send username
  //recieve message
  //send another to join room
  //recieve message
  //then start loop
  //in loop send msg
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
