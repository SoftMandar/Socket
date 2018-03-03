#ifndef __SOCKET__
#define __SOCKET__

#include <stdint.h>
#include <sys/socket.h>
#include <netinet/in.h>

namespace Socket{

  enum AddressFamilyType{
      INET  = AF_INET,
      INET6 = AF_INET6
  };

  enum ProtocolLevel{
      SOCKET = SOL_SOCKET,
      IPPROTO = IPPROTO_IP
  };

  enum ProtocolLevelOpt{
      REUSEADDR = SO_REUSEADDR,
      REUSEPORT = SO_REUSEPORT,
      BROADCAST = SO_BROADCAST,
      RCVBUFF   = SO_SNDBUF,
      SNDBUFF   = SO_RCVBUF
  };
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
  public:
      SocketAddress();
      SocketAddress(const AddressFamilyType & __domain, const char * __s_addrin, const uint16_t & __port);
      SocketAddress(const uint32_t & __s_addrin, const uint16_t & _port);
      SocketAddress& operator=(const SocketAddress& other);
      ~SocketAddress();

      char * getHostAddr();
      inline uint32_t getPort() const;
      inline size_t getSockSize() const;

      struct sockaddr m_sockAddr;
  private:
      inline struct sockaddr_in * getSockAddrIn();
  };

  class BaseSocket{

  public:
      virtual int32_t Bind(const SocketAddress & __s_addr) = 0;
      virtual int32_t Connect(const SocketAddress & __s_toaddr) = 0;
      virtual int32_t Listen(uint16_t backlog = 10) = 0;
      virtual int32_t setSockOpt(const ProtocolLevel & __p_lvl ,
                        const ProtocolLevelOpt & __lvl_opt, const void * __opt_val ) = 0;
      virtual int32_t getSockOpt(const ProtocolLevel & __p_lvl,
                        const ProtocolLevelOpt & __lvl_opt, void * __opt_val) = 0;
      virtual int32_t setBlockingMode(bool __blockMode) = 0;
      virtual inline int32_t getSockFd() const = 0;
      virtual int32_t recv(void * __in_buff, uint32_t & __buff_size) = 0;
      virtual int32_t send(const void * __out_buff, uint32_t & __buff_size) = 0;
      virtual int32_t recv_from(void * __in_buff, uint32_t & __buff_size,
                              SocketAddress & __from_addr) = 0;
      virtual int32_t send_to(const void * __out_buff, uint32_t & __buff_size,
                              const SocketAddress & __to_addr) = 0;
  protected:
    uint32_t sock_fd;
  };
  class Socket: public BaseSocket{

  public:
      Socket();
      Socket(const AddressFamilyType & __domain,
                          const SocketType & __p_type, const SocketProtocolNum & __proto);
      #ifdef TSOCK_STREAM
          int32_t Connect(const SocketAddress & __s_toaddr);
          int32_t Listen(uint16_t backlog = 10);
          int32_t recv(void * __in_buff, uint32_t & __buff_size);
          int32_t send(const void * __out_buff, uint32_t & __buff_size);
      #else
          int32_t recv_from(void * __in_buff, uint32_t & __buff_size,
                                  SocketAddress & __from_addr);
          int32_t send_to(const void * __out_buff, uint32_t & __buff_size,
                                  const SocketAddress & __to_addr);
      #endif
      int32_t Bind(const SocketAddress & __s_addr);
      int32_t Accept(SocketAddress & __cl_addr);
      int32_t setSockOpt(const ProtocolLevel & __p_lvl ,
                        const ProtocolLevelOpt & __lvl_opt, const void * __opt_val );
      int32_t getSockOpt(const ProtocolLevel & __p_lvl,
                        const ProtocolLevelOpt & __lvl_opt, void * __opt_val);
  private:
      uint32_t clt_sock_fd;
  };
  class TCPSocket{};
  class UDPSocket{};
  class ReliableUDPSocket{};
}
#endif // __SOCKET__
