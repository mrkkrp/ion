/***************************************************************************
 *  C++ source
 *
 *  ION Generator
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
#include "egenerator.h"
#include "wizard.h"
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

const UnicodeString GPropertyName[GeneratorPC] =
{
    /* 0 */ "Label",
    /* 1 */ "Panoram",
    /* 2 */ "Frequency",
    /* 3 */ "Volume",
    /* 4 */ "Duration, sec",
    /* 5 */ "Seed",
    /* 6 */ "Depth",
    /* 7 */ "C-Vector Size",
    /* 8 */ "Conditions"
}; // GPropertyName

const int GPropertyImage[GeneratorPC] =
{
    /* 0 */ 28,
    /* 1 */ 29,
    /* 2 */ 29,
    /* 3 */ 29,
    /* 4 */ 27,
    /* 5 */ 26,
    /* 6 */ 26,
    /* 7 */ 26,
    /* 8 */ 30
}; // GPropertyImage

const UnicodeString EPropertyName[ElementPC] =
{
    /* 0 */ "Type",
    /* 1 */ "Label",
    /* 2 */ "Depth",
    /* 3 */ "Duration, sec",
    /* 4 */ "Harmonic Mask",
    /* 5 */ "Conditions"
}; // EPropertyName

const int EPropertyImage[ElementPC] =
{
    /* 0 */ 26,
    /* 1 */ 28,
    /* 2 */ 26,
    /* 3 */ 27,
    /* 4 */ 26,
    /* 5 */ 30
}; // EPropertyImage

const UnicodeString ElementTypeName[ElementTC] =
{
    /* 0 */ "Pause",
    /* 1 */ "Sound",
    /* 2 */ "Segment",
    /* 3 */ "Link",
    /* 4 */ "Record"
}; // ElementTypeName

const FEP aRatingFEP = { 0., 10., 20, gmLinear, 2 };

ionstd::String gnreNoSupport("your device does not support the wave format");

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

__fastcall GeneratingThread::GeneratingThread
    (const ionstd::Generator *GeneratorArg) :
    TThread    (false /* create suspended */),
    aGenerator (GeneratorArg),
    aScore     (NIL)
{ }

void __fastcall GeneratingThread::Execute()
{
    ESTART
    aScore = aGenerator->GenerateScore();
    EEND
}

void __fastcall TMainForm::SelectVoice(int IndexArg)
{
    *GetVoiceIndex() = IndexArg - 1;
    Refresh();
}

void __fastcall TMainForm::SelectElement(int IndexArg)
{
    bool aSelected = *(aSelection.begin() + IndexArg);
    *(aSelection.begin() + IndexArg) = !aSelected;
    *GetElementIndex() = -1;
    for (unsigned int i = 0; i < aSelection.size(); i++)
    {
        if (*(aSelection.begin() + i)) *GetElementIndex() = i;
    }
    Refresh();
}

#define STDWAY_STRING(CAPTION, PROPERTY) \
CieArgForm->Invoke(CAPTION, Adapt(aObject->PROPERTY())); \
if (CieArgForm->ModalResult != mrOk) return; \
aObject->PROPERTY(Adapt(CieArgForm->ResultAsString()));

#define NUMWAY_STRING(CAPTION, PROPERTY, INDEX) \
CieArgForm->Invoke(UnicodeString(CAPTION) + INDEX, \
                   Adapt(aObject->PROPERTY(INDEX))); \
if (CieArgForm->ModalResult != mrOk) return; \
aObject->PROPERTY(Adapt(CieArgForm->ResultAsString()), INDEX);

#define STDWAY_BOOL(CAPTION, PROPERTY) \
CieArgForm->Invoke(CAPTION, aObject->PROPERTY()); \
if (CieArgForm->ModalResult != mrOk) return; \
aObject->PROPERTY(CieArgForm->ResultAsBool());

#define STDWAY_DOUBLE(CAPTION, PROPERTY) \
CieArgForm->Invoke(CAPTION, aObject->PROPERTY()); \
if (CieArgForm->ModalResult != mrOk) return; \
aObject->PROPERTY(CieArgForm->ResultAsDouble());

#define NUMWAY_DOUBLE(CAPTION, PROPERTY, INDEX) \
CieArgForm->Invoke(UnicodeString(CAPTION) + INDEX, aObject->PROPERTY(INDEX)); \
if (CieArgForm->ModalResult != mrOk) return; \
aObject->PROPERTY(CieArgForm->ResultAsDouble(), INDEX);

#define STDWAY_UINT(CAPTION, PROPERTY) \
CieArgForm->Invoke(CAPTION, aObject->PROPERTY()); \
if (CieArgForm->ModalResult != mrOk) return; \
aObject->PROPERTY(CieArgForm->ResultAsInt());

