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

/* Some useful macro */

#ifndef USEFUL_MACROS_H
#define USEFUL_MACROS_H

#define _C_STRINGIFY(x)   #x
#define C_STRINGIFY(x)    _C_STRINGIFY(x)

#define C_FILE_POS        __FILE__ ":" C_STRINGIFY(__LINE__)

#define C_STMT_BEGIN      do
#define C_STMT_END        while(/*CONSTCOND*/0)

#ifdef  __cplusplus
#define C_BEGIN_DECLS     extern "C" {
#define C_END_DECLS       }
#else
#define C_BEGIN_DECLS
#define C_END_DECLS
#endif

/* Side effects! */
#define MAX(a, b) \
		((a) > (b) ? (a) : (b))
#define MIN(a, b) \
		((a) < (b) ? (a) : (b))

#define C_STATIC_ASSERT(name, cond) \
		extern char name[(cond) ? 1 : -1]

#endif

