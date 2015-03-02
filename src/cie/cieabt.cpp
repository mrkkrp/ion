/***************************************************************************
 *  C++ source
 *
 *  Common Interface Elements (CIE) for Interface of Nature - 'About' Form
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
#include "cieabt.h"

#pragma package(smart_init)
#pragma resource "*.dfm"

TCieAbtForm *CieAbtForm;

void __fastcall TCieAbtForm::FormKeyPress(TObject *Sender, System::WideChar &Key)
{
    ModalResult = mrCancel, Hide();
}

void __fastcall TCieAbtForm::FormMouseDown
    (TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y)
{
    long SC_DRAGMOVE = 0xf012;
    if(Button == mbLeft)
    {
        ReleaseCapture();
        SendMessage(Handle, WM_SYSCOMMAND, SC_DRAGMOVE, 0);
    }
}

__fastcall TCieAbtForm::TCieAbtForm(TComponent* Owner) : TForm(Owner)
{ }

void __fastcall TCieAbtForm::SetTitle(UnicodeString TitleArg)
{
    Title->Caption = TitleArg;
}

void __fastcall TCieAbtForm::SetDate(UnicodeString DateArg)
{
    Date->Caption = DateArg;
}

void __fastcall TCieAbtForm::SetVersion(UnicodeString VersionArg)
{
    Version->Caption = VersionArg;
}

void __fastcall TCieAbtForm::SetDescription(UnicodeString DescriptionArg)
{
    Description->Caption = DescriptionArg;
}

void __fastcall TCieAbtForm::SetCopyright(UnicodeString CopyrightArg)
{
    Copyright->Caption = CopyrightArg;
}

void __fastcall TCieAbtForm::Invoke()
{
    ShowModal();
}
