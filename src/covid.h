/*
 *  src/covid.h
 * 
 *  Copyleft (C) 2020  Sun Dro (a.k.a. kala13x)
 *
 * Get and parse COVID-19 case statistics from https://stopcov.ge/
 */

#ifndef __COVID_H__
#define __COVID_H__

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    int nQuarantined;
    int nSupervision;
    int nConfirmed;
    int nRecovered;
    int nDeaths;
} COVIDCases;

int COVID_GetCases(COVIDCases *pCovCases);

#ifdef __cplusplus
}
#endif

#endif /* __COVID_H__ */