/*
 *  src/nbg.h
 * 
 *  Copyleft (C) 2020  Sun Dro (a.k.a. kala13x)
 *
 * Get and parse GEL exchange rate from https://nbg.gov.ge/
 */

#ifndef __NBG_GOV_H__
#define __NBG_GOV_H__

#ifdef __cplusplus
extern "C" {
#endif

#define XRATE_SIZE 16

typedef struct {
    char sUSD2GEL[XRATE_SIZE];
    char sEUR2GEL[XRATE_SIZE];
    char sGBP2GEL[XRATE_SIZE];
    char sAZN2GEL[XRATE_SIZE];
    char sTRY2GEL[XRATE_SIZE];
} NBGRates;

int NBG_GetRates(NBGRates *pRates);

#ifdef __cplusplus
}
#endif

#endif /* __NBG_GOV_H__ */