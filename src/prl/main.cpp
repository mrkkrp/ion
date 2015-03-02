/***************************************************************************
 *  C++ source
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

#include <vcl.h>
#pragma hdrstop
#include "main.h"
#include <cieabt.h>
#include <ciearg.h>
#include <ciechs.h>
#include <ciecnf.h>
#include <cieexc.h>

#pragma package(smart_init)
#pragma resource "*.dfm"

TMainForm *MainForm;

/***************************************************************************
 *  Auxiliary Methods
 **************************************************************************/

UnicodeString __fastcall TMainForm::GetLastName(UnicodeString StringArg) const
{
    if (StringArg.IsEmpty()) return iDefaultName;
    wchar_t *tLastName = StringArg.c_str() + LastDelimiter("\\", StringArg);
    return UnicodeString(tLastName);
}

UnicodeString __fastcall TMainForm::GetFormCaption() const
{
    UnicodeString tFormCaption = iCaption;
    if (!aTabImages.empty()) tFormCaption +=
        " - [" + GetLastName(*GetFileName()) + "]";
    return tFormCaption;
}

UnicodeString __fastcall TMainForm::GetStatisticText() const
{
    if (aTabImages.empty()) return UnicodeString(); // no tabs
    if (!GetParamlist()->PairsCount()) return "no pairs";
    // - pairs -
    UnicodeString StatisticText = // get number of pairs
        IntToStr((int)GetParamlist()->PairsCount()); // as string
    if (GetParamlist()->PairsCount() == 1) StatisticText += " pair in ";
    else StatisticText += " pairs in ";
    // - maps -
    StatisticText += // get number of maps
        IntToStr((int)GetParamlist()->MapsCount());
    if (GetParamlist()->MapsCount() == 1) StatisticText += " map";
    else StatisticText += " maps";
    return StatisticText;
}

void __fastcall TMainForm::Controls(bool ModeArg)
{
    MainText->Visible       = ModeArg;
    Selector->Visible       = ModeArg;
    ASave->Enabled          = ModeArg;
    ASaveAs->Enabled        = ModeArg;
    ASaveAll->Enabled       = ModeArg;
    AClose->Enabled         = ModeArg;
    ACloseAll->Enabled      = ModeArg;
    AProperties->Enabled    = ModeArg;
    AChangeKey->Enabled     = ModeArg;
    AAddReplace->Enabled    = ModeArg;
    ARemove->Enabled        = ModeArg;
    ADuplicate->Enabled     = ModeArg;
    ASendPair->Enabled      = ModeArg;
    ATextSelectAll->Enabled = ModeArg;
    ATextCopy->Enabled      = ModeArg;
    AImportList->Enabled    = ModeArg;
}

inline void MakeSelection(TRichEdit *SubjectArg, TColor ColorArg,
    int StartArg, int EndArg, int NewLinesArg)
{
    CHARRANGE CharRange = { StartArg - NewLinesArg, EndArg - NewLinesArg };
    SendMessage(SubjectArg->Handle, EM_EXSETSEL, 0, (LPARAM)&CharRange);
    Winapi::Richedit::CHARFORMAT CharFormat;
    memset(&CharFormat, 0, sizeof(CharFormat));
    CharFormat.cbSize = sizeof(CharFormat);
    CharFormat.dwMask = CFM_COLOR;
    CharFormat.crTextColor = ColorArg;
    SendMessage(SubjectArg->Handle,
        EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM)&CharFormat);
}

void __fastcall TMainForm::HighlightMainText()
{
    TColor clMap(clRed), clPair(clBlue), clValue(clGreen);
    int aSymbolsNumber = MainText->Lines->Text.Length(),
    aSelStart, aNewLines = 0;
    bool aInPair(false), aInValue(false), aInMap(false);
    wchar_t *aText = MainText->Lines->Text.c_str();
    for (int i = 0; i < aSymbolsNumber; i++)
    {
        switch (*(aText + i))
        {
        case '~' : if (!aInPair && !aInValue) aInMap = true; break;
        case ';' : if (!aInPair) aInMap = false; break;
        case '[' :
        if (!aInPair && !aInValue) aInPair = true, aSelStart = i + 1; break;
        case ']' :
        if (!aInValue)
        {
            aInPair = false;
            if (aInMap) MakeSelection
                (MainText, clPair, aSelStart, i, aNewLines);
            else MakeSelection(MainText, clMap, aSelStart, i, aNewLines);
        }
        break;
        case '>' :
        if (!aInPair && !aInValue) aInValue = true, aSelStart = i + 1; break;
        case VK_RETURN :
        if (aInValue) MakeSelection
            (MainText, clValue, aSelStart, i - 1, aNewLines);
        aInValue = false;
        aInPair = false;
        aNewLines++;
        }
    }
}

