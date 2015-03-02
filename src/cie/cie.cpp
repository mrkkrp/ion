/***************************************************************************
 *  C++ source
 *
 *  Common Interface Elements (CIE) for Interface of Nature
 *
 *  Version 4.2.0
 *
 *  Copyright (c) 2014 Mark Karpov
 *
 *  This program is free software: you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License as
 *  published by the Free Software Foundation, either version 3 of the
 *  License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 *  General Public License for more details.
 **************************************************************************/

#include <System.hpp>
#pragma hdrstop

USEFORM("cieslc.cpp", CieSlcForm);
USEFORM("cieexc.cpp", CieExcForm);
USEFORM("cieabt.cpp", CieAbtForm);
USEFORM("ciearg.cpp", CieArgForm);
USEFORM("ciecnf.cpp", CieCnfForm);
USEFORM("ciechs.cpp", CieChsForm);
USEFORM("ciefnc.cpp", CieFncForm);
USEFORM("cieprg.cpp", CiePrgForm);

#pragma package(smart_init)
#pragma argsused

extern "C" int _libmain(unsigned long reason)
{
	return 1;
}
