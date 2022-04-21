#include <sstream>
#include <cctype>
#include <cassert>
#include "csapp.h"
#include "message.h"
#include "connection.h"

Connection::Connection()
  : m_fd(-1)
  , m_last_result(SUCCESS) {
}

Connection::Connection(int fd)
  : m_fd(fd)
  , m_last_result(SUCCESS) {
    rio_readinitb(&m_fdbuf, fd); 
  // TODO: call rio_readinitb to initialize the rio_t object
}

void Connection::connect(const std::string &hostname, int port) {
  //how to convert into to char aray
  //convert to char array using stringstream
  if (m_fd >= 0) {
    std::string port_str = std::to_string(port);
    m_fd = open_clientfd(hostname.c_str(), port_str.c_str()); 
  rio_readinitb(&m_fdbuf, m_fd); 
  }
 
  // TODO: call open_clientfd to connect to the server
  // TODO: call rio_readinitb to initialize the rio_t object
}

Connection::~Connection() {
  // TODO: close the socket if it is open
  if (is_open) {
    close();
  }
}

bool Connection::is_open() const {
  if (m_fd < 0 ) {
    //how to check bytes 
  }
  //first check if m_fd < 0
  //check if still open by checking number of written bytes vs expected bytes to be written

  
  // TODO: return true if the connection is open

}

void Connection::close() {
  // TODO: close the connection if it is open
    if (is_open) {
    close();
  }
}

bool Connection::send(const Message &msg) {
  if (is_open()) {
    std::stringstream ss; 
    ss << msg.tag << ":" << msg.data; 
    std::string message = ss.str(); 
    return rio_writen(m_fd, message.c_str(), message.length()); 
    //put messagetag:messagedata to a stringstream
    //use rio_write_n to write message
    //use return value of ^^ to check if success
    //if so, return true else false
  }

  // TODO: send a message
  // return true if successful, false if not
  // make sure that m_last_result is set appropriately
}

bool Connection::receive(Message &msg) {
    std::stringstream ss; 
    ss << msg.tag << ":" << msg.data; 
    std::string message = ss.str(); 
    return rio_readn(m_fd, message.c_str(), message.length()); 
    //put messagetag:messagedata to a stringstream
    //use rio_read_n to write message
    //use return value of ^^ to check if success
    //if so, return true else false


  // TODO: send a message, storing its tag and data in msg
  // return true if successful, false if not
  // make sure that m_last_result is set appropriately
}
