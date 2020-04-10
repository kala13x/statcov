/*
 *  xutils/sock.c
 * 
 *  Copyleft (C) Sun Dro (a.k.a. kala13x)
 *
 * This source includes socket operations such as create,
 * bind, connect, listen, select and etc. Use GCC.
 */

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>

#include <netinet/in.h>
#include <sys/fcntl.h>
#include <sys/ioctl.h>
#include <arpa/inet.h>
#include <net/if.h>
#include "sock.h"

#define XSOCK_MIN(a,b) (((a)<(b))?(a):(b))

const char* XSock_Error(XSocket *pSock)
{
    switch(pSock->nStatus)
    {
        case XSOCK_ERR_NONE:
            return "No error";
        case XSOCK_ERR_READ:
            return "Can not read data from the socket";
        case XSOCK_ERR_WRITE:
            return "Can not write data fo the socket";
        case XSOCK_ERR_CONNECT:
            return "Can not connect to the socket";
        case XSOCK_ERR_CREATE:
            return "Can not create the socket";
        case XSOCK_ERR_NOSSL:
            return "No SSL (OpenSSL) support";
        case XSOCK_ERR_SSLCNT:
            return "Can not connect SSL server";
        case XSOCK_FINISH:
            return "Received final packet (FIN)";
        case XSOCK_ERR_INVALID:
            return "Socket is not open";
        default:
            break;
    }

    return "Undefined error";
}

int XSock_IsOpen(XSocket *pSock)
{
    return pSock->nFD != XSOCK_INVALID ? 1 : 0;
}

int XSock_Check(XSocket *pSock)
{
    if (pSock->nFD == XSOCK_INVALID)
    {
        if (pSock->nFD == XSOCK_ERR_NONE)
            pSock->nStatus = XSOCK_ERR_INVALID;
        return 0;
    }

    pSock->nStatus = XSOCK_ERR_NONE;
    return 1;
}

void XSock_Init(XSocket *pSock)
{
#if XSOCK_USE_SSL
    pSock->pSSL = NULL;
    pSock->pSSLCtx = NULL;
#endif

    pSock->nStatus = XSOCK_ERR_NONE;
    pSock->nFD = XSOCK_INVALID;
    pSock->nAddr = 0;
    pSock->nPort = 0;
    pSock->nSSL = 0;
}

void XSock_InitSSL()
{
#ifdef XSOCK_USE_SSL
#if OPENSSL_VERSION_NUMBER < 0x10100000L
    SSL_library_init();
    SSL_load_error_strings();
    OpenSSL_add_all_algorithms();
#else
    OPENSSL_init_ssl(0, NULL);
#endif
#endif
}

void XSock_DeinitSSL()
{
#ifdef XSOCK_USE_SSL
#if OPENSSL_VERSION_NUMBER < 0x10100000L
    ENGINE_cleanup();
    ERR_free_strings();
#else
    EVP_PBE_cleanup();
#endif
    EVP_cleanup();
    CRYPTO_cleanup_all_ex_data();
#endif
}

void XSock_Set(XSocket *pSock, int nFd, int nType)
{
#if XSOCK_USE_SSL
    pSock->pSSL = NULL;
    pSock->pSSLCtx = NULL;
#endif

    pSock->nStatus = XSOCK_ERR_NONE;
    pSock->nAddr = 0;
    pSock->nPort = 0;
    pSock->nSSL = 0;
    pSock->nFD = nFd;
}

void XSock_Close(XSocket *pSock)
{
    if (pSock->nSSL)
    {
 #if XSOCK_USE_SSL
        if (pSock->pSSL != NULL)
        {
            SSL_shutdown(pSock->pSSL);
            pSock->pSSL = NULL;
        }

        if (pSock->pSSLCtx != NULL)
        {
            SSL_CTX_free(pSock->pSSLCtx);
            pSock->pSSLCtx = NULL;
        }
#endif
        pSock->nSSL = 0;
    }

    if (pSock->nFD >= 0)
    {
        shutdown(pSock->nFD, SHUT_RDWR);
        close(pSock->nFD);
    }
    
    pSock->nFD = XSOCK_INVALID;
}

