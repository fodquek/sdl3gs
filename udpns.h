// https://learn.microsoft.com/en-us/windows/win32/winsock/creating-a-basic-winsock-application
// https://beej.us/guide/bgnet/
#ifndef H_UDPNS
#define H_UDPNS

#define UDPNS_WINDOWS

#ifdef UDPNS_WINDOWS
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <winsock2.h>
#include <ws2tcpip.h>
#else
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#endif

#include <cstdlib>
#include <string_view>

namespace UDPNS
{
#ifdef UDPNS_WINDOWS
using sock_fd_t = SOCKET;
using ai_addrlen_t = int;
using msg_len_t = int;
using len_t = size_t;
#else
using sock_fd_t = int;
using ai_addrlen_t = socklen_t;
using msg_len_t = size_t;
using len_t = ssize_t;
#endif

const int GAI_OK{0};
#ifdef UDPNS_WINDOWS
const sock_fd_t BAD_SOCKET{INVALID_SOCKET};
const int BAD_BIND{SOCKET_ERROR};
#else
const sock_fd_t BAD_SOCKET{-1};
const int BAD_BIND{-1};
#endif
const int SENDTO_FLAG{0};
const int RECVFORM_FLAG{0};
const std::string_view MY_PORT{"4951"};
const len_t BUF_LEN{100};
const int ADDR_LEN{18};

void* get_in_addr(struct sockaddr* sa);
#ifdef UDPNS_WINDOWS
bool initWSA();
int stopWSA();
#endif

struct RX_Buf_Slice
{
    char* buf;
    len_t rx_bytes;
};

class UDP
{
public:
    explicit UDP() = default;

    // no copy-ctor
    UDP(const UDP& arg) = delete;
    // no copy-assignment
    UDP& operator=(const UDP& rhs) = delete;
    // get sockaddr, IPv4 or IPv6:
    ~UDP();

    [[nodiscard("MALLOC VARKE")]] bool makeBuffers();

    bool createSocket(std::string_view host, std::string_view port, bool RX);
    bool createRX(std::string_view host, std::string_view port);
    bool createTX(std::string_view host, std::string_view port);
    void freeRXResults();
    void freeTXResults();

    bool rxAllocated();
    bool txAllocated();

    int peek();
    void read();
    bool clearRXBuf();
    len_t transmit(std::string_view msg);
    RX_Buf_Slice getRXBufSlice();
    void clearRX();
    void clearTX();
    void clearAll();

private:
    sock_fd_t tx{BAD_SOCKET};
    sock_fd_t rx{BAD_SOCKET};
    struct addrinfo* rx_results{};
    struct addrinfo* tx_results{};
    struct addrinfo* rx_target{};
    struct addrinfo* tx_target{};
    char* buf{nullptr};
    len_t rx_bytes{0};
};
} // namespace UDPNS

#endif
