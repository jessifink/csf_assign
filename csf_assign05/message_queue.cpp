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
  Guard g (m_lock);
  m_messages.push_back(msg);
  sem_post(&m_avail);
}

Message *MessageQueue::dequeue() {
  struct timespec ts;
  
  // get the current time using clock_gettime:
  // we don't check the return value because the only reason
  // this call would fail is if we specify a clock that doesn't
  // exist
  clock_gettime(CLOCK_REALTIME, &ts);

  // compute a time one second in the future
  ts.tv_sec += 1;
    Message *msg = nullptr;

  if (sem_timedwait(&m_avail, &ts) == 0) {
    Message *return_msg = m_messages.back();
    m_messages.pop_back();
    return return_msg;
  }
  return msg;
}