#define STDWAY_FNC(CAPTION, PROPERTY) \
CieFncForm->Invoke(CAPTION, aObject->PROPERTY(), &PROPERTY ## FEP); \
if (CieFncForm->ModalResult == mrCancel) return;

#define NUMWAY_FNC(CAPTION, PROPERTY, INDEX) \
CieFncForm->Invoke(UnicodeString(CAPTION) + INDEX, \
                   &(*aObject->PROPERTY(INDEX)), &PROPERTY ## FEP); \
if (CieFncForm->ModalResult == mrCancel) return;

void __fastcall TMainForm::EditVoiceProperty
    (unsigned int IndexArg, bool UseOldArg)
{
    ESTART
    int aIndex = *GetVoiceIndex();
    ionstd::Generator *aObject = GetGenerator();
    if (GeneratorSelected())
    switch (IndexArg)
    {
    case 0 : // Label
        STDWAY_STRING("Generator Label", Label); break;
    case 1 : // Panoram
        STDWAY_FNC("Panoram", Panoram); break;
    case 2 : // Frequency
        STDWAY_FNC("Frequency", Frequency); break;
    case 3 : // Volume
        STDWAY_FNC("Volume", Volume); break;
    case 4 : // Duration
        STDWAY_DOUBLE("Duration, sec", RequestedDuration); break;
    case 5 : // Seed
        STDWAY_UINT("Seed", Seed); break;
    case 6 : // Depth
        STDWAY_UINT("Depth", HarmonicDepth); break;
    case 7 : // C-Vector Size
        STDWAY_UINT("Condition Vector Size", CVectorSize); break;
    case 8 : // Conditions
    CieArgForm->Invoke(UnicodeString("Condition Index [0 - ") +
                       UIntToStr(aObject->CVectorSize() - 1) + "]", 0);
    if (CieArgForm->ModalResult != mrOk) return;
    unsigned int aIndex = CieArgForm->ResultAsInt();
    CieArgForm->Invoke(UnicodeString("Condition ") + aIndex,
                       aObject->InitCondition(aIndex));
    if (CieArgForm->ModalResult != mrOk) return;
    aObject->InitCondition(CieArgForm->ResultAsDouble(), aIndex);
    } // switch
    else switch (IndexArg)
    {
    case 0 : // Label
        NUMWAY_STRING("Voice Label ", VoiceLabel, aIndex); break;
    case 1 : // Panoram
        NUMWAY_FNC("Voice Panoram ", VoicePanoram, aIndex); break;
    case 2 : // Frequency
        NUMWAY_FNC("Voice Frequency ", VoiceFrequency, aIndex); break;
    case 3 : // Volume
        NUMWAY_FNC("Voice Volume ", VoiceVolume, aIndex); break;
    }
    EEND
    *GetModified() = true;
    Refresh();
}

bool __fastcall TMainForm::EditElementProperty
    (unsigned int IndexArg, bool UseOldArg)
{
    ESTART
    static ionstd::String aString;
    static ionstd::Function aFunction(0, 0, 0);
    static double aDouble;
    static bool aBool;
    static unsigned int aUInt, aCIndex;
    ionstd::GElementIter aObject = GetAElement();
    switch (IndexArg)
    {
    case 0 : /* Type */ return true; // read only
    case 1 : /* Label */
    if (UseOldArg) aObject->GetElement()->Label(aString);
    else
    {
        CieArgForm->Invoke("Label", Adapt(aObject->GetElement()->Label()));
        if (CieArgForm->ModalResult != mrOk) return false;
        aString = Adapt(CieArgForm->ResultAsString());
        aObject->GetElement()->Label(aString);
        }
    break; /* Label */
    case 2 : /* Depth */ return true; // read only
    case 3 : /* Duration */ return true; // read only
    case 4 : /* Harmonic Mask */
    if (!UseOldArg)
    {
        CieSlcForm->Invoke("Harmonic Mask", aObject->HarmonicMask(),
            GetGenerator()->HarmonicDepth());
        if (CieSlcForm->ModalResult != mrOk) return false;
        aUInt = CieSlcForm->GetMask();
        aObject->HarmonicMask(aUInt);
    }
    aObject->HarmonicMask(aUInt);
    break; /* Harmonic Mask */
    case 5 : /* Conditions */
    if (!UseOldArg) // manually
    {
    CieArgForm->Invoke(UnicodeString("Condition Index [0 - ") +
        UIntToStr(aObject->CVectorSize() - 1) + "]", 0);
    if (CieArgForm->ModalResult != mrOk) return false;
    aCIndex = CieArgForm->ResultAsInt();
    ionstd::ConditionIter C = aObject->GetCondition(aCIndex);
    CieChsForm->Invoke(UnicodeString("Condition ") + aCIndex + ": "
        + FloatToStr(C->Affect()) + ", "
        + MakeFncImage(C->Rating()) + ", "
        + (C->Overwrite() ? "true" : "false"),
        ConditionAspect, "aro");
    if (CieChsForm->ModalResult != mrOk) return false;
    aUInt = CieChsForm->GetIndex();
    if (aUInt == 0)
    {
        CieArgForm->Invoke
        (UnicodeString("Affect ") + aCIndex, C->Affect());
        if (CieArgForm->ModalResult != mrOk) return false;
        aDouble = CieArgForm->ResultAsDouble();
    }
    if (aUInt == 1)
    {
        aFunction = *C->Rating();
        CieFncForm->Invoke
        (UnicodeString("Rating ") + aCIndex, &aFunction, &RatingFEP);
        if (CieFncForm->ModalResult != mrOk) return false;
    }
    if (aUInt == 2)
    {
        CieArgForm->Invoke
        (UnicodeString("Overwrite ") + aCIndex, C->Overwrite());
        if (CieArgForm->ModalResult != mrOk) return false;
        aBool = CieArgForm->ResultAsBool();
    }
    } // manually
    if (aUInt == 0) aObject->GetCondition(aCIndex)->Affect(aDouble);
    if (aUInt == 1) *aObject->GetCondition(aCIndex)->Rating() = aFunction;
    if (aUInt == 2) aObject->GetCondition(aCIndex)->Overwrite(aBool);
    }
    EEND
    *GetModified() = true;
    Refresh();
    return true;
}

void __fastcall TMainForm::EditTimeSetting(unsigned int IndexArg)
{
    ESTART
    UnicodeString aCaption;
    switch (IndexArg)
    {
        case 0 : aCaption = "Forward Margin";  break;
        case 1 : aCaption = "Start Time";      break;
        case 2 : aCaption = "End Time";        break;
        case 3 : aCaption = "Backward Margin"; break;
    }
    double aValue;
    switch (IndexArg)
    {
        case 0 : aValue = GetITP()->ForwardMargin();  break;
        case 1 : aValue = GetITP()->StartTime();      break;
        case 2 : aValue = GetITP()->EndTime();        break;
        case 3 : aValue = GetITP()->BackwardMargin(); break;
    }
    CieArgForm->Invoke(aCaption, aValue);
    if (CieArgForm->ModalResult != mrOk) return;
    aValue = CieArgForm->ResultAsDouble();
    switch (IndexArg)
    {
        case 0 : GetITP()->ForwardMargin(aValue);  break;
        case 1 : GetITP()->StartTime(aValue);      break;
        case 2 : GetITP()->EndTime(aValue);        break;
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

UnicodeString __fastcall TMainForm::MakeHMaskImage(unsigned int HMaskArg) const
{
    UnicodeString aResult;
    unsigned int aStart = 0, aLength = 0;
    for (unsigned int i = 0; i < 32; i++) // 32 bit ver
    {
        if ((HMaskArg >> i) & 1)
        {
            if (!aLength)
            {
                aStart = i;
                if (aResult.Length()) aResult += ", ";
            }
            aLength++;
        }
        else
        {
            if (aLength == 1) aResult += UIntToStr(aStart);
            if (aLength > 1) aResult += UIntToStr(aStart) +
                " - " + UIntToStr(aStart + aLength - 1);
            aLength = 0;
        }
    }
    return (aResult.Length() ? aResult : UnicodeString("none"));
}

UnicodeString __fastcall TMainForm::MakeVoicePropertyD
    (unsigned int IndexArg) const
{
    const ionstd::Generator *aObject = GetGenerator();
    switch (IndexArg)
    {
    case 0 : /* Label */
    if (GeneratorSelected()) return Adapt(aObject->Label());
    else return Adapt(aObject->VoiceLabel(*GetVoiceIndex()));
    case 1 : /* Panoram */
    if (GeneratorSelected()) return MakeFncImage(aObject->Panoram());
    else return MakeFncImage(&(*aObject->VoicePanoram(*GetVoiceIndex())));
    case 2 : /* Frequency */
    if (GeneratorSelected()) return MakeFncImage(aObject->Frequency());
    else return MakeFncImage(&(*aObject->VoiceFrequency(*GetVoiceIndex())));
    case 3 : /* Volume */
    if (GeneratorSelected()) return MakeFncImage(aObject->Volume());
    else return MakeFncImage(&(*aObject->VoiceVolume(*GetVoiceIndex())));
    case 4 : /* Duration */ return FloatToStr(aObject->RequestedDuration());
    case 5 : /* Seed */ return UIntToStr(aObject->Seed());
    case 6 : /* Depth */ return UIntToStr(aObject->HarmonicDepth());
    case 7 : /* CVector Size */ return UIntToStr(aObject->CVectorSize());
    case 8 : /* Conditions */ return "click";
    }
    return "UNKNOWN";
}

UnicodeString __fastcall TMainForm::MakeElementPropertyD
    (unsigned int IndexArg) const
{
    ionstd::ConstElementPointer aObject = GetAElement()->GetElement();
    ionstd::ConstFunctionIter aFunction;
    switch (IndexArg)
    {
    case 0 : /* Type */ return ElementTypeName[aObject->Type()];
    case 1 : /* Label */ return Adapt(aObject->Label());
    case 2 : /* Depth */ return UIntToStr(aObject->VoicesCount());
    case 3 : /* Duration */ return FloatToStr(aObject->Time());
    case 4 : /* Harmonic Mask */
        return MakeHMaskImage(GetAElement()->HarmonicMask());
    case 5 : /* Conditions */ return "click";
    }
    return "UNKNOWN";
}

UnicodeString __fastcall TMainForm::MakeElementD(unsigned int IndexArg) const
{
    ionstd::ConstElementPointer aElement =
        GetGenerator()->GetGElement(IndexArg)->GetElement();
    UnicodeString aResult;
    if (aElement->Type() == ionstd::etSegment)
    {
        const ionstd::Segment *aSegment =
            dynamic_cast<const ionstd::Segment *>(aElement);
        aResult += "[", aResult +=
            UIntToStr(aSegment->ElementsCount()) + "] ";
    }
    aResult += FloatToStr(aElement->Time()) + " sec, ";
    aResult += UIntToStr(aElement->VoicesCount()) + " vcs";
    return aResult;
}

UnicodeString __fastcall TMainForm::MakeTimeSettingD
    (unsigned int IndexArg) const
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

bool __fastcall TMainForm::AnyElementSelected() const
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
    ionstd::Generator *aGenerator = GetGenerator();
    SelectionVector::iterator i = aSelection.begin();
    while (i != aSelection.end())
    {
        if (*i)
        {
            ionstd::GElement *aGElement = new ionstd::GElement
            (*aGenerator->GetGElement(i - aSelection.begin()));
            aClipboard.push_back(aGElement);
        }
        i++;
    }
}

void __fastcall TMainForm::DeleteSelected()
{
    ionstd::Generator *aGenerator = GetGenerator();
    int aCorrection = 0;
    SelectionVector::iterator i = aSelection.begin();
    while (i != aSelection.end())
    {
        if (*i)
        {
            aGenerator->Erase
            ((i - aSelection.begin()) - aCorrection++);
            *i = false;
        }
        i++;
    }
    *GetElementIndex() = -1;
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
    return aTabImages.empty() ? "" : "Generator";
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

void __fastcall TMainForm::AdjustFor
    (ionstd::Generator &GeneratorArg, ionstd::ITP *ITPArg) const
{
    ionstd::Segment aSegment;
    aSegment.Kind(ionstd::harmonic);
    unsigned int aCount = GeneratorArg.HarmonicDepth();
    while (aCount--)
    {
        ionstd::Sound *aSound = new ionstd::Sound;
        aSound->SetDuration(GeneratorArg.RequestedDuration());
        aSegment.PushBack(aSound);
    }
    ITPArg->AdjustFor(aSegment);
}

bool __fastcall TMainForm::eOpen(UnicodeString FileNameArg)
{
    ESTART
    ionstd::Paramlist aParamlist;
    aParamlist.Load(Adapt(FileNameArg));
    ionstd::String aTopMap = Adapt(aDefaultLabel);
    TabImage aTabImage;
    aTabImage.aGenerator.ReadFrom(aParamlist, aTopMap);
    ApplyDefaults(&aTabImage.aITP);
    if (ionstd::HasITP(aParamlist, aTopMap))
        aTabImage.aITP.ReadFor(aParamlist, aTopMap);
    else AdjustFor(aTabImage.aGenerator, &aTabImage.aITP);
    aTabImage.aFileName = FileNameArg;
    aTabImage.aModified = false;
    aTabImage.aVoiceIndex = -1;
    aTabImage.aGElementIndex = -1;
    aTabImage.aName = Adapt(aParamlist.Name());
    aTabImage.aAuthor = Adapt(aParamlist.Author());
    aTabImage.aComment = Adapt(aParamlist.Comment());
    aTabImage.aCopyright = Adapt(aParamlist.Copyright());
    aTabImages.push_back(aTabImage);
    // make new tab:
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
    GetGenerator()->WriteTo(aParamlist);
    GetITP()->WriteFor(aParamlist, GetGenerator()->Label());
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
    aTabImages.erase(aTabImages.begin() + aSelector); // erase image
    Selector->Items->Delete(aSelector); // delete button
    // if it was last, then shift left
    if (aSelector == aTabImages.size()) aSelector--;
    Refresh();
}

void __fastcall TMainForm::EnableControls(bool Arg)
{
    Selector->Enabled = Arg;
    Voices->Enabled = Arg;
    VoiceProperties->Enabled = Arg;
    ElementProperties->Enabled = Arg;
    Elements->Enabled = Arg;
    TimeSettings->Enabled = Arg;
    Sonorities->Enabled = Arg;
}

void __fastcall TMainForm::Refresh()
{
    ESTART
    // first: refresh controlls
    bool aTabsExist = !aTabImages.empty();
    bool aAnySelected = AnyElementSelected();
    Voices->Visible = aTabsExist;
    Selector->Visible = aTabsExist;
    ElementProperties->Visible = aTabsExist;
    Elements->Visible = aTabsExist;
    VoiceProperties->Visible = aTabsExist;
    Sonorities->Visible = aTabsExist;
    TimeSettings->Visible = aTabsExist;
    ASave->Enabled = aTabsExist && *GetModified();
    ASaveAs->Enabled = aTabsExist;
    ASaveAll->Enabled = aTabsExist;
    AClose->Enabled = aTabsExist;
    ACloseAll->Enabled = aTabsExist;
    AProperties->Enabled = aTabsExist;
    AGenerate->Enabled = aTabsExist;
    ASaveSoundBank->Enabled = aTabsExist;
    ACut->Enabled = aTabsExist && aAnySelected;
    ACopy->Enabled = aTabsExist && aAnySelected;
    APaste->Enabled = aTabsExist && !aClipboard.empty();
    ASelectAll->Enabled = aTabsExist;
    AClearSelection->Enabled = aTabsExist;
    ANewElement->Enabled = aTabsExist;
    ARemoveElement->Enabled = aTabsExist && aAnySelected;
    AReassign->Enabled = aTabsExist && aAnySelected;
    APreview->Enabled = aTabsExist && !aSoundPreview;
    AStopPreview->Enabled = aTabsExist && aSoundPreview;
    // second: statistic
    Caption = GenerateFormCaption();
    StatusBar->Panels->Items[1]->Text = GenerateStatisticText();
    if (aTabsExist)
    StatusBar->Panels->Items[2]->Text = *GetModified() ? "Modified" : "";
    else StatusBar->Panels->Items[2]->Text = "";
    if (!aTabsExist) return;
    // third: create all the buttons
    TGrpButtonItem *aButton;
    // 3.1 - show properties of generator / voice
    VoiceProperties->Items->BeginUpdate();
    VoiceProperties->Items->Clear();
    int aCount = GeneratorSelected() ? GeneratorPC : VoicePC;
    for (int i = 0; i < aCount; i++)
    {
        aButton = VoiceProperties->Items->Add();
        aButton->ImageIndex = *(GPropertyImage + i);
        aButton->Caption = *(GPropertyName + i);
        aButton->Caption += " = ";
        aButton->Caption += MakeVoicePropertyD(i);
    }
    VoiceProperties->Items->EndUpdate();
    // 3.2 - show list of voices (and generator)
    Voices->Items->BeginUpdate();
    Voices->Items->Clear();
    aCount = GetGenerator()->HarmonicDepth() + 1;
    for (int i = 0; i < aCount; i++)
    {
        aButton = Voices->Items->Add();
        aButton->ImageIndex = i ? 1 : 0;
        if (*GetVoiceIndex() == i - 1) aButton->ImageIndex += 2;
        aButton->Caption = Adapt(i ?
            GetGenerator()->VoiceLabel(i - 1) :
            GetGenerator()->Label());
    }
    Voices->Items->EndUpdate();
    // 3.3 - show properties of Element
    ElementProperties->Items->BeginUpdate();
    ElementProperties->Items->Clear();
    if (GetGenerator()->GElementsCount() && (*GetElementIndex() != -1))
    {
    aCount = ElementPC;
    for (int i = 0; i < aCount; i++)
    {
        aButton = ElementProperties->Items->Add();
        aButton->ImageIndex = *(EPropertyImage + i);
        aButton->Caption = *(EPropertyName + i);
        aButton->Caption += " = ";
        aButton->Caption += MakeElementPropertyD(i);
    }
    } // if
    ElementProperties->Items->EndUpdate();
    // 3.4 - show list of generative elements
    bool aVirgin = !Elements->Items->Count;
    if (aVirgin) Elements->Items->BeginUpdate();
    aCount = GetGenerator()->GElementsCount();
    aSelection.resize(aCount, false);
    for (int i = 0; i < aCount; i++)
    {
        if (i < Elements->Items->Count)
            aButton = Elements->Items->operator[](i);
        else aButton = Elements->Items->Add();
        ionstd::ElementPointer aElement =
            GetGenerator()->GetGElement(i)->GetElement();
        aButton->Caption = Adapt(aElement->Label()) + ", ";
        aButton->Caption += MakeElementD(i);
        aButton->ImageIndex = MakeImageIndex
            (aElement, *(aSelection.begin() + i));
    }
    while (aCount < Elements->Items->Count) Elements->Items->Delete(aCount);
    if (aVirgin) Elements->Items->EndUpdate();
    // 3.5 - refresh ITP time settings
    TimeSettings->Items->BeginUpdate();
    AdjustFor(*GetGenerator(), GetITP());
    aCount = TimeSettings->Items->Count;
    for (int i = 0; i < aCount; i++)
    {
        aButton = TimeSettings->Items->operator[](i);
        UnicodeString aCaption = aButton->Caption;
        aButton->Caption = aCaption.SubString(0, aCaption.Pos("="))
            + ' ' + MakeTimeSettingD(i);
    }
    TimeSettings->Items->EndUpdate();
    // 3.6 - show ITP sonorities and load sonorities if needed
    Sonorities->Items->BeginUpdate();
    Sonorities->Items->Clear();
    aPRVSonorities.resize(GetGenerator()->HarmonicDepth(), NIL);
    aCount = GetGenerator()->HarmonicDepth();
    for (int i = 0; i < aCount; i++)
    {
        // show buttons:
        aButton = Sonorities->Items->Add();
        aButton->ImageIndex = 31;
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
    TGrpButtonItem *aItem = Selector->Items->operator[](aSelector);
    if (*GetModified()) aItem->ImageIndex = 23;
    else aItem->ImageIndex = 22;
    // fifth: resize
    FormResize(this);
    EEND
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
        delete aPRVWaveStruct;
        delete aPRVTranslator;
        delete aPRVElement;
        aSoundPreview = false;
        APreview->Enabled = true;
        AStopPreview->Enabled = false;
        EnableControls(true);
    }
    else TMainForm::Dispatch(&MessageArg);
}

__fastcall TMainForm::TMainForm(TComponent* Owner) :
    TForm             (Owner),
    aSelector         (-1),
    aClipboard        (NIL),
    RatingFEP         (aRatingFEP),
    PanoramFEP        (PanoramDefaults),
    VolumeFEP         (DefaultPreferences),
    FrequencyFEP      (DefaultPreferences),
    VoicePanoramFEP   (PanoramDefaults),
    VoiceVolumeFEP    (DefaultPreferences),
    VoiceFrequencyFEP (DefaultPreferences)
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
    if (aList.Exists("gnr", "dLabel"))
    aDefaultLabel = Adapt(aList.Data<ionstd::String>("gnr", "dLabel"));
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
    EEND
    // tune additional parameters:
    int aParamsCount = ParamCount() + 1;
    for (int i = 1; i < aParamsCount; eOpen(ParamStr(i++)));
    // adjust preview parameters
    ESTART
    aSoundPreview = false;
    aPRVWaveStruct = new ionstd::WaveStruct
        (aPRVRate, 32, aPRVChannels, ionstd::WaveFormatIEEEFloat);
    WAVEFORMATEX aWaveFmt;
    aWaveFmt.wFormatTag = aPRVWaveStruct->Format();
    aWaveFmt.nChannels = aPRVWaveStruct->Channels();
    aWaveFmt.nSamplesPerSec = aPRVWaveStruct->SampleRate();
    aWaveFmt.nAvgBytesPerSec = aPRVWaveStruct->AvgBytesPerSec();
    aWaveFmt.nBlockAlign = aPRVWaveStruct->BlockAlign();
    aWaveFmt.wBitsPerSample = aPRVWaveStruct->BitsPerSample();
    aWaveFmt.cbSize = 0;
    int Result = waveOutOpen(&aPRVHandle, aPRVDevice, &aWaveFmt,
        MAKELONG(Handle, 0), 0, CALLBACK_WINDOW);
    waveOutSetVolume(aPRVHandle, 0xffffffffu);
    delete aPRVWaveStruct; // delete temporary wave structure
    if (Result == WAVERR_BADFORMAT) THROW_RE(gnreNoSupport);
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
    CieChsForm->Invoke("Create Generator", CreateGenerator, "sw");
    if (CieChsForm->ModalResult != mrOk) return;
    TabImage aTabImage;
        aTabImage.aModified = true;
    if (CieChsForm->GetIndex())
    {
                GWizardForm->UseDefaultLabel(aDefaultLabel);
        GWizardForm->ShowModal();
        if (GWizardForm->ModalResult != mrOk) return;
        aTabImage.aGenerator = GWizardForm->GetGenerator();
    }
    ApplyDefaults(&aTabImage.aITP);
    AdjustFor(aTabImage.aGenerator, &aTabImage.aITP);
    aTabImage.aGenerator.Label(Adapt(aDefaultLabel));
    aTabImage.aVoiceIndex = -1;
    aTabImage.aGElementIndex = -1;
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

double GenerationProgress()
{
    return MainForm->aGenerator->Progress();
}

void AfterGeneration()
{
    // ...
}

void __fastcall TMainForm::AGenerateExecute(TObject *Sender)
{
    ESTART
    if (!SaveScoreDialog->Execute()) return;
    aGenerator = GetGenerator();
    GeneratingThread *aGeneratingThread = new GeneratingThread(aGenerator);
    CiePrgForm->Invoke("Score Generation",
        GenerationProgress, AfterGeneration);
    ionstd::ElementPointer aScore = aGeneratingThread->GetScore();
    ionstd::Paramlist aList;
    aList.Name(Adapt(*GetName()));
    aList.Author(Adapt(*GetAuthor()));
    aList.Comment(Adapt(*GetComment()));
    aList.Copyright(Adapt(*GetCopyright()));
    aScore->WriteTo(aList);
    GetITP()->WriteFor(aList, aScore->Label());
    aList.Save(Adapt(SaveScoreDialog->FileName));
    delete aScore;
    delete aGeneratingThread;
    aGenerator = NIL;
    EEND
}

void __fastcall TMainForm::ASaveSoundBankExecute(TObject *Sender)
{
    ESTART
    if (!SaveScoreDialog->Execute()) return;
    ionstd::Segment aSegment;
    aSegment.Label(GetGenerator()->Label());
    aSegment.Kind(ionstd::melodic);
    unsigned int aCount = GetGenerator()->GElementsCount();
    for (unsigned int i = 0; i < aCount; i++)
    {
        ionstd::ElementPointer aElement = ionstd::CopyElement
            (GetGenerator()->GetGElement(i)->GetElement());
        aSegment.PushBack(aElement);
    }
    ionstd::Paramlist aList;
    aSegment.WriteTo(aList);
    aList.Save(Adapt(SaveScoreDialog->FileName));
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
    ionstd::Generator *aGenerator = GetGenerator();
    ClipboardStore::const_iterator i = aClipboard.begin();
    while (i != aClipboard.end()) aGenerator->PushBack(**i++);
    EEND
    *GetModified() = true;
    Refresh();
}

void __fastcall TMainForm::ASelectAllExecute(TObject *Sender)
{
    SelectionVector::iterator i = aSelection.begin();
    while (i != aSelection.end()) *i++ = true;
    *GetElementIndex() = aSelection.size() - 1;
    Refresh();
}

void __fastcall TMainForm::AClearSelectionExecute(TObject *Sender)
{
    SelectionVector::iterator i = aSelection.begin();
    while (i != aSelection.end()) *i++ = false;
    *GetElementIndex() = -1;
    Refresh();
}

/***************************************************************************
 *  Element
 **************************************************************************/

void __fastcall TMainForm::ANewElementExecute(TObject *Sender)
{
    ESTART
    ionstd::Generator *aGenerator = GetGenerator();
    CieChsForm->Invoke("Create Elements", CreateElements, "esgr");
    if (CieChsForm->ModalResult != mrOk) return;
    switch (CieChsForm->GetIndex())
    {
        case 0 : /* Import Element */
        case 1 : /* Import All Subelements */
        {
        if (!OpenScoreDialog->Execute()) return;
        CieArgForm->Invoke("Element Label", aDefaultLabel);
        if (CieArgForm->ModalResult != mrOk) return;
        ionstd::String
        aPath = Adapt(OpenScoreDialog->FileName),
        aMapKey = Adapt(CieArgForm->ResultAsString());
        ionstd::Paramlist aList;
        aList.Load(aPath);
        if (CieChsForm->GetIndex())
            aGenerator->ImportSubelements(aList, aMapKey);
        else aGenerator->ImportElement(aList, aMapKey);
        }
        break; /* Import Element or Subelements */
        case 2 : /* Use Elements Generator */
        {
        EGeneratorForm->DoForGenerator(aGenerator);
        EGeneratorForm->ShowModal();
        if (EGeneratorForm->ModalResult == mrCancel) return;
        }
        break; /* Use Elements Generator */
        case 3 : /* Import Record */
        {
        OpenRecordDialog->Options =
            OpenRecordDialog->Options << ofAllowMultiSelect;
        if (!OpenRecordDialog->Execute()) return;
        for (int i = 0; i < OpenRecordDialog->Files->Count; i++)
        {
        ionstd::WaveStruct aWave(Adapt((*OpenRecordDialog->Files)[i]));
        ionstd::String aLabel = Adapt((*OpenRecordDialog->Files)[i]);
        ionstd::ElementPointer aElement = new ionstd::Record(aWave);
        unsigned int
        aStart = aLabel.find_last_of('\\') + 1,
        aEnd = aLabel.find_last_of('.');
        aElement->Label(aLabel.substr(aStart, aEnd - aStart));
        aGenerator->PushBack(ionstd::GElement(aElement));
        }
        }
        break; /* Import Record */
    }
    EEND
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

void __fastcall TMainForm::AReassignExecute(TObject *Sender)
{
    ESTART
    // first: element or record
    CieChsForm->Invoke("Reassign Element", ReassignElement, "er");
    if (CieChsForm->ModalResult != mrOk) return;
    // second: get the element
    ionstd::ElementPointer aElement = NIL;
    if (CieChsForm->GetIndex()) /* Import Record */
    {
        OpenRecordDialog->Options =
            OpenRecordDialog->Options >> ofAllowMultiSelect;
        if (!OpenRecordDialog->Execute()) return;
        ionstd::WaveStruct aWave(Adapt(OpenRecordDialog->FileName));
        aElement = new ionstd::Record(aWave);
    } /* Import Record */
    else /* Import Element */
    {
        if (!OpenScoreDialog->Execute()) return;
        CieArgForm->Invoke("Element Label", aDefaultLabel);
        if (CieArgForm->ModalResult != mrOk) return;
        ionstd::String
        aPath = Adapt(OpenScoreDialog->FileName),
        aMapKey = Adapt(CieArgForm->ResultAsString());
        ionstd::Paramlist aList;
        aList.Load(aPath);
        aElement = ionstd::ReadElement(aList, aMapKey);
    } /* Import Element */
    // third: swap all selected elements
    ionstd::Generator *aGenerator = GetGenerator();
    SelectionVector::const_iterator i = aSelection.begin();
    while (i != aSelection.end())
    {
        if (*i)
        {
            ionstd::GElementIter aGElement =
                aGenerator->GetGElement(i - aSelection.begin());
            ionstd::String aLabel = aGElement->GetElement()->Label();
            delete aGElement->GetElement();
            aGElement->SetElement(ionstd::CopyElement(aElement));
            aGElement->GetElement()->Label(aLabel);
        }
        i++;
    }
    delete aElement;
    EEND
    // fourth: refresh
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
    aPRVWaveStruct = new ionstd::WaveStruct
        (aPRVRate, 32, aPRVChannels, ionstd::WaveFormatIEEEFloat);
    aPRVElement = GetGenerator()->GenerateScore();
    double aReleaseTime = aPRVRelease;
    aPRVTranslator = new ionstd::Translator(aPRVElement, aPRVWaveStruct);
        aPRVTranslator->EndTime(aPRVElement->Time() + aReleaseTime);
    aPRVTranslator->ForwardMargin(aPRVFMargin);
    aPRVTranslator->BackwardMargin(aPRVBMargin);
    int aLength = aPRVElement->VoicesCount();
    for (int i = 0; i < aLength; i++)
    {
        aPRVTranslator->SetTimbre(
            ((*GetVoiceIndex() == -1) || (*GetVoiceIndex() == i)) ?
            *(aPRVSonorities.begin() + i) :
            NIL, i);
        aPRVTranslator->SetGain(GetITP()->SonorityGain(i), i);
    }
    aPRVTranslator->GetReady();
    // second: put sonorities into the work
    for (int i = 0; i < aLength; i++)
    {
        VoiceThread *aThread = new VoiceThread(aPRVTranslator, i);
        aPRVThreads.push_back(aThread);
    }
    // thrid: let it sound
    aSoundPreview = true;
    APreview->Enabled = false;
    AStopPreview->Enabled = true;
    EnableControls(false);
    aPRVHeader.lpData = (char *)aPRVWaveStruct->GetData();
    aPRVHeader.dwBufferLength = aPRVWaveStruct->BytesInData();
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
    aWidth /= 3.;
    VoiceProperties->Width = aWidth;
    ElementProperties->Width = aWidth;
    Elements->Width = aWidth;
    TimeSettings->ButtonWidth = ClientWidth / 5;
    if (!aTabImages.empty())
        Sonorities->ButtonWidth =
            ClientWidth / (GetGenerator()->HarmonicDepth() + 1);
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
    AClearSelection->Execute();
    Refresh();
}

void __fastcall TMainForm::VoicePropertiesButtonClicked
    (TObject *Sender, int IndexArg)
{
    EditVoiceProperty(IndexArg);
}

void __fastcall TMainForm::VoicesButtonClicked(TObject *Sender, int IndexArg)
{
    SelectVoice(IndexArg);
}

void __fastcall TMainForm::ElementPropertiesButtonClicked
    (TObject *Sender, int IndexArg)
{
    if (!AnyElementSelected()) return;
    bool aAuto = false;
    SelectionVector::const_iterator i = aSelection.begin();
    while (i != aSelection.end())
    {
        if (*i)
        {
            *GetElementIndex() = i - aSelection.begin();
            bool aCancelled = !EditElementProperty(IndexArg, aAuto);
            if (aCancelled) return;
            aAuto = true;
        }
        i++;
    }
}

void __fastcall TMainForm::ElementsButtonClicked(TObject *Sender, int IndexArg)
{
    SelectElement(IndexArg);
}

void __fastcall TMainForm::TimeSettingsButtonClicked
    (TObject *Sender, int IndexArg)
{
    EditTimeSetting(IndexArg);
}

void __fastcall TMainForm::SonoritiesButtonClicked
    (TObject *Sender, int IndexArg)
{
    EditSonority(IndexArg);
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
