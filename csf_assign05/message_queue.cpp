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
 //how to lock and unlock using guard 
 //this is copied from the slides 
//  pthread_mutex_lock(&bq->lock);
// bq->data[bq->head] = item;
//  m_messages.emplace_front(msg);
// m_messages.front = m_messages.front+1 % m_messages.max_size; //how to 
 //pthread_mutex_unlock(&m_lock);
  sem_post(&m_avail);

  // TODO: put the specified message on the queue

  // be sure to notify any thread waiting for a message to be
  // available by calling sem_post
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

  if (sem_timedwait(&m_avail, &ts) == -1) {
    return msg;
  } else {
    pthread_mutex_lock(&m_lock);
    //this is copied from the slides 
   void * item = m_messages;
   }
    // void *item = m_messages->data[m_messages->tail];
// m_messages->tail = (m_messages->tail + 1) % m_messages->max_items;
    //pthread_mutex_unlock(&m_lock);

// remove message from queue
//     return msg

  

  // TODO: call sem_timedwait to wait up to 1 second for a message
  //       to be available, return nullptr if no message is available

  // TODO: remove the next message from the queue, return it
  return msg;
}
