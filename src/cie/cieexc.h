/***************************************************************************
 *  C++ header
 *
 *  Common Interface Elements (CIE) for Interface of Nature -
 *  Exception Form and Macro
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

#ifndef CIEEXCH
#define CIEEXCH

#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Graphics.hpp>
#include <prl.h>

// definitions for exceptions processing

#define ESTART try {
#define EEND } catch(std::exception &Arg) \
{ CieExcForm->Invoke(Arg.what()); } \
catch(...) \
{ CieExcForm->Invoke("some shit happened!"); }

// prototypes for string-adapt functions

ionstd::String PACKAGE Adapt(UnicodeString);
UnicodeString PACKAGE Adapt(std::string);
ionstd::String PACKAGE MergePath(ionstd::String);

class PACKAGE TCieExcForm : public TForm
{
    __published:    // IDE-managed Components
    TImage *HornedToad;
    TLabel *ExceptionLabel;
    void __fastcall FormKeyPress(TObject *Sender, System::WideChar &Key);
    void __fastcall FormMouseDown(TObject *Sender, TMouseButton Button,
                                  TShiftState Shift, int X, int Y);

    public:     // User Declarations
    __fastcall TCieExcForm(TComponent* Owner);
    void __fastcall Invoke(UnicodeString);

}; // class PACKAGE TCieExcForm

extern PACKAGE TCieExcForm *CieExcForm;

#endif // CIEEXCH
