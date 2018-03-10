#include <iostream>
#include <arpa/inet.h> // For converions functions inet_aton, inet_ntoa, inet_pton, inet_ntop
#include <string.h> // memset , memcpy
#include <errno.h> // for error number
#include "Socket.h" // Socket definitions

using namespace Socket;
using namespace std;

SocketAddress::SocketAddress(){

}

SocketAddress::SocketAddress(const AddressFamilyType & __domain, const char * __s_addrin, const uint16_t & __port){

}

SocketAddress::SocketAddress(const AddressFamilyType & __domain, const uint32_t & __s_addrin, const uint16_t & _port){

}
SocketAddress::SocketAddress(const AddressFamilyType & __domain, const uint64_t & __s_addrin, const uint16_t & _port){

}

SocketAddress::~SocketAddress(){

}
