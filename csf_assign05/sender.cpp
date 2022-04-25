#include <iostream>
#include <string>
#include <sstream>
#include <stdexcept>
#include "csapp.h"
#include "message.h"
#include "connection.h"
#include "client_util.h"
//quit and reciever 
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
  Connection conn;
  conn.connect(server_hostname, server_port);
  if (!conn.is_open()) {
    std::cerr << "Connection Failed\n";
    return 1;
  }

  conn.send(Message(TAG_SLOGIN, username));
  Message ok_msg;
  bool res = conn.receive(ok_msg);
  if (!res) {
    if (ok_msg.tag == TAG_ERR) {
      std::cout << ok_msg.data << "\n";
      conn.close();
      return 1;
    }

  /*
    if (conn.get_last_result() == Connection::INVALID_MSG) {
      std::cerr << "Invalid Message";
      return 1;
    } else if (conn.get_last_result() == Connection::EOF_OR_ERROR) {
      std::cerr << "Invalid Message";
      return 1;
    }*/

  }

  while (1) { 
   //getline()
   std::string str;
   getline(std::cin, str);
   std::string payload;
   std::string command;
   std::stringstream ss;
   ss << str;
   ss >> command; 
   //std::cout << "red";
  if (command.compare("/join") == 0) {
    ss >> payload;
    conn.send(Message(TAG_JOIN, payload));
    conn.receive(ok_msg);
    //std::cout << "orange";
    if (ok_msg.tag == TAG_ERR) {
      std::cerr << payload  << "\n";
    }
  } else if (command.compare("/leave") == 0) {
      conn.send(Message(TAG_LEAVE, ""));
      conn.receive(ok_msg);
      //std::cout << "yellow";
      if (ok_msg.tag == TAG_ERR) {
        std::cerr << payload << "\n";
      }
  } else if (command.compare("/quit") == 0) {
    //std::cout << "magenta";
      conn.send(Message(TAG_QUIT, ""));
      //std::cout << "maroon";
      conn.receive(ok_msg);
    if (ok_msg.tag == TAG_ERR) {
        std::cerr << payload << "\n";
      }
    //std::cout << "blue";
    //conn.close();
      return 0;
  } else { 
      conn.send(Message(TAG_SENDALL, str));
      conn.receive(ok_msg);
      //std::cout << "purple";

      if (ok_msg.tag == TAG_ERR) {
        std::cerr << payload << "\n";
      }
    }
  }
  conn.close();
  return 0;
}
