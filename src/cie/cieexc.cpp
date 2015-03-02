/***************************************************************************
 *  C++ source
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

#include <vcl.h>
#pragma hdrstop
#include "cieexc.h"

#pragma package(smart_init)
#pragma resource "*.dfm"

TCieExcForm *CieExcForm;

ionstd::String Adapt(UnicodeString TargetArg)
{
    AnsiString aString = TargetArg;
    return ionstd::String(aString.c_str());
}

UnicodeString Adapt(std::string TargetArg)
{
    UnicodeString aResult(TargetArg.c_str());
    return aResult;
}

ionstd::String MergePath(ionstd::String PathArg)
{
    ionstd::String aPath = Adapt(ParamStr(0));
    ionstd::numerator aPos = aPath.rfind("Interface of Nature");
    if (aPos == ionstd::String::npos) aPos = aPath.length();
    else aPos += 20;
    aPath.erase(aPos, aPath.length() - aPos);
    aPath += PathArg;
    return aPath;
}

__fastcall TCieExcForm::TCieExcForm(TComponent* Owner)
    : TForm(Owner)
{ }

void __fastcall TCieExcForm::FormKeyPress(TObject *Sender, System::WideChar &Key)
{
    ModalResult = mrOk, Hide();
}

void __fastcall TCieExcForm::Invoke(UnicodeString TextArg)
{
    ExceptionLabel->Caption = TextArg;
    ShowModal();
}

void __fastcall TCieExcForm::FormMouseDown
    (TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y)
{
    long SC_DRAGMOVE = 0xf012;
    if(Button == mbLeft)
    {
        ReleaseCapture();
        SendMessage(Handle, WM_SYSCOMMAND, SC_DRAGMOVE, 0);
    }
}
