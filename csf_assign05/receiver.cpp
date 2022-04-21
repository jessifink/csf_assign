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

  //send rlogin msg as first message
  Message send_msg(TAG_RLOGIN, username);
  conn.send(send_msg);
  Message ok_msg(TAG_OK, username);
  Message err_msg(TAG_ERR, username);
  if (conn.recieve(err_msg)) {
      stderr << "Login Error\n";
  }
  if (conn.get_last_result() == true) {
    std::cerr << "Login Error\n";
    conn.send(Message(TAG_QUIT, "quit"));
  }
  conn.send(Message(TAG_JOIN, room_name));
  Message ok_msg2(TAG_OK, room_name);
  conn.receive(ok_msg2);


  Message msg(TAG_DELIVERY, "");
  //how do you recieve the message/maintain the loop

  while (conn.get_last_result != EOF_OR_ERROR) { //is this right
    conn.receive(msg);
    std::vector message = msg.split_payload();
    std::cout << message.at(1) << ":" << message.at(2) << "\n";

    //using split_payload
    //delivery:[room]:[sender]:[message]
    //print [username of sender]: [message text]

  }
  conn.close();





  //send rlogin (TAG_RLOGIN)
  //first send username
  //recieve message
  //send another to join room
  //recieve message
  //send roomname
  //recieve msg

  //loop is recieve and dealing w message
  //recieve(connection, message);

  //then start loop
  //in loop send msg
  

/*
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
  */

  // TODO: connect to server

  // TODO: send rlogin and join messages (expect a response from
  //       the server for each one)

  // TODO: loop waiting for messages from server
  //       (which should be tagged with TAG_DELIVERY)


  return 0;
}