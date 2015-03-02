/***************************************************************************
 *  C++ source
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

#include <vcl.h>
#pragma hdrstop
#include "main.h"
#include <cieabt.h>
#include <ciearg.h>
#include <ciecnf.h>
#include <cieexc.h>

#pragma package(smart_init)
#pragma resource "*.dfm"

TMainForm *MainForm;

ionstd::String wmeNoSupport("your device does not support the wave format");

namespace ionstd { PLACEEXCEPTIONFNCHERE }

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

UnicodeString __fastcall TMainForm::MakeDescription(unsigned int IndexArg)
{
    switch (IndexArg)
    {
    case 0 : // Duration
        return FloatToStr(GetWave()->Duration());
    case 1 : // Samples in Data
        return UIntToStr(GetWave()->SamplesInData());
    case 2 : // Bytes in Data
        return UIntToStr(GetWave()->BytesInData());
    case 3 : // Sample Rate
        return UIntToStr(GetWave()->SampleRate());
    case 4 : // Channels
        return IntToStr(GetWave()->Channels());
    case 5 : // Format
        return IntToStr(GetWave()->Format());
    case 6 : // Bits per Sample
        return IntToStr(GetWave()->BitsPerSample());
    case 7 : // Bitrate
        return UIntToStr(GetWave()->Bitrate());
    case 8 : // Normal Gain
        return FloatToStr(GetWave()->NormalGain());
    case 9 : // Additional Chunks
        return UIntToStr(GetWave()->UnknownChunksCount());
    }
    return "UNKNOWN";
}

#define STDWAY(CAPTION, PROPERTY) \
CieArgForm->Invoke(CAPTION, GetWave()->PROPERTY()); \
if (CieArgForm->ModalResult != mrOk) return; \
GetWave()->PROPERTY(CieArgForm->ResultAsInt());

void __fastcall TMainForm::EditProperty(unsigned int IndexArg)
{
    ESTART
    switch (IndexArg)
    {
    case 0 : // Duration
        return; // read only
    case 1 : // Samples in Data
        return; // read only
    case 2 : // Bytes in Data
        return; // read only
    case 3 : // Sample Rate
        STDWAY("Sample Rate, Hz", SampleRate);    break;
    case 4 : // Channels
        STDWAY("Channels", Channels);             break;
    case 5 : // Format
        STDWAY("Format", Format);                 break;
    case 6 : // Bits per Sample
        STDWAY("Bits per Sample", BitsPerSample); break;
    case 7 : // Bitrate
        return; // read only
    case 8 : // Normal Gain
        return; // read only
    case 9 : // Additional Chunks
        return; // read only
    }
    EEND
    *GetModified() = true;
    Refresh();
}

void __fastcall TMainForm::Refresh()
{
    // first: workspace & actions
    bool aTabsExist = !aTabImages.empty();
    Selector->Visible = aTabsExist;
    MainPanel->Visible = aTabsExist;
    ASave->Enabled = aTabsExist && *GetModified();
    ASaveAs->Enabled = aTabsExist;
    ASaveAll->Enabled = aTabsExist;
    AClose->Enabled = aTabsExist;
    ACloseAll->Enabled = aTabsExist;
    AGain->Enabled = aTabsExist;
    ANormalize->Enabled = aTabsExist;
    ADither->Enabled = aTabsExist;
    APreview->Enabled = aTabsExist && !aSoundPreview;
    AStopPreview->Enabled = aTabsExist && aSoundPreview;
    // second: statistic
    Caption = GetFormCaption();
    if (aTabsExist)
    StatusBar->Panels->Items[2]->Text = (*GetModified()) ? "Modified" : "";
    else StatusBar->Panels->Items[2]->Text = "";
    if (!aTabsExist) return;
    // third: creating all the buttons
    MainPanel->Items->BeginUpdate();
    unsigned int aCount = MainPanel->Items->Count;
    TGrpButtonItem *aButton;
    for (unsigned int i = 0; i < aCount; i++)
    {
        aButton = MainPanel->Items->operator[](i);
        UnicodeString aCaption = aButton->Caption;
        aButton->Caption = aCaption.SubString(0, aCaption.Pos("="))
            + ' ' + MakeDescription(i);
    }
    MainPanel->Items->EndUpdate();
    // fourth: refresh icons of active files
    if (*GetModified()) Selector->Items->operator[](aSelector)->ImageIndex = 12;
    else Selector->Items->operator[](aSelector)->ImageIndex = 11;
}

void __fastcall TMainForm::eClose()
{
        delete GetWave();
    aTabImages.erase(aTabImages.begin() + aSelector);
    Selector->Items->Delete(aSelector); // delete button
    if (aSelector == aTabImages.size()) aSelector--;
    Refresh();
}

void __fastcall TMainForm::eOpen(UnicodeString FileNameArg)
{
    ESTART
    TabImage aTabImage;
    aTabImage.aFileName = FileNameArg;
    aTabImage.aModified = false;
    CieArgForm->Invoke("Start Time, sec", 0);
    if (CieArgForm->ModalResult != mrOk) return;
    double aStart = CieArgForm->ResultAsDouble();
    CieArgForm->Invoke("End Time, sec", 0);
    if (CieArgForm->ModalResult != mrOk) return;
    double aEnd = CieArgForm->ResultAsDouble();
    aTabImage.aWave = new ionstd::WaveStruct
        (Adapt(FileNameArg), aStart, aEnd);
    aTabImages.push_back(aTabImage);
    // now select our new tab:
    TGrpButtonItem *aNewTab;
    aNewTab = Selector->Items->Add();
    aNewTab->Caption = GetLastName(FileNameArg);
    // now select our new tab:
    aSelector = aTabImages.size() - 1;
    EEND
}

void __fastcall TMainForm::eSave(UnicodeString FileNameArg)
{
    ESTART
    GetWave()->Save(Adapt(FileNameArg));
    *GetFileName() = FileNameArg;
    EEND
}

void __fastcall TMainForm::EnableControls(bool Arg)
{
    Selector->Enabled = Arg;
    MainPanel->Enabled = Arg;
}

__fastcall TMainForm::TMainForm(TComponent* Owner) :
    TForm         (Owner),
    aSelector     (-1),
    aSoundPreview (false)
{ }

void __fastcall TMainForm::FormCloseQuery(TObject *Sender, bool &CanClose)
{
    ACloseAll->Execute();
    CanClose = !aClosingCancel;
    aClosingCancel = false;
}

void __fastcall TMainForm::FormCreate(TObject *Sender)
{
    CieAbtForm = new TCieAbtForm(Application);
    CieArgForm = new TCieArgForm(Application);
    CieCnfForm = new TCieCnfForm(Application);
    CieExcForm = new TCieExcForm(Application);
    // setting 'about' data
    CieAbtForm->SetTitle(iCaption);
    CieAbtForm->SetDate(__DATE__);
    CieAbtForm->SetVersion(iVersion);
    UnicodeString aDescriptions = Adapt(ionstd::GetPRLDescription());
    aDescriptions += "\nVersion " + Adapt(ionstd::GetPRLVersion()) + '\n';
    aDescriptions += Adapt(ionstd::GetWAVDescription());
    aDescriptions += "\nVersion " + Adapt(ionstd::GetWAVVersion()) + '\n';
    CieAbtForm->SetDescription(aDescriptions);
    CieAbtForm->SetCopyright("(c) 2014 Mark Karpov");
    // loading parameters from ion.prl
    ESTART
    ionstd::Paramlist aList;
    aList.Load(MergePath("ion.prl"));
    aPRVDevice = aList.Data<ionstd::numerator>("prv", "Device");
    EEND
    int aParamsCount = ParamCount() + 1;
    for (int i = 1; i < aParamsCount; eOpen(ParamStr(i++)));
        Refresh();
}

void __fastcall TMainForm::FormKeyDown
    (TObject *Sender, WORD &Key, TShiftState Shift)
{
    if (Key == VK_SPACE)
    {
        if (aSoundPreview) AStopPreview->Execute();
        else APreview->Execute();
    }
}

void __fastcall TMainForm::FormResize(TObject *Sender)
{
    StatusBar->Panels->Items[0]->Width = 0.57 * Width;
    StatusBar->Panels->Items[1]->Width = 0.29 * Width;
}

void __fastcall TMainForm::MainPanelButtonClicked(TObject *Sender, int IndexArg)
{
    EditProperty(IndexArg);
}

void __fastcall TMainForm::SelectorButtonClicked(TObject *Sender, int IndexArg)
{
    aSelector = IndexArg;
    Refresh();
}

void __fastcall TMainForm::OnOpenOther(TWMCopyData &MessageArg)
{
    UnicodeString aFileName = (wchar_t *)MessageArg.CopyDataStruct->lpData;
    bool AlreadyExists = false;
    for(TabImagesStore::const_iterator i = aTabImages.begin();
        (i != aTabImages.end()); i++)
    if (aFileName == i->aFileName)
    {
        aSelector = i - aTabImages.begin();
        Refresh();
        AlreadyExists = true;
    }
    if (!AlreadyExists)
    {
        eOpen(aFileName);
        Refresh();
    }
}

void __fastcall TMainForm::WndProc(TMessage &MessageArg)
{
    if(MessageArg.Msg == UWM_AREYOUME)
    {
        MessageArg.Result = true;
    }
    else if (MessageArg.Msg == MM_WOM_DONE)
    {
        waveOutUnprepareHeader(aPRVHandle, &aPRVHeader, sizeof(WAVEHDR));
                waveOutClose(aPRVHandle);
        aSoundPreview = false;
        APreview->Enabled = true;
        AStopPreview->Enabled = false;
        EnableControls(true);
    }
    else TMainForm::Dispatch(&MessageArg);
}

/***************************************************************************
 *  File
 **************************************************************************/

