#include <arpa/inet.h>
#include <cstring>
#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <sys/socket.h>
#include <unistd.h>
#include <vector>

#include "packet.h"

int fd = -1;

bool TryOpenConnection(std::uint16_t port, const std::string &mc_ip, const std::string &interface_ip)
{
    if (fd >= 0)
    {
        return true;
    }

    // Create a socket
    fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (fd < 0)
    {
        std::cerr << "[MC] socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP) failed. err=" << std::strerror(errno) << std::endl;
        return false;
    }

    // RAII pattern to ensure socket gets closed on errors
    auto socket_guard =
        std::unique_ptr<int, std::function<void(const int *)>>(&fd, [](const int *fd_) { *fd_ >= 0 && close(*fd_); });

    // Allow address reuse
    if (auto reuse_addr = 1; setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &reuse_addr, sizeof(reuse_addr)) < 0)
    {
        std::cerr << "[MC] setsockopt(SO_REUSEADDR) failed. err=" << std::strerror(errno) << std::endl;
        return false;
    }

    // Loopback
    if (std::uint8_t loopback = 0; setsockopt(fd, IPPROTO_IP, IP_MULTICAST_LOOP, &loopback, sizeof(loopback)) < 0)
    {
        std::cerr << "[MC] setsockopt(IP_MULTICAST_LOOP) failed. err=" << std::strerror(errno) << std::endl;
        return false;
    }

    const in_addr interface_addr = { .s_addr = inet_addr(interface_ip.c_str()) };

    // Multicast interface
    if (setsockopt(fd, IPPROTO_IP, IP_MULTICAST_IF, &interface_addr, sizeof(interface_addr)) < 0)
    {
        std::cerr << "[MC] setsockopt(IP_MULTICAST_IF:" << interface_ip << ") failed. err=" << std::strerror(errno)
                  << std::endl;
        return false;
    }

    const ip_mreq join_addr = {
        .imr_multiaddr = { .s_addr = inet_addr(mc_ip.c_str()) },
        .imr_interface = { .s_addr = inet_addr(interface_ip.c_str()) },
    };

    // Add membership
    if (setsockopt(fd, IPPROTO_IP, IP_ADD_MEMBERSHIP, &join_addr, sizeof(join_addr)) < 0)
    {
        std::cerr << "[MC] setsockopt(IP_ADD_MEMBERSHIP) failed. err=" << std::strerror(errno) << std::endl;
        return false;
    }

    // Bind local address
    const sockaddr_in my_addr = {
        .sin_family = AF_INET,
        .sin_port = htons(port),
        .sin_addr = { .s_addr = htonl(INADDR_ANY) },
    };

    if (bind(fd, reinterpret_cast<const sockaddr *>(&my_addr), sizeof(my_addr)) < 0)
    {
        std::cerr << "[MC] bind() failed. err=" << std::strerror(errno) << std::endl;
        return false;
    }

    // Successfully opened the socket, release the guard to prevent closing
    socket_guard.release();

    std::cout << "[MC] Opened connection to " << mc_ip << ":" << port << " on " << interface_ip << std::endl;
    return true;
}

int main(void)
{
    while (TryOpenConnection(61217, "239.239.239.170", "127.0.0.1"))
    {
        auto rx_buf = std::vector<std::byte>(AMI_PKT_BUF_LEN, std::byte{ 0 });
        auto fvm_address = sockaddr_in();
        socklen_t addr_len = sizeof(fvm_address);

        auto bytes_received =
            recvfrom(fd, rx_buf.data(), rx_buf.size(), 0, reinterpret_cast<sockaddr *>(&fvm_address), &addr_len);
        if (bytes_received < 0)
        {
            if (errno == EINTR)
            {
                std::cout << "[MC] recvfrom() interrupted. retry..." << std::endl;
            }
            else
            {
                std::cerr << "[MC] recvfrom() failed. err=" << std::strerror(errno) << std::endl;
                close(fd);
                fd = -1;
            }

            continue;
        }

        if (bytes_received == 0)
        {
            std::cerr << "[MC] recvfrom() no data" << std::endl;
            close(fd);
            fd = -1;
            continue;
        }

        auto &msg_header = *reinterpret_cast<T_AMI_PKT_HDR *>(rx_buf.data());
        auto &fvm_heartbeat = *reinterpret_cast<FVMHeartbeat *>(msg_header.payload);

        if (msg_header.xid != 0x4500'0101U)
        {
            std::cerr << "[MC] Received packet with invalid XID" << std::endl;
            continue;
        }
        if (fvm_heartbeat.fvm_status != 0)
        {
            std::cerr << "[MC] Received packet with invalid FVM status" << std::endl;
            continue;
        }

        auto ip = fvm_address.sin_addr.s_addr;
        auto serial_number = std::string(fvm_heartbeat.serial_number);
        auto sw_version =
            std::to_string(fvm_heartbeat.sw_version.major) + "." + std::to_string(fvm_heartbeat.sw_version.minor) + "." +
            std::to_string(fvm_heartbeat.sw_version.patch) + "." + std::to_string(fvm_heartbeat.sw_version.build);

        std::cout << "[MC] Received heartbeat from " << ip << " serial=" << serial_number << " sw=" << sw_version
                  << std::endl;
    }
}