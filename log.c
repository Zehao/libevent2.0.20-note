/*	$OpenBSD: err.c,v 1.2 2002/06/25 15:50:15 mickey Exp $	*/

/*
 * log.c
 *
 * log函数
 * 函数名后面没有x的，会把strerror(errno)前置到输出内容中；
 * 函数名有x的，不输出errno的字符串；
 * 函数名有sock的，获取socket的errno内容；

 */

#include "event2/event-config.h"

#ifdef WIN32
#include <winsock2.h>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#undef WIN32_LEAN_AND_MEAN
#endif
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <errno.h>
#include "event2/event.h"
#include "event2/util.h"

#include "log-internal.h"

static void _warn_helper(int severity, const char *errstr, const char *fmt,
    va_list ap);
static void event_log(int severity, const char *msg);
static void event_exit(int errcode) EV_NORETURN;

static event_fatal_cb fatal_fn = NULL;

void
event_set_fatal_callback(event_fatal_cb cb)
{
	fatal_fn = cb;
}

static void
event_exit(int errcode)
{
	if (fatal_fn) {
		fatal_fn(errcode);
		exit(errcode); /* should never be reached */
	} else if (errcode == _EVENT_ERR_ABORT)
		abort();
	else
		exit(errcode);
}

void
event_err(int eval, const char *fmt, ...)
{
	va_list ap;

	va_start(ap, fmt);
	_warn_helper(_EVENT_LOG_ERR, strerror(errno), fmt, ap);
	va_end(ap);
	event_exit(eval);
}

void
event_warn(const char *fmt, ...)
{
	va_list ap;

	va_start(ap, fmt);
	_warn_helper(_EVENT_LOG_WARN, strerror(errno), fmt, ap);
	va_end(ap);
}

void
event_sock_err(int eval, evutil_socket_t sock, const char *fmt, ...)
{
	va_list ap;
	int err = evutil_socket_geterror(sock);

	va_start(ap, fmt);
	_warn_helper(_EVENT_LOG_ERR, evutil_socket_error_to_string(err), fmt, ap);
	va_end(ap);
	event_exit(eval);
}

void
event_sock_warn(evutil_socket_t sock, const char *fmt, ...)
{
	va_list ap;
	int err = evutil_socket_geterror(sock);

	va_start(ap, fmt);
	_warn_helper(_EVENT_LOG_WARN, evutil_socket_error_to_string(err), fmt, ap);
	va_end(ap);
}

void
event_errx(int eval, const char *fmt, ...)
{
	va_list ap;

	va_start(ap, fmt);
	_warn_helper(_EVENT_LOG_ERR, NULL, fmt, ap);
	va_end(ap);
	event_exit(eval);
}

void
event_warnx(const char *fmt, ...)
{
	va_list ap;

	va_start(ap, fmt);
	_warn_helper(_EVENT_LOG_WARN, NULL, fmt, ap);
	va_end(ap);
}

void
event_msgx(const char *fmt, ...)
{
	va_list ap;

	va_start(ap, fmt);
	_warn_helper(_EVENT_LOG_MSG, NULL, fmt, ap);
	va_end(ap);
}

void
_event_debugx(const char *fmt, ...)
{
	va_list ap;

	va_start(ap, fmt);
	_warn_helper(_EVENT_LOG_DEBUG, NULL, fmt, ap);
	va_end(ap);
}

static void
_warn_helper(int severity, const char *errstr, const char *fmt, va_list ap)
{
	char buf[1024];
	size_t len;

	if (fmt != NULL)
		evutil_vsnprintf(buf, sizeof(buf), fmt, ap);
	else
		buf[0] = '\0';

	if (errstr) {
		len = strlen(buf);
		if (len < sizeof(buf) - 3) {
			evutil_snprintf(buf + len, sizeof(buf) - len, ": %s", errstr);
		}
	}

	event_log(severity, buf);
}

static event_log_cb log_fn = NULL;

void
event_set_log_callback(event_log_cb cb)
{
	log_fn = cb;
}

static void
event_log(int severity, const char *msg)
{
	if (log_fn)
		log_fn(severity, msg);
	else {
		const char *severity_str;
		switch (severity) {
		case _EVENT_LOG_DEBUG:
			severity_str = "debug";
			break;
		case _EVENT_LOG_MSG:
			severity_str = "msg";
			break;
		case _EVENT_LOG_WARN:
			severity_str = "warn";
			break;
		case _EVENT_LOG_ERR:
			severity_str = "err";
			break;
		default:
			severity_str = "???";
			break;
		}
		(void)fprintf(stderr, "[%s] %s\n", severity_str, msg);
	}
}
