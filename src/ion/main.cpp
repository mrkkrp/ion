/***************************************************************************
 *  C++ source
 *
 *  ION Launcher
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
#include "main.h"

#pragma package(smart_init)
#pragma resource "*.dfm"

const UnicodeString Apps[AppsNumber] =
{ "scr.exe ", "prl.exe ", "snr.exe ", "wm.exe ", "gnr.exe " }; // Apps

const UnicodeString Exts[AppsNumber] =
  { "ionscr", "prl", "ionsnr", "wav", "iongnr" }; // Exts

TMainForm *MainForm;

void __fastcall TMainForm::Highlight(TObject *Sender, TColor ColorArg)
{
    TGraphicControl *aControl = dynamic_cast<TGraphicControl *>(Sender);
    Canvas->Pen->Color = ColorArg;
    Canvas->Brush->Style = bsClear;
    Canvas->Rectangle(aControl->Left - 1,
                      aControl->Top - 1,
                      aControl->Left + aControl->Width + 1,
                      aControl->Top + aControl->Height + 1);
}

UnicodeString TMainForm::AppByExtension(UnicodeString ExtensionArg)
{
    for (int i = 0; i < AppsNumber; i++)
    {
        if (Exts[i] == ExtensionArg) return Apps[i];
    }
    return "";
}

__fastcall TMainForm::TMainForm(TComponent* Owner) : TForm(Owner)
{ }

void __fastcall TMainForm::FormMouseDown
    (TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y)
{
    long SC_DRAGMOVE = 0xf012;
    if(Button == mbLeft)
    {
        ReleaseCapture();
        SendMessage(Handle, WM_SYSCOMMAND, SC_DRAGMOVE, 0);
    }
}

void __fastcall TMainForm::ScoreImageClick(TObject *Sender)
{
    WinExec("scr.exe", SW_RESTORE);
}

void __fastcall TMainForm::WaveImageClick(TObject *Sender)
{
    WinExec("wm.exe", SW_RESTORE);
}

void __fastcall TMainForm::GeneratorImageClick(TObject *Sender)
{
    WinExec("gnr.exe", SW_RESTORE);
}

void __fastcall TMainForm::PrlImageClick(TObject *Sender)
{
    WinExec("prl.exe", SW_RESTORE);
}

void __fastcall TMainForm::SonorityImageClick(TObject *Sender)
{
    WinExec("snr.exe", SW_RESTORE);
}

void __fastcall TMainForm::FormKeyPress(TObject *Sender, System::WideChar &Key)
{
    switch (Key)
    {
        case 's' :
        case 'S' : ScoreImageClick(this); break;
        case 'p' :
        case 'P' : PrlImageClick(this); break;
        case 'n' :
        case 'N' : SonorityImageClick(this); break;
        case 'w' :
        case 'W' : WaveImageClick(this); break;
        case 'g' :
        case 'G' : GeneratorImageClick(this); break;
        case  VK_ESCAPE : ModalResult = mrCancel; Close();
        default : return;
    }
}

void __fastcall TMainForm::ScoreImageMouseEnter(TObject *Sender)
{
    Highlight(Sender, clHighlightText);
}

void __fastcall TMainForm::ScoreImageMouseLeave(TObject *Sender)
{
    Highlight(Sender, clAppWorkSpace);
}

void __fastcall TMainForm::AOpenExecute(TObject *Sender)
{
    if (!OpenDialog->Execute()) return;
    for (int i = 0; i < OpenDialog->Files->Count; i++)
    {
        UnicodeString aFileName = (*OpenDialog->Files)[i];
        int aIndex = aFileName.LastDelimiter('.');
        UnicodeString aExtension = aFileName.SubString
            (aIndex + 1, aFileName.Length() - aIndex);
        AnsiString aCommand = AppByExtension(aExtension)
            + '"' + aFileName + '"';
        WinExec(aCommand.c_str(), SW_RESTORE);
    }
}

void __fastcall TMainForm::AKeepOnTopExecute(TObject *Sender)
{
    AKeepOnTop->Checked = !AKeepOnTop->Checked;
    FormStyle = AKeepOnTop->Checked ? fsStayOnTop : fsNormal;
    TopOff->Visible = !AKeepOnTop->Checked;
    TopOn->Visible = AKeepOnTop->Checked;
}

void __fastcall TMainForm::ACloseExecute(TObject *Sender)
{
    Close();
}
