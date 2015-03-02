/***************************************************************************
 *  C++ header
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

#ifndef MAINH
#define MAINH

#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Graphics.hpp>
#include <Vcl.ImgList.hpp>
#include <Vcl.AppEvnts.hpp>
#include <System.Actions.hpp>
#include <Vcl.ActnList.hpp>
#include <Vcl.ActnMan.hpp>
#include <Vcl.PlatformDefaultStyleActnCtrls.hpp>
#include <Vcl.Dialogs.hpp>

const AppsNumber (5);

class TMainForm : public TForm
{
    __published:    // IDE-managed Components
    TImage *ScoreImage;
    TImage *SonorityImage;
    TImage *GeneratorImage;
    TImage *PrlImage;
    TImage *WaveImage;
    TLabel *IONLabel;
    TLabel *ScoreLabel;
    TLabel *PrlLabel;
    TLabel *SonorityLabel;
    TLabel *WaveLabel;
    TLabel *GeneratorLabel;
    TActionManager *ActionManager;
    TAction *AOpen;
    TOpenDialog *OpenDialog;
    TImage *TopOff;
    TImage *TopOn;
    TAction *AKeepOnTop;
    TAction *AClose;
    void __fastcall FormMouseDown
    (TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall ScoreImageClick(TObject *Sender);
    void __fastcall WaveImageClick(TObject *Sender);
    void __fastcall GeneratorImageClick(TObject *Sender);
    void __fastcall PrlImageClick(TObject *Sender);
    void __fastcall SonorityImageClick(TObject *Sender);
    void __fastcall FormKeyPress(TObject *Sender, System::WideChar &Key);
    void __fastcall ScoreImageMouseEnter(TObject *Sender);
    void __fastcall ScoreImageMouseLeave(TObject *Sender);
    void __fastcall AOpenExecute(TObject *Sender);
    void __fastcall AKeepOnTopExecute(TObject *Sender);
    void __fastcall ACloseExecute(TObject *Sender);

    public:     // User Declarations
    void __fastcall Highlight(TObject *, TColor);
        UnicodeString AppByExtension(UnicodeString);

    public:     // User Declarations
    __fastcall TMainForm(TComponent* Owner);

}; // class TLauncherForm

extern PACKAGE TMainForm *MainForm;

#endif
