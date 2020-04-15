/*
 *  examples/statcov.c
 * 
 *  Copyleft (C) 2020  Sun Dro (a.k.a. kala13x)
 *
 * Parse COVID-19 case statistics from https://stopcov.ge/
 */

#include "stdinc.h"
#include "covid.h"
#include "sock.h"
#include "nbg.h"

#ifndef LINE_MAX
#define LINE_MAX 256
#endif

static char *WhiteSpace(int nLength)
{
    static char sRetVal[128];
    memset(sRetVal, 0, sizeof(sRetVal));
    int i, nSpaces = 22 - nLength;

    for (i = 0; i < nSpaces; i++)
        sRetVal[i] = ' ';

    return sRetVal;
}

void PrintStats(COVIDCases *pCovCases, NBGRates *pRates)
{
    char sLine[LINE_MAX];

    printf("========================================\n");
    int nBytes = snprintf(sLine, sizeof(sLine), "Confirmed Cases: %d", pCovCases->nConfirmed);
    printf("%s%s| USD/GEL: %s\n", sLine, WhiteSpace(nBytes), pRates->sUSD2GEL);

    nBytes = snprintf(sLine, sizeof(sLine), "Recovered Cases: %s%d%s", XCLR_GREEN, pCovCases->nRecovered, XCLR_RESET);
    printf("%s%s| EUR/GEL: %s\n", sLine, WhiteSpace(nBytes-(strlen(XCLR_GREEN)+strlen(XCLR_RESET))), pRates->sEUR2GEL);

    nBytes = snprintf(sLine, sizeof(sLine), "Quarantined: %s%d%s", XCLR_YELLOW, pCovCases->nQuarantined, XCLR_RESET);
    printf("%s%s| GBP/GEL: %s\n", sLine, WhiteSpace(nBytes-(strlen(XCLR_YELLOW)+strlen(XCLR_RESET))), pRates->sGBP2GEL);

    nBytes = snprintf(sLine, sizeof(sLine), "Supervision: %s%d%s", XCLR_BLUE, pCovCases->nSupervision, XCLR_RESET);
    printf("%s%s| AZN/GEL: %s\n", sLine, WhiteSpace(nBytes-(strlen(XCLR_BLUE)+strlen(XCLR_RESET))), pRates->sAZN2GEL);

    nBytes = snprintf(sLine, sizeof(sLine), "Deaths: %s%d%s", XCLR_RED, pCovCases->nDeaths, XCLR_RESET);
    printf("%s%s| TRY/GEL: %s\n", sLine, WhiteSpace(nBytes-(strlen(XCLR_RED)+strlen(XCLR_RESET))), pRates->sTRY2GEL);
    printf("========================================\n");
}

int main(int argc, char* argv[])
{
    XSock_InitSSL();
    COVIDCases covCases;
    NBGRates gelRates;

    if (!(COVID_GetCases(&covCases))) return 0;
    if (!(NBG_GetRates(&gelRates))) return 0;

    PrintStats(&covCases, &gelRates);
    XSock_DeinitSSL();
    return 0;
}