void __fastcall TMainForm::AOpenExecute(TObject *Sender)
{
    ESTART
    if (!OpenDialog->Execute()) return; // user did't press OK
    for(int d = 0; (d < OpenDialog->Files->Count); d++)
    {
        bool AlreadyExists = false;
        for(TabImagesStore::const_iterator i = aTabImages.begin();
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
            case mrYes: ASave->Execute();
            case mrNo:  aClosingCancel = false; break;
            case mrCancel:  aClosingCancel = true; break;
        }
    }
    if (!aClosingCancel) eClose();
}

void __fastcall TMainForm::ACloseAllExecute(TObject *Sender)
{
    aClosingCancel = false;
    while (!aTabImages.empty() && !aClosingCancel) AClose->Execute();
}

void __fastcall TMainForm::AGainExecute(TObject *Sender)
{
    ESTART
    CieArgForm->Invoke("Gain, db", 0);
    if (CieArgForm->ModalResult != mrOk) return;
    GetWave()->Gain(CieArgForm->ResultAsDouble());
    EEND
    *GetModified() = true;
    Refresh();
}

void __fastcall TMainForm::ANormalizeExecute(TObject *Sender)
{
    ESTART
    GetWave()->Normalize();
    EEND
    *GetModified() = true;
    Refresh();
}

