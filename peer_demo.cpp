#include <iostream>
#include <string_view>
#include <thread>
#include "udpns.h"

bool CON_CON{true};

void rxThread(UDPNS::UDP* udpMan)
{
    while (true) {
        if (!CON_CON) { 
            std::cerr << "CON_CON DEAD SO RX...\n";
            break;
        }
        const UDPNS::RX_Recv_Res peek_res{udpMan->peek()};
        if (peek_res == UDPNS::RX_Recv_Res::MinusOne) {
            std::cerr << "RECEIVE <MinusONE>\n";
            break;
        }
        udpMan->read();
        const auto rxBufSlice{udpMan->getRXBufSlice()};
        if (std::string_view{rxBufSlice.buf} == "") {
            continue;
        }
        std::cerr << rxBufSlice.rx_bytes << " - " << rxBufSlice.buf << "\n";
        if (std::string_view(rxBufSlice.buf) == "KAPAT") {
            std::cerr << "RX KAPAT GELDI\n";
            CON_CON = false;
            break;
        }
        udpMan->clearRXBuf();
    }
    std::cerr << "RX DOWN\n";
}

void txThread(UDPNS::UDP* udpMan)
{
    while (true) {
        if (!CON_CON) {
            std::cout << "CON_CON DEAD SO TX...\n";
        }
        std::string msg;
        std::cin >> msg;
        if (msg == "e") {
            for (int i {0}; i < 10; ++i)
                udpMan->transmit("");
            continue;
        }
        if (msg == "KAPAT") {
            std::cerr << "TX KAPAT GONDERDIK\n";
            CON_CON = false;
            udpMan->transmit(msg);
            break;
        }
        udpMan->transmit(msg);
    }
    std::cerr << "TX DOWN\n";
}

int main([[maybe_unused]] int argc, [[maybe_unused]] char** argv)
{
    if (argc != 4) {
        std::cerr << "USAGE: peer RX_PORT TX_HOST TX_PORT\n";
        return -1;
    }

#ifdef UDPNS_WINDOWS
    if (!UDPNS::initWSA()) {
        std::cerr << "WSA DOES NOT INIT, terminating...\n";
        return -2;
    }
#endif

    UDPNS::UDP udpMan;

    udpMan.createRX("", argv[1]);
    if (!udpMan.makeBuffers()) {
        std::cerr << "MAKEBUFFER!!\n";
        return -3;
    }
    std::cerr << "RX UP\n";

    udpMan.createTX(argv[2], argv[3]);
    std::cerr << "TX UP\n";

    std::thread rxHandle(rxThread, &udpMan);
    std::thread txHandle(txThread, &udpMan);
    rxHandle.join();
    txHandle.join();
    std::cerr << "END MAIN\n";

#ifdef UDPNS_WINDOWS
    UDPNS::stopWSA();
#endif

    return 0;
}