void __fastcall TMainForm::Refresh()
{
    ESTART
    // first: caption of main form
    Caption = GetFormCaption();
    // second: status bar's content
    if (aTabImages.empty())
    {
        StatusBar->Panels->Items[1]->Text = UnicodeString();
        StatusBar->Panels->Items[2]->Text = UnicodeString();
        return;
    }
    StatusBar->Panels->Items[1]->Text = GetStatisticText();
    StatusBar->Panels->Items[2]->Text = *GetModified() ? "Modified" : "";
    // third: text (representation of paramlist)
    SCROLLINFO aScrollInfo;
    aScrollInfo.cbSize = sizeof(SCROLLINFO);
    aScrollInfo.fMask = SIF_POS;
    GetScrollInfo(MainText->Handle, SB_VERT, &aScrollInfo);
    (aTabImages.begin() + aLastTab)->aScrollPos = aScrollInfo.nPos;
    int EventMask = SendMessage(MainText->Handle, EM_SETEVENTMASK, 0, 0);
    MainText->Lines->Text = Adapt(GetParamlist()->AsText());
    SendMessage(MainText->Handle, WM_SETREDRAW, false, 0);
    MainText->HideSelection = true;
    HighlightMainText();
    MainText->HideSelection = false;
    SendMessage(MainText->Handle, WM_SETREDRAW, true, 0);
    InvalidateRect(MainText->Handle, 0, true);
    SendMessage(MainText->Handle, EM_SETEVENTMASK, 0, EventMask);
    MainText->SelStart = 0;
    MainText->SelLength = 0;
    aLastTab = aSelector;
    // fourth: availability of actions that depend on status of paramlist
    ASave->Enabled = *GetModified();
    ADuplicate->Enabled = !GetParamlist()->Empty();
    ASendPair->Enabled  = !GetParamlist()->Empty();
    ARemove->Enabled    = GetParamlist()->PairsCount();
    ACloseAll->Enabled  = !aTabImages.empty();
    ASaveAll->Enabled   = !aTabImages.empty();
    AChangeKey->Enabled = !GetParamlist()->Empty();
    // fifth: sroll bar position correction
    SendMessage(MainText->Handle, WM_VSCROLL, MAKEWPARAM(SB_THUMBPOSITION,
        (aTabImages.begin() + aLastTab)->aScrollPos), 0);
    // sixth: refresh icons of active files
    if (*GetModified()) Selector->Items->operator[](aSelector)->ImageIndex = 20;
    else Selector->Items->operator[](aSelector)->ImageIndex = 19;
    EEND
}

void __fastcall TMainForm::eClose()
{
    aTabImages.erase(aTabImages.begin() + aSelector);
    Selector->Items->Delete(aSelector);
    // if it was the last item, then perform left shift
    if (aSelector == aTabImages.size()) aSelector--;
    if (aTabImages.empty()) Controls(false); // no tabs remains
}

void __fastcall TMainForm::eOpen(UnicodeString FileNameArg)
{
    ESTART
    TabImage aTabImage;
    aTabImage.aParamlist.Load(Adapt(FileNameArg));
    aTabImage.aFileName = FileNameArg;
    aTabImage.aModified = false;
    aTabImage.aScrollPos = 0;
    aTabImages.push_back(aTabImage);
    TGrpButtonItem *aNewTab = Selector->Items->Add();
    aNewTab->Caption = GetLastName(FileNameArg);
    aSelector = aTabImages.size() - 1;
    Controls(!aTabImages.empty());
    EEND
}

void __fastcall TMainForm::eSave(UnicodeString FilenameArg)
{
    ESTART
    GetParamlist()->Save(Adapt(FilenameArg));
    EEND
}

