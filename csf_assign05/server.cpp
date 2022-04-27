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
  ConnInfo *info_ = static_cast<ConnInfo *>(arg);
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

  std::string username = msg.data;
  if (!info->conn->send(Message(TAG_OK, "welcome " + username))) {
    return nullptr;
  }
  bool slogin = false;
  if (msg.tag == TAG_SLOGIN) {
    slogin = true;
  } 

  // Just loop reading messages and sending an ok response for each one
  while (true) {
    if (!info->conn->receive(msg)) {
      if (info->conn->get_last_result() == Connection::INVALID_MSG) {
        info->conn->send(Message(TAG_ERR, "invalid message"));
      }
      break;
    } else {
      if (slogin) {
        //chat_with_sender(msg, info, );
      } else {
        //chat_with_reciever(msg);
      }
      
    }

    if (!info->conn->send(Message(TAG_OK, "this is just a dummy response"))) {
      break;
    }
  }

  return nullptr;
}

}
void chat_with_sender(Message msg, ConnInfo *info, std::string username) {
  std::string room_name;
  Room *room;
  if (msg.tag == TAG_JOIN) {
    room_name = msg.data; 
    room = info->server->find_or_create_room(room_name);
  } else if (msg.tag == TAG_SENDALL) {
    //Room room;
    room->broadcast_message(username, msg.data);
  } else if (msg.tag == TAG_LEAVE) {
    //

} else if (msg.tag == TAG_QUIT) {

}
}

void chat_with_receiver(Message msg, ConnInfo *info, std::string username) {
  User *user;
  user->username = username;
  std::string room_name;
  Room *room;
  if (msg.tag == TAG_JOIN) {
    room_name = msg.data; 
    room = info->server->find_or_create_room(room_name);
    room->add_member(user);
  } else if (msg.tag == TAG_DELIVERY) {
    //?????
  }
}
  //recieve(join)
//  register receiver to room
  //send(ok)
  //send delivery room sender message 
  //you must terminate the loop and tear down the client thread if any message transmission fails, or if a valid quit message is received.

/*chat_with_sender() {
  //remember: For example, if Alice sends a message, 
  //all receivers logged in as Alice must not receive that message.
  you must terminate the loop and tear down the client thread if any message fails to send, 
  //as there is no other way to detect a client disconnect.
  conn.receive(join)
  
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
