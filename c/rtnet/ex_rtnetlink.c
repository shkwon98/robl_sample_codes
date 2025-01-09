#include <arpa/inet.h>
#include <linux/if_link.h>
#include <linux/netlink.h>
#include <linux/rtnetlink.h>
#include <net/if.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define BUFFER_SIZE 8192

typedef struct
{
    int index;
    char name[IF_NAMESIZE];
    unsigned int flags;
    unsigned char hw_addr[8]; // 최대 8바이트 MAC 주소
    int hw_addr_len;
    char ipv4_addrs[10][INET_ADDRSTRLEN]; // 최대 10개의 IP 주소 저장
    int ipv4_addr_count;
} Interface;

// Helper 함수: 속성(attribute) 디코딩
void parse_rtattr(struct rtattr *tb[], int max, struct rtattr *rta, int len)
{
    memset(tb, 0, sizeof(struct rtattr *) * (max + 1));
    while (RTA_OK(rta, len))
    {
        if (rta->rta_type <= max)
        {
            tb[rta->rta_type] = rta;
        }
        rta = RTA_NEXT(rta, len);
    }
}

// 네트워크 인터페이스 정보 가져오기 (RTM_GETLINK)
int get_interfaces(int sock_fd, Interface *interfaces)
{
    struct
    {
        struct nlmsghdr nlh;
        struct rtgenmsg rtgen;
    } req;

    char buffer[BUFFER_SIZE];
    struct sockaddr_nl addr;
    struct iovec iov = { buffer, sizeof(buffer) };
    struct msghdr msg = {
        .msg_name = &addr,
        .msg_namelen = sizeof(addr),
        .msg_iov = &iov,
        .msg_iovlen = 1,
    };

    memset(&req, 0, sizeof(req));
    req.nlh.nlmsg_len = NLMSG_LENGTH(sizeof(req.rtgen));
    req.nlh.nlmsg_type = RTM_GETLINK;
    req.nlh.nlmsg_flags = NLM_F_REQUEST | NLM_F_DUMP;
    req.rtgen.rtgen_family = AF_PACKET;

    if (send(sock_fd, &req, req.nlh.nlmsg_len, 0) < 0)
    {
        perror("send");
        return -1;
    }

    int interface_count = 0;

    while (1)
    {
        int len = recvmsg(sock_fd, &msg, 0);
        if (len < 0)
        {
            perror("recvmsg");
            return -1;
        }

        struct nlmsghdr *nlh = (struct nlmsghdr *)buffer;
        while (NLMSG_OK(nlh, len))
        {
            if (nlh->nlmsg_type == NLMSG_DONE)
            {
                return interface_count;
            }

            if (nlh->nlmsg_type == NLMSG_ERROR)
            {
                fprintf(stderr, "Netlink error\n");
                return -1;
            }

            if (nlh->nlmsg_type == RTM_NEWLINK)
            {
                struct ifinfomsg *ifi = NLMSG_DATA(nlh);
                struct rtattr *rta = IFLA_RTA(ifi);
                int rta_len = IFLA_PAYLOAD(nlh);

                struct rtattr *tb[IFLA_MAX + 1];
                parse_rtattr(tb, IFLA_MAX, rta, rta_len);

                if (tb[IFLA_IFNAME])
                {
                    Interface *iface = &interfaces[interface_count];
                    strcpy(iface->name, (char *)RTA_DATA(tb[IFLA_IFNAME]));
                    iface->index = ifi->ifi_index;
                    iface->flags = ifi->ifi_flags;
                    iface->ipv4_addr_count = 0;

                    // 하드웨어 주소(MAC 주소) 확인
                    if (tb[IFLA_ADDRESS])
                    {
                        iface->hw_addr_len = RTA_PAYLOAD(tb[IFLA_ADDRESS]);
                        memcpy(iface->hw_addr, RTA_DATA(tb[IFLA_ADDRESS]), iface->hw_addr_len);
                    }
                    else
                    {
                        iface->hw_addr_len = 0;
                    }

                    interface_count++;
                }
            }

            nlh = NLMSG_NEXT(nlh, len);
        }
    }
}

