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
  //  std::cout<< "hello";

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
  conn.receive(ok_msg);

  if (ok_msg.tag == TAG_ERR) { 
     std::cerr << ok_msg.data << "\n";
     //     conn.close();
     exit(1);
    }


  Message msg;
  conn.send(Message(TAG_JOIN, room_name));
  conn.receive(msg);
  
  if (msg.tag == TAG_ERR) {                                  
      std::cerr << msg.data << "\n";
      // conn.close();
      exit(1);
    }

  while (1) {
    Message msg2;
  conn.receive(msg2);
  
  if  (msg2.tag == TAG_ERR) {                                  
    std::cerr << msg2.data << "\n";
    //      conn.close();
      return 1;
    }

  //  std::cout << msg.tag;
  if (msg2.tag == TAG_DELIVERY) {
    
    //    std::cout<< "red:"; //data = cafe:alice:message
    int colon1  = msg2.data.find(":");
    std::string past_room = msg2.data.substr(colon1 + 1);
    int colon2 = past_room.find(":");
    //past_room =  bob:hi alice
    //std::cout << "red" + past_room; // bob:hi
    //    std::string user = past_room.substr(0,colon2 - colon1);
    std::string past_user = past_room.substr(colon2 + 1);
    int user_length = past_room.length() - past_user.length() - 1;
    std::string user = past_room.substr(0, user_length);
    std::cout << user << ": "  << past_user <<"\n";
    //std::cout << "looooopppppppp \n";
    //  } else if ((!res) || msg.tag == TAG_ERR)) {
    // std::cerr << msg.data << "\n";
    // conn.close();
      // return 1;
      // }
  }
  }   
  conn.close();
  return 0;
}
