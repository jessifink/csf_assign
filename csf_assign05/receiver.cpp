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
  std::cout << "line 23";
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
  } else if (conn.get_last_result() == Connection::EOF_OR_ERROR) {
    std::cerr << "Invalid Message";
  }
}

  conn.send(Message(TAG_JOIN, room_name));
  Message receive_msg(TAG_EMPTY, "");
  conn.receive(receive_msg);
  if (receive_msg.tag == TAG_OK) {
    while (conn.get_last_result() != conn.EOF_OR_ERROR) { //is this right
    Message msg (TAG_EMPTY, "");
    conn.receive(msg);

    std::vector<std::string> msg_vec = msg.split_payload();
    std::cout << msg_vec.at(0) << ":" << msg_vec.at(1) << "\n";

    //using split_payload
    //delivery:[room]:[sender]:[message]
    //print [username of sender]: [message text]
    }
  } else if (receive_msg.tag == TAG_ERR) {
    std::cerr << "Join Error\n";
  }



  Message msg(TAG_DELIVERY, "");
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