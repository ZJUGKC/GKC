/*
** Copyright (c) 2020, Xin YUAN, courses of Zhejiang University
** All rights reserved.
**
** This program is free software; you can redistribute it and/or
** modify it under the terms of the 2-Clause BSD License.
**
** Author contact information:
**   yxxinyuan@zju.edu.cn
**
*/

/*
This file contains rdp functions.
*/

////////////////////////////////////////////////////////////////////////////////

#include "PreComp.h"

#include "_GkcCompiler.h"

#include "rdp/base/_RIdTable.h"
#include "rdp/base/_RScanner.h"
#include "rdp/base/_RTdTable.h"
#include "rdp/base/_RParser.h"

#include "rdp/RdDef.h"
#include "rdp/RdScannerTables.h"
#include "rdp/RdScanner.h"
#include "rdp/RdParserTables.h"
#include "rdp/RdParser.h"

////////////////////////////////////////////////////////////////////////////////

// RdScannerTables

void _RdScannerTables_Create(GKC::ShareCom<_IRdScannerTables>& sp, GKC::CallResult& cr) noexcept
{
	_CREATE_COMPONENT_INSTANCE(GKC::RdScannerTables, _IRdScannerTables, sp, cr);
}

// RdScanner

void _RdScanner_Create(GKC::ShareCom<_IRdScanner>& sp, GKC::CallResult& cr) noexcept
{
	_CREATE_COMPONENT_INSTANCE(GKC::RdScanner, _IRdScanner, sp, cr);
}

// RdParserTables

void _RdParserTables_Create(GKC::ShareCom<_IRdParserTables>& sp, GKC::CallResult& cr) noexcept
{
	_CREATE_COMPONENT_INSTANCE(GKC::RdParserTables, _IRdParserTables, sp, cr);
}

// RdParser

void _RdParser_Create(GKC::ShareCom<_IRdParser>& sp, GKC::CallResult& cr) noexcept
{
	_CREATE_COMPONENT_INSTANCE(GKC::RdParser, _IRdParser, sp, cr);
}

////////////////////////////////////////////////////////////////////////////////