// IPv4 주소 출력하기 (RTM_GETADDR)
void get_ip_addresses(int sock_fd, Interface *interfaces, int interface_count)
{
    struct
    {
        struct nlmsghdr nlh;
        struct ifaddrmsg ifa;
    } req;

    char buffer[BUFFER_SIZE];
    struct sockaddr_nl addr;
    struct iovec iov = { buffer, sizeof(buffer) };
    struct msghdr msg = {
        .msg_name = &addr,
        .msg_namelen = sizeof(addr),
        .msg_iov = &iov,
        .msg_iovlen = 1,
    };

    memset(&req, 0, sizeof(req));
    req.nlh.nlmsg_len = NLMSG_LENGTH(sizeof(req.ifa));
    req.nlh.nlmsg_type = RTM_GETADDR;
    req.nlh.nlmsg_flags = NLM_F_REQUEST | NLM_F_DUMP;
    req.ifa.ifa_family = AF_INET;

    if (send(sock_fd, &req, req.nlh.nlmsg_len, 0) < 0)
    {
        perror("send");
        return;
    }

    while (1)
    {
        int len = recvmsg(sock_fd, &msg, 0);
        if (len < 0)
        {
            perror("recvmsg");
            return;
        }

        struct nlmsghdr *nlh = (struct nlmsghdr *)buffer;
        while (NLMSG_OK(nlh, len))
        {
            if (nlh->nlmsg_type == NLMSG_DONE)
            {
                return;
            }

            if (nlh->nlmsg_type == NLMSG_ERROR)
            {
                fprintf(stderr, "Netlink error\n");
                return;
            }

            if (nlh->nlmsg_type == RTM_NEWADDR)
            {
                struct ifaddrmsg *ifa = NLMSG_DATA(nlh);
                struct rtattr *rta = IFA_RTA(ifa);
                int rta_len = IFA_PAYLOAD(nlh);

                struct rtattr *tb[IFA_MAX + 1];
                parse_rtattr(tb, IFA_MAX, rta, rta_len);

                for (int i = 0; i < interface_count; i++)
                {
                    if (interfaces[i].index == ifa->ifa_index)
                    {
                        if (tb[IFA_ADDRESS])
                        {
                            char ip[INET_ADDRSTRLEN];
                            inet_ntop(AF_INET, RTA_DATA(tb[IFA_ADDRESS]), ip, sizeof(ip));
                            if (interfaces[i].ipv4_addr_count < 10)
                            {
                                strcpy(interfaces[i].ipv4_addrs[interfaces[i].ipv4_addr_count++], ip);
                            }
                        }
                    }
                }
            }

            nlh = NLMSG_NEXT(nlh, len);
        }
    }
}

void print_interfaces(Interface *interfaces, int count)
{
    for (int i = 0; i < count; i++)
    {
        printf("Index: %d\n", interfaces[i].index);
        printf("  Interface Name: %s\n", interfaces[i].name);
        printf("  Flags: 0x%x\n", interfaces[i].flags);
        if (interfaces[i].hw_addr_len > 0)
        {
            printf("  Hardware Address: ");
            for (int j = 0; j < interfaces[i].hw_addr_len; j++)
            {
                printf("%02x%c", interfaces[i].hw_addr[j], j == interfaces[i].hw_addr_len - 1 ? '\n' : ':');
            }
        }
        else
        {
            printf("  Hardware Address: (none)\n");
        }
        for (int j = 0; j < interfaces[i].ipv4_addr_count; j++)
        {
            printf("  IPv4 Address: %s\n", interfaces[i].ipv4_addrs[j]);
        }
    }
}

int main()
{
    struct sockaddr_nl addr = { AF_NETLINK, 0, 0, 0 };

    Interface interfaces[128];

    // Netlink 소켓 생성
    int sock_fd = socket(AF_NETLINK, SOCK_RAW, NETLINK_ROUTE);
    if (sock_fd < 0)
    {
        perror("socket");
        return EXIT_FAILURE;
    }

    if (bind(sock_fd, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        perror("bind");
        close(sock_fd);
        return EXIT_FAILURE;
    }

    // 네트워크 인터페이스 정보 가져오기
    int interface_count = get_interfaces(sock_fd, interfaces);
    if (interface_count < 0)
    {
        close(sock_fd);
        return EXIT_FAILURE;
    }

    // IP 주소 정보 가져오기
    get_ip_addresses(sock_fd, interfaces, interface_count);

    // 인터페이스 정보 출력
    print_interfaces(interfaces, interface_count);

    close(sock_fd);
    return EXIT_SUCCESS;
}