PairSelection __fastcall TMainForm::eSelection
    (bool PSArg, bool STArg, bool SAArg, UnicodeString CaptionArg)
{
    PairSelection aResult;
    aResult.aCancelled = false;
    if (!PSArg)
    {
        CieChsForm->Invoke("Target Type", TargetType, "pm");
        if (CieChsForm->ModalResult == mrCancel)
        { aResult.aCancelled = true; return aResult; }
        if (CieChsForm->GetIndex()) aResult.aMapSelected = true;
        else aResult.aMapSelected = false;
    }
    else aResult.aMapSelected = false;
    CieArgForm->Invoke("Target Map", UnicodeString("target-map"));
    if (CieArgForm->ModalResult == mrCancel)
    { aResult.aCancelled = true; return aResult; }
    aResult.aMapKey = Adapt(CieArgForm->ResultAsString());
    if (!aResult.aMapSelected)
    {
        CieArgForm->Invoke("Target Pair", UnicodeString("target-pair"));
        if (CieArgForm->ModalResult == mrCancel)
        { aResult.aCancelled = true; return aResult; }
        aResult.aPairKey = Adapt(CieArgForm->ResultAsString());
    }
    if (STArg)
    {
        CieChsForm->Invoke("Data Type", DataType, "bids");
        if (CieArgForm->ModalResult == mrCancel)
        { aResult.aCancelled = true; return aResult; }
        switch (CieChsForm->GetIndex())
        {
            case 0 : aResult.aType = stBool; break;
            case 1 : aResult.aType = stUInt; break;
            case 2 : aResult.aType = stDouble; break;
            case 3 : aResult.aType = stString; break;
        }
    }
    if (SAArg)
    {
        CieArgForm->Invoke(CaptionArg, UnicodeString("argument"));
        if (CieArgForm->ModalResult == mrCancel)
        { aResult.aCancelled = true; return aResult; }
        aResult.aArgument = Adapt(CieArgForm->ResultAsString());
        }
    return aResult;
}

__fastcall TMainForm::TMainForm(TComponent* Owner) : TForm(Owner)
{ }

void __fastcall TMainForm::FormCreate(TObject *Sender)
{
    // creation of CIEs
    CieAbtForm = new TCieAbtForm(Application);
    CieArgForm = new TCieArgForm(Application);
    CieChsForm = new TCieChsForm(Application);
    CieCnfForm = new TCieCnfForm(Application);
    CieExcForm = new TCieExcForm(Application);
    // setting 'about' data
    CieAbtForm->SetTitle(iCaption);
    CieAbtForm->SetDate(__DATE__);
    CieAbtForm->SetVersion(iVersion);
    UnicodeString aPRLDescription = Adapt(ionstd::GetPRLDescription());
    aPRLDescription += "\nVersion " +  Adapt(ionstd::GetPRLVersion());
    CieAbtForm->SetDescription(aPRLDescription);
    CieAbtForm->SetCopyright("(c) 2014 Mark Karpov");
    Controls(false);
    // now we look at the command line and try open specified files
    int aParamsCount = ParamCount() + 1;
    for (int i = 1; i < aParamsCount; eOpen(ParamStr(i++)));
    aLastTab = 0;
    Refresh();
    MainTextToBegin();
}

void __fastcall TMainForm::FormResize(TObject *Sender)
{
    StatusBar->Panels->Items[0]->Width = 0.57 * Width;
    StatusBar->Panels->Items[1]->Width = 0.29 * Width;
}

void __fastcall TMainForm::FormCloseQuery(TObject *Sender, bool &CanClose)
{
    ACloseAll->Execute();
    CanClose = !aClosingCancel;
    aClosingCancel = false;
}

void __fastcall TMainForm::SelectorButtonClicked(TObject *Sender, int IndexArg)
{
    aSelector = IndexArg;
    Refresh();
}

void __fastcall TMainForm::OnOpenOther(TWMCopyData &MessageArg)
{
    UnicodeString aFilename = (wchar_t *)MessageArg.CopyDataStruct->lpData;
    bool AlreadyExists = false;
    for(TabImageStore::const_iterator i = aTabImages.begin();
        (i != aTabImages.end()); i++)
    if (aFilename == i->aFileName)
    {
        aSelector = i - aTabImages.begin();
        Refresh();
        AlreadyExists = true;
    }
    if (!AlreadyExists)
    {
        eOpen(aFilename);
        Refresh();
        MainTextToBegin();
    }
}

