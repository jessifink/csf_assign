#include <sstream>
#include <iostream>
#include <cctype>
#include <cassert>
#include <stdexcept>
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
  //if (m_fd >= 0) {
    std::string port_str = std::to_string(port);
    m_fd = open_clientfd(hostname.c_str(), port_str.c_str()); 
    rio_readinitb(&m_fdbuf, m_fd); 
  
  //}
 
  // TODO: call open_clientfd to connect to the server
  // TODO: call rio_readinitb to initialize the rio_t object
}

Connection::~Connection() {
  // TODO: close the socket if it is open
  if (is_open()) {
    close();
  }
}

bool Connection::is_open() const {
  if (m_fd < 0 ) {
    return false;
  }
  return true;
}

void Connection::close() {
    if (is_open()) {
    close();
  }
}

bool Connection::send(const Message &msg) {
  if (is_open()) {
    std::stringstream ss; 
    ss << msg.tag << ":" << msg.data << "\n"; 
    std::string message = ss.str(); 
    const char * c_str = message.c_str();
    
    if (checkTagError(msg.tag.c_str())) {
        std::cerr << "Error: Tag not found";
        m_last_result = INVALID_MSG;
        return false;
    }

    if (rio_writen(m_fd, c_str, message.length()) == -1) {
      m_last_result = EOF_OR_ERROR;
      return false;
    }
    m_last_result = SUCCESS;
    return true;
  }
}

bool Connection::receive(Message &msg) {
    char str[msg.MAX_LEN];
    int n = Rio_readlineb(&m_fdbuf, str, msg.MAX_LEN);
    if (n < 0) {
      //std::cerr << "ERROR";
      return false;
    }
    std::string string = str;
    std::string result;


    int index = 0; 
    for (int i = 0; i == string.length(); i++) {
      if (string[i] == ':') {
        msg.tag = string.substr(index, i);
        index = i + 1;

        if (checkTagError(msg.tag.c_str())) {
          std::cerr << "Error: Tag not found";
          m_last_result = INVALID_MSG;
          return false;
        }
      }
      if (index == 0) { //error no colon found
        std::cerr << "Error: invalid message format";
        m_last_result = INVALID_MSG;
        return false;
      }
    }
    msg.data = string.substr(index,string.length());
    m_last_result = SUCCESS;
    return true;
}

bool Connection::checkTagError(const char * toFind) { //returns true if error, false if not
  std::stringstream ts;
  ts << TAG_DELIVERY << TAG_EMPTY << TAG_ERR << TAG_JOIN << TAG_LEAVE << TAG_LEAVE <<
  TAG_OK << TAG_QUIT << TAG_RLOGIN << TAG_SENDALL << TAG_SENDUSER << TAG_SLOGIN;
  std::string tss = ts.str();
  const char * tags = tss.c_str();

  if (!(strstr(tags, toFind))) { 
    return true;
  }
  return false;
}

