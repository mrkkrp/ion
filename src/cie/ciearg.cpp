/***************************************************************************
 *  C++ source
 *
 *  Common Interface Elements (CIE) for Interface of Nature - Standard
 *  Argument Form
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
#include "ciearg.h"

#pragma package(smart_init)
#pragma resource "*.dfm"

TCieArgForm *CieArgForm;

void __fastcall TCieArgForm::FieldKeyPress
    (TObject *Sender, System::WideChar &Key)
{
    if (Key == VK_RETURN) ModalResult = mrOk, Hide();
    if (Key == VK_ESCAPE) ModalResult = mrCancel, Hide();
}

void __fastcall TCieArgForm::FormShow(TObject *Sender)
{
    Field->SetFocus();
    Field->SelectAll();
}

void __fastcall TCieArgForm::FormMouseDown
    (TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y)
{
    long SC_DRAGMOVE = 0xf012;
    if(Button == mbLeft)
    {
        ReleaseCapture();
        SendMessage(Handle, WM_SYSCOMMAND, SC_DRAGMOVE, 0);
    }
}

__fastcall TCieArgForm::TCieArgForm(TComponent* Owner) : TForm(Owner)
{ }

void __fastcall TCieArgForm::Invoke(UnicodeString CaptionArg, bool ValueArg)
{
    Invoke(CaptionArg, UnicodeString(ValueArg ? "true" : "false"));
}

void __fastcall TCieArgForm::Invoke(UnicodeString CaptionArg, int ValueArg)
{
    Invoke(CaptionArg, IntToStr(ValueArg));
}

void __fastcall TCieArgForm::Invoke(UnicodeString CaptionArg, unsigned int ValueArg)
{
    Invoke(CaptionArg, UIntToStr(ValueArg));
}

void __fastcall TCieArgForm::Invoke(UnicodeString CaptionArg, double ValueArg)
{
    Invoke(CaptionArg, FloatToStr(ValueArg));
}

void __fastcall TCieArgForm::Invoke
    (UnicodeString CaptionArg, UnicodeString ValueArg)
{
    DescriptionLabel->Caption = CaptionArg;
    Field->Text = ValueArg;
    ShowModal();
}

bool __fastcall TCieArgForm::ResultAsBool() const
{
        String Temp = Field->Text.LowerCase();
    if (Temp == "false") return false;
    if (Temp == "0") return false;
    return true;
}

int __fastcall TCieArgForm::ResultAsInt() const
{
    return StrToInt(Field->Text);
}

double __fastcall TCieArgForm::ResultAsDouble() const
{
    return StrToFloat(Field->Text);
}

UnicodeString __fastcall TCieArgForm::ResultAsString() const
{
    return Field->Text;
}
