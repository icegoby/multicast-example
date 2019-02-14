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

    sock = socket(AF_INET, SOCK_DGRAM, 0);

    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = inet_addr("239.192.1.2");
    sin.sin_port = htons(12345);

#if 1 /* for external */
    in_addr_t addr = inet_addr("127.0.0.1");
    if (setsockopt(sock,
                IPPROTO_IP,
                IP_MULTICAST_IF,
                (char *)&addr, sizeof(addr)) != 0) {
        perror("setsockopt");
        close(sock);
        return 1;
    }
#else /* for loopback */
    char loopch = 1;
    if (setsockopt(sock,
                IPPROTO_IP,
                IP_MULTICAST_LOOP,
                (char *)&loopch, sizeof(loopch)) != 0) {
        perror("setsockopt");
        close(sock);
        return 1;
    }
#endif
    sendto(sock, "HELLO", 5, 0, (struct sockaddr *)&sin, sizeof(sin));
    close(sock);
    return 0;
}
