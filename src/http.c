/*
 *  xutils/http.c
 * 
 *  Copyleft (C) 2018 Sun Dro (a.k.a. kala13x)
 *
 * This source includes functions for working with http request/response.
 */

#include "stdinc.h"
#include "version.h"
#include "http.h"

const char* HTTPHeader_GetCodeStr(int nCode)
{
    switch(nCode)
    {
        case 100: return "Continue";
        case 101: return "Switching Protocol";
        case 102: return "Processing";
        case 103: return "Early Hints";
        case 200: return "OK";
        case 201: return "Created";
        case 202: return "Accepted";
        case 203: return "Non-Authoritative Information";
        case 204: return "No Content";
        case 205: return "Reset Content";
        case 206: return "Partial Content";
        case 300: return "Multiple Choice";
        case 301: return "Moved Permanently";
        case 302: return "Found";
        case 303: return "See Other";
        case 304: return "Not Modified";
        case 305: return "Use Proxy";
        case 306: return "Unused";
        case 307: return "Temporary Redirect";
        case 308: return "Permanent Redirect";
        case 400: return "Bad Request";
        case 401: return "Unauthorized";
        case 402: return "Payment Required";
        case 403: return "Forbidden";
        case 404: return "Not Found";
        case 405: return "Method Not Allowed";
        case 406: return "Not Acceptable";
        case 407: return "Proxy Authentication Required";
        case 408: return "Request Timeout";
        case 409: return "Conflict";
        case 410: return "Gone";
        case 411: return "Length Required";
        case 412: return "Precondition Failed";
        case 413: return "Payload Too Large";
        case 414: return "URI Too Long";
        case 415: return "Unsupported Media Type";
        case 416: return "Requested Range Not Satisfiable";
        case 417: return "Expectation Failed";
        case 418: return "I'm a teapot";
        case 500: return "Internal Server Error";
        case 501: return "Not Implemented";
        case 502: return "Bad Gateway";
        case 503: return "Service Unavailable";
        case 504: return "Gateway Timeout";
        case 505: return "HTTP Version Not Supported";
        case 506: return "Variant Also Negotiates";
        case 507: return "Insufficient Storage";
        case 508: return "Loop Detected";
        case 510: return "Not Extended";
        case 511: return "Network Authentication Required";
        default: break;
    }

    return "Unknown";
}

void HTTPCopySafe(char* pStr, size_t nSize, const char* pSrc, size_t nSrcSize)
{
    if (nSize <= 0) return;
    while(--nSize > 0 && nSrcSize > 0 && *pSrc)
    {
        *pStr++ = *pSrc++; 
        nSrcSize--; 
    }

    *pStr = 0;
}

const char* HTTPHeader_GetOptStr(XHTTP_HEADER_OPT_E eOpt)
{
    switch(eOpt)
    {
        case XHTTP_ACCEPT_LANGUAGE: return "Accept-Language";
        case XHTTP_ACCEPT_ENCODING: return "Accept-Encoding";
        case XHTTP_ACCEPT_CHARSET: return "Accept-Charset";
        case XHTTP_ACCEPT_RANGES: return "Accept-Ranges";
        case XHTTP_CACHE_CONTROL: return "Cache-Control";
        case XHTTP_CONTENT_TYPE: return "Content-Type";
        case XHTTP_USER_AGENT: return "User-Agent";
        case XHTTP_CONNECTION: return "Connection";
        case XHTTP_VERSION: return "Version";
        case XHTTP_EXPIRES: return "Expires";
        case XHTTP_REFERER: return "Referer";
        case XHTTP_ACCEPT: return "Accept";
        case XHTTP_SERVER: return "Server";
        case XHTTP_COOKIE: return "Cookie";
        case XHTTP_ORIGIN: return "Origin";
        case XHTTP_HOST: return "Host";
        case XHTTP_DATE: return "Date";
        default: break;
    }

    return NULL;
}

void HTTPHeader_SafeCat(HTTPHeader *pHeader, const char *pStr)
{
    if (pHeader->nAvail <= 0) return;
    strncat(pHeader->pData, pStr, pHeader->nAvail);
    pHeader->nAvail -= strlen(pStr);
}

void HTTPHeader_SafeCatF(HTTPHeader *pHeader, char *pFmt, ...)
{
    if (pHeader->nAvail <= 0) return;
    char sBuffer[pHeader->nAvail];

    va_list args;
    va_start(args, pFmt);
    vsnprintf(sBuffer, sizeof(sBuffer), pFmt, args);
    va_end(args);

    strncat(pHeader->pData, sBuffer, pHeader->nAvail);
    pHeader->nAvail -= strlen(sBuffer);
}

void HTTPHeader_Add(HTTPHeader *pHeader, const char *pOpt, char *pStr)
{
    HTTPHeader_SafeCat(pHeader, pOpt);
    HTTPHeader_SafeCat(pHeader, ": ");
    HTTPHeader_SafeCat(pHeader, pStr);
    HTTPHeader_SafeCat(pHeader, "\r\n");
}

