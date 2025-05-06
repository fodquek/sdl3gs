#include "udpns.h"
#include <iostream>

namespace UDPNS
{

void* get_in_addr(struct sockaddr* sa)
{
    if (sa->sa_family == AF_INET) {
        return &((reinterpret_cast<struct sockaddr_in*>(sa))->sin_addr);
    }
    return &((reinterpret_cast<struct sockaddr_in6*>(sa))->sin6_addr);
}

#ifdef UDPNS_WINDOWS
bool initWSA()
{
    WSADATA wsaData;
    if (int rc{WSAStartup(MAKEWORD(2, 2), &wsaData)}; rc != 0) {
        std::cerr << "WSAStartup failed " << rc << '\n';
        return false;
    }
    // std::cout << "VERSION: " << static_cast<int>(LOBYTE(wsaData.wVersion)) << " | " <<
    // static_cast<int>(HIBYTE(wsaData.wVersion)) << '\n';
    if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2) {
        std::cerr << "Version 2.2 of Winsock not available.\n";
        WSACleanup();
        return false;
    }
    return true;
}
int stopWSA()
{
    return WSACleanup();
}
#endif

bool UDP::makeBuffers()
{
    buf = new char[BUF_LEN];
    if (buf == nullptr) {
        return false;
    }
    return true;
}

void UDP::freeRXResults()
{
    if (rx_results) {
        freeaddrinfo(rx_results);
        rx_results = nullptr;
    }
}

void UDP::freeTXResults()
{
    if (tx_results) {
        freeaddrinfo(tx_results);
        tx_results = nullptr;
    }
}

bool UDP::createSocket(std::string_view host, std::string_view port, bool RX)
{
    char* name{nullptr};
    char* serv{nullptr};
    struct addrinfo req{};
    req.ai_family = AF_INET;      // IPv4
    req.ai_socktype = SOCK_DGRAM; // datagram (UDP)

    if (host.empty()) {
        req.ai_flags = AI_PASSIVE; // use my IP
    } else {
        name = const_cast<char*>(host.data());
    }

    if (port.empty()) {
        serv = const_cast<char*>(MY_PORT.data());
    } else {
        serv = const_cast<char*>(port.data());
    }
    auto sockfd{BAD_SOCKET};
    if (RX) {
        if (rx != BAD_SOCKET) {
            std::cerr << "RX SOCKET ALIVE YET CREATING NEW ONE?!";
            return false;
        }
        freeRXResults(); // check and free <addrinfo* results>
        if (auto rv{getaddrinfo(name, serv, &req, &rx_results)}; rv != GAI_OK) {
            std::cerr << "getaddrinfo: " << gai_strerror(rv) << '\n';
            return false;
        }
        for (rx_target = rx_results; rx_target; rx_target = rx_target->ai_next) {
            if (sockfd =
                    socket(rx_target->ai_family, rx_target->ai_socktype, rx_target->ai_protocol);
                sockfd == BAD_SOCKET) {
                std::cerr << "socket miss\n";
                continue;
            }
            if (bind(sockfd, rx_target->ai_addr,
                     static_cast<ai_addrlen_t>(rx_target->ai_addrlen)) == BAD_BIND) {
#ifdef UDPNS_WINDOWS
                closesocket(sockfd);
#else
                close(sockfd);
#endif
                std::cerr << "bind miss\n";
                continue;
            }

            break; // this one is good so leave without setting to next addrinfo...
        }
        if (rx_target == nullptr) {
            std::cerr << "failed to get RX socket/bind\n";
            return false;
        }
        rx = sockfd;
    } else {
        if (tx != BAD_SOCKET) {
            std::cerr << "TX SOCKET ALIVE YET CREATING NEW ONE?!";
            return false;
        }
        freeTXResults(); // check and free <addrinfo* results>
        if (auto rv{getaddrinfo(name, serv, &req, &tx_results)}; rv != GAI_OK) {
            std::cerr << "getaddrinfo: " << gai_strerror(rv) << '\n';
            return false;
        }
        for (tx_target = tx_results; tx_target; tx_target = tx_target->ai_next) {
            if (sockfd =
                    socket(tx_target->ai_family, tx_target->ai_socktype, tx_target->ai_protocol);
                sockfd == BAD_SOCKET) {
                std::cerr << "socket miss\n";
                continue;
            }
            break; // this one is good so leave without setting to next addrinfo...
        }
        if (tx_target == nullptr) {
            std::cerr << "failed to get TX socket/bind\n";
            return false;
        }
        tx = sockfd;
    }
    return true; // all good
}

bool UDP::createRX(std::string_view host, std::string_view port)
{
    return createSocket(host, port, true);
}

bool UDP::createTX(std::string_view host, std::string_view port)
{
    return createSocket(host, port, false);
}

bool UDP::txAllocated()
{
    return !(tx == BAD_SOCKET);
}

bool UDP::rxAllocated()
{
    return !(rx == BAD_SOCKET);
}

int UDP::peek()
{
    const auto peek_bytes = recv(rx, (buf + rx_bytes), (BUF_LEN - 1 - rx_bytes), MSG_PEEK);
    // if (static_cast<RX_Recv_Res>(peek_bytes) == RX_Recv_Res::Empty) {
    //     read(); // flush buffer cuz its empty AND IT AINT PILE UP LATER ON...
    // }
    return peek_bytes;
}

void UDP::read()
{
    rx_bytes += recv(rx, (buf + rx_bytes), (BUF_LEN - 1 - rx_bytes), 0);
    buf[rx_bytes] = '\0';
}

bool UDP::clearRXBuf()
{
    if (rx_bytes == 0) {
        return false;
    }
    rx_bytes = 0;
    buf[rx_bytes] = '\0';
    return true;
}

RX_Buf_Slice UDP::getRXBufSlice()
{
    return {buf, rx_bytes};
}

len_t UDP::transmit(std::string_view msg)
{
    const len_t tx_bytes =
        sendto(tx, msg.data(), static_cast<msg_len_t>(msg.length()), SENDTO_FLAG,
               tx_target->ai_addr, static_cast<ai_addrlen_t>(tx_target->ai_addrlen));
    if (tx_bytes == -1) {
        std::cerr << "send error\n";
        return false;
    }
    std::cout << "send successful\n";
    return true;
}

void UDP::clearRX()
{
    if (rx != BAD_SOCKET) {
#ifdef UDPNS_WINDOWS
        closesocket(rx);
#else
        close(rx);
#endif
        rx = BAD_SOCKET;
    }
    if (buf) {
        delete[] buf;
        buf = nullptr;
    }
}

void UDP::clearTX()
{
    if (tx != BAD_SOCKET) {
#ifdef UDPNS_WINDOWS
        closesocket(tx);
#else
        close(tx);
#endif
        tx = BAD_SOCKET;
    }
}

void UDP::clearAll()
{
    freeRXResults();
    clearRX();
    freeTXResults();
    clearTX();
}

UDP::~UDP()
{
    clearAll();
    std::cout << "~UDP\n";
}

} // namespace UDPNS
