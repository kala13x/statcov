/*
 *  src/version.c
 *
 *  Copyleft (C) Sun Dro (a.k.a. kala13x)
 *
 * Get additional information about project
 */

#include "stdinc.h"
#include "version.h"

const char* StatCov_Version()
{
    static char versionStr[128];
    snprintf(versionStr, sizeof(versionStr), "%d.%d build %d (%s)", 
        STATCOV_VERSION_MAX, STATCOV_VERSION_MIN, STATCOV_BUILD_NUMBER, __DATE__);

    return versionStr;
}

const char* StatCov_VersionShort()
{
    static char versionStr[128];
    snprintf(versionStr, sizeof(versionStr), "%d.%d.%d", 
        STATCOV_VERSION_MAX, STATCOV_VERSION_MIN, STATCOV_BUILD_NUMBER);

    return versionStr;
}
