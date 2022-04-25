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
  std::cout<< "hello";

  std::string server_hostname = argv[1];
  int server_port = std::stoi(argv[2]);
  std::string username = argv[3];
  std::string room_name = argv[4];

  Connection conn;
  conn.connect(server_hostname, server_port);
  if (!conn.is_open()) {
    std::cerr << "Connection Failed\n";
    return 1;
  }

  //send rlogin msg as first message
  Message send_msg(TAG_RLOGIN, username);
  conn.send(send_msg);
  Message ok_msg;
  bool res = conn.receive(ok_msg);

  if ((!res) || (ok_msg.tag == TAG_ERR)) { 
     std::cout << ok_msg.data << "\n";
     conn.close();
     return(1);
    }


  Message msg;
  conn.send(Message(TAG_JOIN, room_name));
  res = conn.receive(msg);
  
  if ((!res) || (ok_msg.tag == TAG_ERR)) {                                  
      std::cerr << ok_msg.data << "\n";
      conn.close();
      return(1);
    }

  while (1) {
  conn.receive(msg);
  if ((!res) || (ok_msg.tag == TAG_ERR)) {                                  
      std::cerr << ok_msg.data << "\n";
      //conn.close();
      //return(1);
    }

  std::cout << msg.tag;
  if (msg.tag == TAG_DELIVERY) {
      std::cout << msg.tag << ":" << room_name << ":" << username << ":" <<msg.data << "\n";
      //help with this 
    } else if ((!res) || (ok_msg.tag == TAG_ERR)) {
      std::cerr << msg.data << "\n";
      //conn.close();
      //return 1;
    }
  }
   
  conn.close();
  return 0;
}
