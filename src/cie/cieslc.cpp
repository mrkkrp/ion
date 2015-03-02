/***************************************************************************
 *  C++ source
 *
 *  Common Interface Elements (CIE) for Interface of Nature -
 *  Selection Form
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
#include "cieslc.h"

#pragma package(smart_init)
#pragma resource "*.dfm"

TCieSlcForm *CieSlcForm;

__fastcall TCieSlcForm::TCieSlcForm(TComponent* Owner) : TForm(Owner)
{ }

void __fastcall TCieSlcForm::FormKeyPress
    (TObject *Sender, System::WideChar &Key)
{
    if (Key == VK_RETURN) ModalResult = mrOk, Hide();
    if (Key == VK_ESCAPE) ModalResult = mrCancel, Hide();
}

void __fastcall TCieSlcForm::FormShow(TObject *Sender)
{
    for (unsigned int i = 0; i < aCount; i++)
    {
        TCheckBox *aBox = new TCheckBox(CieSlcForm);
        aBox->Parent = CieSlcForm;
        aBox->Top = 27;
        aBox->Left = 8 + 20 * i;
        aBox->Height = 16;
        aBox->Width = 16;
        aBox->Alignment = taRightJustify;
        aBox->Caption = UnicodeString();
        aBox->Hint = UIntToStr(i);
        aBox->OnKeyPress = FormKeyPress;
        aBox->Checked = (aMask >> i) & 1;
        *(aBoxes + i) = aBox;
    }
}

void __fastcall TCieSlcForm::Invoke
    (UnicodeString CaptionArg, unsigned int MaskArg, unsigned int CountArg)
{
    DescriptionLabel->Caption = CaptionArg;
    aMask = MaskArg;
    if (CountArg > MaxSlotsSlc) CountArg = MaxSlotsSlc;
    aCount = CountArg;
    ShowModal();
}

unsigned int __fastcall TCieSlcForm::GetMask() const
{
    return aMask;
}

void __fastcall TCieSlcForm::FormHide(TObject *Sender)
{
    aMask = 0;
    for (unsigned int i = 0; i < aCount; i++)
    {
        aMask |= (unsigned int)((**(aBoxes + i)).Checked) << i;
    }
    for (unsigned int i = 0; i < aCount; i++)
    {
        delete *(aBoxes + i);
    }
}

void __fastcall TCieSlcForm::FormMouseDown
    (TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y)
{
    long SC_DRAGMOVE = 0xf012;
    if(Button == mbLeft)
    {
        ReleaseCapture();
        SendMessage(Handle, WM_SYSCOMMAND, SC_DRAGMOVE, 0);
    }
}
