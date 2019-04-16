/***************************************************//**
 * @file    DllDecl .h
 * @date    September 2012
 * @author  Ocean Optics, Inc.
 *
 * Copyright (C) 2012 Ocean Optics, Inc.
 * All rights reserved.
 *
 * This class provides the DLL import/export decorations
 * required for Windows DLLs.  It is encapsulated to
 * reduce copy-paste and to allow a single point of
 * maintenance.
 ******************************************************/

#ifndef SEABREEZE_DLL_DECL_H
#define SEABREEZE_DLL_DECL_H

#ifdef _WINDOWS
    #ifdef BUILD_DLL
        #define DLL_DECL __declspec(dllexport)
    #else
        #define DLL_DECL __declspec(dllimport)
    #endif
#else
    #define DLL_DECL
#endif

#endif
