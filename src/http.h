/*
 *  xutils/http.h
 * 
 *  Copyleft (C) 2018 Sun Dro (a.k.a. kala13x)
 *
 * This source includes functions for working with http request/response.
 */

#ifndef __S_UTILS_HTTP_H__
#define __S_UTILS_HTTP_H__

#include "stdinc.h"

#ifdef __cplusplus
extern "C" {
#endif

#define XHTTP_TOKEN_MAX     128
#define XHTTP_VER_MAX       32

typedef enum {
    XHTTP_ACCEPT_LANGUAGE = 1,
    XHTTP_ACCEPT_ENCODING,
    XHTTP_ACCEPT_CHARSET,
    XHTTP_ACCEPT_RANGES,
    XHTTP_CACHE_CONTROL,
    XHTTP_CONTENT_TYPE,
    XHTTP_USER_AGENT,
    XHTTP_CONNECTION,
    XHTTP_VERSION,
    XHTTP_EXPIRES,
    XHTTP_REFERER,
    XHTTP_ACCEPT,
    XHTTP_SERVER,
    XHTTP_COOKIE,
    XHTTP_ORIGIN,
    XHTTP_HOST,
    XHTTP_DATE
} XHTTP_HEADER_OPT_E;

typedef struct {
    char sVersion[XHTTP_VER_MAX];
    unsigned int nAlloc:1;
    unsigned int nPost:1;
    size_t nContentLength;
    size_t nSize;
    char *pData;
    int nAvail;
} HTTPHeader;

const char* HTTPHeader_GetOptStr(XHTTP_HEADER_OPT_E eOpt);
const char* HTTPHeader_GetCodeStr(int nCode);

HTTPHeader *HTTPHeader_Alloc(size_t nSize);
void HTTPHeader_Free(HTTPHeader *pHeader);

void HTTPHeader_Init(HTTPHeader *pHeader, char *pBuf, size_t nSize);
void HTTPHeader_InitDefaults(HTTPHeader *pHeader, char *pBuf, size_t nSize);
void HTTPHeader_SetDefaults(HTTPHeader *pHeader);

void HTTPHeader_Set(HTTPHeader *pHeader,  XHTTP_HEADER_OPT_E eOpt, char *pStr, ...);
void HTTPHeader_Add(HTTPHeader *pHeader, const char *pOpt, char *pStr);
void HTTPHeader_AddF(HTTPHeader *pHeader, const char *pOpt, char *pStr, ...);
void HTTPHeader_AddContent(HTTPHeader *pHeader, const char *pContent);

void HTTPHeader_SafeCat(HTTPHeader *pHeader, const char *pStr);
void HTTPHeader_SafeCatF(HTTPHeader *pHeader, char *pFmt, ...);

void HTTPHeader_CreateRequest(HTTPHeader *pHeader, const char *pUri, char *pRequest, int nLength);
void HTTPHeader_CreateResponse(const HTTPHeader *pHeader, int nCode, char *pResponse, int nLength);

#ifdef __cplusplus
}
#endif

#endif /* __S_UTILS_HTTP_H__ */
