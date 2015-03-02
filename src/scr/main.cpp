/***************************************************************************
 *  C++ source
 *
 *  ION Score Editor
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
#include <cieprg.h>
#include <cieslc.h>

#pragma package(smart_init)
#pragma resource "*.dfm"

TMainForm *MainForm;

const UnicodeString PropertyName[SegmentPC] =
{
    /* 0 */ "Type",
    /* 1 */ "Label",
    /* 2 */ "Depth",
    /* 3 */ "Duration",
    /* 4 */ "Repeats",
    /* 5 */ "Reversal",
    /* 6 */ "Panoram",
    /* 7 */ "Frequency",
    /* 8 */ "Volume",
    /* 9 */ "Harmonic"
}; // PropertyName

const int PropertyTypeImage[SegmentPC] =
{
    /* 0 */ 24,
    /* 1 */ 27,
    /* 2 */ 25,
    /* 3 */ 26,
    /* 4 */ 25,
    /* 5 */ 24,
    /* 6 */ 28,
    /* 7 */ 28,
    /* 8 */ 28,
    /* 9 */ 24
}; // PropertyTypeImage

const UnicodeString ElementTypeName[ElementTC] =
{
    /* 0 */ "Pause",
    /* 1 */ "Sound",
    /* 2 */ "Segment",
    /* 3 */ "Link",
    /* 4 */ "Record"
}; // ElementTypeName

ionstd::String screNoSupport("your device does not support the wave format");

namespace ionstd { PLACEEXCEPTIONFNCHERE }

/***************************************************************************
 *  Auxiliary Methods
 **************************************************************************/

__fastcall VoiceThread::VoiceThread
    (const ionstd::Translator *TranslatorArg, unsigned int VoiceArg) :
    TThread(false /* create suspended */),
    aTranslator(TranslatorArg),
    aVoice(VoiceArg)
{ }

void __fastcall VoiceThread::Execute()
{
    ESTART
    while (!Terminated && aTranslator->TranslateBlock(aVoice));
    EEND
}

void __fastcall TMainForm::GoIn(unsigned int IndexArg)
{
    ionstd::Segment *aSegment = GetActualAsSegment();
    ionstd::ElementPointer aElement = aSegment->GetElement(IndexArg);
    PushToPath(aElement);
    AClearSelection->Execute();
    Refresh();
}

void __fastcall TMainForm::GoBack(unsigned int CountArg)
{
    while (!(GetDepth() < 2) && CountArg--) PopFromPath();
    AClearSelection->Execute();
    Refresh();
}

