/**
 * Taken over from http://beej.us/guide/bgnet/output/html/singlepage/bgnet.html#datagram
 * edited by Lukas Forst
 * */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <pthread.h>

#include "udp_server.h"
#include "sets.h"
#include "color.h"
#include "main.h"

void *udp_listener(void *args) {
    int sockfd;
    struct addrinfo hints, *servinfo, *p;
    int rv;
    int numbytes;
    struct sockaddr_storage their_addr;
    char buf[MAXBUFLEN];
    socklen_t addr_len;
    char s[INET6_ADDRSTRLEN];

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC; // set to AF_INET to force IPv4
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_flags = AI_PASSIVE; // use my IP

    if ((rv = getaddrinfo(NULL, MYPORT, &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return NULL;
    }

    // loop through all the results and bind to the first we can
    for (p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype,
                             p->ai_protocol)) == -1) {
            perror("listener: socket");
            continue;
        }

        if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd);
            perror("listener: bind");
            continue;
        }

        break;
    }

    if (p == NULL) {
        fprintf(stderr, "listener: failed to bind socket\n");
        return udp_listener(NULL);
    }

    freeaddrinfo(servinfo);
    printf("Receiving upd...\n");
    addr_len = sizeof their_addr;
    if ((numbytes = recvfrom(sockfd, buf, MAXBUFLEN - 1, 0,
                             (struct sockaddr *) &their_addr, &addr_len)) == -1) {
        perror("recvfrom");
        close(sockfd);
        return udp_listener(NULL);
    }
    buf[numbytes] = '\0';
    printf("listener: packet contains \"%s\"\n", buf);
    close(sockfd);

    //Parse parameters from upd <x> <y> <c>
    char *tmp_number = (char *) calloc(numbytes, sizeof(char));
    int tmp_number_idx = 0, number_of_spaces = 0;
    double x = 0, y = 0;
    uint32_t c = 0;

    for (int i = 0; i <= numbytes; i++) {
        int flag = 0;

        if (*(buf + i) != ' ' && *(buf + i) != '\0') {
            *(tmp_number + tmp_number_idx++) = *(buf + i);
        } else {
            switch (number_of_spaces) {
                case 0:
                    x = atof(tmp_number);
                    tmp_number_idx = 0;
                    break;
                case 1:
                    y = atof(tmp_number);
                    tmp_number_idx = 0;
                    break;
                case 2:
                    c = atoi(tmp_number) % NUMBER_OF_STORED_C;
                    tmp_number_idx = 0;
                    flag = 1;
                    break;
            }
            number_of_spaces++;
        }
        if (flag == 1) {
            break;
        }
    }
    free(tmp_number);
    printf("udp parameters - %f %f %d\n", x, y, c);

    c_set **generated_sets = get_c_list(); //stored c in structure
    double c_real = (*(generated_sets + c))->real;
    double c_imag = (*(generated_sets + c))->imaginary;

    color **fractal = generate_julia(WIDTH, HEIGHT, x, y, c_real, c_imag, 500);

    pthread_t drawing_sets;
    pthread_create(&drawing_sets, NULL, draw_set, fractal);

    for (int i = 0; i < NUMBER_OF_STORED_C; i++) {
        free(*(generated_sets + i));
    }
    free(generated_sets);
    return udp_listener(NULL); //call it again
}

