/***************************************************************************
 *  C++ source
 *
 *  ION Sonority Editor
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

const FEP
FrequencyChange = { 0.,         10.,       10, gmLinear, 2 },
PhaseChange     = { -2. * M_PI, 4. * M_PI, 10, gmLinear, 5 },
SignalShape     = { -1.,        2.,        20, gmLinear, 5 };

ionstd::String snreNoSupport("your device does not support the wave format");

namespace ionstd { PLACEEXCEPTIONFNCHERE }

/***************************************************************************
 *  Auxiliary Methods
 **************************************************************************/

__fastcall VoiceThread::VoiceThread
    (const ionstd::Translator *TranslatorArg, unsigned int VoiceArg) :
    TThread     (false /* create suspended */),
    aTranslator (TranslatorArg),
    aVoice      (VoiceArg)
{ }

void __fastcall VoiceThread::Execute()
{
    ESTART
    while (!Terminated && aTranslator->TranslateBlock(aVoice));
    EEND
}

#define STDWAY_STRING(CAPTION, PROPERTY) \
CieArgForm->Invoke(CAPTION, Adapt(GetSonority()->PROPERTY())); \
if (CieArgForm->ModalResult != mrOk) return; \
GetSonority()->PROPERTY(Adapt(CieArgForm->ResultAsString()));

#define STDWAY_BOOL(CAPTION, PROPERTY) \
CieArgForm->Invoke(CAPTION, GetSonority()->PROPERTY()); \
if (CieArgForm->ModalResult != mrOk) return; \
GetSonority()->PROPERTY(CieArgForm->ResultAsBool());

#define STDWAY_DOUBLE(CAPTION, PROPERTY) \
CieArgForm->Invoke(CAPTION, GetSonority()->PROPERTY()); \
if (CieArgForm->ModalResult != mrOk) return; \
GetSonority()->PROPERTY(CieArgForm->ResultAsDouble());

#define STDWAY_UINT(CAPTION, PROPERTY) \
CieArgForm->Invoke(CAPTION, GetSonority()->PROPERTY()); \
if (CieArgForm->ModalResult != mrOk) return; \
GetSonority()->PROPERTY(CieArgForm->ResultAsInt());

