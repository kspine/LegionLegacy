/*
    This file is a part of libcds - Concurrent Data Structures library

    (C) Copyright Maxim Khizhinsky (libcds.dev@gmail.com) 2006-2017

    Source code repo: http://github.com/khizmax/libcds/
    Download: http://sourceforge.net/projects/libcds/files/

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are met:

    * Redistributions of source code must retain the above copyright notice, this
      list of conditions and the following disclaimer.

    * Redistributions in binary form must reproduce the above copyright notice,
      this list of conditions and the following disclaimer in the documentation
      and/or other materials provided with the distribution.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
    AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
    IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
    DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
    FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
    DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
    SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
    CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
    OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
    OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef CDSLIB_COMPILER_CLANG_DEFS_H
#define CDSLIB_COMPILER_CLANG_DEFS_H

// Compiler version
#define CDS_COMPILER_VERSION (__clang_major__ * 10000 + __clang_minor__ * 100 + __clang_patchlevel__)

// Compiler name
#define  CDS_COMPILER__NAME    ("clang " __clang_version__)
#define  CDS_COMPILER__NICK    "clang"

#if CDS_COMPILER_VERSION < 30600
#   error "Compiler version error. Clang version 3.6.0 and above is supported"
#endif

#if defined(_LIBCPP_VERSION) && !defined(CDS_USE_BOOST_ATOMIC) && CDS_COMPILER_VERSION < 30700
    // Note: Clang libc++ atomic leads to program crash.
    // So, we use libcds atomic implementation
#   define CDS_USE_LIBCDS_ATOMIC
#endif

// clang for Windows
#if defined( _MSC_VER )
#   define CDS_OS_INTERFACE     CDS_OSI_WINDOWS
#   if defined(_WIN64)
#       define CDS_OS_TYPE      CDS_OS_WIN64
#       define CDS_OS__NAME     "Win64"
#       define CDS_OS__NICK     "Win64"
#   elif defined(_WIN32)
#       define CDS_OS_TYPE      CDS_OS_WIN32
#       define CDS_OS__NAME     "Win32"
#       define CDS_OS__NICK     "Win32"
#   endif
#endif

#include <cds/compiler/gcc/compiler_macro.h>

#define alignof __alignof__

// C++11 inline namespace
#define CDS_CXX11_INLINE_NAMESPACE_SUPPORT

#define CDS_CONSTEXPR    constexpr

#define CDS_NOEXCEPT_SUPPORT        noexcept
#define CDS_NOEXCEPT_SUPPORT_(expr) noexcept(expr)

// C++11 thread_local keyword
#if !(CDS_OS_TYPE == CDS_OS_OSX && CDS_COMPILER_VERSION < 30600)
    // OS X error?
    // See http://stackoverflow.com/questions/23791060/c-thread-local-storage-clang-503-0-40-mac-osx
    // http://stackoverflow.com/questions/28094794/why-does-apple-clang-disallow-c11-thread-local-when-official-clang-supports
    // clang 3.6 ok?..
#   define CDS_CXX11_THREAD_LOCAL_SUPPORT
#endif

// Full SFINAE support
#define CDS_CXX11_SFINAE

// Inheriting constructors
#define CDS_CXX11_INHERITING_CTOR

// Attributes
#if CDS_COMPILER_VERSION >= 30400
#   if __cplusplus < 201103
#       define CDS_DEPRECATED( reason ) __attribute__((deprecated( reason )))
#   elif __cplusplus == 201103 // C++11
#       define CDS_DEPRECATED( reason ) [[gnu::deprecated(reason)]]
#   else  // C++14
#       define CDS_DEPRECATED( reason ) [[deprecated(reason)]]
#   endif
#else
#   define CDS_DEPRECATED( reason ) __attribute__((deprecated( reason )))
#endif

#define CDS_NORETURN __attribute__((__noreturn__))

// *************************************************
// Features
#if defined(__has_feature) && __has_feature(thread_sanitizer)
#   ifndef CDS_THREAD_SANITIZER_ENABLED
#       define CDS_THREAD_SANITIZER_ENABLED
#   endif
#endif

#if defined(__has_feature) && __has_feature(address_sanitizer)
#   ifndef CDS_ADDRESS_SANITIZER_ENABLED
#       define CDS_ADDRESS_SANITIZER_ENABLED
#   endif
#endif


// *************************************************
// Alignment macro

#define CDS_TYPE_ALIGNMENT(n)   __attribute__ ((aligned (n)))
#define CDS_CLASS_ALIGNMENT(n)  __attribute__ ((aligned (n)))
#define CDS_DATA_ALIGNMENT(n)   __attribute__ ((aligned (n)))


// likely/unlikely

#define cds_likely( expr )   __builtin_expect( !!( expr ), 1 )
#define cds_unlikely( expr ) __builtin_expect( !!( expr ), 0 )

// Exceptions

#if defined( __EXCEPTIONS ) && __EXCEPTIONS == 1
#   define CDS_EXCEPTION_ENABLED
#endif


// double-width CAS support - only for libc++
// You can manually suppress wide-atomic support by defining in compiler command line:
//  for 64bit platform: -DCDS_DISABLE_128BIT_ATOMIC
//  for 32bit platform: -DCDS_DISABLE_64BIT_ATOMIC
#ifdef _LIBCPP_VERSION
#   if CDS_BUILD_BITS == 64
#       if !defined( CDS_DISABLE_128BIT_ATOMIC ) && defined( __GCC_HAVE_SYNC_COMPARE_AND_SWAP_16 )
#           define CDS_DCAS_SUPPORT
#       endif
#   else
#       if !defined( CDS_DISABLE_64BIT_ATOMIC ) && defined( __GCC_HAVE_SYNC_COMPARE_AND_SWAP_8 )
#           define CDS_DCAS_SUPPORT
#       endif
#   endif
#endif

#include <cds/compiler/gcc/compiler_barriers.h>

#endif // #ifndef CDSLIB_COMPILER_GCC_DEFS_H
