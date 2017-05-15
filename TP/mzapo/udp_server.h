/**
 * @author Lukas Forst
 * */

#ifndef TP_UDP_SERVER_H
#define TP_UDP_SERVER_H

#define MYPORT "44444"    // the port users will be connecting to
#define MAXBUFLEN 1024 //max size of the received datagram

/**
 * Simple UDP listener which is generating Julia sets according to the given parameters from udp datagram.
 * */
void *udp_listener(void *args);


#endif //TP_UDP_SERVER_H
