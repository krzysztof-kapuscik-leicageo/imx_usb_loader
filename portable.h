#ifndef __PORTABLE_H__
#define __PORTABLE_H__

#include <stdio.h>
#include <stdarg.h>

#include "osal.h"

extern int debugmode;

static inline void printf_wrapper(const char* format, ...)
{
	va_list args;
	va_start(args, format);
	imx_get_osal()->print(0, format, args);
	va_end(args);
}

static inline void fprintf_wrapper(FILE* fstr, const char* format, ...)
{
	if (fstr == stdout) {
		va_list args;
		va_start(args, format);
		imx_get_osal()->print(0, format, args);
		va_end(args);
	} else if (fstr == stderr) {
		va_list args;
		va_start(args, format);
		imx_get_osal()->print(1, format, args);
		va_end(args);
	} else {
		va_list args;
		va_start(args, format);
		vfprintf(fstr, format, args);
		va_end(args);
	}
}

#define printf		printf_wrapper
#define fprintf		fprintf_wrapper

#ifndef WIN32

#define dbg_printf(fmt, args...) \
	do{ if(debugmode) fprintf(stderr, fmt, ## args); } while(0)
#define dbg_dump_long(src, cnt, addr, skip) \
	do{ if(debugmode) dump_long(src, cnt, addr, skip); } while(0)

#else

#define dbg_printf(fmt, ...) \
	do{ if(debugmode) fprintf(stderr, fmt, __VA_ARGS__); } while(0)

#define dbg_dump_long(src, cnt, addr, skip) \
	do{ if(debugmode) dump_long(src, cnt, addr, skip); } while(0)

#endif

#ifndef _MSC_VER
#include <unistd.h>
#endif
#ifdef WIN32
#include <Windows.h>
#include <direct.h>
#include <io.h>
#endif
#ifdef __linux__
#include <linux/limits.h>
#endif
#ifdef __APPLE__
#include <sys/syslimits.h>
#endif
#ifdef __FreeBSD__
#include <sys/param.h>
#endif

#ifndef WIN32
#define PATH_SEPARATOR '/'
#define msleep(ms) usleep((ms) * 1000)
#else
#define PATH_MAX MAX_PATH
#define PATH_SEPARATOR '\\'
#define msleep(ms) Sleep(ms)
#endif

#ifdef _MSC_VER
#define R_OK 04

#define open(filename,oflag)	_open(filename,oflag)
#define write(fd,buffer,count)	_write(fd,buffer,count)
#define read(fd,buffer,count)	_read(fd,buffer,count)
#define close(fd)				_close(fd)
#define access(filename,oflag)	_access(filename,oflag)
#define getcwd(buffer, maxlen)	_getcwd(buffer, maxlen)
#endif

#ifdef __GNUC__
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
#define BE32(x) __builtin_bswap32(x)
#if (__GNUC__ > 4) || ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 8))
#define BE16(x) __builtin_bswap16(x)
#else
#define BE16(x) \
({ \
	typeof(x) __x = (x); \
	(((__x << 8) & 0xffff) | ((__x >> 8) & 0xff)); \
})
#endif
#else
#define BE32(x) x
#define BE16(x) x
#endif
#elif _MSC_VER // assume little endian...
#define BE32(x) _byteswap_ulong(x)
#define BE16(x) _byteswap_ushort(x)
#endif

#endif /* __PORTABLE_H__ */
