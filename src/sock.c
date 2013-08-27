/*
 * sock.c
 *
 *  Created on: 2013年8月6日
 *  From:https://github.com/lattera/libsock
 */

#include "sock.h"

EXPORTED_SYM SOCK *SocketClient(const char *hostname, const char *port, int protocol)
{
    SOCK *s;

    switch (protocol) {
        case SOCK_TCP:
        case SOCK_UDP:
            break;
        default:
            return NULL;
    }

    s = get_sock_client(hostname, port, protocol);

    return s;
}

EXPORTED_SYM ssize_t SocketWrite(SOCK *sock, const void *data, size_t sz)
{
    ssize_t n=0, i;

    if ((sock->flags & SOCK_FLAG_ACTIVE) != SOCK_FLAG_ACTIVE)
        return 0;

    switch (sock->protocol) {
        case SOCK_TCP:
            do {
                i = send(sock->sockfd, data+n, sz-n, 0);
                if (i < 0 && (errno == EAGAIN))
                    continue;
                if (i < 0)
                    return -1;
                if (i == 0)
                    return n;

                n += i;
            } while (n != sz);
            break;
        case SOCK_UDP:
            do {
                i = sendto(sock->sockfd, data, sz, 0, sock->addrinfo->ai_addr, sock->addrinfo->ai_addrlen);
                if (i < 0 && (errno == EAGAIN))
                    continue;
                if (i < 0)
                    return -1;
                if (i == 0)
                    return n;

                n += i;
            } while (n != sz);
            break;
    }

    return n;
}

EXPORTED_SYM ssize_t SocketRead(SOCK *sock, void *data, size_t sz)
{
    ssize_t n=0, i;

    if ((sock->flags & SOCK_FLAG_ACTIVE) != SOCK_FLAG_ACTIVE)
        return 0;

    switch (sock->protocol) {
        case SOCK_TCP:
            do {
                i = recv(sock->sockfd, data, sz, 0);
                debug("errno :%d\n",errno);
                if (i <= 0 && (errno == EAGAIN))
                    continue;
                n = i;
            } while (n == 0);
            break;
        case SOCK_UDP:
            do {
                n = udp_read(sock, data, sz);
            } while (n == 0);
            break;
    }

    return n;
}

ssize_t udp_read(SOCK *sock, void *data, size_t sz)
{
    ssize_t n=0, i;
    socklen_t len=sock->addrinfo->ai_addrlen;
    struct sockaddr *reply_addr=NULL;
    char *name=NULL;

    reply_addr = calloc(1, sock->addrinfo->ai_addrlen);
    do {
        i = recvfrom(sock->sockfd, data, sz, 0, reply_addr, &len);
        if (i <= 0) {
            if (errno == EAGAIN) {
                continue;
            } else {
                free(reply_addr);
                return -1;
            }
        }

        n = i;
    } while (n == 0);

    if (len != sock->addrinfo->ai_addrlen) {
        free(reply_addr);
        return 0;
    }

    name = get_udp_packet_ip(sock, reply_addr);
    if (!(name)) {
        fprintf(stderr, "Could not get name!\n");
        perror("get_udp_packet");
        free(reply_addr);
        return 0;
    }

    if (strcmp(name, sock->ipaddr))
        n = 0;

    free(reply_addr);
    free(name);
    return n;
}

EXPORTED_SYM void SocketClose(SOCK *sock)
{
    if ((sock->flags & SOCK_FLAG_ACTIVE) == SOCK_FLAG_ACTIVE)
        close(sock->sockfd);

    sock->flags &= ~SOCK_FLAG_ACTIVE;

    freeaddrinfo(sock->servinfo);
    free(sock->ipaddr);
    free(sock);
}

SOCK *get_sock_client(const char *host, const char *port, int protocol)
{
    SOCK *sock;
    struct addrinfo hints, *servinfo, *p;
    int rv;

    switch (protocol) {
        case SOCK_TCP:
        case SOCK_UDP:
            break;
        default:
            return NULL;
    }

    memset(&hints, 0x00, sizeof(struct addrinfo));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = protocol;

    rv = getaddrinfo(host, port, &hints, &servinfo);
    if (rv != 0)
        return NULL;

    sock = calloc(1, sizeof(SOCK));
    sock->protocol = protocol;

    for (p = servinfo; p != NULL; p = p->ai_next) {
        sock->sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (sock->sockfd == -1)
            continue;

        if (protocol == SOCK_TCP) {
            if (connect(sock->sockfd, p->ai_addr, p->ai_addrlen) == -1) {
                close(sock->sockfd);
                continue;
            }
        }

        sock->flags |= SOCK_FLAG_ACTIVE;

        break;
    }

    if (!(p)) {
        free(sock);
        return NULL;
    }

    sock->servinfo = servinfo;
    sock->addrinfo = p;
    sock->ipaddr = get_udp_packet_ip(sock, p->ai_addr);

    return sock;
}

char *get_udp_packet_ip(SOCK *s, void *addrinfo)
{
    char *name;

    name = calloc(1, INET6_ADDRSTRLEN+1);
    if (!(name))
        return NULL;

    switch (s->addrinfo->ai_family) {
        case AF_INET:
            inet_ntop(AF_INET, &(((struct sockaddr_in *)addrinfo)->sin_addr), name, INET_ADDRSTRLEN);
            break;
        case AF_INET6:
            inet_ntop(AF_INET6, &(((struct sockaddr_in6 *)addrinfo)->sin6_addr), name, INET6_ADDRSTRLEN);
            break;
        default:
            free(name);
            return NULL;
    }

    return name;
}