int XSock_Read(XSocket *pSock, void *ptr, size_t nLen)
{
    if (!XSock_Check(pSock)) return XSOCK_INVALID;
    if (nLen <= 0) return 1;
    int nSize = 0;

    if (pSock->nSSL)
    {
#ifdef XSOCK_USE_SSL
        nSize = SSL_read(pSock->pSSL, ptr, nLen);
#endif
    }
    else
    {
#ifdef EINTR
        do nSize = read(pSock->nFD, ptr, nLen);
        while (nSize < 0 && errno == EINTR);
#else
        nSize = read(pSock->nFD, ptr, nLen);
#endif
    }

    if (nSize <= 0)
    {
        pSock->nStatus = (nSize < 0) ? XSOCK_ERR_READ : XSOCK_FINISH;
        XSock_Close(pSock);
    }

    return nSize;
}

int XSock_Write(XSocket *pSock, void *pBuf, int *pLen)
{
    if (!XSock_Check(pSock)) return XSOCK_INVALID;
    int nBytes = 0;

    if (pSock->nSSL)
    {
#ifdef XSOCK_USE_SSL
        nBytes = SSL_write(pSock->pSSL, pBuf, *pLen);
#endif
    }
    else
    {
        nBytes = write(pSock->nFD, pBuf, *pLen);
    }
    
    if (nBytes < 0)
    {
        pSock->nStatus = XSOCK_ERR_WRITE;
        XSock_Close(pSock);
    }

    return nBytes;
}

int XSock_AddrInfo(const char *pHost, XSocketInfo *pInfo)
{
    struct addrinfo hints, *res = NULL;
    void *ptr = NULL;

    memset(&hints, 0, sizeof (hints));
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_family = PF_UNSPEC;
    hints.ai_flags |= AI_CANONNAME;
    hints.ai_canonname = NULL;
    hints.ai_addr = NULL;
    hints.ai_next = NULL;

    if (getaddrinfo(pHost, NULL, &hints, &res)) return 0;
    inet_ntop(res->ai_family, res->ai_addr->sa_data, pInfo->sAddr, sizeof(pInfo->sAddr));

    switch (res->ai_family)
    {
        case AF_INET:
            ptr = &((struct sockaddr_in *) res->ai_addr)->sin_addr;
            break;
        case AF_INET6:
            ptr = &((struct sockaddr_in6 *) res->ai_addr)->sin6_addr;
            break;
    }

    inet_ntop(res->ai_family, ptr, pInfo->sAddr, sizeof(pInfo->sAddr));
    strncpy(pInfo->sName, res->ai_canonname, sizeof(pInfo->sName)-1);
    pInfo->nFamily = res->ai_family == PF_INET6 ? 6 : 4;
    freeaddrinfo(res);

    return 1;
}

int XSock_ConnectSSL(XSocket *pSock)
{
    if (!XSock_Check(pSock)) return XSOCK_INVALID;

 #if XSOCK_USE_SSL
    pSock->pSSLCtx = SSL_CTX_new(SSLv23_client_method());
    pSock->pSSL = SSL_new(pSock->pSSLCtx);
    SSL_set_fd(pSock->pSSL, pSock->nFD);
    if (SSL_connect(pSock->pSSL) < 0)
    {
        pSock->nStatus = XSOCK_ERR_SSLCNT;
        XSock_Close(pSock);
        return XSOCK_INVALID;
    }

    pSock->nSSL = 1;
    return pSock->nFD;
#endif

    pSock->nStatus = XSOCK_ERR_NOSSL;
    XSock_Close(pSock);
    return XSOCK_INVALID;
}

int XSock_Connect(XSocket *pSock) 
{
    if (connect(pSock->nFD, (struct sockaddr *)&pSock->inAddr, sizeof(pSock->inAddr)) < 0)
    {
        pSock->nStatus = XSOCK_ERR_CONNECT;
        XSock_Close(pSock);
        return XSOCK_INVALID;
    }

    return pSock->nFD;
}

int XSock_Create(XSocket *pSock, const char *pAddr, uint16_t nPort)
{
    pSock->nAddr = htonl(INADDR_ANY);
    pSock->nStatus = XSOCK_ERR_NONE;
    pSock->nPort = nPort;
    pSock->nSSL = 0;

    if (pAddr != NULL)
    {
        struct in_addr addr;
        inet_pton(AF_INET, pAddr, &addr);
        pSock->nAddr = addr.s_addr;
    }

    pSock->inAddr.sin_family = AF_INET;
    pSock->inAddr.sin_port = htons(nPort);
    pSock->inAddr.sin_addr.s_addr = pSock->nAddr;

    pSock->nFD = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (pSock->nFD < 0)
    {
        pSock->nStatus = XSOCK_ERR_CREATE;
        return XSOCK_INVALID;
    }

    XSock_Connect(pSock);
    return pSock->nFD;
}
