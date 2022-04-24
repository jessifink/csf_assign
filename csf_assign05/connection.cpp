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
  return true; //come back to this

  //first check if m_fd < 0
  //check if still open by checking number of written bytes vs expected bytes to be written

  
  // TODO: return true if the connection is open

}

void Connection::close() {
  // TODO: close the connection if it is open
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
    //void * msg_str = &c_str;
    
<<<<<<< HEAD
    if (rio_writen(m_fd, c_str, sizeof(c_str)) == -1) {
      m_last_result == EOF_OR_ERROR;
=======
    if (rio_writen(m_fd, c_str, message.length()) == -1) {
      m_last_result = EOF_OR_ERROR;
>>>>>>> ed6461183e94f074ca1f5301d779390abfc1c203
      return false;
    }
    m_last_result = SUCCESS;
    return true;

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
<<<<<<< HEAD
    /*std::stringstream ss; 
    ss << msg.tag << ":" << msg.data; 
    std::string message = ss.str(); 
    const char * c_str = message.c_str();
*/

    char str[msg.MAX_LEN];
    //void * msg_str = &c_str;
    int n = Rio_readlineb(&m_fdbuf, str, msg.MAX_LEN);
    //if this fails, error. non pisitive n
    if (n < 0) {
      //error
    }
    //break line by colons
    //store into msg

    std::vector<std::string> result;
    std::string string = str;
    int index = 0; 
    for (int i = 0; i = string.length(); i++) {
      std::string msg;
      if (string[i] == ':') {
        std::string tag = string.substr(index, i);
        //index = i + 1;
        result.push_back(tag);
        std::string payload = string.substr(i);
        result.push_back(payload);
      }
      //if something wrong with msg format - no colon, tag formatted incorrectly
    }

    msg.tag = result.at(0);
    msg.data = result.at(1);


    /*if (Rio_readlineb(&m_fd, str, sizeof(msg_str)) == -1) {
      m_last_result = EOF_OR_ERROR;
      return false;
    }*/
=======
    char str[msg.MAX_LEN];
    //void * msg_str = &c_str;
    int n = Rio_readlineb(&m_fdbuf, str, msg.MAX_LEN);
    //std::cout << str;
    //if this fails, error. non pisitive n
    if (n < 0) {
      //std::cerr << "ERROR";
      return 1;
    }
    std::string string = str;
    std::string result;
    int index = 0; 
    for (int i = 0; i == string.length(); i++) {
      if (string[i] == ':') {
        msg.tag = string.substr(index, i);
        index = i + 1;
      }
    }
      msg.data = string.substr(index,string.length());
      //if something wrong with msg format - no colon, tag formatted incorrectly

>>>>>>> ed6461183e94f074ca1f5301d779390abfc1c203
    m_last_result = SUCCESS;
    return true;
}

