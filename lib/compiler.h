/*
 * Copyright (c) 2011 Makarov Alexey
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the author nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 */

/* A macros set for compiler C extensions safe use */

#ifndef COMPILER_DEPENDENT_H
#define COMPILER_DEPENDENT_H

/*
 * Macroses:
 *
 * inline                   might be redefined under some conditions
 * likely(cond)             cond is likely to be true
 * unlikely(cond)           cond is unlikely to be true
 * C_ATTR_NORETURN          function cannot return
 * C_ATTR_CONST             function has no side effects, and return value
 *                          depends only on the parameters             
 * C_ATTR_PRINTF(fmt, arg)  function fmt-th parameter is printf-style format string,
 *                          and arg-th parameter is first argument to be checked
 * C_ATTR_MALLOC            any non-null pointer function returns cannot alias any
 *                          other pointer valid when the function returns
 * C_ATTR_PURE              function has no side effects, and return value depends
 *                          only on the parameters and/or global variables
 * C_ATTR_DEPRECATED(msg)   this results in warning if the function is used
 */

#ifdef __GNUC__
#define GNUC_ATLEAST(maj, min) \
	((__GNUC__ > maj) || (__GNUC__ == maj && __GNUC_MINOR__ >= min))
#else
#define GNUC_ATLEAST(maj, min)  0
#endif

#ifdef __STRICT_ANSI__
#ifdef __GNUC__
#define inline                     __inline__
#else
#define inline
#endif
#endif

#if GNUC_ATLEAST(2, 96)
#define likely(cond)               __builtin_expect(!!(cond), 1)
#define unlikely(cond)             __builtin_expect(!!(cond), 0)
#else
#define likely(x)                  (x)
#define unlikely(x)                (x)
#endif

#if GNUC_ATLEAST(2, 5)
#define C_ATTR_NORETURN            __attribute__((noreturn))
#define C_ATTR_CONST               __attribute__((const))
#define C_ATTR_PRINTF(fmt, arg)    __attribute__((format (printf, fmt, arg)))
#else
#define C_ATTR_NORETURN
#define C_ATTR_CONST
#define C_ATTR_PRINTF(fmt, arg)
#endif

#if GNUC_ATLEAST(2, 96)
#define C_ATTR_MALLOC              __attribute__((malloc))
#define C_ATTR_PURE                __attribute__((pure))
#else
#define C_ATTR_MALLOC
#define C_ATTR_PURE
#endif

#if GNUC_ATLEAST(3, 1)
#define C_ATTR_DEPRECATED(msg)     __attribute__((deprecated))
#else
#define C_ATTR_DEPRECATED(msg)
#endif

#endif

