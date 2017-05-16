/**
 *@author Lukas Forst
 * */

#ifndef TP_SHOW_WINDOW_H
#define TP_SHOW_WINDOW_H

#include <stdbool.h>

/**
 * Setting this to the true will cause interrupt of the <i>show_window()</i>.
 * */
bool stop_show_window;

/**
 * Infinite loop with nice animation and lot of sets in the database.<br> Can be interrupted by
 * udp datagram or with pressing any button.
 * */
void show_window();

#endif //TP_SHOW_WINDOW_H
