/*
 *  src/covid.c
 * 
 *  Copyleft (C) 2020  Sun Dro (a.k.a. kala13x)
 *
 * Get and parse COVID-19 case statistics from https://stopcov.ge/
 */

#include "http.h"
#include "sock.h"
#include "covid.h"
#include "version.h"

int COVID_ParseCase(const char *pSource, const char *pCase)
{
    char *pCases = strstr(pSource, pCase);
    if (pCases != NULL)
    {
        char *pOffset = strstr(pCases, "numver");
        if (pOffset != NULL) 
        {
            pOffset += 8;
            return atoi(pOffset);
        }
    }

    return -1;
}

int COVID_ReadCases(XSocket *pSock, char *pRequest, COVIDCases *pCovCases)
{
    char sResponse[XBUFF_SIZE];
    int nLength = strlen(pRequest);
    int nBytes = 0;

    XSock_Write(pSock, pRequest, &nLength);
    memset(pCovCases, 0, sizeof(COVIDCases));

    while ((nBytes = XSock_Read(pSock, sResponse, sizeof(sResponse))) > 0)
    {
        sResponse[nBytes] = '\0';
        
        if (pCovCases->nConfirmed <= 0)
            pCovCases->nConfirmed = COVID_ParseCase(sResponse, "დადასტურებული შემთხვევა");

        if (pCovCases->nRecovered <= 0)
            pCovCases->nRecovered = COVID_ParseCase(sResponse, "მათ შორის გამოჯანმრთელებული");

        if (pCovCases->nSupervision <= 0)
            pCovCases->nSupervision = COVID_ParseCase(sResponse, "მეთვალყურეობის ქვეშ");

        if (pCovCases->nQuarantined <= 0)
            pCovCases->nQuarantined = COVID_ParseCase(sResponse, "კარანტინის რეჟიმში");

        if (pCovCases->nDeaths <= 0)
            pCovCases->nDeaths = COVID_ParseCase(sResponse, "მათ შორის გარდაცვლილი");
    }

    return pCovCases->nConfirmed;
}

int COVID_GetCases(COVIDCases *pCovCases)
{
    HTTPHeader *pHdr = HTTPHeader_Alloc(XBUFF_SIZE);
    HTTPHeader_Set(pHdr, XHTTP_HOST, COVID_ADDR);
    HTTPHeader_Set(pHdr, XHTTP_ACCEPT, "*/*");
    HTTPHeader_Set(pHdr, XHTTP_VERSION, "1.0");
    HTTPHeader_Set(pHdr, XHTTP_USER_AGENT, "statcov/%s", StatCov_VersionShort());

    char sRequest[XBUFF_SIZE];
    HTTPHeader_CreateRequest(pHdr, "/", sRequest, sizeof(sRequest));
    HTTPHeader_Free(pHdr);

    XSocketInfo sockInfo;
    if (!(XSock_AddrInfo(COVID_ADDR, &sockInfo)))
    {
        printf("Can not resolve address: %s\n", COVID_ADDR);
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

    int nStatus = COVID_ReadCases(&sock, sRequest, pCovCases);
    if (!nStatus) printf("%s\n", XSock_Error(&sock));

    XSock_Close(&sock);
    return nStatus;
}