#define STDWAY_FNC(CAPTION, PROPERTY) \
CieFncForm->Invoke(CAPTION, GetSonority()->PROPERTY(), &PROPERTY ## FEP); \
if (CieFncForm->ModalResult == mrCancel) return;

void __fastcall TMainForm::EditDynamic(unsigned int IndexArg)
{
    ESTART
    ionstd::String aString;
    bool aBool;
    ionstd::FunctionPointer aFunction;
    switch (IndexArg)
    {
    case 0 : // Attack Time
        STDWAY_DOUBLE("Attack Time, sec", AttackTime); break;
    case 1 : // Release Time
        STDWAY_DOUBLE("Release Time, sec", ReleaseTime); break;
    case 2 : // Attack P-Shape
        STDWAY_FNC("Attack P-Shape", APShape); break;
    case 3 : // Attack F-Shape
        STDWAY_FNC("Attack F-Shape", AFShape); break;
    case 4 : // Attack V-Shape
        STDWAY_FNC("Attack V-Shape", AVShape); break;
    case 5 : // Release P-Shape
        STDWAY_FNC("Release P-Shape", RPShape); break;
    case 6 : // Release F-Shape
        STDWAY_FNC("Release F-Shape", RFShape); break;
    case 7 : // Release V-Shape
        STDWAY_FNC("Release V-Shape", RVShape); break;
    case 8 : // Panoram Mod
        STDWAY_FNC("Panoram Mod", PMod); break;
    case 9 : // Frequency Mod
        STDWAY_FNC("Frequency Mod", FMod); break;
    case 10 : // Volume Mod
        STDWAY_FNC("Volume Mod", VMod); break;
    default : return;
    }
    EEND
    *GetModified() = true;
    Refresh();
}

void __fastcall TMainForm::EditSignal(unsigned int IndexArg)
{
    ESTART
    ionstd::String aString;
    bool aBool;
    ionstd::FunctionPointer aFunction;
    switch (IndexArg)
    {
    case 0 : // open sample
    {
        if (!OpenSample->Execute()) return;
        ionstd::WaveStruct aWave(Adapt(OpenSample->FileName));
        GetSonority()->UseSample(aWave);
    }
    break; // open sample
    case 1 : // Nominal Frequency
        STDWAY_DOUBLE("NominalFrequency, Hz", NominalFrequency); break;
    case 2 : // R-Noise
        STDWAY_DOUBLE("Relative Noise, db", RNoise); break;
    case 3 : // A-Noise
        STDWAY_DOUBLE("Absolute Noise, db", ANoise); break;
    case 4 : // A-Noise Probability
        STDWAY_DOUBLE("Absolute Noise Probability", ANoiseP); break;
    default : return;
    }
    EEND
    *GetModified() = true;
    Refresh();
}

inline int MatchPi(double Arg)
{
    Arg /= M_PI;
    if (Arg - std::floor(Arg) < 1e-5) return Arg;
    else return 0;
}

UnicodeString __fastcall TMainForm::MakeFncImage
    (ionstd::ConstFunctionPointer FunctionArg) const
{
    UnicodeString aResult;
    double aValue = FunctionArg->Min();
    int aMatchPi = MatchPi(aValue);
    if (aMatchPi) aResult += IntToStr(aMatchPi) + u"\u03c0";
    else aResult += aValue;
    aResult += " : ";
    aValue = FunctionArg->Max();
    aMatchPi = MatchPi(aValue);
    if (aMatchPi) aResult += IntToStr(aMatchPi) + u"\u03c0";
    else aResult += aValue;
    aResult += ", " + UIntToStr(FunctionArg->PointsCount()) + "p ";
    aResult += FunctionArg->Smooth() ? "l" : "s";
    return aResult;
}

#define SUBMIT_DOUBLE(PROPERTY) return FloatToStr(GetSonority()->PROPERTY());
#define SUBMIT_FNC(PROPERTY) return MakeFncImage(GetSonority()->PROPERTY());
#define SUBMIT_UINT(PROPERTY) return UIntToStr(GetSonority()->PROPERTY());
#define SUBMIT_BOOL(PROPERTY) return GetSonority()->PROPERTY() ? "true" : "false";

UnicodeString __fastcall TMainForm::MakeDynamicD(unsigned int IndexArg) const
{
    switch (IndexArg)
    {
    case 0 : /* Attack Time */     SUBMIT_DOUBLE(AttackTime);
    case 1 : /* Release Time */    SUBMIT_DOUBLE(ReleaseTime);
    case 2 : /* Attack P-Shape */  SUBMIT_FNC(APShape);
    case 3 : /* Attack F-Shape */  SUBMIT_FNC(AFShape);
    case 4 : /* Attack V-Shape */  SUBMIT_FNC(AVShape);
    case 5 : /* Release P-Shape */ SUBMIT_FNC(RPShape);
    case 6 : /* Release F-Shape */ SUBMIT_FNC(RFShape);
    case 7 : /* Release V-Shape */ SUBMIT_FNC(RVShape);
    case 8 : /* Panoram Mod */     SUBMIT_FNC(PMod);
    case 9 : /* Frequency Mod */   SUBMIT_FNC(FMod);
    case 10 : /* Volume MOd */     SUBMIT_FNC(VMod);
    }
    return "UNKNOWN";
}

UnicodeString __fastcall TMainForm::MakeSignalD(unsigned int IndexArg) const
{
    switch (IndexArg)
    {
    case 0 : /* Select Sample */       SUBMIT_DOUBLE(SampleDuration);
    case 1 : /* Nominal Frequency */   SUBMIT_DOUBLE(NominalFrequency);
    case 2 : /* R-Noise */             SUBMIT_DOUBLE(RNoise);
    case 3 : /* A-Noise */             SUBMIT_DOUBLE(ANoise);
    case 4 : /* A-Noise Probability */ SUBMIT_DOUBLE(ANoiseP);
    }
    return "UNKNOWN";
}

ionstd::String __fastcall TMainForm::GetPrediction(UnicodeString StringArg) const
{
    ionstd::String aPrediction = Adapt(GetLastName(StringArg));
    unsigned int aPosition = aPrediction.find_last_of('.', aPrediction.length());
    if (aPosition == ionstd::String::npos) aPosition = aPrediction.length();
    else aPosition;
    int aLength = aPrediction.length() - aPosition;
    aPrediction.erase(aPosition, aLength);
    return aPrediction;
}

UnicodeString __fastcall TMainForm::GetLastName(UnicodeString StringArg) const
{
    if (StringArg.IsEmpty()) return iDefaultName;
    wchar_t *tLastName = StringArg.c_str() + LastDelimiter("\\", StringArg);
    return UnicodeString(tLastName);
}

UnicodeString __fastcall TMainForm::GenerateFormCaption() const
{
    UnicodeString aFormCaption = iCaption;
    if (!aTabImages.empty()) aFormCaption +=
        " - [" + GetLastName(*GetFileName()) + "]";
    return aFormCaption;
}

UnicodeString __fastcall TMainForm::GenerateStatisticText() const
{
        return aTabImages.empty() ? "" : "Sonority";
}

bool __fastcall TMainForm::eOpen(UnicodeString FileNameArg)
{
    ESTART
    ionstd::Paramlist aParamlist;
    ionstd::Sonority aSonority;
    aParamlist.Load(Adapt(FileNameArg));
    aSonority.ReadFrom(aParamlist, GetPrediction(FileNameArg));
    TabImage aTabImage;
    aTabImage.aSonority = aSonority;
    aTabImage.aFileName = FileNameArg;
    aTabImage.aModified = false;
    aTabImage.aName = Adapt(aParamlist.Name());
    aTabImage.aAuthor = Adapt(aParamlist.Author());
    aTabImage.aComment = Adapt(aParamlist.Comment());
    aTabImage.aCopyright = Adapt(aParamlist.Copyright());
    aTabImages.push_back(aTabImage);
    // now select our new tab:
    TGrpButtonItem *aNewTab;
    aNewTab = Selector->Items->Add();
    aNewTab->Caption = GetLastName(FileNameArg);
    // now select our new tab:
    aSelector = aTabImages.size() - 1;
    EEND
    return true;
}

void __fastcall TMainForm::eSave(UnicodeString FileNameArg)
{
    ESTART
    ionstd::Paramlist aParamlist;
    GetSonority()->Label(GetPrediction(FileNameArg));
    GetSonority()->WriteTo(aParamlist);
    aParamlist.Name(Adapt(*GetName()));
    aParamlist.Author(Adapt(*GetAuthor()));
    aParamlist.Comment(Adapt(*GetComment()));
    aParamlist.Copyright(Adapt(*GetCopyright()));
    aParamlist.Save(Adapt(FileNameArg));
    *GetFileName() = FileNameArg;
    EEND
}

void __fastcall TMainForm::eClose()
{
    // delete image:
    aTabImages.erase(aTabImages.begin() + aSelector);
    Selector->Items->Delete(aSelector); // delete button
    // if it was the last, then perform left shift
    if (aSelector == aTabImages.size()) aSelector--;
    Refresh();
}

void __fastcall TMainForm::EnableControls(bool Arg)
{
    Selector->Enabled = Arg;
    Dynamic->Enabled = Arg;
    Signal->Enabled = Arg;
    PreviewPanel->Enabled = Arg;
}

void __fastcall TMainForm::LoadPreviewDirectories()
{
    TSearchRec aSRec;
    UnicodeString aSearchDir, aFile;
    aSearchDir = Adapt(MergePath("scores\\probes\\*"));
    PreviewCategory->Items->Clear();
    if (!FindFirst(aSearchDir, faDirectory, aSRec))
    {
        do if ((aSRec.Attr & faDirectory)
            && (aSRec.Name != "..")
            && (aSRec.Name != "."))
            PreviewCategory->Items->Add(aSRec.Name);
        while (!FindNext(aSRec));
    }
    FindClose(aSRec);
}

void __fastcall TMainForm::LoadPreviewProbes()
{
    TSearchRec aSRec;
    UnicodeString aSearchDir, aFile;
    aSearchDir = Adapt(MergePath("scores\\probes\\"));
    aSearchDir += PreviewCategory->Text + "\\*.ionscr";
    PreviewProbe->Items->Clear();
    if (!FindFirst(aSearchDir, faAnyFile, aSRec))
    {
        do if (aSRec.Attr & faAnyFile)
        {
            UnicodeString aItem = aSRec.Name;
            aItem = aItem.SubString(0, aItem.Pos(".") - 1);
            PreviewProbe->Items->Add(aItem);
        }
        while (!FindNext(aSRec));
    }
    FindClose(aSRec);
    PreviewProbe->ItemIndex = 0;
}

void __fastcall TMainForm::LoadProbe()
{
    ionstd::String aCurrent = "scores\\probes\\";
    aCurrent += Adapt(PreviewCategory->Text) + '\\';
    aCurrent += Adapt(PreviewProbe->Text) + ".ionscr";
    aCurrent = MergePath(aCurrent);
    if (aCurrent != aPRVElementFile)
    {
        ESTART
        if (aPRVElement) delete aPRVElement;
        ionstd::Paramlist aList;
        aList.Load(aCurrent);
        aPRVElement = ionstd::ReadElement(aList, aPRVSourceLabel);
        aPRVElementFile = aCurrent;
        EEND
    }
}

void __fastcall TMainForm::Refresh()
{
    // first: workspace & actions
    bool aTabsExist = !aTabImages.empty();
    Selector->Visible = aTabsExist;
    Signal->Visible = aTabsExist;
    Dynamic->Visible = aTabsExist;
    PreviewPanel->Visible = aTabsExist;
    ASave->Enabled = aTabsExist && *GetModified();
    ASaveAs->Enabled = aTabsExist;
    ASaveAll->Enabled = aTabsExist;
    AClose->Enabled = aTabsExist;
    ACloseAll->Enabled = aTabsExist;
    AProperties->Enabled = aTabsExist;
    APreview->Enabled = aTabsExist && !aSoundPreview;
    AStopPreview->Enabled = aTabsExist && aSoundPreview;
    // second: statistic
    Caption = GenerateFormCaption();
    StatusBar->Panels->Items[1]->Text = GenerateStatisticText();
    if (aTabsExist)
    StatusBar->Panels->Items[2]->Text = (*GetModified()) ? "Modified" : "";
    else StatusBar->Panels->Items[2]->Text = "";
    if (!aTabsExist) return;
    // third: creating all the buttons
    // 3.1 - dynamic
    Dynamic->Items->BeginUpdate();
    unsigned int aCount = Dynamic->Items->Count;
        TGrpButtonItem *aButton;
    for (unsigned int i = 0; i < aCount; i++)
    {
        aButton = Dynamic->Items->operator[](i);
        UnicodeString aCaption = aButton->Caption;
        aButton->Caption = aCaption.SubString(0, aCaption.Pos("="))
            + ' ' + MakeDynamicD(i);
    }
    Dynamic->Items->EndUpdate();
    // 3.2 - signal
    Signal->Items->BeginUpdate();
    aCount = Signal->Items->Count;
    for (unsigned int i = 0; i < aCount; i++)
    {
        aButton = Signal->Items->operator[](i);
        UnicodeString aCaption = aButton->Caption;
        aButton->Caption = aCaption.SubString(0, aCaption.Pos("="))
            + ' ' + MakeSignalD(i);
    }
    Signal->Items->EndUpdate();
    // fourth: refresh icons of active files
    if (*GetModified()) Selector->Items->operator[](aSelector)->ImageIndex = 22;
    else Selector->Items->operator[](aSelector)->ImageIndex = 21;
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
        aPRVThread->Terminate();
        delete aPRVThread;
        delete aWaveStruct;
        delete aTranslator;
        delete aPRVSonority;
        aSoundPreview = false;
        APreview->Enabled = true;
        AStopPreview->Enabled = false;
        EnableControls(true);
    }
    else TMainForm::Dispatch(&MessageArg);
}

