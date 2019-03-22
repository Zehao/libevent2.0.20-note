/*
 * READ
 */
#ifndef _EVSIGNAL_H_
#define _EVSIGNAL_H_

#ifndef evutil_socket_t
#include "event2/util.h"
#endif
#include <signal.h>

typedef void (*ev_sighandler_t)(int);

/* Data structure for the default signal-handling implementation in signal.c
 */
struct evsig_info {
	/* Event watching ev_signal_pair[1] */
	struct event ev_signal;
	/* Socketpair used to send notifications from the signal handler */
	evutil_socket_t ev_signal_pair[2];
	/* True iff we've added the ev_signal event yet. */
	int ev_signal_added;
	/* Count of the number of signals we're currently watching. */
	int ev_n_signals_added;

	/* Array of previous signal handler objects before Libevent started
	 * messing with them.  Used to restore old signal handlers. */
#ifdef _EVENT_HAVE_SIGACTION
	struct sigaction **sh_old;
#else
	ev_sighandler_t **sh_old;
#endif
	/* Size of sh_old. */
	int sh_old_max;
};
int evsig_init(struct event_base *);
void evsig_dealloc(struct event_base *);

void evsig_set_base(struct event_base *base);

#endif /* _EVSIGNAL_H_ */
