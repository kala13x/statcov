/*
 *  src/version.h
 *
 *  Copyleft (C) Sun Dro (a.k.a. kala13x)
 *
 * Get additional information about project
 */

#ifndef __STATCOV_VERSION_H__
#define __STATCOV_VERSION_H__

#define STATCOV_VERSION_MAX     1
#define STATCOV_VERSION_MIN     0
#define STATCOV_BUILD_NUMBER    3

#ifdef __cplusplus
extern "C" {
#endif

const char* StatCov_Version();
const char* StatCov_VersionShort();

#ifdef __cplusplus
}
#endif

#endif /* __STATCOV_VERSION_H__ */