#define STDFNCEDITORCALL(TITLE, CASE)\
if (IsActualSound()) \
{ \
    aFunction = GetActualAsSound()->CASE(); \
    CieFncForm->Invoke(TITLE, aFunction, &s ## CASE ## FEP); \
    if (CieFncForm->ModalResult == mrCancel) return; \
} \
if (IsActualSegment()) \
{ \
    aFunction = GetActualAsSegment()->CASE(); \
    CieFncForm->Invoke(TITLE, aFunction, &a ## CASE ## FEP); \
    if (CieFncForm->ModalResult == mrCancel) return; \
}

void __fastcall TMainForm::EditProperty(unsigned int IndexArg)
{
    ESTART
    ionstd::FunctionPointer aFunction;
    double aDuration;
    unsigned int aRepeats;
    bool aReversal;
    switch (IndexArg)
    {
    case 0 : /* Type */ return; // read only
    case 1 : /* Label */
    CieArgForm->Invoke("Label", Adapt(GetActual()->Label()));
    if (CieArgForm->ModalResult != mrOk) return;
    GetActual()->Label(Adapt(CieArgForm->ResultAsString()));
    break; /* Label */
    case 2 : /* Harmonic Depth */ return; // read only
    case 3 : /* Duration */
    if (IsActualSegment() || IsActualLink()) return; // read only
    if (IsActualRecord())
    {
        if (AskForWave(GetActual())) break;
        else return;
        }
    if (IsActualPause()) aDuration = GetActualAsPause()->GetDuration();
    if (IsActualSound()) aDuration = GetActualAsSound()->GetDuration();
    CieArgForm->Invoke("Duration", aDuration);
    if (CieArgForm->ModalResult != mrOk) return;
    aDuration = CieArgForm->ResultAsDouble();
    if (IsActualPause()) GetActualAsPause()->SetDuration(aDuration);
    if (IsActualSound()) GetActualAsSound()->SetDuration(aDuration);
    break; /* Duration */
    case 4 : /* Repeats */
    if (IsActualSound()) aRepeats = GetActualAsSound()->Repeats();
    if (IsActualSegment()) aRepeats = GetActualAsSound()->Repeats();
    CieArgForm->Invoke("Repeats", aRepeats);
    if (CieArgForm->ModalResult != mrOk) return;
    aRepeats = CieArgForm->ResultAsInt();
    if (IsActualSound()) GetActualAsSound()->Repeats(aRepeats);
    if (IsActualSegment()) GetActualAsSegment()->Repeats(aRepeats);
    break; /* Repeats */
    case 5 : /* Reversal */
    if (IsActualSound()) aReversal = GetActualAsSound()->Reversal();
    if (IsActualSegment()) aReversal = GetActualAsSegment()->Reversal();
    CieArgForm->Invoke("Reversal Mode", aReversal);
    if (CieArgForm->ModalResult != mrOk) return;
    aReversal = CieArgForm->ResultAsBool();
    if (IsActualSound()) GetActualAsSound()->Reversal(aReversal);
    if (IsActualSegment()) GetActualAsSegment()->Reversal(aReversal);
    break; /* Reversal */
    case 6 : /* Panoram */ STDFNCEDITORCALL("Panoram", Panoram); break;
    case 7 : /* Frequency */ STDFNCEDITORCALL("Frequency", Frequency); break;
    case 8 : /* Volume */ STDFNCEDITORCALL("Volume", Volume); break;
    case 9 : /* Harmonic */
    CieArgForm->Invoke("Harmonic Mode", GetActualAsSegment()->Kind());
    if (CieArgForm->ModalResult != mrOk) return;
    GetActualAsSegment()->Kind(CieArgForm->ResultAsBool());
    break; /* Harmonic */
    default : return;
    }
    EEND
    *GetModified() = true;
    Refresh();
}

void __fastcall TMainForm::EditTimeSetting(unsigned int IndexArg)
{
    ESTART
    UnicodeString aCaption;
    switch (IndexArg)
    {
        case 0 : aCaption = "Forward Margin"; break;
        case 1 : aCaption = "Start Time"; break;
        case 2 : aCaption = "End Time"; break;
        case 3 : aCaption = "Backward Margin"; break;
    }
    double aValue;
    switch (IndexArg)
    {
        case 0 : aValue = GetITP()->ForwardMargin(); break;
        case 1 : aValue = GetITP()->StartTime(); break;
        case 2 : aValue = GetITP()->EndTime(); break;
        case 3 : aValue = GetITP()->BackwardMargin(); break;
    }
    CieArgForm->Invoke(aCaption, aValue);
    if (CieArgForm->ModalResult != mrOk) return;
    aValue = CieArgForm->ResultAsDouble();
    switch (IndexArg)
    {
        case 0 : GetITP()->ForwardMargin(aValue); break;
        case 1 : GetITP()->StartTime(aValue); break;
        case 2 : GetITP()->EndTime(aValue); break;
        case 3 : GetITP()->BackwardMargin(aValue); break;
    }
    EEND
    *GetModified() = true;
    Refresh();
}

void __fastcall TMainForm::EditSonority(unsigned int IndexArg)
{
    ESTART
    UnicodeString aIndex = UIntToStr(IndexArg);
    CieArgForm->Invoke("Sonority " + aIndex,
        Adapt(GetITP()->SonorityLabel(IndexArg)));
    if (CieArgForm->ModalResult != mrOk) return;
    ionstd::String aLabel = Adapt(CieArgForm->ResultAsString());
    GetITP()->SonorityFile(aLabel + ".ionsnr", IndexArg);
    GetITP()->SonorityLabel(aLabel, IndexArg);
    CieArgForm->Invoke("Gain " + aIndex,
        GetITP()->SonorityGain(IndexArg));
    if (CieArgForm->ModalResult != mrOk) return;
    double aGain = CieArgForm->ResultAsDouble();
    GetITP()->SonorityGain(aGain, IndexArg);
    EEND
    *GetModified() = true;
    Refresh();
}

UnicodeString __fastcall TMainForm::MakeFncImage
    (ionstd::ConstFunctionPointer FunctionArg) const
{
    UnicodeString aResult;
    aResult += FunctionArg->Min();
    aResult += " : ";
    aResult += FunctionArg->Max();
    aResult += ", " + UIntToStr(FunctionArg->PointsCount()) + "p ";
    aResult += FunctionArg->Smooth() ? "l" : "s";
    return aResult;
}

UnicodeString __fastcall TMainForm::MakePropertyD(unsigned int IndexArg) const
{
    ionstd::FunctionPointer aFunction;
    switch (IndexArg)
    {
    case 0 : /* Type */ return ElementTypeName[GetActual()->Type()];
    case 1 : /* Label */ return Adapt(GetActual()->Label());
    case 2 : /* Harmonic Depth */ return UIntToStr(GetActual()->VoicesCount());
    case 3 : /* Duration */
    if (IsActualPause()) return FloatToStr(GetActualAsPause()->GetDuration());
    if (IsActualSound()) return FloatToStr(GetActualAsSound()->GetDuration());
    if (IsActualSegment()) return FloatToStr(GetActualAsSegment()->GetDuration());
    if (IsActualLink()) return FloatToStr(GetActualAsLink()->Time());
    if (IsActualRecord()) return FloatToStr(GetActualAsRecord()->Time());
    case 4 : /* Number of Repeats */
    if (IsActualSound()) return UIntToStr(GetActualAsSound()->Repeats());
    if (IsActualSegment()) return UIntToStr(GetActualAsSegment()->Repeats());
    case 5 : /* Reversal Mode */
    bool aReversal;
    if (IsActualSound()) aReversal = GetActualAsSound()->Reversal();
    if (IsActualSegment()) aReversal = GetActualAsSegment()->Reversal();
    return (aReversal) ? "true" : "false";
    case 6 : /* Panoram */
    if (IsActualSound()) aFunction = GetActualAsSound()->Panoram();
    if (IsActualSegment()) aFunction = GetActualAsSegment()->Panoram();
    return MakeFncImage(aFunction);
    case 7 : /* Frequency */
    if (IsActualSound()) aFunction = GetActualAsSound()->Frequency();
    if (IsActualSegment()) aFunction = GetActualAsSegment()->Frequency();
    return MakeFncImage(aFunction);
    case 8 : /* Volume */
    if (IsActualSound()) aFunction = GetActualAsSound()->Volume();
    if (IsActualSegment()) aFunction = GetActualAsSegment()->Volume();
    return MakeFncImage(aFunction);
    case 9 : /* Harmonic Mode */
    bool aHarmonic = GetActualAsSegment()->Kind();
    return (aHarmonic ? "true" : "false");
    }
    return "UNKNOWN";
}

UnicodeString __fastcall TMainForm::MakeElementD(unsigned int IndexArg) const
{
    ionstd::ElementPointer aElement = GetActualAsSegment()->GetElement(IndexArg);
    UnicodeString aResult;
    if (aElement->Type() == ionstd::etSegment)
    {
        ionstd::Segment *aSegment = dynamic_cast<ionstd::Segment *>(aElement);
        aResult += "[", aResult += UIntToStr(aSegment->ElementsCount()) + "] ";
    }
    aResult += FloatToStr(aElement->Time()) + " sec, ";
    aResult += UIntToStr(aElement->VoicesCount()) + " vcs";
    return aResult;
}

UnicodeString __fastcall TMainForm::MakeTimeSettingD(unsigned int IndexArg) const
{
    switch (IndexArg)
    {
        case 0 : return GetITP()->ForwardMargin();
        case 1 : return GetITP()->StartTime();
        case 2 : return GetITP()->EndTime();
        case 3 : return GetITP()->BackwardMargin();
    }
    return "UNKNOWN";
}

int __fastcall TMainForm::MakeImageIndex
    (ionstd::ElementPointer Arg, bool SelectedArg) const
{
        int aResult;
    switch (Arg->Type())
    {
        case 0 : /* Pause */ aResult = 0; break;
        case 1 : /* Sound */ aResult = 1; break;
        case 2 : /* Segment */
        if (dynamic_cast<ionstd::Segment *>(Arg)->Kind()) aResult = 3;
        else aResult = 2;
        break;
        case 3 : /* Link */ aResult = 4; break;
        case 4 : /* Record */ aResult = 5; break;
        default : return -1;
    }
    if (SelectedArg) aResult += 6;
    return aResult;
}

bool __fastcall TMainForm::AnyoneSelected() const
{
    bool aResult = false;
    SelectionVector::const_iterator i = aSelection.begin();
    while (i != aSelection.end()) aResult |= *i++;
    return aResult;
}

void __fastcall TMainForm::ClearClipboard()
{
    ClipboardStore::iterator i = aClipboard.begin();
    while (i != aClipboard.end()) delete *i++;
    aClipboard.clear();
}

void __fastcall TMainForm::SelectedToClipboard()
{
    ionstd::Segment *aSegment = GetActualAsSegment();
    SelectionVector::iterator i = aSelection.begin();
    while (i != aSelection.end())
    {
        if (*i) aClipboard.push_back
            (ionstd::CopyElement(aSegment->
                GetElement(i - aSelection.begin())));
        i++;
    }
}

void __fastcall TMainForm::DeleteSelected()
{
    ionstd::Segment *aSegment = GetActualAsSegment();
    int aCorrection = 0;
    SelectionVector::iterator i = aSelection.begin();
    while (i != aSelection.end())
    {
        if (*i)
        {
            aSegment->Erase
                ((i - aSelection.begin()) - aCorrection++);
            *i = false;
        }
        i++;
    }
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
    if (aTabImages.empty()) return UnicodeString();
    UnicodeString aResult = ElementTypeName[GetActual()->Type()];
    if (IsActualSegment())
    {
        ionstd::Segment *aSegment = GetActualAsSegment();
        aResult += " [", aResult += UIntToStr(aSegment->ElementsCount()) + "]";
    }
    aResult += " \"";
    aResult += Adapt(GetActual()->Label()) + "\" ";
    aResult += FloatToStr(GetActual()->Time()) + " sec, ";
    aResult += UIntToStr(GetActual()->VoicesCount()) + " vcs";
    return aResult;
}

bool __fastcall TMainForm::AskForDuration(ionstd::ElementPointer ElementArg)
{
    if (ElementArg->Type() == ionstd::etSegment) return true;
    if (ElementArg->Type() == ionstd::etLink) return true;
    if (ElementArg->Type() == ionstd::etRecord) return true;
    double aDuration;
    if (ElementArg->Type() == ionstd::etPause) aDuration =
        (dynamic_cast<ionstd::Pause *>(ElementArg))->GetDuration();
    if (ElementArg->Type() == ionstd::etSound) aDuration =
        (dynamic_cast<ionstd::Sound *>(ElementArg))->GetDuration();
    CieArgForm->Invoke("Duration", aDuration);
    if (CieArgForm->ModalResult == mrOk)
    {
        aDuration = CieArgForm->ResultAsDouble();
        if (ElementArg->Type() == ionstd::etPause)
        {
            ionstd::Pause *aPause =
                dynamic_cast<ionstd::Pause *>(ElementArg);
            aPause->SetDuration(aDuration);
        }
        else /* sound */
        {
            ionstd::Sound *aSound =
                dynamic_cast<ionstd::Sound *>(ElementArg);
            aSound->SetDuration(aDuration);
            aSound->Panoram()->Max(aDuration);
            aSound->Frequency()->Max(aDuration);
            aSound->Volume()->Max(aDuration);
        }
        return true;
    }
    else return false;
}

bool __fastcall TMainForm::AskForWave(ionstd::ElementPointer ElementArg)
{
    if (ElementArg->Type() != ionstd::etRecord) return true;
    if (!ImportPCM->Execute()) return false;
    ionstd::WaveStruct aWave(Adapt(ImportPCM->FileName));
    (dynamic_cast<ionstd::Record *>(ElementArg))->AdaptRecord(aWave);
    return true;
}

void __fastcall TMainForm::ApplyDefaults(ionstd::ITP *ITPArg) const
{
    ITPArg->DefaultSonorityFile(dSonorityFile);
    ITPArg->DefaultSonorityLabel(dSonorityLabel);
    ITPArg->DefaultSonorityGain(dSonorityGain);
    ITPArg->ForwardMargin(dFMargin);
    ITPArg->BackwardMargin(dBMargin);
    ITPArg->StartTime(dStart);
}

bool __fastcall TMainForm::eOpen(UnicodeString FileNameArg)
{
    ESTART
    ionstd::Paramlist aParamlist;
    aParamlist.Load(Adapt(FileNameArg));
    ionstd::String aTopMap = Adapt(aDefaultLabel);
    ionstd::ElementPointer aScore = ionstd::ReadElement(aParamlist, aTopMap);
    TabImage aTabImage;
    ApplyDefaults(&aTabImage.aITP);
    if (ionstd::HasITP(aParamlist, aTopMap))
        aTabImage.aITP.ReadFor(aParamlist, aTopMap);
    else aTabImage.aITP.AdjustFor(*aScore);
    aTabImage.aScore = aScore;
    aTabImage.aFileName = FileNameArg;
    aTabImage.aModified = false;
    aTabImage.aName = Adapt(aParamlist.Name());
    aTabImage.aAuthor = Adapt(aParamlist.Author());
    aTabImage.aComment = Adapt(aParamlist.Comment());
    aTabImage.aCopyright = Adapt(aParamlist.Copyright());
    aTabImage.aPath.push_back(aTabImage.aScore); // root of structure
    aTabImages.push_back(aTabImage); // one more tab image
    // make new tab:
    TGrpButtonItem *aNewTab;
    aNewTab = Selector->Items->Add();
    aNewTab->Caption = GetLastName(FileNameArg);
    // now select our new tab:
    aSelector = aTabImages.size() - 1;
    EEND
    return true; // if the opening is successful, we return true
}

void __fastcall TMainForm::eSave(UnicodeString FileNameArg)
{
    ESTART
    ionstd::Paramlist aParamlist;
    GetScore()->WriteTo(aParamlist);
    GetITP()->WriteFor(aParamlist, GetScore()->Label());
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
    delete GetScore();
    // delete image:
    aTabImages.erase(aTabImages.begin() + aSelector); // erase image
    Selector->Items->Delete(aSelector); // delete button
    // if it was the last tab, then perform left shift
    if (aSelector == aTabImages.size()) aSelector--;
    Refresh();
}

void __fastcall TMainForm::EnableControls(bool Arg)
{
    Selector->Enabled = Arg;
    Position->Enabled = Arg;
    Properties->Enabled = Arg;
    Subelements->Enabled = Arg;
    TimeSettings->Enabled = Arg;
    Sonorities->Enabled = Arg;
}

void __fastcall TMainForm::Refresh()
{
    // first: refresh controlls
    bool aTabsExist = !aTabImages.empty();
    Position->Visible = aTabsExist;
    Selector->Visible = aTabsExist;
    Properties->Visible = aTabsExist;
    Subelements->Visible = aTabsExist;
    Sonorities->Visible = aTabsExist;
    TimeSettings->Visible = aTabsExist;
    ASave->Enabled = aTabsExist && (*GetModified());
    ASaveAs->Enabled = aTabsExist;
    ASaveAll->Enabled = aTabsExist;
    AClose->Enabled = aTabsExist;
    ACloseAll->Enabled = aTabsExist;
    AProperties->Enabled = aTabsExist;
    bool aAnyoneSelected = AnyoneSelected();
    ACut->Enabled = aTabsExist && aAnyoneSelected;
    ACopy->Enabled = aTabsExist && aAnyoneSelected;
    APaste->Enabled = aTabsExist && !aClipboard.empty() && IsActualSegment();
    ASelectAll->Enabled = aTabsExist && IsActualSegment()
        && GetActualAsSegment()->ElementsCount();
    AClearSelection->Enabled = aTabsExist && IsActualSegment()
        && AnyoneSelected();
    ANewElement->Enabled = aTabsExist && IsActualSegment();
    ARemoveElement->Enabled = aTabsExist && AnyoneSelected();
    AShiftUp->Enabled = aTabsExist && aAnyoneSelected;
    AShiftDown->Enabled = aTabsExist && aAnyoneSelected;
    ATranslate->Enabled = aTabsExist;
    APreview->Enabled = aTabsExist && !aSoundPreview;
    AStopPreview->Enabled = aTabsExist && aSoundPreview;
    // second: statistic
    Caption = GenerateFormCaption();
    StatusBar->Panels->Items[1]->Text = GenerateStatisticText();
    if (aTabsExist)
    StatusBar->Panels->Items[2]->Text = *GetModified() ? "Modified" : "";
    else StatusBar->Panels->Items[2]->Text = "";
    if (!aTabsExist) return;
    // third: creating all the buttons
    if (GetScore()->Type() == ionstd::etSegment)
        (dynamic_cast<ionstd::Segment *>(GetScore()))->Refresh();
    // 3.1 - show position stack
    TGrpButtonItem *aButton;
    Position->Items->BeginUpdate();
    Position->Items->Clear();
    PathVector::iterator e = (aTabImages.begin() + aSelector)->aPath.begin();
    while (e != (aTabImages.begin() + aSelector)->aPath.end())
    {
        aButton = Position->Items->Add();
        aButton->Caption = Adapt((**e).Label());
        aButton->ImageIndex = MakeImageIndex(*e++, false);
    }
    Position->Items->EndUpdate();
    // 3.2 - show properties
    Properties->Items->BeginUpdate();
    Properties->Items->Clear();
    int aCount;
    if (IsActualPause()) aCount = PausePC;
    if (IsActualSound()) aCount = SoundPC;
    if (IsActualSegment()) aCount = SegmentPC;
    if (IsActualLink()) aCount = LinkPC;
    if (IsActualRecord()) aCount = RecordPC;
    for (int i = 0; i < aCount; i++)
    {
        aButton = Properties->Items->Add();
        aButton->Caption = *(PropertyName + i) + " = ";
        aButton->Caption += MakePropertyD(i);
        aButton->ImageIndex = *(PropertyTypeImage + i);
    }
    Properties->Items->EndUpdate();
    // 3.3 - show subelements
    if (!AnyoneSelected()) Subelements->Items->BeginUpdate();
    aCount = IsActualSegment() ? GetActualAsSegment()->ElementsCount() : 0;
    aSelection.resize(aCount, false);
    for (int i = 0; i < aCount; i++)
    {
        if (i < Subelements->Items->Count)
            aButton = Subelements->Items->operator[](i);
        else aButton = Subelements->Items->Add();
        ionstd::ElementPointer aItem = GetActualAsSegment()->GetElement(i);
        aButton->Caption = Adapt(aItem->Label()) + ", ";
        aButton->Caption += MakeElementD(i);
        aButton->ImageIndex = MakeImageIndex
            (aItem, *(aSelection.begin() + i));
    }
    while (aCount < Subelements->Items->Count)
        Subelements->Items->Delete(aCount);
    if (!AnyoneSelected()) Subelements->Items->EndUpdate();
    // 3.4 - refresh ITP time settings
    TimeSettings->Items->BeginUpdate();
    GetITP()->AdjustFor(*GetScore());
    aCount = TimeSettings->Items->Count;
    for (int i = 0; i < aCount; i++)
    {
        aButton = TimeSettings->Items->operator[](i);
        UnicodeString aCaption = aButton->Caption;
        aButton->Caption = aCaption.SubString(0, aCaption.Pos("="))
            + ' ' + MakeTimeSettingD(i);
    }
    TimeSettings->Items->EndUpdate();
    // 3.5 - show ITP sonorities and load sonorities if needed
    Sonorities->Items->BeginUpdate();
    Sonorities->Items->Clear();
    aPRVSonorities.resize(GetScore()->VoicesCount(), NIL);
    aCount = GetScore()->VoicesCount();
    for (int i = 0; i < aCount; i++)
    {
        // show buttons:
        aButton = Sonorities->Items->Add();
        aButton->ImageIndex = 29;
        aButton->Caption = Adapt(GetITP()->SonorityLabel(i));
        double aGain = GetITP()->SonorityGain(i);
        aButton->Caption += (aGain >= 0) ? " +" : " ";
        aButton->Caption += FloatToStrF(aGain, ffFixed, 2, 2);
        // load sonority:
        if (!*(aPRVSonorities.begin() + i))
            *(aPRVSonorities.begin() + i) = new ionstd::Sonority;
        if ((**(aPRVSonorities.begin() + i)).Label()
            != GetITP()->SonorityLabel(i))
        {
            ESTART
            ionstd::Paramlist aList;
            aList.Load(MergePath(ionstd::String("sonorities\\")
                + GetITP()->SonorityFile(i)));
            (**(aPRVSonorities.begin() + i)).
                ReadFrom(aList, GetITP()->SonorityLabel(i));
            EEND
        }
    }
    Sonorities->Items->EndUpdate();
    // fourth: refresh icons of active files
    if (*GetModified()) Selector->Items->operator[](aSelector)->ImageIndex = 23;
    else Selector->Items->operator[](aSelector)->ImageIndex = 22;
    // fifth: resize
    FormResize(this);
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
        PRVThreads::iterator i = aPRVThreads.begin();
        while (i != aPRVThreads.end())
        {
            (**i).Terminate();
            delete *i++;
        }
        aPRVThreads.clear();
        delete aWaveStruct;
        delete aTranslator;
        aSoundPreview = false;
        APreview->Enabled = true;
        AStopPreview->Enabled = false;
        EnableControls(true);
    }
    else TMainForm::Dispatch(&MessageArg);
}

__fastcall TMainForm::TMainForm(TComponent* Owner) :
    TForm         (Owner),
    aSelector     (-1),
    sPanoramFEP   (PanoramDefaults),
    sFrequencyFEP (FrequencyDefaults),
    sVolumeFEP    (VolumeDefaults),
    aPanoramFEP   (PanoramDefaults),
    aFrequencyFEP (DefaultPreferences),
    aVolumeFEP    (DefaultPreferences)
{ }

void __fastcall TMainForm::FormCreate(TObject *Sender)
{
    CieAbtForm = new TCieAbtForm(Application);
    CieArgForm = new TCieArgForm(Application);
    CieChsForm = new TCieChsForm(Application);
    CieCnfForm = new TCieCnfForm(Application);
    CieExcForm = new TCieExcForm(Application);
    CieFncForm = new TCieFncForm(Application);
    CiePrgForm = new TCiePrgForm(Application);
    CieSlcForm = new TCieSlcForm(Application);
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
    // load default part:
    if (aList.Exists("scr", "dLabel"))
    aDefaultLabel = Adapt(aList.Data<ionstd::String>("scr", "dLabel"));
    // load ITP parameters:
    if (aList.Exists("itp", "dSonorityFile"))
    dSonorityFile = aList.Data<ionstd::String>("itp", "dSonorityFile");
    if (aList.Exists("itp", "dSonorityLabel"))
    dSonorityLabel = aList.Data<ionstd::String>("itp", "dSonorityLabel");
    if (aList.Exists("itp", "dSonorityGain"))
    dSonorityGain = aList.Data<ionstd::real>("itp", "dSonorityGain");
    else dSonorityGain = 0.;
    if (aList.Exists("itp", "dFMargin"))
    dFMargin = aList.Data<ionstd::time>("itp", "dFMargin");
    else dFMargin = 0.;
    if (aList.Exists("itp", "dBMargin"))
    dBMargin = aList.Data<ionstd::time>("itp", "dBMargin");
    else dBMargin = 0.;
    if (aList.Exists("itp", "dStart"))
    dStart = aList.Data<ionstd::time>("itp", "dStart");
    else dStart = 0.;
    // load paramters for preview:
    aPRVRate = aList.Data<ionstd::numerator>("prv", "Rate");
    aPRVChannels = aList.Data<ionstd::numerator>("prv", "Channels");
    aPRVDevice = aList.Data<ionstd::numerator>("prv", "Device");
    aPRVRelease = aList.Data<ionstd::time>("prv", "dRelease");
    aPRVFMargin = aList.Data<ionstd::time>("prv", "dFMargin");
    aPRVBMargin = aList.Data<ionstd::time>("prv", "dBMargin");
    // load defaults for translation:
    tdSampleRate = aList.Data<ionstd::numerator>("iwt", "dRate");
    tdBitDepth = aList.Data<ionstd::numerator>("iwt", "dBitDepth");
    tdMono = aList.Data<bool>("iwt", "dMono");
    tdFormat = aList.Data<ionstd::numerator>("iwt", "dFormat");
    tdNormalization = aList.Data<bool>("iwt", "dNormalization");
    tdDither = aList.Data<bool>("iwt", "dDither");
    EEND
    // tune additional parameters
    int aParamsCount = ParamCount() + 1;
    for (int i = 1; i < aParamsCount; eOpen(ParamStr(i++)));
    // adjust preview parameters
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
    if (Result == WAVERR_BADFORMAT) THROW_RE(screNoSupport);
    EEND
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
    CieChsForm->Invoke("Element Type", ElementType, "psmhlr");
    if (CieChsForm->ModalResult == mrCancel) return;
    ionstd::ElementPointer aElement = NIL;
    switch (CieChsForm->GetIndex())
    {
        case 0 : /* Pause */ aElement = new ionstd::Pause; break;
        case 1 : /* Sound */ aElement = new ionstd::Sound; break;
        case 2 : /* Melodic Segment */ aElement = new ionstd::Segment;
        dynamic_cast<ionstd::Segment *>(aElement)->Kind(ionstd::melodic);
        break;
        case 3 : /* Harmonic Segment */ aElement = new ionstd::Segment;
        dynamic_cast<ionstd::Segment *>(aElement)->Kind(ionstd::harmonic);
        break;
        case 4 : /* Link */ aElement = new ionstd::Link; break;
        case 5 : /* Record */ aElement = new ionstd::Record; break;
    }
    aTabImage.aScore = aElement;
    ApplyDefaults(&aTabImage.aITP);
    aTabImage.aITP.AdjustFor(*(aTabImage.aScore));
    aTabImage.aScore->Label(Adapt(aDefaultLabel));
    if (!(AskForDuration(aTabImage.aScore) && AskForWave(aTabImage.aScore)))
    {
        delete aTabImage.aScore;
        return;
    }
    ionstd::Paramlist aParamlist;
    aTabImage.aName = Adapt(aParamlist.Name());
    aTabImage.aAuthor = Adapt(aParamlist.Author());
    aTabImage.aComment = Adapt(aParamlist.Comment());
    aTabImage.aCopyright = Adapt(aParamlist.Copyright());
    aTabImage.aPath.push_back(aTabImage.aScore);
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
    if (!OpenDialog->Execute()) return;
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
            case mrYes  : ASave->Execute();
            case mrNo   : aClosingCancel = false; break;
            case mrCancel   : aClosingCancel = true; break;
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

double TranslationProgress()
{
    return MainForm->aTranslator->Progress();
}

void AfterTranslation()
{
    ESTART
    MainForm->aTranslator->Reset();
    if (MainForm->tdNormalization) MainForm->aWaveStruct->Normalize();
    if (MainForm->tdDither) MainForm->aWaveStruct->Dither();
    MainForm->aWaveStruct->Save(MainForm->aOut);
    EEND
}

void __fastcall TMainForm::ATranslateExecute(TObject *Sender)
{
    ESTART
    ionstd::ElementPointer aSource = GetScore();
    // first: get some info
    if (!ExportPCM->Execute()) return;
    aOut = Adapt(ExportPCM->FileName);
    CieArgForm->Invoke("Sample Rate, kHz", tdSampleRate / 1000.);
    if (CieArgForm->ModalResult == mrCancel) return;
    tdSampleRate = CieArgForm->ResultAsDouble() * 1000;
    CieArgForm->Invoke("Bit Depth", tdBitDepth);
    if (CieArgForm->ModalResult == mrCancel) return;
    tdBitDepth = CieArgForm->ResultAsInt();
    CieArgForm->Invoke("Format (1 = int, 3 = float)", tdFormat);
    if (CieArgForm->ModalResult == mrCancel) return;
    tdFormat = CieArgForm->ResultAsInt();
    CieArgForm->Invoke("Mono", tdMono);
    if (CieArgForm->ModalResult == mrCancel) return;
    tdMono = CieArgForm->ResultAsBool();
    CieArgForm->Invoke("Normalization", tdNormalization);
    if (CieArgForm->ModalResult == mrCancel) return;
    tdNormalization = CieArgForm->ResultAsBool();
    CieArgForm->Invoke("Dither", tdDither);
    if (CieArgForm->ModalResult == mrCancel) return;
    tdDither = CieArgForm->ResultAsBool();
    CieSlcForm->Invoke("Render", 0xffffffff, aSource->VoicesCount());
    if (CieSlcForm->ModalResult == mrCancel) return;
    // second: prepare essences for translation
    aWaveStruct = new ionstd::WaveStruct
        (tdSampleRate, tdBitDepth, tdMono ? 1 : 2, tdFormat);
    aTranslator = new ionstd::Translator(aSource, aWaveStruct);
    for (ionstd::numerator i = 0; i < aSource->VoicesCount(); i++)
    {
        if ((CieSlcForm->GetMask() >> i) & 1)
            aTranslator->SetTimbre(aPRVSonorities[i], i);
        aTranslator->SetGain(GetITP()->SonorityGain(i), i);
    }
    aTranslator->StartTime(GetITP()->StartTime());
    aTranslator->EndTime(GetITP()->EndTime());
    aTranslator->ForwardMargin(GetITP()->ForwardMargin());
    aTranslator->BackwardMargin(GetITP()->BackwardMargin());
    aTranslator->GetReady();
    for (ionstd::numerator i = 0; i < aSource->VoicesCount(); i++)
    {
        VoiceThread *aThread = new VoiceThread(aTranslator, i);
        aPRVThreads.push_back(aThread);
    }
    // third: put it into practice
    CiePrgForm->Invoke("Rendering Audio File",
        TranslationProgress, AfterTranslation);
    for (unsigned int i = 0; i < aPRVThreads.size(); delete aPRVThreads[i++]);
    aPRVThreads.clear();
    delete aTranslator;
    delete aWaveStruct;
    EEND
}

void __fastcall TMainForm::AExitExecute(TObject *Sender)
{
    Close();
}

/***************************************************************************
 *  Edit
 **************************************************************************/

void __fastcall TMainForm::ACutExecute(TObject *Sender)
{
    ESTART
    ClearClipboard();
    SelectedToClipboard();
    DeleteSelected();
    EEND
    *GetModified() = true;
    Refresh();
}

void __fastcall TMainForm::ACopyExecute(TObject *Sender)
{
    ESTART
    ClearClipboard();
    SelectedToClipboard();
    EEND
    Refresh();
}

void __fastcall TMainForm::APasteExecute(TObject *Sender)
{
    ESTART
        ionstd::Segment *aSegment = GetActualAsSegment();
    ClipboardStore::const_iterator i = aClipboard.begin();
    while (i != aClipboard.end())
    {
        aSegment->PushBack(ionstd::CopyElement(*i++));
    }
    EEND
    *GetModified() = true;
    Refresh();
}

void __fastcall TMainForm::ASelectAllExecute(TObject *Sender)
{
    SelectionVector::iterator i = aSelection.begin();
    while (i != aSelection.end()) *i++ = true;
    Refresh();
}

void __fastcall TMainForm::AClearSelectionExecute(TObject *Sender)
{
    SelectionVector::iterator i = aSelection.begin();
    while (i != aSelection.end()) *i++ = false;
    Refresh();
}

/***************************************************************************
 *  Element
 **************************************************************************/

void __fastcall TMainForm::ANewElementExecute(TObject *Sender)
{
    ESTART
    // first: get segment
    ionstd::Segment *aSegment = GetActualAsSegment();
    // second: select type of subelement
    CieChsForm->Invoke("Element Type", ElementType, "psmhlr");
    if (CieChsForm->ModalResult == mrCancel) return;
    ionstd::ElementPointer aElement = NIL;
    switch (CieChsForm->GetIndex())
    {
        case 0 : /* Pause */ aElement = new ionstd::Pause; break;
        case 1 : /* Sound */ aElement = new ionstd::Sound; break;
        case 2 : /* Melodic Segment */ aElement = new ionstd::Segment;
        dynamic_cast<ionstd::Segment *>(aElement)->Kind(ionstd::melodic);
        break;
        case 3 : /* Harmonic Segment */ aElement = new ionstd::Segment;
        dynamic_cast<ionstd::Segment *>(aElement)->Kind(ionstd::harmonic);
        break;
        case 4 : /* Link */ aElement = new ionstd::Link; break;
        case 5 : /* Record */ aElement = new ionstd::Record; break;
    }
    if (!(AskForDuration(aElement) && AskForWave(aElement)))
        { delete aElement; return; }
    CieArgForm->Invoke("Label", Adapt(aElement->Label()));
    if (CieArgForm->ModalResult != mrOk)
        { delete aElement; return; }
    aElement->Label(Adapt(CieArgForm->ResultAsString()));
    // third: add element
    aSegment->PushBack(aElement);
    EEND
    // fourth: refresh
    *GetModified() = true;
    Refresh();
}

void __fastcall TMainForm::ARemoveElementExecute(TObject *Sender)
{
    ESTART
    DeleteSelected();
    EEND
    *GetModified() = true;
    Refresh();
}

void __fastcall TMainForm::AShiftUpExecute(TObject *Sender)
{
    ESTART
    ionstd::Segment *aSegment = GetActualAsSegment();
    SelectionVector::const_iterator i = aSelection.begin();
    while (i != aSelection.end())
    {
        int aIndex = i - aSelection.begin();
        if (!*i++ || !aIndex) continue;
        ionstd::ElementPointer aElement =
            ionstd::CopyElement(aSegment->GetElement(aIndex));
        aSegment->Erase(aIndex);
        aSelection.erase(aSelection.begin() + aIndex);
        aSegment->Insert(aElement, aIndex - 1);
        aSelection.insert(aSelection.begin() + aIndex - 1, true);
    }
    EEND
    *GetModified() = true;
    Refresh();
}

void __fastcall TMainForm::AShiftDownExecute(TObject *Sender)
{
    ESTART
    ionstd::Segment *aSegment = GetActualAsSegment();
    SelectionVector::const_iterator i = aSelection.end() - 1;
    while (i >= aSelection.begin())
    {
        unsigned int aIndex = i - aSelection.begin();
        if (!*i-- || (aIndex == aSegment->ElementsCount() - 1)) continue;
        ionstd::ElementPointer aElement =
            ionstd::CopyElement(aSegment->GetElement(aIndex));
        aSegment->Erase(aIndex);
        aSelection.erase(aSelection.begin() + aIndex);
        if (++aIndex != aSegment->ElementsCount())
        {
            aSegment->Insert(aElement, aIndex);
            aSelection.insert(aSelection.begin() + aIndex, true);
        }
        else
        {
            aSegment->PushBack(aElement);
            aSelection.push_back(true);
        }
    }
    EEND
    *GetModified() = true;
    Refresh();
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
    ionstd::ElementPointer aElement = GetActual();
    double aReleaseTime = aPRVRelease;
    aTranslator = new ionstd::Translator(aElement, aWaveStruct);
    aTranslator->EndTime(aElement->Time() + aReleaseTime);
    aTranslator->ForwardMargin(aPRVFMargin);
    aTranslator->BackwardMargin(aPRVBMargin);
    unsigned int aHI = 0;
    PathVector *aPath = &((aTabImages.begin() + aSelector)->aPath);
    for (unsigned int aLevel = 0; aLevel < (GetDepth() - 1); aLevel++)
    {
        if ((**(aPath->begin() + aLevel)).Type() == ionstd::etSegment)
        {
            ionstd::Segment *aSegment = dynamic_cast<ionstd::Segment *>
                (*(aPath->begin() + aLevel));
            if (aSegment->Kind() != ionstd::harmonic) continue;
            for (unsigned int i = 0; i < aSegment->ElementsCount(); i++)
            {
                ionstd::ElementPointer aElement =
                    aSegment->GetElement(i);
                if (aElement != *(aPath->begin() + aLevel + 1))
                    aHI += aElement->VoicesCount();
                else break;
            }
        }
    }
    unsigned int aLength = aElement->VoicesCount();
    for (unsigned int i = 0; i < aLength; i++)
    {
        aTranslator->SetTimbre(*(aPRVSonorities.begin() + i + aHI), i);
        aTranslator->SetGain(GetITP()->SonorityGain(i + aHI), i);
    }
    aTranslator->GetReady();
    // second: put sonorities into the work
    for (unsigned int i = 0; i < aLength; i++)
    {
        VoiceThread *aThread = new VoiceThread(aTranslator, i);
        aPRVThreads.push_back(aThread);
    }
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

void __fastcall TMainForm::AHelpExecute(TObject *Sender)
{
    CieAbtForm->Invoke();
}

/***************************************************************************
 *  Controls
 **************************************************************************/

void __fastcall TMainForm::FormResize(TObject *)
{
    TimeSettings->ButtonWidth = ClientWidth / 5;
    if (!aTabImages.empty())
        Sonorities->ButtonWidth =
            ClientWidth / (GetScore()->VoicesCount() + 1);
    StatusBar->Panels->Items[0]->Width = 0.57 * ClientWidth;
    StatusBar->Panels->Items[1]->Width = 0.29 * ClientWidth;
}

void __fastcall TMainForm::FormCloseQuery(TObject *, bool &CanClose)
{
    ACloseAll->Execute();
    CanClose = !aClosingCancel;
    aClosingCancel = false;
}

void __fastcall TMainForm::SelectorButtonClicked(TObject *Sender, int IndexArg)
{
    aSelector = IndexArg;
    AClearSelection->Execute();
    Refresh();
}

void __fastcall TMainForm::PositionButtonClicked(TObject *Sender, int IndexArg)
{
    GoBack(Position->Items->Count - IndexArg - 1);
}

void __fastcall TMainForm::PropertiesButtonClicked(TObject *Sender, int IndexArg)
{
    EditProperty(IndexArg);
}

void __fastcall TMainForm::SubelementsButtonClicked(TObject *Sender, int IndexArg)
{
    GoIn(IndexArg);
}

void __fastcall TMainForm::TimeSettingsButtonClicked(TObject *Sender, int IndexArg)
{
    EditTimeSetting(IndexArg);
}

void __fastcall TMainForm::SonoritiesButtonClicked(TObject *Sender, int IndexArg)
{
    EditSonority(IndexArg);
}

void __fastcall TMainForm::FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
    switch (Key)
    {
        case VK_BACK :  GoBack(1); break;
        case VK_SPACE :
            if (aSoundPreview) AStopPreview->Execute();
            else APreview->Execute();
    }
}

void __fastcall TMainForm::SubelementsMouseDown
(TObject *Sender, TMouseButton ButtonArg, TShiftState Shift, int XArg, int YArg)
{
    if (ButtonArg != mbRight) return;
    int aIndex = Subelements->TargetIndexAt(XArg, YArg - 10);
    bool aSelected = *(aSelection.begin() + aIndex);
    *(aSelection.begin() + aIndex) = !aSelected;
    Refresh();
}