__fastcall TMainForm::TMainForm(TComponent* Owner) :
    TForm          (Owner),
    aSelector      (-1),
    APShapeFEP     (PanoramDefaults),
    AFShapeFEP     (DefaultPreferences),
    AVShapeFEP     (DefaultPreferences),
    RPShapeFEP     (PanoramDefaults),
    RFShapeFEP     (DefaultPreferences),
    RVShapeFEP     (DefaultPreferences),
    PModFEP        (PanoramDefaults),
    FModFEP        (DefaultPreferences),
    VModFEP        (DefaultPreferences),
    SignalShapeFEP (SignalShape),
    PhaseAltFEP    (PhaseChange)
{ }

void __fastcall TMainForm::FormCreate(TObject *Sender)
{
    CieAbtForm = new TCieAbtForm(Application);
    CieArgForm = new TCieArgForm(Application);
    CieCnfForm = new TCieCnfForm(Application);
    CieExcForm = new TCieExcForm(Application);
    CieFncForm = new TCieFncForm(Application);
    // setting 'about' data
    CieAbtForm->SetTitle(iCaption);
    CieAbtForm->SetDate(__DATE__);
    CieAbtForm->SetVersion(iVersion);
    UnicodeString aDescriptions = Adapt(ionstd::GetPRLDescription());
    aDescriptions += "\nVersion " +  Adapt(ionstd::GetPRLVersion()) + '\n';
    aDescriptions += Adapt(ionstd::GetWAVDescription());
    aDescriptions += "\nVersion " + Adapt(ionstd::GetWAVVersion()) + '\n';
    aDescriptions += Adapt(ionstd::GetIONDescription());
    aDescriptions += "\nVersion " + Adapt(ionstd::GetIONVersion());
    CieAbtForm->SetDescription(aDescriptions);
    CieAbtForm->SetCopyright("(c) 2014 Mark Karpov");
    // loading parameters from ion.prl
    ESTART
    ionstd::Paramlist aList;
    aList.Load(MergePath("ion.prl"));
    // load paramters for preview:
    aPRVRate = aList.Data<ionstd::numerator>("prv", "Rate");
    aPRVChannels = aList.Data<ionstd::numerator>("prv", "Channels");
    aPRVDevice = aList.Data<ionstd::numerator>("prv", "Device");
    aPRVRelease = aList.Data<ionstd::time>("prv", "dRelease");
    aPRVFMargin = aList.Data<ionstd::time>("prv", "dFMargin");
    aPRVBMargin = aList.Data<ionstd::time>("prv", "dBMargin");
    aPRVCategory = Adapt(aList.Data<ionstd::String>("prv", "dCategory"));
    aPRVProbe = Adapt(aList.Data<ionstd::String>("prv", "dProbe"));
    aPRVSourceLabel = aList.Data<ionstd::String>("prv", "dSourceLabel");
    EEND
    // tune additional parameters
    int aParamsCount = ParamCount() + 1;
    for (int i = 1; i < aParamsCount; eOpen(ParamStr(i++)));
    // adjust preview paramters
    ESTART
    aSoundPreview = false;
    aWaveStruct = new ionstd::WaveStruct
        (aPRVRate, 32, aPRVChannels, ionstd::WaveFormatIEEEFloat);
    WAVEFORMATEX aWaveFmt;
    aWaveFmt.wFormatTag = aWaveStruct->Format();
    aWaveFmt.nChannels = aWaveStruct->Channels();
    aWaveFmt.nSamplesPerSec = aWaveStruct->SampleRate();
    aWaveFmt.nAvgBytesPerSec = aWaveStruct->AvgBytesPerSec();
    aWaveFmt.nBlockAlign = aWaveStruct->BlockAlign();
    aWaveFmt.wBitsPerSample = aWaveStruct->BitsPerSample();
    aWaveFmt.cbSize = 0;
    int Result = waveOutOpen(&aPRVHandle, aPRVDevice, &aWaveFmt,
        MAKELONG(Handle, 0), 0, CALLBACK_WINDOW);
    waveOutSetVolume(aPRVHandle, 0xffffffffu);
    delete aWaveStruct; // delete temporary wave structure
    if (Result == WAVERR_BADFORMAT) THROW_RE(snreNoSupport);
    EEND
    aPRVElement = NIL;
    LoadPreviewDirectories();
    for (int i = 0; i < PreviewCategory->Items->Count; i++)
    {
        if (PreviewCategory->Items->operator[](i) == aPRVCategory)
        {
            PreviewCategory->ItemIndex = i;
            break;
        }
    }
    LoadPreviewProbes();
    for (int i = 0; i < PreviewProbe->Items->Count; i++)
    {
        if (PreviewProbe->Items->operator[](i) == aPRVProbe)
        {
            PreviewProbe->ItemIndex = i;
            break;
        }
    }
    PreviewProbe->OnChange(this);
    Refresh();
}

