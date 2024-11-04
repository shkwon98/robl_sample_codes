#include <filesystem>
#include <iostream>
#include <string>

#include <arpa/inet.h>
#include <netinet/sctp.h>
#include <sys/socket.h>
#include <unistd.h>

#define MY_PORT_NUM 62324

static void Server(void)
{
    auto listen_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_SCTP);
    if (listen_fd < 0)
    {
        throw std::runtime_error("socket()");
    }

    const sockaddr_in server_addr = {
        .sin_family = AF_INET,
        .sin_port = htons(MY_PORT_NUM),
        .sin_addr = { .s_addr = htonl(INADDR_ANY) },
    };
    if (bind(listen_fd, reinterpret_cast<const sockaddr *>(&server_addr), sizeof(server_addr)) < 0)
    {
        throw std::runtime_error("bind()");
    }

    const sctp_initmsg initmsg = {
        .sinit_num_ostreams = 5,
        .sinit_max_instreams = 5,
        .sinit_max_attempts = 4,
    };
    if (setsockopt(listen_fd, IPPROTO_SCTP, SCTP_INITMSG, &initmsg, sizeof(initmsg)) < 0)
    {
        throw std::runtime_error("setsockopt()");
    }

    if (listen(listen_fd, initmsg.sinit_max_instreams) < 0)
    {
        throw std::runtime_error("listen()");
    }

    int conn_fd = -1;
    sctp_sndrcvinfo sndrcvinfo = {};

    while (true)
    {
        char buffer[1024];

        std::cout << "Waiting for connection" << std::endl;

        conn_fd = accept(listen_fd, nullptr, nullptr);
        if (conn_fd < 0)
        {
            throw std::runtime_error("accept()");
        }

        std::cout << "New client connected" << std::endl;

        auto rb = sctp_recvmsg(conn_fd, buffer, sizeof(buffer), nullptr, 0, &sndrcvinfo, nullptr);
        if (rb > 0)
        {
            std::cout << "Received data: " << buffer << std::endl;
        }

        close(conn_fd);
    }
}

static void Client(void)
{
    std::string msg = "Hello, Server!";

    auto conn_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_SCTP);
    if (conn_fd < 0)
    {
        throw std::runtime_error("socket()");
    }

    const sockaddr_in server_addr = {
        .sin_family = AF_INET,
        .sin_port = htons(MY_PORT_NUM),
        .sin_addr = { .s_addr = inet_addr("127.0.0.1") },
    };

    if (connect(conn_fd, reinterpret_cast<const sockaddr *>(&server_addr), sizeof(server_addr)) < 0)
    {
        throw std::runtime_error("connect()");
    }

    if (sctp_sendmsg(conn_fd, msg.c_str(), msg.size(), nullptr, 0, 0, 0, 0, 0, 0) < 0)
    {
        throw std::runtime_error("sctp_sendmsg()");
    }

    close(conn_fd);
}

int main(int argc, char **argv)
{
    try
    {
        if (std::filesystem::path(argv[0]).filename() == "server")
        {
            std::cout << "Starting SCTP server" << std::endl;
            Server();
        }
        else
        {
            std::cout << "Starting SCTP client" << std::endl;
            Client();
        }
    }
    catch (const std::exception &e)
    {
        std::perror(e.what());
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}