void HTTPHeader_AddF(HTTPHeader *pHeader, const char *pOpt, char *pStr, ...)
{
    if (pHeader->nAvail <= 0) return;
    char sBuffer[pHeader->nAvail];

    va_list args;
    va_start(args, pStr);
    vsnprintf(sBuffer, sizeof(sBuffer), pStr, args);
    va_end(args);

    HTTPHeader_Add(pHeader, pOpt, sBuffer);
}

void HTTPHeader_Set(HTTPHeader *pHeader, XHTTP_HEADER_OPT_E eOpt, char *pStr, ...)
{
    const char *pOpt = HTTPHeader_GetOptStr(eOpt);
    if (pOpt != NULL && pHeader->nAvail > 0) 
    {
        char sBuffer[pHeader->nAvail];
        va_list args;

        va_start(args, pStr);
        vsnprintf(sBuffer, sizeof(sBuffer), pStr, args);
        va_end(args);

        if (eOpt != XHTTP_VERSION) HTTPHeader_Add(pHeader, pOpt, sBuffer);
        else HTTPCopySafe(pHeader->sVersion, sizeof(pHeader->sVersion), sBuffer, strlen(sBuffer));
    }
}

void HTTPHeader_Init(HTTPHeader *pHeader, char *pBuf, size_t nSize)
{
    if (pBuf == NULL || nSize <= 0) return;
    memset(pHeader->sVersion, 0, sizeof(pHeader->sVersion));
    pHeader->nContentLength = 0;
    pHeader->nAlloc = 0;
    pHeader->nPost = 0;
    pHeader->pData = pBuf;
    pHeader->nSize = nSize;
    pHeader->nAvail = nSize;
}

HTTPHeader *HTTPHeader_Alloc(size_t nSize)
{
    HTTPHeader *pHeader = (HTTPHeader*)malloc(sizeof(HTTPHeader));
    if (pHeader == NULL) return NULL;

    pHeader->pData = (char*)malloc(nSize);
    if (pHeader->pData == NULL)
    {
        free(pHeader);
        return NULL;
    }

    memset(pHeader->pData, 0, nSize);
    HTTPHeader_Init(pHeader, pHeader->pData, nSize);
    pHeader->nAlloc = 1;
    return pHeader;
}

void HTTPHeader_Free(HTTPHeader *pHeader)
{
    if (pHeader != NULL && pHeader->nAlloc)
    {
        if (pHeader->pData != NULL)
        {
            free(pHeader->pData);
            pHeader->pData = NULL;
        }

        pHeader->nAlloc = 0;
        pHeader->nAvail = 0;
        pHeader->nSize = 0;
        free(pHeader);
    }
}

void HTTPHeader_SetDefaults(HTTPHeader *pHeader)
{
    HTTPHeader_Add(pHeader, "Accept", "*/*");
    HTTPHeader_AddF(pHeader, "User-Agent", "statcov/%s", StatCov_VersionShort());
    strncpy(pHeader->sVersion, "1.0", sizeof(pHeader->sVersion));
}

void HTTPHeader_InitDefaults(HTTPHeader *pHeader, char *pBuf, size_t nSize)
{
    HTTPHeader_Init(pHeader, pBuf, nSize);
    HTTPHeader_SetDefaults(pHeader);
}

void HTTPHeader_AddContent(HTTPHeader *pHeader, const char *pContent)
{
    if (pContent != NULL && (pHeader->nContentLength = strlen(pContent)) > 0)
    {
        HTTPHeader_SafeCatF(pHeader, "Content-Length: %d", pHeader->nContentLength);
        HTTPHeader_SafeCat(pHeader, "\r\n\r\n");
        HTTPHeader_SafeCat(pHeader, pContent);
    }
}

void HTTPHeader_CreateRequest(HTTPHeader *pHeader, const char *pUri, char *pRequest, int nLength)
{
    const char *pMethod = pHeader->nPost ? "POST" : "GET";
    int nBytes = snprintf(pRequest, nLength, "%s %s HTTP/%s\r\n%s", 
        pMethod, pUri, pHeader->sVersion, pHeader->pData);

    if (!pHeader->nContentLength && nBytes > 0)
    {
        nLength -= nBytes;
        strncat(pRequest, "\r\n", nLength);
    }
}

void HTTPHeader_CreateResponse(const HTTPHeader *pHeader, int nCode, char *pResponse, int nLength)
{
    const char *pCode = HTTPHeader_GetCodeStr(nCode);
    int nBytes = snprintf(pResponse, nLength, "HTTP/%s %d %s\r\n%s", 
        pHeader->sVersion, nCode, pCode, pHeader->pData);
    
    if (!pHeader->nContentLength && nBytes > 0)
    {
        nLength -= nBytes;
        strncat(pResponse, "\r\n", nLength);
    }
}
