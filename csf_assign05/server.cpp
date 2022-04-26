#include <pthread.h>
#include <iostream>
#include <sstream>
#include <memory>
#include <set>
#include <vector>
#include <cctype>
#include <cassert>
#include "message.h"
#include "connection.h"
#include "user.h"
#include "room.h"
#include "guard.h"
#include "server.h"

////////////////////////////////////////////////////////////////////////
// Server implementation data types
////////////////////////////////////////////////////////////////////////

// TODO: add any additional data types that might be helpful
//       for implementing the Server member functions

////////////////////////////////////////////////////////////////////////
// Client thread functions
////////////////////////////////////////////////////////////////////////
struct ConnInfo {
  Connection *conn;
  Server *server;
};

namespace {

void *worker(void *arg) {
  pthread_detach(pthread_self());

//HELP!!! 


  // TODO: use a static cast to convert arg from a void* to
  //       whatever pointer type describes the object(s) ????? needed
  //       to communicate with a client (sender or receiver)
  Message msg;
  ConnInfo coninfo; 
  // conninfo.conn.receive(msg);
  // if (msg.tag == TAG_SLOGIN) {
//conninfo.conn.send(OK_TAG);
//register sender thread w username
   //chat_with_sender();

  // } 
  // if (msg.tag == TAG_RLOGIN)  {
    //register sender thread w username
  //   conninfo.conn.send(OK_TAG);
  // chat_with_receiver();
  // }
  // TODO: read login message (should be tagged either with
  //       TAG_SLOGIN or TAG_RLOGIN), send response

  // TODO: depending on whether the client logged in as a sender or
  //       receiver, communicate with the client (implementing
  //       separate helper functions for each of these possibilities
  //       is a good idea)

  return nullptr;
}

}
/*chat_with_sender() {
  //remember: For example, if Alice sends a message, 
  //all receivers logged in as Alice must not receive that message.
  you must terminate the loop and tear down the client thread if any message fails to send, 
  //as there is no other way to detect a client disconnect.

  //recieve join
  register sender to room
  //send ok
  //recive sendall
  synch and broadcast message
  //send ok
  recieve leave
  deregister from room 
  send ok
  recieve quit 
  destory connection data 
  send ok

}
*/
/*chat_with_reciever() {
  //recieve(join)
  register receiver to room
  //send(ok)
  //send delivery room sender message 
  //you must terminate the loop and tear down the client thread if any message transmission fails, or if a valid quit message is received.

}
*/
////////////////////////////////////////////////////////////////////////
// Server member function implementation
////////////////////////////////////////////////////////////////////////

Server::Server(int port)
  : m_port(port)
  , m_ssock(-1) {
  // TODO: initialize mutex
  pthread_mutex_init(&m_lock, NULL);
}

Server::~Server() {
  pthread_mutex_destroy(&m_lock);
  // TODO: destroy mutex
}

bool Server::listen() {
  std::string port_str = std::to_string(m_port);
  int s_fd = open_listenfd(port_str.c_str());
	if (s_fd < 0) { 
    std::cerr << "error opening server";
		return false;
	}
  return true;
  // TODO: use open_listenfd to create the server socket, return true
  //       if successful, false if not
}

void Server::handle_client_requests() {
  // TODO: infinite loop calling accept or Accept, starting a new
  //       pthread for each connected client
while (1) {
  int client_fd = accept(m_ssock, NULL, NULL);
if (client_fd < 0) { /* error accepting a connection from a client */ }
ConnInfo *info = new ConnInfo;
info->conn = new Connection(client_fd);
info->server = this;
pthread_t thr;
if (pthread_create(&thr, NULL, worker, static_cast<void*>(info)) != 0) {
  std::cerr << "error in pthread create";
}
}
}

Room *Server::find_or_create_room(const std::string &room_name) {
  if (m_rooms.find(room_name) != m_rooms.end()) {
      return m_rooms.at(room_name);
  } else {
    Room r (room_name);
    Room * room = &r;
    //m_rooms.insert(room_name, room); //why does this not work?? 
  }
  // TODO: return a pointer to the unique Room object representing
  //       the named chat room, creating a new one if necessary

  
}
