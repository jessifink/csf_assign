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
  ConnInfo(Connection *conn, Server *server) : conn(conn), server(server) { }
  ~ConnInfo() {
    // destroy connection when ConnInfo object is destroyed
    delete conn;
  }
};


namespace {

void *worker(void *arg) {
  pthread_detach(pthread_self());
  ConnInfo * info_ = static_cast<ConnInfo *>(arg);
    // use a std::unique_ptr to automatically destroy the ConnInfo object
  // when the worker function finishes; this will automatically ensure
  // that the Connection object is destroyed
  std::unique_ptr<ConnInfo> info(info_);
  Message msg;
  if (!info->conn->receive(msg)) {
    if (info->conn->get_last_result() == Connection::INVALID_MSG) {
      info->conn->send(Message(TAG_ERR, "invalid message"));
    }
    return nullptr;
  }
  if (msg.tag != TAG_SLOGIN && msg.tag != TAG_RLOGIN) {
    info->conn->send(Message(TAG_ERR, "first message should be slogin or rlogin"));
    return nullptr;
  }

std::cerr << "bluebery :           " + msg.tag;  
  std::string username = msg.data;
std::cerr << "pear :           " + username;  
if (!info->conn->send(Message(TAG_OK, "welcome " + username))) {
    std::cerr << "apple";
    return nullptr;
  }
  bool slogin = false;
  if (msg.tag == TAG_SLOGIN) {
    std::cerr << "apple :           " + msg.data;
    slogin = true;
  } 

  // Just loop reading messages and sending an ok response for each one
   std::string room_name = "";
  // Room r(room_name);
  // Room * room = &r;
  while (true) {
    if (!info->conn->receive(msg)) {
      if (info->conn->get_last_result() == Connection::INVALID_MSG) {
        info->conn->send(Message(TAG_ERR, "invalid message"));
      }
      break;
    } else {
      if (slogin) {
        room_name = info->server->chat_with_sender(info->conn, info->server, msg, username);
        if (room_name.compare("quit") == 0) {
          //SENDER QUITS PROGRAM SOMEHOW IDK IF RIGHT
          break;
        }
      } else {
        info->server->chat_with_receiver(info->conn, info->server, msg, username);
      }
      
    }
    }
  return nullptr;
}
}



////////////////////////////////////////////////////////////////////////
// Server member function implementation
////////////////////////////////////////////////////////////////////////
std::string Server::chat_with_sender(Connection *conn, Server *server, Message msg, std::string username) {
  std::string room_name = "";
  Room r(room_name);
  Room * room = &r;
 while(true) {
  conn->receive(msg); 
  std::cerr << "orange :           " + msg.tag;
  if (msg.tag == TAG_JOIN) { 

    room = server->find_or_create_room(msg.data);
          // room->add_member(username);
          // conn->send(TAG_OK, "ok");
    } else if (msg.tag == TAG_SENDALL) {
      std::cerr << "banana :           " + msg.data;
      if (room->get_room_name().compare(nullptr)) {
        std::cerr << "Error: Sender not in room"; //DONT KNOW IF THIS ERROR SHOULD BE HANDLED IN SERVER?
      } else {
      room->broadcast_message(username, msg.data);
      conn->send(Message(TAG_OK, "ok"));
      }
    } else if (msg.tag == TAG_LEAVE) {
      room_name = "";
    } else if (msg.tag == TAG_QUIT) {
      room_name = "quit";
    } else {
      conn->send(Message(TAG_ERR, "invalid tag"));
    }
  }
  return room_name;
}

void Server::chat_with_receiver(Connection *conn, Server *server, Message msg, std::string username) { //shoudl this be in chat_with_receiver
 User *user = new User (msg.data);

  std::string room_name = "";
  Room *room;

  if (msg.tag == TAG_JOIN) {
    room_name = msg.data; 
    std::cerr << "kiwi :           " + msg.data;
    room = server->find_or_create_room(room_name);
    room->add_member(user);
  }
  while (true) {
    Message *msg = user->mqueue.dequeue(); 
    if (msg != NULL) {
      std::cerr << "plum";
      conn->send(Message(TAG_DELIVERY, (*msg).data));
      delete &msg;
    } else {
      break;
    }
  }
  room->remove_member(user);
}

Server::Server(int port)
  : m_port(port)
  , m_ssock(-1) {
  // TODO: initialize mutex
  pthread_mutex_init(&m_lock, nullptr);
}

Server::~Server() {
  pthread_mutex_destroy(&m_lock);
  // TODO: destroy mutex
}

bool Server::listen() {
   std::string port = std::to_string(m_port);
  m_ssock = open_listenfd(port.c_str());
  return m_ssock >= 0;
}

void Server::handle_client_requests() {
    assert(m_ssock >= 0);

  while (true) {
    int clientfd = accept(m_ssock, nullptr, nullptr);
    if (clientfd < 0) {
      std::cerr << "Error accepting connection\n";
      return;
    }
    ConnInfo *info = new ConnInfo(new Connection(clientfd), this);
    pthread_t thr_id;
    if (pthread_create(&thr_id, nullptr, worker, static_cast<void *>(info)) != 0) {
      std::cerr << "Could not create thread\n";
      return;
    }
  }
}

Room *Server::find_or_create_room(const std::string &room_name) {
  Guard g(m_lock);
  Room *room;
  auto i = m_rooms.find(room_name);
  if (i == m_rooms.end()) {
    // room does not exist yet, so create it and add it to the map
    room = new Room(room_name);
    m_rooms[room_name] = room;
  } else {
    room = i->second;
  }

  return room;
  }