void __fastcall TMainForm::FormDestroy(TObject *Sender)
{
    waveOutClose(aPRVHandle);
}

/***************************************************************************
 *  File
 **************************************************************************/

void __fastcall TMainForm::ANewExecute(TObject *Sender)
{
    ESTART
    TabImage aTabImage;
    aTabImage.aModified = true;
    ionstd::Paramlist aParamlist;
    aTabImage.aName = Adapt(aParamlist.Name());
    aTabImage.aAuthor = Adapt(aParamlist.Author());
    aTabImage.aComment = Adapt(aParamlist.Comment());
    aTabImage.aCopyright = Adapt(aParamlist.Copyright());
    aTabImages.push_back(aTabImage);
    aSelector = aTabImages.size() - 1;
        TGrpButtonItem *aNewTab = Selector->Items->Add();
    aNewTab->Caption = GetLastName(UnicodeString());
    EEND
    Refresh();
}

void __fastcall TMainForm::AOpenExecute(TObject *Sender)
{
    ESTART
    if (!OpenDialog->Execute()) return; // user did't press ok
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
            if (eOpen((*OpenDialog->Files)[d]))
            {
                Refresh();
            }
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

void __fastcall TMainForm::APropertiesExecute(TObject *Sender)
{
    ESTART
    CieArgForm->Invoke("Paramlist Name", *GetName());
    if (CieArgForm->ModalResult == mrCancel) return;
    *GetName() = CieArgForm->ResultAsString();
    *GetModified() = true;
    Refresh();
    CieArgForm->Invoke("Author", *GetAuthor());
    if (CieArgForm->ModalResult == mrCancel) return;
    *GetAuthor() = CieArgForm->ResultAsString();
    *GetModified() = true;
    Refresh();
    CieArgForm->Invoke("Comment", *GetComment());
    if (CieArgForm->ModalResult == mrCancel) return;
    *GetComment() = CieArgForm->ResultAsString();
    *GetModified() = true;
    Refresh();
    CieArgForm->Invoke("Copyright", *GetCopyright());
    if (CieArgForm->ModalResult == mrCancel) return;
    *GetCopyright() = CieArgForm->ResultAsString();
    *GetModified() = true;
    Refresh();
    EEND
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
    // first: create a translator
    aWaveStruct = new ionstd::WaveStruct
        (aPRVRate, 32, aPRVChannels, ionstd::WaveFormatIEEEFloat);
    double aReleaseTime = aPRVRelease;
    aTranslator = new ionstd::Translator(aPRVElement, aWaveStruct);
    aTranslator->EndTime(aPRVElement->Time() + aReleaseTime);
    aTranslator->ForwardMargin(aPRVFMargin);
    aTranslator->BackwardMargin(aPRVBMargin);
    aTranslator->SetTimbre(GetSonority(), 0);
    aTranslator->SetGain(0., 0); // unnecessary
    aTranslator->GetReady();
    // second: put sonorities into the work
    aPRVThread = new VoiceThread(aTranslator, 0);
    // thrid: let it sound
    aSoundPreview = true;
    APreview->Enabled = false;
    AStopPreview->Enabled = true;
    EnableControls(false);
    aPRVHeader.lpData = (char *)aWaveStruct->GetData();
    aPRVHeader.dwBufferLength = aWaveStruct->BytesInData();
    waveOutPrepareHeader(aPRVHandle, &aPRVHeader, sizeof(WAVEHDR));
    waveOutWrite(aPRVHandle, &aPRVHeader, sizeof(WAVEHDR));
    EEND
}

void __fastcall TMainForm::AStopPreviewExecute(TObject *Sender)
{
    waveOutReset(aPRVHandle);
}

/***************************************************************************
 *  Help
 **************************************************************************/

void __fastcall TMainForm::AAboutExecute(TObject *Sender)
{
    CieAbtForm->Invoke();
}

/***************************************************************************
 *  Controls
 **************************************************************************/

void __fastcall TMainForm::FormResize(TObject *Sender)
{
    int aWidth = ClientWidth;
    aWidth /= 2.;
    Dynamic->Width = aWidth;
    Signal->Width = aWidth;
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

void __fastcall TMainForm::DynamicButtonClicked(TObject *Sender, int IndexArg)
{
    EditDynamic(IndexArg);
}

void __fastcall TMainForm::SignalButtonClicked(TObject *Sender, int IndexArg)
{
    EditSignal(IndexArg);
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

void __fastcall TMainForm::PreviewCategoryChange(TObject *Sender)
{
    LoadPreviewProbes();
    LoadProbe();
}

void __fastcall TMainForm::PreviewProbeChange(TObject *Sender)
{
    LoadProbe();
}
