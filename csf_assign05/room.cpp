#include "guard.h"
#include "message.h"
#include "message_queue.h"
#include "user.h"
#include "room.h"

Room::Room(const std::string &room_name)
  : room_name(room_name) {
    pthread_mutex_init(&lock, NULL);


  // TODO: initialize the mutex
}

Room::~Room() {
  pthread_mutex_destroy(&lock);

  // TODO: destroy the mutex
}

void Room::add_member(User *user) {
  Guard g(lock); //idk if this is right/enough to lock 
  members.insert(user); 

  // ... code to add the user object pointer to the collection ...
  // TODO: add User to the room
}

void Room::remove_member(User *user) {
  Guard g(lock); //idk if this is right/enough to lock 
  members.erase(user);
  // TODO: remove User from the room
}

//
void Room::broadcast_message(const std::string &sender_username, const std::string &message_text) {
  Guard g(lock); 
  std::set<User *> :: iterator it;
  Message msg;
  msg.data = message_text;
  msg.tag = TAG_DELIVERY; // i think ? is it always this 
  for (it = members.begin(); it != members.end(); it++) {
    (*it)->mqueue.enqueue(&msg);
    //send message to every receiver
  }
  //iterate through all users in a room and pushes a message into every messageQueue
  // TODO: send a message to every (receiver) User in the room
}


  //iterate through all users in a room and pushes a message into every messageQueue
  // TODO: send a message to every (receiver) User in the room

