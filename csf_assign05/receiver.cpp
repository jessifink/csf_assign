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
    std::cerr << "Connection Failed\n";
    return 1;//ERROR
  }

  //send rlogin msg as first message
  Message send_msg(TAG_RLOGIN, username);
  conn.send(send_msg);
  Message ok_msg(TAG_OK, username);
  //Message err_msg(TAG_ERR, username);

  if (!conn.receive(ok_msg)) {
  if (conn.get_last_result() == Connection::INVALID_MSG) {
    std::cerr << "Invalid Message";
    return 1;
  } else if (conn.get_last_result() == Connection::EOF_OR_ERROR) {
    std::cerr << "Invalid Message";
    return 1;
  }
}
//when to use tag delivery 
//
  conn.send(Message(TAG_JOIN, room_name));
  Message receive_msg(TAG_DELIVERY, "");
  /*conn.receive(receive_msg);
  if (receive_msg.tag == TAG_OK) {*/
  while (1) { //is this right
  conn.receive(receive_msg);
    if (receive_msg.tag == TAG_DELIVERY) {
      std::cout << receive_msg.tag << ":" << receive_msg.data << "\n";

      //using split_payload
      //delivery:[room]:[sender]:[message]
      //print [username of sender]: [message text]
    } else if (receive_msg.tag == TAG_ERR) {
      std::cerr << "Join Error\n";
    }
  }
   



  //Message msg(TAG_DELIVERY, "");
  //how do you recieve the message/maintain the loop


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
  


  // TODO: connect to server

  // TODO: send rlogin and join messages (expect a response from
  //       the server for each one)

  // TODO: loop waiting for messages from server
  //       (which should be tagged with TAG_DELIVERY)


  return 0;
}
