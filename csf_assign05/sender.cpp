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
  Message ok_msg(TAG_OK, username);
  conn.receive(ok_msg);
  std::string word;
  while (std::cin >> word) {
    if (word.compare("/join") == 0) {
      std::string room_name;
      std::cin >> room_name;
      conn.send(Message(TAG_JOIN, room_name));
      ok_msg.data = room_name;
      conn.receive(ok_msg);
    }

    std::string text;
    std::cin >> text;
    conn.send(Message(TAG_SENDALL, text));
    ok_msg.data = text;
    conn.receive(ok_msg);

    if (word.compare("/leave") == 0) {

   
    }


    





  }


  //send rlogin (TAG_RLOGIN)
  //first send username
  //recieve message
  //send another to join room
  //recieve message
  //then start loop
  //in loop send msg

  
  while (std::cin >> word) {
    if (word.compare("/leave") == 0) {
      conn.send(Message(TAG_QUIT, username));
      //if room is null send msg back saying not in room
      //else remove user from room and send msg saying has left room

    } else if (word.compare("/quit")== 0) {
      conn.send(Message(TAG_LEAVE, username));
      // quit the program
    } else if (word.substr(0,5).compare("./join") == 0) {
      conn.send(Message(TAG_JOIN, username));
    } else {
      std::cerr << "ERROR"; 
    }
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