void __fastcall TMainForm::ADitherExecute(TObject *Sender)
{
    ESTART
    GetWave()->Dither();
    EEND
    *GetModified() = true;
    Refresh();
}

void __fastcall TMainForm::AExitExecute(TObject *Sender)
{
    Close();
}

/***************************************************************************
 *  Preview
 **************************************************************************/

void __fastcall TMainForm::APreviewExecute(TObject *Sender)
{
    ESTART
    aSoundPreview = true;
    APreview->Enabled = false;
    AStopPreview->Enabled = true;
    EnableControls(false);
    // adjust preview parameters
    WAVEFORMATEX aWaveFmt;
    aWaveFmt.wFormatTag = ionstd::WaveFormatIEEEFloat;
    aWaveFmt.nChannels = GetWave()->Channels();
    aWaveFmt.nSamplesPerSec = GetWave()->SampleRate();
    aWaveFmt.nBlockAlign = 4 * aWaveFmt.nChannels;
    aWaveFmt.nAvgBytesPerSec = aWaveFmt.nBlockAlign * aWaveFmt.nSamplesPerSec;
    aWaveFmt.wBitsPerSample = 32;
    aWaveFmt.cbSize = 0;
    int Result = waveOutOpen(&aPRVHandle, aPRVDevice, &aWaveFmt,
        MAKELONG(Handle, 0), 0, CALLBACK_WINDOW);
    waveOutSetVolume(aPRVHandle, 0xffffffffu);
    if (Result == WAVERR_BADFORMAT) THROW_RE(wmeNoSupport);
    aPRVHeader.lpData = (char *)GetWave()->GetData();
    aPRVHeader.dwBufferLength =
        aWaveFmt.nBlockAlign * GetWave()->SamplesInData();
    waveOutPrepareHeader(aPRVHandle, &aPRVHeader, sizeof(WAVEHDR));
    waveOutWrite(aPRVHandle, &aPRVHeader, sizeof(WAVEHDR));
    EEND
}

void __fastcall TMainForm::AStopPreviewExecute(TObject *Sender)
{
    waveOutReset(aPRVHandle);
    waveOutClose(aPRVHandle);
}

/***************************************************************************
 *  Help
 **************************************************************************/

void __fastcall TMainForm::AAboutExecute(TObject *Sender)
{
    CieAbtForm->Invoke();
}
