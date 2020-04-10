/*
 *  xutils/sock.h
 * 
 *  Copyleft (C) Sun Dro (a.k.a. kala13x)
 *
 * This source includes socket operations such as create,
 * bind, connect, listen, select and etc. Use GCC.
 */

#ifndef __XUTILS_XSOCK_H__
#define __XUTILS_XSOCK_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/tcp.h>

#ifdef _XSOCK_USE_SSL
#define XSOCK_USE_SSL _XSOCK_USE_SSL
#endif

#ifdef XSOCK_USE_SSL
#include <openssl/opensslv.h>
#include <openssl/ssl.h>
#include <openssl/rsa.h>
#include <openssl/x509.h>
#include <openssl/evp.h>
#endif

/* Socket errors */
#define XSOCK_ERR_NONE      0
#define XSOCK_ERR_READ      1
#define XSOCK_ERR_WRITE     2
#define XSOCK_ERR_CONNECT   3
#define XSOCK_ERR_INVALID   4
#define XSOCK_ERR_CREATE    5
#define XSOCK_ERR_SSLCNT    6
#define XSOCK_ERR_NOSSL     7

#define XSOCK_FINISH        8
#define XSOCK_INVALID       -1
#define XSOCK_SUCCESS       XSOCK_ERR_NONE

#define XSOCK_INFO_MAX      256

/* X Socket */
typedef struct {
    struct sockaddr_in inAddr;
    uint32_t nAddr;
    uint16_t nPort;
    int nStatus;
    int nSSL;
    int nFD;
#ifdef XSOCK_USE_SSL
    SSL_CTX *pSSLCtx;
    SSL *pSSL;
#endif
} XSocket;

/* Socket info */
typedef struct {
    char sAddr[XSOCK_INFO_MAX];
    char sName[XSOCK_INFO_MAX];
    int nFamily;
} XSocketInfo;

void XSock_InitSSL();
void XSock_DeinitSSL();

void XSock_Init(XSocket *pSock);
void XSock_Close(XSocket *pSock);
void XSock_Set(XSocket *pSock, int nFd, int nType);

const char* XSock_Error(XSocket *pSock);
int XSock_Check(XSocket *pSock);
int XSock_IsOpen(XSocket *pSock);
int XSock_AddrInfo(const char *pHost, XSocketInfo *pInfo);

int XSock_Read(XSocket *pSock, void *ptr, size_t nLen);
int XSock_Write(XSocket *pSock, void *pBuf, int *pLen);

int XSock_ConnectSSL(XSocket *pSock);
int XSock_Connect(XSocket *pSock);
int XSock_Create(XSocket *pSock, const char *pAddr, uint16_t nPort);

#ifdef __cplusplus
}
#endif

#endif /* __XUTILS_XSOCK_H__ */
