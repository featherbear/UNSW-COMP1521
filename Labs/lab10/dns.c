#include <stdio.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>


int main(int argc, char **argv) {
    struct in_addr ip;   // see "man 3 inet_aton"
    struct hostent *hp;  // see "man 3 gethostbyname"

    if (argc < 2) {
        printf("Usage: %s HostName|IPaddr\n", argv[0]);
        return 1;
    }

    if (inet_aton(argv[1], &ip)) {
        if (!(hp = gethostbyaddr(&ip, sizeof(ip), AF_INET))) {
            fprintf(stderr, "The address %s doesn't resolve to any hostname!\n", argv[1]);
            return 254;
        };

    } else {
        // not an IP, assume hostname
        if (!(hp = gethostbyname(argv[1]))) {
            switch (h_errno) {
                case HOST_NOT_FOUND:
                    fprintf(stderr, "The hostname %s doesn't exist!\n", argv[1]);
                    return 254;
                default:
                    fprintf(stderr, "An error occurred trying to resolve %s!\n", argv[1]);
                    return 255;
            }
        };
    }
    printf("Host: %s\n", hp->h_name);

    if (*hp->h_aliases) {
        printf("Aliases: ");
        for (char **alias = hp->h_aliases; *alias != NULL; alias++) {
            if (alias != hp->h_aliases) {
                printf(", ");
            }
            printf("%s", *alias);
        }
        putchar('\n');
    }

    if (*hp->h_addr_list) {
        printf("Addresses: ");
        for (char **addr = hp->h_addr_list; *addr != NULL; addr++) {
            if (addr != hp->h_addr_list) {
                printf(", ");
            }
            printf("%s", inet_ntoa(*(struct in_addr *) (char *) *addr));
        }
        putchar('\n');
    }

    return 0;
}
