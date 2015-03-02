/***************************************************************************
 *  C++ source
 *
 *  Common Interface Elements (CIE) for Interface of Nature -
 *  Alternative Progress Bar
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
#include "cieprg.h"

#pragma package(smart_init)
#pragma resource "*.dfm"

TCiePrgForm *CiePrgForm;

void __fastcall TCiePrgForm::UpdateBar(double ProgressArg)
{
    ProgressLabel->Caption = IntToStr((int)ProgressArg) + '%';
    ProgressLabel->Left = PB->Left
        + (PB->Width - ProgressLabel->Width) / 2;
    PB->Canvas->Pen->Color = clrBorder;
    PB->Canvas->Brush->Color = clrBack;
    PB->Canvas->Rectangle(0, 0, PB->Width, PB->Height);
    PB->Canvas->Brush->Color = clrFill;
    PB->Canvas->Rectangle(0, 0, PB->Width * ProgressArg / 100, PB->Height);
}

__fastcall TCiePrgForm::TCiePrgForm(TComponent* Owner) : TForm(Owner)
{ }

void __fastcall TCiePrgForm::TimerTimer(TObject *Sender)
{
    double aProgress = aPF(); // progress in percents
    UpdateBar(aProgress);
    if (aProgress >= 100) // after completion
    {
        Timer->Enabled = false;
        aAF();
        ModalResult = mrOk;
        Hide();
    }
}

void __fastcall TCiePrgForm::Invoke
    (UnicodeString CaptionArg, ProgressFnc PFArg, AfterFnc AFArg)
{
    DescriptionLabel->Caption = CaptionArg;
    aPF = PFArg;
    aAF = AFArg;
    UpdateBar(0);
    Timer->Enabled = true;
    ShowModal();
}

void __fastcall TCiePrgForm::FormMouseDown
    (TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y)
{
    long SC_DRAGMOVE = 0xf012;
    if(Button == mbLeft)
    {
        ReleaseCapture();
        SendMessage(Handle, WM_SYSCOMMAND, SC_DRAGMOVE, 0);
    }
}
