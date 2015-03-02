/***************************************************************************
 *  C++ source
 *
 *  Common Interface Elements (CIE) for Interface of Nature -
 *  Confirmation Form
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
#include "ciecnf.h"

#pragma package(smart_init)
#pragma resource "*.dfm"

TCieCnfForm *CieCnfForm;

__fastcall TCieCnfForm::TCieCnfForm(TComponent* Owner) : TForm(Owner)
{ }

void __fastcall TCieCnfForm::FormKeyPress
    (TObject *Sender, System::WideChar &Key)
{
    switch (Key)
    {
        case 'y' :
        case 'Y' : SaveImageClick(this); break;
        case 'n' :
        case 'N' : DiscardImageClick(this); break;
        case  VK_ESCAPE : ModalResult = mrCancel; Hide();
        default : return;
    }
}

void __fastcall TCieCnfForm::SaveImageClick(TObject *Sender)
{
    ModalResult = mrYes;
    Hide();
}

void __fastcall TCieCnfForm::DiscardImageClick(TObject *Sender)
{
    ModalResult = mrNo;
    Hide();
}

void __fastcall TCieCnfForm::Invoke(UnicodeString FileNameArg)
{
    FileNameLabel->Caption = FileNameArg;
    ShowModal();
}
void __fastcall TCieCnfForm::FormMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y)
{
    long SC_DRAGMOVE = 0xf012;
    if(Button == mbLeft)
    {
        ReleaseCapture();
        SendMessage(Handle, WM_SYSCOMMAND, SC_DRAGMOVE, 0);
    }
}

void __fastcall TCieCnfForm::Highlight(TObject *Sender, TColor ColorArg)
{
    TGraphicControl *aControl = dynamic_cast<TGraphicControl *>(Sender);
    Canvas->Pen->Color = ColorArg;
    Canvas->Brush->Style = bsClear;
    Canvas->Rectangle(aControl->Left - 1,
                      aControl->Top - 1,
                      aControl->Left + aControl->Width + 1,
                      aControl->Top + aControl->Height + 1);
}

void __fastcall TCieCnfForm::SaveImageMouseEnter(TObject *Sender)
{
    Highlight(Sender, clHighlightText);
}

void __fastcall TCieCnfForm::SaveImageMouseLeave(TObject *Sender)
{
    Highlight(Sender, clAppWorkSpace);
}

void __fastcall TCieCnfForm::DiscardImageMouseEnter(TObject *Sender)
{
    Highlight(Sender, clHighlightText);
}

void __fastcall TCieCnfForm::DiscardImageMouseLeave(TObject *Sender)
{
    Highlight(Sender, clAppWorkSpace);
}
