/***************************************************************************
 *  C++ header
 *
 *  Parameter Lists Editor
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
#include <Vcl.ActnCtrls.hpp>
#include <Vcl.ActnList.hpp>
#include <Vcl.ActnMan.hpp>
#include <Vcl.ActnMenus.hpp>
#include <Vcl.ActnPopup.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.Dialogs.hpp>
#include <Vcl.ExtDlgs.hpp>
#include <Vcl.ImgList.hpp>
#include <Vcl.Menus.hpp>
#include <Vcl.PlatformDefaultStyleActnCtrls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.ToolWin.hpp>
#include <Vcl.Tabs.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.DockTabSet.hpp>
#include <System.Actions.hpp>
#include <Vcl.ButtonGroup.hpp>
#include <prl.h>

// constants

static const UINT UWM_AREYOUME =
    ::RegisterWindowMessage(L"PRL32-60394a0d-022f-473f-a18e-9fe2130e8427");

const UnicodeString
iDefaultName ("foo"),
iVersion     ("4.2.0"),
iCaption     ("Parameter Lists Editor");

// tab-oriented structure

enum SimplifiedType { stBool, stUInt, stDouble, stString };

struct TabImage
{
    UnicodeString aFileName;
    ionstd::Paramlist aParamlist;
    int aScrollPos;
    bool aModified;
}; // struct TabImage

typedef std::vector<TabImage>               TabImageStore;

struct PairSelection
{
    bool aMapSelected, aCancelled;
    ionstd::String aMapKey, aPairKey, aArgument;
    SimplifiedType aType;
}; // struct PairSelection

class TMainForm : public TForm
{
    __published:    // IDE-managed Components
    TStatusBar *StatusBar;
    TActionToolBar *ToolBar;
    TPopupActionBar *PopupMenu;
    TImageList *ImageList;
    TOpenDialog *OpenDialog;
    TSaveDialog *SaveDialog;
    TActionMainMenuBar *MainMenu;
    TActionManager *ActionManager;
    TAction *ANew;
    TAction *AOpen;
    TAction *ASave;
    TAction *ASaveAs;
    TAction *AProperties;
    TAction *AClose;
    TAction *AExit;
    TAction *AChangeKey;
    TAction *AAddReplace;
    TAction *ARemove;
    TAction *AAbout;
    TMenuItem *ChangeId1;
    TMenuItem *N1;
    TMenuItem *AddReplace1;
    TMenuItem *Remove1;
    TMenuItem *N2;
    TAction *ADuplicate;
    TAction *ASendPair;
    TAction *ACloseAll;
    TAction *ASaveAll;
    TMenuItem *Duplicate1;
    TMenuItem *SendPair1;
    TPopupMenu *SelectorMenu;
    TMenuItem *New1;
    TMenuItem *Close1;
    TMenuItem *CloseAll1;
    TMenuItem *N4;
    TMenuItem *Open1;
    TMenuItem *N5;
    TAction *ATextSelectAll;
    TAction *ATextCopy;
    TAction *AImportList;
    TRichEdit *MainText;
    TButtonGroup *Selector;
    TImageList *DataType;
    TImageList *TargetType;
    void __fastcall AAboutExecute(TObject *Sender);
    void __fastcall FormResize(TObject *Sender);
    void __fastcall APropertiesExecute(TObject *Sender);
    void __fastcall ANewExecute(TObject *Sender);
    void __fastcall AOpenExecute(TObject *Sender);
    void __fastcall ASaveExecute(TObject *Sender);
    void __fastcall ASaveAsExecute(TObject *Sender);
    void __fastcall ACloseExecute(TObject *Sender);
    void __fastcall AExitExecute(TObject *Sender);
    void __fastcall AChangeKeyExecute(TObject *Sender);
    void __fastcall AAddReplaceExecute(TObject *Sender);
    void __fastcall ARemoveExecute(TObject *Sender);
    void __fastcall FormCreate(TObject *Sender);
    void __fastcall FormCloseQuery(TObject *Sender, bool &CanClose);
    void __fastcall ADuplicateExecute(TObject *Sender);
    void __fastcall ASendPairExecute(TObject *Sender);
    void __fastcall ACloseAllExecute(TObject *Sender);
    void __fastcall ASaveAllExecute(TObject *Sender);
    void __fastcall ATextSelectAllExecute(TObject *Sender);
    void __fastcall ATextCopyExecute(TObject *Sender);
    void __fastcall AImportListExecute(TObject *Sender);
    void __fastcall SelectorButtonClicked(TObject *Sender, int Index);

    private:
    TabImageStore aTabImages;
    bool aClosingCancel;
    unsigned int aLastTab, aSelector;

    bool *GetModified()
    { return &((aTabImages.begin() + aSelector)->aModified); }
    const bool *GetModified() const
    { return &((aTabImages.begin() + aSelector)->aModified); }
    UnicodeString *GetFileName()
    { return &((aTabImages.begin() + aSelector)->aFileName); }
    const UnicodeString *GetFileName() const
    { return &((aTabImages.begin() + aSelector)->aFileName); }
    ionstd::Paramlist *GetParamlist()
    { return &((aTabImages.begin() + aSelector)->aParamlist); }
    const ionstd::Paramlist *GetParamlist() const
    { return &((aTabImages.begin() + aSelector)->aParamlist); }

    void MainTextToBegin()
    { SendMessage(MainText->Handle, WM_VSCROLL, SB_TOP, 0); }
    void __fastcall OnOpenOther(TWMCopyData &);
    void __fastcall WndProc(TMessage &);

    UnicodeString __fastcall GetLastName(UnicodeString) const;
    UnicodeString __fastcall GetFormCaption() const;
    UnicodeString __fastcall GetStatisticText() const;

    void __fastcall Controls(bool);
    void __fastcall HighlightMainText();
    void __fastcall Refresh();
    void __fastcall eClose();
    void __fastcall eOpen(UnicodeString);
    void __fastcall eSave(UnicodeString);
    PairSelection __fastcall eSelection(bool, bool, bool, UnicodeString = "");

    public:
    __fastcall TMainForm(TComponent* Owner);

    BEGIN_MESSAGE_MAP
        MESSAGE_HANDLER(WM_COPYDATA, TWMCopyData, OnOpenOther);
    END_MESSAGE_MAP(TComponent)

}; // class TMainForm

extern PACKAGE TMainForm *MainForm;

#endif // MAINH
