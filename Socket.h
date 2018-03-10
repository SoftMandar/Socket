#ifndef __SOCKET__
#define __SOCKET__

#include <stdint.h>
#include <sys/socket.h>
#include <netinet/in.h>

/**
    Socket.h
    Purpose:  Defines the base types for manipulating socket types
    and socket addresses
    @author Istoc Rares
    @version  1.0.0 10/3/18
*/

namespace Socket{

  //Socket address family type IPv4 and IPv6
  enum AddressFamilyType{
      INET  = AF_INET,
      INET6 = AF_INET6
  };
  //Protocol level which will e places as a parameter for set and get sockoption
  enum ProtocolLevel{
      SOCKET = SOL_SOCKET,
      IPPROTO = IPPROTO_IP
  };
  //Protocol level option will be set according to the specified protocol level
  enum ProtocolLevelOpt{
      REUSEADDR = SO_REUSEADDR,
      REUSEPORT = SO_REUSEPORT,
      BROADCAST = SO_BROADCAST,
      RCVBUFF   = SO_SNDBUF,
      SNDBUFF   = SO_RCVBUF
  };
  //Socket type according to the following protocols: TCP,UDP and RAW
  enum SocketType{
      STREAM = SOCK_STREAM,
      DGRAM  = SOCK_DGRAM,
      RAW    = SOCK_RAW
  };

  enum SocketProtocolNum{

      IPROTOIP  = IPPROTO_IP,
      IPROTOTCP = IPPROTO_TCP,
      IPROTOUDP = IPPROTO_UDP,
      IPROTORAW = IPPROTO_RAW
  };

  class SocketAddress{
  /**
    Abstraction of: sockaddr ,sockaddr_in(IPv4) and sockaddr_in6(IPv6)
    base structures which will handle the associate types for manipulating Internet
    protocol addresses ipv4 => *.*.*.*:port_num
                       ipv6 => *:*:*:*:*:*:port_num

    SocketAddress():
        Default constructor
        @param none
        @return none
    The next three overloaded constructors will provide different
    initializaion modes corresponding to the apropiate IP address.

    ~SocketAddress():
        Deconstructor
  */
  public:
      SocketAddress();
      SocketAddress(const AddressFamilyType & __domain, const char * __s_addrin, const uint16_t & __port);
      SocketAddress(const AddressFamilyType & __domain, const uint32_t & __s_addrin, const uint16_t & _port);
      SocketAddress(const AddressFamilyType & __domain, const uint64_t & __s_addrin, const uint16_t & _port);
      SocketAddress& operator=(const SocketAddress& other);
      ~SocketAddress();
      /**
          Get host address in string form for a better readabilty
          @param none
          @return (char *) String form of IP address
      */
      char * getHostAddr();
      /**
          Get the port number in it's short form for better readability
          @param none
          @return (uint16_t) Port number
      */
      inline uint16_t getPort() const;

      /**
          Get the size of the socket address for later functionalities
          @param none
          @return (size_t) Size of socket address
      */
      inline size_t getSockSize() const;

      /*
        Base socket address structure  which is a general definition of
        of ip addresses.
      */

      struct sockaddr m_sockAddr;
  private:
      /*
          Converts base sockaddr structure into an specific ipv4 structure
          definition for manipulating those type of addresses;
      */
      inline struct sockaddr_in * getSockAddrIn();
  };

  class BaseSocket{
    /**
        Interface which define a number of standard pure absract socket
        functions which expose socket manipulation actions for both TCP and UDPSocket
        transport protocols , those functions will be inherited by specific
        child socket classes;
    */
  public:
      /**
          Binds the ip address and the port (which are base components of SocketAddress
          object) to indicate the operating system to redirect each network packet
          that indicate to the specified target address.

          @param  SocketAddress object (IPv4 or IPv6).
          @return Integer which indicates if the bind system call was succesfull
                  or throws an socket exception insted.
      */
      virtual int32_t Bind(const SocketAddress & __s_addr) = 0;

      /**
          Initialize connection to the specified ip address , this will be only used for
          TCP type sockets.

          @param  Desired ip address to initilize connection.
          @return Integer which indicates that the connect system call was succesfull
                  or throws an socket exception insted.
      */
      virtual int32_t Connect(const SocketAddress & __s_toaddr) = 0;

      /**
          Marks the socket as a passive socket that will be use allong Accept  function
          to accept incoming connections.

          @param  Length of a queue which specifies the maximum number of incoming connections
          @return Positive integer if the system call was succesfull or throws and socket exception
                  usualy indicating that the queue reached the maximum length.
      */
      virtual int32_t Listen(uint16_t backlog = 10) = 0;

      /**
          Used to chance the behaviour of the socket at a specified layer

          @param  Protocol layer level , Prococol level option and option value
          @return Positive integer if the function call was succesfull or throws a socket
                  exception instead
      */
      virtual int32_t setSockOpt(const ProtocolLevel & __p_lvl ,
                        const ProtocolLevelOpt & __lvl_opt, const void * __opt_val ) = 0;

