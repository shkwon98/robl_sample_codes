#include <errno.h>
#include <ifaddrs.h>
#include <linux/netlink.h>
#include <linux/rtnetlink.h>
#include <net/if.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

static int create_sock(const char *nic)
{
    struct sockaddr_nl addr;
    int sock;

    memset(&addr, 0, sizeof(addr));
    addr.nl_family = AF_NETLINK;
    addr.nl_groups = RTMGRP_LINK | RTMGRP_IPV4_IFADDR | RTMGRP_IPV6_IFADDR;

    sock = socket(PF_NETLINK, SOCK_RAW, NETLINK_ROUTE);
    if (sock < 0)
    {
        printf("failed to open NETLINK_ROUTE socket for %s - %s(%d)\n", nic, strerror(errno), errno);
        return -1;
    }

    if (bind(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        printf("failed to bind NETLINK_ROUTE socket for %s - %s(%d)\n", nic, strerror(errno), errno);
        close(sock);
        return -1;
    }

    return sock;
}

static int ip_changed(int sock, const char *nic)
{
    struct nlmsghdr *nlh;
    char buffer[4096];
    int len;
    int idx;
    int found;

    len = recv(sock, buffer, sizeof(buffer), 0);
    if (len <= 0)
    {
        printf("NETLINK_ROUTE socket recv() failed\n");
        return -1;
    }

    found = 0;
    idx = if_nametoindex(nic);

    for (nlh = (struct nlmsghdr *)buffer; NLMSG_OK(nlh, len); nlh = NLMSG_NEXT(nlh, len))
    {
        if (nlh->nlmsg_type == NLMSG_DONE)
        {
            break;
        }
        if (nlh->nlmsg_type == NLMSG_ERROR)
        {
            continue;
        }
        if (!(NLMSG_OK(nlh, len)))
        {
            continue;
        }

        switch (nlh->nlmsg_type)
        {
        case RTM_NEWADDR: {
            struct ifaddrmsg *ifa = (struct ifaddrmsg *)NLMSG_DATA(nlh);

            if (ifa->ifa_index == idx)
            {
                found = 1;
            }
        }
        break;
        case RTM_NEWLINK: {
            struct ifinfomsg *ifi = (struct ifinfomsg *)NLMSG_DATA(nlh);

            if (ifi->ifi_index == idx)
            {
                found = 1;
            }
        }
        break;
        default:
            break;
        }
    }

    return found;
}

static int get_nic_addr(const char *nic, struct ifaddrs *ifaddr, int wanted_family, char *host, int host_len, int *active)
{
    struct ifaddrs *ifa;

    for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next)
    {
        int family;
        int s;

        if (ifa->ifa_addr == NULL)
        {
            continue;
        }

        if (strcmp(ifa->ifa_name, nic))
        {
            continue;
        }

        /* Skip unwanted families. */
        family = ifa->ifa_addr->sa_family;
        if (family != wanted_family)
        {
            continue;
        }

        *active = (ifa->ifa_flags & IFF_RUNNING) ? 1 : 0;

        s = getnameinfo(ifa->ifa_addr, family == AF_INET ? sizeof(struct sockaddr_in) : sizeof(struct sockaddr_in6), host,
                        host_len, NULL, 0, NI_NUMERICHOST);
        if (s != 0)
        {
            printf("failed to getnameinfo() for %s - %s(%d)\n", ifa->ifa_name, gai_strerror(s), s);
            continue;
        }

        /* Get the address of only the first network interface card. */
        return 1;
    }

    return 0;
}

static void print_ip(const char *nic)
{
    struct ifaddrs *ifaddr;
    char addr[NI_MAXHOST];
    int active;

    if (getifaddrs(&ifaddr) == -1)
    {
        printf("failed to getifaddrs() - %s(%d)\n", strerror(errno), errno);
        return;
    }

    if (!get_nic_addr(nic, ifaddr, AF_INET, addr, sizeof(addr), &active))
        if (!get_nic_addr(nic, ifaddr, AF_INET6, addr, sizeof(addr), &active))
        {
            strcpy(addr, "127.0.0.1");
            active = 0;
        }

    freeifaddrs(ifaddr);

    printf("%s is %s (link %s)\n", nic, addr, active ? "active" : "inactive");
}

int main(void)
{
    char *nic = "enp4s0";
    int sock;

    print_ip(nic);

    sock = create_sock(nic);
    if (sock < 0)
    {
        return -1;
    }

    while (1)
    {
        int ret;

        ret = ip_changed(sock, nic);
        if (ret < 0)
        {
            return -1;
        }

        if (ret)
        {
            print_ip(nic);
        }
    }

    close(sock);

    return 0;
}
