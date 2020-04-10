/*
 *  src/stdinc.h
 * 
 *  Copyleft (C) Sun Dro (a.k.a. kala13x)
 *
 * Standart includes.
 */

#ifndef __XUTILS_INC_H__
#define __XUTILS_INC_H__

/* C includes */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <stdarg.h>
#include <signal.h>
#include <limits.h>
#include <dirent.h>
#include <ctype.h>
#include <fcntl.h>
#include <netdb.h>
#include <errno.h>
#include <time.h>

/* Linux includes */
#include <netinet/in.h>
#include <sys/syscall.h>
#include <sys/utsname.h>
#include <sys/socket.h>
#include <sys/fcntl.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <net/if.h>

/* Supported ANSI colors */
#define XCLR_NORMAL   "\x1B[0m"
#define XCLR_RED      "\x1B[31m"
#define XCLR_GREEN    "\x1B[32m"
#define XCLR_YELLOW   "\x1B[33m"
#define XCLR_BLUE     "\x1B[34m"
#define XCLR_NAGENTA  "\x1B[35m"
#define XCLR_CYAN     "\x1B[36m"
#define XCLR_WHITE    "\x1B[37m"
#define XCLR_RESET    "\033[0m"

#define COVID_ADDR      "stopcov.ge"
#define NBG_ADDR        "nbg.gov.ge"

#define XSSL_PORT       443
#define XBUFF_SIZE      4098

#endif /* __XUTILS_INC_H__ */