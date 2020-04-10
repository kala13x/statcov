/*
 *  src/nbg.c
 * 
 *  Copyleft (C) 2020  Sun Dro (a.k.a. kala13x)
 *
 * Get and parse GEL exchange rate from https://nbg.gov.ge/
 */

#include "version.h"
#include "stdinc.h"
#include "http.h"
#include "sock.h"
#include "nbg.h"

int NBG_ParseRate(const char *pSource, const char *pCase, char *pRate)
{
    char *pCases = strstr(pSource, pCase);
    if (pCases != NULL)
    {
        char *pOffset = strstr(pCases, "<td>");
        if (pOffset != NULL) 
        {
            strncpy(pRate, pOffset + 4, 6);
            return 1;
        }
    }

    return -1;
}


int NBG_ReadRates(XSocket *pSock, char *pRequest, NBGRates *pRates)
{
    char sResponse[XBUFF_SIZE];
    int nLength = strlen(pRequest);
    int nBytes = 0;

    XSock_Write(pSock, pRequest, &nLength);
    memset(pRates, 0, sizeof(NBGRates));

    while ((nBytes = XSock_Read(pSock, sResponse, sizeof(sResponse))) > 0)
    {
        sResponse[nBytes] = '\0';

        if (strlen(pRates->sEUR2GEL) <= 0)
            NBG_ParseRate(sResponse, "1 ევრო", pRates->sEUR2GEL);

        if (strlen(pRates->sUSD2GEL) <= 0)
            NBG_ParseRate(sResponse, "აშშ დოლარი", pRates->sUSD2GEL);

        if (strlen(pRates->sGBP2GEL) <= 0)
            NBG_ParseRate(sResponse, "გირვანქა სტერლინგი", pRates->sGBP2GEL);

        if (strlen(pRates->sAZN2GEL) <= 0)
            NBG_ParseRate(sResponse, "აზერბაიჯანული მანათი", pRates->sAZN2GEL);

        if (strlen(pRates->sTRY2GEL) <= 0)
            NBG_ParseRate(sResponse, "თურქული ლირა", pRates->sTRY2GEL);
    }

    return strlen(pRates->sUSD2GEL);
}


int NBG_GetRates(NBGRates *pRates)
{
    HTTPHeader *pHdr = HTTPHeader_Alloc(XBUFF_SIZE);
    HTTPHeader_Set(pHdr, XHTTP_HOST, "%s", NBG_ADDR);
    HTTPHeader_Set(pHdr, XHTTP_ACCEPT, "*/*");
    HTTPHeader_Set(pHdr, XHTTP_VERSION, "1.0");
    HTTPHeader_Set(pHdr, XHTTP_USER_AGENT, "statcov/%s", StatCov_VersionShort());

    char sRequest[XBUFF_SIZE];
    HTTPHeader_CreateRequest(pHdr, "/rss.php", sRequest, sizeof(sRequest));
    HTTPHeader_Free(pHdr);

    XSocketInfo sockInfo;
    if (!(XSock_AddrInfo(NBG_ADDR, &sockInfo)))
    {
        printf("Can not resolve address: %s\n", NBG_ADDR);
        return 0;
    }

    XSocket sock;
    XSock_Create(&sock, sockInfo.sAddr, XSSL_PORT);
    XSock_ConnectSSL(&sock);

    if (sock.nFD == XSOCK_INVALID)
    {
        printf("%s\n", XSock_Error(&sock));
        return 0;
    }

    int nStatus = NBG_ReadRates(&sock, sRequest, pRates);
    if (!nStatus) printf("%s\n", XSock_Error(&sock));

    XSock_Close(&sock);
    return nStatus;
}