      /**
          Used to get the option of an desired layer level option

          @param  Protocol layer level , Prococol level option and option value
          @return Positive integer if the function call was succesfull or throws a socket
                  exception instead
      */
      virtual int32_t getSockOpt(const ProtocolLevel & __p_lvl,
                        const ProtocolLevelOpt & __lvl_opt, void * __opt_val) = 0;

      /**
          Used to set the socket as blocking or nonblocking.

          @param  Boolean value which will indicate the above behavior
          @return Positive integer if the function call was succesfull

      */
      virtual int32_t setBlockingMode(bool __blockMode) = 0;

      /**
          Waits for incoming packets to arrive from the network , will be implemented in different way
          corresponding to the desired transport protocol (TCP or UDP).

          @param  Byte buffer which contain the desired data to be unpacked , the buffer size and the
                  SocketAddress (if the socket will be defined as UDP or NULL for TCP).
          @return Number of bytes that was recieved after the end of the system call or throws
                  I/O exception instead.
      */
      virtual int32_t recv(void * __in_buff, uint32_t & __buff_size, SocketAddress * __from_addr) = 0;
      /**
          Sends network packet along the network , will be implemented in different way
          corresponding to the desired transport protocol (TCP or UDP).

          @param  Byte buffer to which contain the desired data to be sent , buffer size and the
                  SocketAddress (if the socket will be defined as UDP or NULL for TCP).
          @return Number of bytes that was sent after the end of the system call or throws
                  I/O exception instead.
      */
      virtual int32_t send(const void * __out_buff, uint32_t & __buff_size, SocketAddress * __to_addr) = 0;

      /**
          @param none
          @return The socket file descriptor
      */
      virtual inline int32_t getSockFd() const = 0;
  protected:

    // Socket file descriptor
    uint32_t sock_fd;
  };
  class Socket: public BaseSocket{

  public:
      Socket();
      Socket(const AddressFamilyType & __domain,
                          const SocketType & __p_type, const SocketProtocolNum & __proto_num);

      int32_t Connect(const SocketAddress & __s_toaddr);
      int32_t Listen(uint16_t backlog = 10);
      int32_t recv(void * __in_buff, uint32_t & __buff_size,  SocketAddress * __from_addr);
      int32_t send(const void * __out_buff, uint32_t & __buff_size,  SocketAddress * __to_addr);
      int32_t Bind(const SocketAddress & __s_addr);
      /**
          Accepts the connection of an client session, (used only with
          TCP socket ).

          @param  SocketAddress object which will contain the client ip address.
          @return Client file descriptor after the system call was succesful , used to comunicate
                  with the client.
      */
      int32_t Accept(SocketAddress & __cl_addr);
      int32_t setBlockingMode(bool __blockMode);
      int32_t setSockOpt(const ProtocolLevel & __p_lvl,
                        const ProtocolLevelOpt & __lvl_opt, const void * __opt_val );
      int32_t getSockOpt(const ProtocolLevel & __p_lvl,
                        const ProtocolLevelOpt & __lvl_opt, void * __opt_val);
      inline int32_t getSockFd() const ;
  private:
      //client file descriptor
      uint32_t clt_sock_fd;
  };
  class TCPSocket: public BaseSocket{
  public:
      TCPSocket();
      TCPSocket(const AddressFamilyType & __domain);
      int32_t Bind(const SocketAddress & __s_addr);
      int32_t Listen(uint16_t backlog = 10);
      int32_t recv(void * __in_buff, uint32_t & __buff_size,  SocketAddress * __from_addr);
      int32_t send(const void * __out_buff, uint32_t & __buff_size,  SocketAddress * __to_addr);
      int32_t setBlockingMode(bool __blockMode);
      int32_t setSockOpt(const ProtocolLevel & __p_lvl,
                        const ProtocolLevelOpt & __lvl_opt, const void * __opt_val );
      int32_t getSockOpt(const ProtocolLevel & __p_lvl,
                        const ProtocolLevelOpt & __lvl_opt, void * __opt_val);
      int32_t Connect(const SocketAddress & __s_toaddr);
      inline int32_t getSockFd() const;
      /**
          Accepts the connection of an client session.

          @param  SocketAddress object which will contain the client ip address.
          @return TCPSocket object which will contain the client socket file descrptor and the client ip address
                  after the system call was succesful , used to comunicate with the client.
      */
      TCPSocket * Accept(SocketAddress & __in_sockAddr);
  private:
      TCPSocket(const uint32_t&, const SocketAddress& clientAddr);
  };
  class UDPSocket: public BaseSocket{

  public:
    UDPSocket();
    UDPSocket(const AddressFamilyType & __domain);
    int32_t Bind(const SocketAddress & __s_addr);
    int32_t recv(void * __in_buff, uint32_t & __buff_size,  SocketAddress * __from_addr);
    int32_t send(const void * __out_buff, uint32_t & __buff_size,  SocketAddress * __to_addr);
    int32_t setSockOpt(const ProtocolLevel & __p_lvl,
                      const ProtocolLevelOpt & __lvl_opt, const void * __opt_val );
    int32_t getSockOpt(const ProtocolLevel & __p_lvl,
                      const ProtocolLevelOpt & __lvl_opt, void * __opt_val);

  };
  //class ReliableUDPSocket{};
}
#endif // __SOCKET__