void __fastcall TMainForm::WndProc(TMessage &Message)
{
    if(Message.Msg == UWM_AREYOUME)
    {
        Message.Result = true;
    }
    else TMainForm::Dispatch(&Message);
}

/***************************************************************************
 *  File
 **************************************************************************/

void __fastcall TMainForm::ANewExecute(TObject *Sender)
{
    ESTART
    TabImage aTabImage;
    aTabImage.aParamlist.Name(Adapt(iDefaultName));
    aTabImage.aModified = true;
    aTabImage.aScrollPos = 0;
    aTabImage.aFileName = UnicodeString();
    aTabImages.push_back(aTabImage);
    TGrpButtonItem *aNewTab = Selector->Items->Add();
    aNewTab->Caption = GetLastName(UnicodeString());
    aSelector = aTabImages.size() - 1;
    EEND
    Controls(true);
    Refresh();
    MainTextToBegin();
}

void __fastcall TMainForm::AOpenExecute(TObject *Sender)
{
    ESTART
    OpenDialog->Title = AOpen->Caption;
    if (!OpenDialog->Execute()) return;
    for(int d = 0; (d < OpenDialog->Files->Count); d++)
    {
        bool AlreadyExists = false;
        for(TabImageStore::const_iterator i = aTabImages.begin();
                (i != aTabImages.end()); i++)
        if ((*OpenDialog->Files)[d] == i->aFileName)
        {
            aSelector = i - aTabImages.begin();
            AlreadyExists = true;
        }
        if (!FileExists((*OpenDialog->Files)[d])) continue;
        if (!AlreadyExists)
        {
            eOpen((*OpenDialog->Files)[d]);
            Refresh();
            MainTextToBegin();
        }
    }
    EEND
}

void __fastcall TMainForm::ASaveExecute(TObject *Sender)
{
    ESTART
    if (GetFileName()->IsEmpty())
    {
        SaveDialog->Title = ASave->Caption;
        if (!SaveDialog->Execute()) return;
        eSave(SaveDialog->FileName);
        Selector->Items->operator[](aSelector)->Caption =
            GetLastName(SaveDialog->FileName);
        *GetFileName() = SaveDialog->FileName;
    }
    else eSave(*GetFileName());
    EEND
    *GetModified() = false;
    Refresh();
}

void __fastcall TMainForm::ASaveAsExecute(TObject *Sender)
{
    ESTART
    SaveDialog->Title = ASaveAs->Caption;
    if (!SaveDialog->Execute()) return;
    eSave(SaveDialog->FileName);
    Selector->Items->operator[](aSelector)->Caption =
        GetLastName(SaveDialog->FileName);
    *GetFileName() = SaveDialog->FileName;
    EEND
    *GetModified() = false;
    Refresh();
}

void __fastcall TMainForm::ASaveAllExecute(TObject *Sender)
{
    int tPosition = aSelector;
    for(aSelector = 0; aSelector < aTabImages.size(); aSelector++)
    {
        Refresh();
        ASave->Execute();
    }
    aSelector = tPosition;
    Refresh();
}

void __fastcall TMainForm::ACloseExecute(TObject *Sender)
{
    if (*GetModified())
    {
        CieCnfForm->Invoke(GetLastName(*GetFileName()));
        switch(CieCnfForm->ModalResult)
        {
            case mrYes  : ASave->Execute();
            case mrNo   : aClosingCancel = false; break;
            case mrCancel   : aClosingCancel = true; break;
        }
    }
    if (!aClosingCancel) eClose(), Refresh();
}

void __fastcall TMainForm::ACloseAllExecute(TObject *Sender)
{
    aClosingCancel = false;
    while (!aTabImages.empty() && !aClosingCancel) AClose->Execute();
}

