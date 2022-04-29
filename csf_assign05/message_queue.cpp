#include <cassert>
#include <ctime>
#include "message_queue.h"
#include "guard.h"

MessageQueue::MessageQueue() {
  pthread_mutex_init(&m_lock, NULL);
  sem_init(&m_avail, 0, 0);
  
  // TODO: initialize the mutex and the semaphore
}

MessageQueue::~MessageQueue() {
  pthread_mutex_destroy(&m_lock);
  sem_destroy(&m_avail);
  // TODO: destroy the mutex and the semaphore
}

void MessageQueue::enqueue(Message *msg) {
  pthread_mutex_lock(&m_lock);
  m_messages.push_back(msg);
  pthread_mutex_unlock(&m_lock);
  sem_post(&m_avail);
}

Message *MessageQueue::dequeue() {
  Message *msg = nullptr;
  sem_wait(&m_avail);
  Guard g (m_lock);
  

  if(!m_messages.empty()) {
    msg = m_messages.front();
    m_messages.pop_front();
  }
  return msg;
}
