/***************************************************************************
 *  C++ header
 *
 *  Wave Master for Interface of Nature
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
#include <System.Actions.hpp>
#include <Vcl.ActnList.hpp>
#include <Vcl.ActnMan.hpp>
#include <Vcl.Dialogs.hpp>
#include <Vcl.ImgList.hpp>
#include <Vcl.PlatformDefaultStyleActnCtrls.hpp>
#include <Vcl.ActnCtrls.hpp>
#include <Vcl.ActnMenus.hpp>
#include <Vcl.ButtonGroup.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.Menus.hpp>
#include <Vcl.ToolWin.hpp>
#include <mmsystem.h>
#include <wav.h>

// constants

static const UINT UWM_AREYOUME =
    ::RegisterWindowMessage(L"WM32-2561621b-f00e-458b-9c27-c58e9f05e7ad");

const UnicodeString
iDefaultName ("foo"),
iVersion     ("4.2.0"),
iCaption     ("Wave Master");

// tab-oriented structure

struct TabImage
{
    UnicodeString aFileName;
    ionstd::WaveStruct *aWave;
    bool aModified;
}; // struct TabImage

typedef std::vector<TabImage> TabImagesStore;

class TMainForm : public TForm
{
    __published:    // IDE-managed Components
    TActionManager *ActionManager;
    TOpenDialog *OpenDialog;
    TSaveDialog *SaveDialog;
    TImageList *ImageList;
    TPopupMenu *SelectorMenu;
    TMenuItem *New1;
    TMenuItem *N5;
    TMenuItem *Close1;
    TMenuItem *CloseAll1;
    TActionMainMenuBar *MainMenu;
    TActionToolBar *ToolBar;
    TStatusBar *StatusBar;
    TButtonGroup *Selector;
    TButtonGroup *MainPanel;
    TAction *AOpen;
    TAction *ASave;
    TAction *ASaveAs;
    TAction *ASaveAll;
    TAction *AClose;
    TAction *ACloseAll;
    TAction *AExit;
    TAction *APreview;
    TAction *AStopPreview;
    TAction *AAbout;
    TAction *AGain;
    TAction *ANormalize;
    TAction *ADither;
    void __fastcall FormCloseQuery(TObject *Sender, bool &CanClose);
    void __fastcall FormCreate(TObject *Sender);
    void __fastcall FormResize(TObject *Sender);
    void __fastcall AOpenExecute(TObject *Sender);
    void __fastcall ASaveExecute(TObject *Sender);
    void __fastcall ASaveAsExecute(TObject *Sender);
    void __fastcall ASaveAllExecute(TObject *Sender);
    void __fastcall ACloseExecute(TObject *Sender);
    void __fastcall ACloseAllExecute(TObject *Sender);
    void __fastcall AGainExecute(TObject *Sender);
    void __fastcall ANormalizeExecute(TObject *Sender);
    void __fastcall ADitherExecute(TObject *Sender);
    void __fastcall AExitExecute(TObject *Sender);
    void __fastcall APreviewExecute(TObject *Sender);
    void __fastcall AStopPreviewExecute(TObject *Sender);
    void __fastcall AAboutExecute(TObject *Sender);
    void __fastcall FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
    void __fastcall MainPanelButtonClicked(TObject *Sender, int Index);
    void __fastcall SelectorButtonClicked(TObject *Sender, int Index);

    private:    // User Declarations
    TabImagesStore aTabImages; // all our tabs
    bool aClosingCancel, aSoundPreview;
    unsigned int aSelector;
    // next part really only for preview:
    unsigned int aPRVDevice;
    HWAVEOUT aPRVHandle;
    WAVEHDR aPRVHeader;

    bool *GetModified()
    { return &((aTabImages.begin() + aSelector)->aModified); }
    const bool *GetModified() const
    { return &((aTabImages.begin() + aSelector)->aModified); }
    UnicodeString *GetFileName()
    { return &((aTabImages.begin() + aSelector)->aFileName); }
    const UnicodeString *GetFileName() const
    { return &((aTabImages.begin() + aSelector)->aFileName); }
    ionstd::WaveStruct *GetWave()
    { return (aTabImages.begin() + aSelector)->aWave; }
    const ionstd::WaveStruct *GetWave() const
    { return (aTabImages.begin() + aSelector)->aWave; }

    UnicodeString __fastcall MakeDescription(unsigned int);
    void __fastcall EditProperty(unsigned int);

    void __fastcall OnOpenOther(TWMCopyData &);
    void __fastcall WndProc(TMessage &);

    UnicodeString __fastcall GetLastName(UnicodeString) const;
    UnicodeString __fastcall GetFormCaption() const;

    void __fastcall Refresh();
    void __fastcall eClose();
    void __fastcall eOpen(UnicodeString);
    void __fastcall eSave(UnicodeString);
    void __fastcall EnableControls(bool);

    public:     // User Declarations
    __fastcall TMainForm(TComponent* Owner);

    BEGIN_MESSAGE_MAP
        MESSAGE_HANDLER(WM_COPYDATA, TWMCopyData, OnOpenOther);
    END_MESSAGE_MAP(TComponent)

}; // class TMainForm

extern PACKAGE TMainForm *MainForm;

#endif // MAINH