void __fastcall TMainForm::APropertiesExecute(TObject *Sender)
{
    ESTART
    CieArgForm->Invoke("Paramlist Name", Adapt(GetParamlist()->Name()));
    if (CieArgForm->ModalResult == mrCancel) return;
    GetParamlist()->Name(Adapt(CieArgForm->ResultAsString()));
    *GetModified() = true;
    Refresh();
    CieArgForm->Invoke("Author", Adapt(GetParamlist()->Author()));
    if (CieArgForm->ModalResult == mrCancel) return;
    GetParamlist()->Author(Adapt(CieArgForm->ResultAsString()));
    *GetModified() = true;
    Refresh();
    CieArgForm->Invoke("Comment", Adapt(GetParamlist()->Comment()));
    if (CieArgForm->ModalResult == mrCancel) return;
    GetParamlist()->Comment(Adapt(CieArgForm->ResultAsString()));
    *GetModified() = true;
    Refresh();
    CieArgForm->Invoke("Copyright", Adapt(GetParamlist()->Copyright()));
    if (CieArgForm->ModalResult == mrCancel) return;
    GetParamlist()->Copyright(Adapt(CieArgForm->ResultAsString()));
    *GetModified() = true;
    Refresh();
    EEND
}

void __fastcall TMainForm::AImportListExecute(TObject *Sender)
{
    ESTART
    ionstd::Paramlist tParamlist;
    OpenDialog->Title = AImportList->Caption;
    if (!OpenDialog->Execute()) return;
    tParamlist.Load(Adapt(OpenDialog->FileName));
    GetParamlist()->Fuse(tParamlist);
    EEND
    Refresh();
}

void __fastcall TMainForm::AExitExecute(TObject *Sender)
{
    Close();
}

/***************************************************************************
 *  Pair
 **************************************************************************/

void __fastcall TMainForm::AAddReplaceExecute(TObject *Sender)
{
    ESTART
    PairSelection S = eSelection(true, true, true, "Value");
    if (S.aCancelled) return;
    switch (S.aType)
    {
    case stBool : GetParamlist()->
    Add<bool>(S.aMapKey, S.aPairKey, CieArgForm->ResultAsBool()); break;
    case stUInt : GetParamlist()->
    Add<unsigned int>(S.aMapKey, S.aPairKey, CieArgForm->ResultAsInt()); break;
    case stDouble : GetParamlist()->
    Add<double>(S.aMapKey, S.aPairKey, CieArgForm->ResultAsDouble()); break;
    case stString : GetParamlist()->
    Add<ionstd::String>(S.aMapKey, S.aPairKey, S.aArgument); break;
    }
    EEND
    *GetModified() = true;
    Refresh();
}

void __fastcall TMainForm::ARemoveExecute(TObject *Sender)
{
    ESTART
    PairSelection S = eSelection(false, false, false);
    if (S.aCancelled) return;
    if (S.aMapSelected) GetParamlist()->Clear(S.aMapKey);
    else GetParamlist()->Remove(S.aMapKey, S.aPairKey);
    EEND
    *GetModified() = true;
    Refresh();
}

void __fastcall TMainForm::AChangeKeyExecute(TObject *Sender)
{
    ESTART
    PairSelection S = eSelection(false, false, true, "New Key");
    if (S.aCancelled) return;
    if (S.aMapSelected) GetParamlist()->MapKey(S.aMapKey, S.aArgument);
    else GetParamlist()->PairKey(S.aMapKey, S.aPairKey, S.aArgument);
    EEND
    *GetModified() = true;
    Refresh();
}

void __fastcall TMainForm::ADuplicateExecute(TObject *Sender)
{
    ESTART
    PairSelection S = eSelection(false, false, true, "Target Key");
    if (S.aCancelled) return;
    if (S.aMapSelected) GetParamlist()->DuplicateMap(S.aMapKey, S.aArgument);
    else GetParamlist()->CopyPair(S.aMapKey, S.aPairKey, S.aArgument);
    EEND
    *GetModified() = true;
    Refresh();
}

void __fastcall TMainForm::ASendPairExecute(TObject *Sender)
{
    ESTART
    PairSelection S = eSelection(true, false, true, "Target Key");
    if (S.aCancelled) return;
    GetParamlist()->SendPair(S.aMapKey, S.aPairKey, S.aArgument);
    EEND
    *GetModified() = true;
    Refresh();
}

/***************************************************************************
 *  Text
 **************************************************************************/

void __fastcall TMainForm::ATextSelectAllExecute(TObject *Sender)
{
    MainText->SelectAll();
}

void __fastcall TMainForm::ATextCopyExecute(TObject *Sender)
{
    MainText->CopyToClipboard();
}

/***************************************************************************
 *  Help
 **************************************************************************/

void __fastcall TMainForm::AAboutExecute(TObject *Sender)
{
    CieAbtForm->Invoke();
}
