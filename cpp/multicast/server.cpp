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
    while (TryOpenConnection(61217, "239.239.239.170", "192.168.0.170"))
    {
        std::vector<std::uint8_t> pkt_buf(AMI_PKT_LEN);
        auto &pkt_hdr = *reinterpret_cast<T_AMI_PKT_HDR *>(pkt_buf.data());
        auto &pkt_payload = *reinterpret_cast<FVMHeartbeat *>(pkt_buf.data() + AMI_PKT_HDR__SZ);

        pkt_payload.seq = 0;
        std::strncpy(pkt_payload.part_number, "ABCDEFG", sizeof(pkt_payload.part_number) - 1);
        std::strncpy(pkt_payload.serial_number, "1234567", sizeof(pkt_payload.serial_number) - 1);
        pkt_payload.sw_version.build = 6;
        pkt_payload.sw_version.patch = 7;
        pkt_payload.sw_version.minor = 8;
        pkt_payload.sw_version.major = 9;
        pkt_payload.fvm_status = 0;
        pkt_payload.sensor_connection = 0;

        pkt_hdr.magic = AMI_PKT_MAGIC;
        pkt_hdr.spid = 0;
        pkt_hdr.dpid = 0;
        pkt_hdr.tpn = 1;
        pkt_hdr.psn = 0;
        pkt_hdr.tpl = sizeof(FVMHeartbeat);
        pkt_hdr.xid = 0x4500'0101U;
        pkt_hdr.tick = 0;
        pkt_hdr.pl = sizeof(FVMHeartbeat);
        pkt_hdr.crc32 = 0;
        pkt_hdr.crc32 = CalculateCrc32(reinterpret_cast<std::uint8_t *>(&pkt_hdr), AMI_PKT_HDR__SZ + pkt_hdr.pl, 0U);

        sockaddr_in mc_addr = {
            .sin_family = AF_INET,
            .sin_port = htons(61217),
            .sin_addr = { .s_addr = inet_addr("239.239.239.170") },
        };

        if (sendto(fd, pkt_buf.data(), pkt_buf.size(), 0, reinterpret_cast<const sockaddr *>(&mc_addr), sizeof(mc_addr)) < 0)
        {
            std::cerr << "[MC] sendto() failed. err=" << std::strerror(errno) << std::endl;
            close(fd);
            fd = -1;
            continue;
        }

        std::cout << "[MC] Sent packet" << std::endl;
        sleep(1);
    }
}