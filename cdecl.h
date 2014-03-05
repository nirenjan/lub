/**********************************************************************
 * C Wrapper for include files
 **********************************************************************
 * Copyright (C) 2013 Nirenjan Krishnan (nirenjan@gmail.com)
 *
 * When building an application that has both C and C++ source files,
 * it is standard practice to wrap the declaration in extern "C" { },
 * but only if __cplusplus is defined. Rather than having to create the
 * #ifdef conditions everytime, this file provides a simple wrapper.
 *
 * Just include this file, and instead of the checks, just use
 * __CDECL_BEGIN and __CDECL_END
 *********************************************************************/

#ifndef __CDECL_H
#define __CDECL_H

#ifdef __cplusplus
#define __CDECL_BEGIN   extern "C" {
#define __CDECL_END     }
#else
#define __CDECL_BEGIN
#define __CDECL_END
#endif

#endif /* !defined __CDECL_H */
