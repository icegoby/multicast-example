#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int
main()
{
    int sock;
    struct sockaddr_in sin;
    char buf[2048];
    char *ifname="lo";
    struct ip_mreq mreq;

    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    setsockopt(sock, SOL_SOCKET, SO_BINDTODEVICE, ifname, 3);

    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = INADDR_ANY;
    // sin.sin_addr.s_addr = inet_addr("127.0.0.1");;
    sin.sin_port = htons(12345);

    bind(sock, (struct sockaddr *)&sin, sizeof(sin));

    memset(&mreq, 0, sizeof(mreq));
    mreq.imr_interface.s_addr = inet_addr("127.0.0.1");
    mreq.imr_multiaddr.s_addr = inet_addr("239.192.1.2");
    if (setsockopt(sock,
                IPPROTO_IP,
                IP_ADD_MEMBERSHIP,
                (char *)&mreq, sizeof(mreq)) != 0) {
        printf("failed to setsockopt\n");
        close(sock);
        return -1;
    }

    memset(buf, 0, sizeof(buf));
    printf("ready\n");
    recv(sock, buf, sizeof(buf), 0);
    printf("%s\n", buf);
    close(sock);
    return 0;
}
