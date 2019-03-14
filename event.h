/*
 * READ
 *
 *
 */
#ifndef _EVENT_H_
#define _EVENT_H_

/** @file event.h

  A library for writing event-driven network servers.

  The <event.h> header is deprecated in Libevent 2.0 and later; please
  use <event2/event.h> instead.  Depending on what functionality you
  need, you may also want to include more of the other event2/
  headers.
 */

#ifdef __cplusplus
extern "C" {
#endif

#include <event2/event-config.h>
#ifdef _EVENT_HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif
#ifdef _EVENT_HAVE_SYS_TIME_H
#include <sys/time.h>
#endif
#ifdef _EVENT_HAVE_STDINT_H
#include <stdint.h>
#endif
#include <stdarg.h>

/* For int types. */
#include <evutil.h>

#ifdef WIN32
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <winsock2.h>
#include <windows.h>
#undef WIN32_LEAN_AND_MEAN
typedef unsigned char u_char;
typedef unsigned short u_short;
#endif

#include <event2/event_struct.h>
#include <event2/event.h>
#include <event2/event_compat.h>
#include <event2/buffer.h>
#include <event2/buffer_compat.h>
#include <event2/bufferevent.h>
#include <event2/bufferevent_struct.h>
#include <event2/bufferevent_compat.h>
#include <event2/tag.h>
#include <event2/tag_compat.h>

#ifdef __cplusplus
}
#endif

#endif /* _EVENT_H_ */
