/***************************************************************************
 *  C++ source
 *
 *  Wizard for ION Generator
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
#include "wizard.h"
#include <cieexc.h>

#pragma package(smart_init)
#pragma resource "*.dfm"

TGWizardForm *GWizardForm;

__fastcall TGWizardForm::TGWizardForm(TComponent* Owner) :
    TForm        (Owner),
    PanoramFEP   (PanoramDefaults),
    FrequencyFEP (DefaultPreferences),
    VolumeFEP    (DefaultPreferences)
{
    aGWizard.Label(Adapt(aDefaultLabel));
}

void __fastcall TGWizardForm::Refresh()
{
    ESTART
    // show parameters of GWizard on the form
    // 1. General Setup
    QuarterTime->Text = FloatToStr(aGWizard.WholeNote()) / 4.;
    Duration->Text = FloatToStr(aGWizard.RequestedDuration());
    Seed->Text = UIntToStr(aGWizard.Seed());
    Temperament->ItemIndex = aGWizard.Temperament() - 12;
    VoicesCount->ItemIndex = aGWizard.VoicesCount() - 1;
    Scale->Items->BeginUpdate();
    Scale->Items->Clear();
    unsigned int aTemperament = aGWizard.Temperament();
    for (unsigned int i = 0; i < aTemperament; i++)
    {
        TGrpButtonItem *aItem = Scale->Items->Add();
        aItem->Caption = Adapt(ionstd::FrequencyDescription
            (ionstd::ProduceFrequency(i, aTemperament)));
        int aImage = -1;
        switch(aGWizard.Scale(i))
        {
            case ionstd::ttPassing : aImage = 1; break;
            case ionstd::ttScale : aImage = 2; break;
            case ionstd::ttTonal : aImage = 3; break;
        }
        aItem->ImageIndex = aImage;
    }
    Scale->Items->EndUpdate();
    // 2. Voice Setup
    unsigned int aSelectedVoice = VoiceSelector->ItemIndex;
    VoiceSelector->Clear();
    for (unsigned int i = 0; i < aGWizard.VoicesCount(); i++)
    {
        VoiceSelector->Items->Add(UnicodeString("voice-") + UIntToStr(i));
    }
    if (aSelectedVoice >= aGWizard.VoicesCount())
        aSelectedVoice = (aGWizard.VoicesCount() - 1);
    VoiceSelector->ItemIndex = aSelectedVoice;
    ionstd::GVParamsIter aVoice = aGWizard.GetGVParams(aSelectedVoice);
    UnicodeString aDividers;
    for (unsigned int i = 0; i < aVoice->DividersCount(); i++)
    {
        aDividers += FloatToStr(aVoice->Divider(i)) + ' ';
    }
    Dividers->Text = aDividers;
    Repetitivity->Text = FloatToStr(aVoice->Repetitivity());
    TonalIntensity->Text = FloatToStr(aVoice->TonalIntensity());
    ModalIntensity->Text = FloatToStr(aVoice->ModalIntensity());
    LowAddiction->Text = FloatToStr(aVoice->LowAddiction());
    HighAddiction->Text = FloatToStr(aVoice->HighAddiction());
    Smoothness->Text = FloatToStr(aVoice->Smoothness());
    Leaping->Text = FloatToStr(aVoice->Leaping());
    Retention->Text = FloatToStr(aVoice->Retention());
    Velocity->Text = FloatToStr(aVoice->Velocity());
    Alignment->Text = FloatToStr(aVoice->Alignment());
    BeatDivider->Text = FloatToStr(aVoice->BeatDivider());
    SilenceShare->Text = FloatToStr(aVoice->SilenceShare());
    SilenceClustering->Text = FloatToStr(aVoice->SilenceClustering());
        FromOctave->ItemIndex = aVoice->FromPitch() / aTemperament;
    int aStart = FromOctave->ItemIndex * aTemperament;
    int aEnd = aStart + aTemperament;
    FromNote->Clear();
    for (int i = aStart; i < aEnd; i++)
    {
        double aFrequency = ionstd::ProduceFrequency(i, aTemperament);
        UnicodeString aItemName =
            Adapt(ionstd::FrequencyDescription(aFrequency));
        FromNote->Items->Add(aItemName);
    }
    FromNote->ItemIndex = aVoice->FromPitch() % aTemperament;
        ToOctave->ItemIndex = aVoice->ToPitch() / aTemperament;
    aStart = ToOctave->ItemIndex * aTemperament;
    aEnd = aStart + aTemperament;
    ToNote->Clear();
    for (int i = aStart; i < aEnd; i++)
    {
        double aFrequency = ionstd::ProduceFrequency(i, aTemperament);
        UnicodeString aItemName =
            Adapt(ionstd::FrequencyDescription(aFrequency));
        ToNote->Items->Add(aItemName);
    }
    ToNote->ItemIndex = aVoice->ToPitch() % aTemperament;
    PanoramScalling->Checked = aVoice->ScalePanoram();
    FrequencyScalling->Checked = aVoice->ScaleFrequency();
    VolumeScalling->Checked = aVoice->ScaleVolume();
    EEND
}

double __fastcall TGWizardForm::CalculateWholeNote()
{
    return 4 * StrToFloat(QuarterTime->Text);
}

void __fastcall TGWizardForm::FormShow(TObject *Sender)
{
    Refresh();
}

void __fastcall TGWizardForm::QuarterTimeExit(TObject *Sender)
{
    ESTART
    aGWizard.WholeNote(CalculateWholeNote());
    EEND
}

void __fastcall TGWizardForm::UseDurationClick(TObject *Sender)
{
    ESTART
    aGWizard.WholeNote(CalculateWholeNote());
    EEND
}

void __fastcall TGWizardForm::UseTempoClick(TObject *Sender)
{
    ESTART
    aGWizard.WholeNote(CalculateWholeNote());
    EEND
}

void __fastcall TGWizardForm::DurationExit(TObject *Sender)
{
    ESTART
    aGWizard.RequestedDuration(StrToFloat(Duration->Text));
    EEND
}

void __fastcall TGWizardForm::TemperamentChange(TObject *Sender)
{
    ESTART
    aGWizard.Temperament(StrToInt(Temperament->Text));
    EEND
    Refresh();
}

void __fastcall TGWizardForm::VoicesCountChange(TObject *Sender)
{
    ESTART
    aGWizard.VoicesCount(StrToInt(VoicesCount->Text));
    EEND
    Refresh();
}

void __fastcall TGWizardForm::ScaleButtonClicked(TObject *Sender, int IndexArg)
{
    ESTART
    ionstd::ToneType aType = aGWizard.Scale(IndexArg);
    switch (aType)
    {
        case ionstd::ttPassing : aType = ionstd::ttScale; break;
        case ionstd::ttScale : aType = ionstd::ttTonal; break;
        case ionstd::ttTonal : aType = ionstd::ttPassing;
    }
    aGWizard.Scale(aType, IndexArg);
    EEND
    Refresh();
}

void __fastcall TGWizardForm::VoiceSelectorChange(TObject *Sender)
{
    Refresh();
}

void __fastcall TGWizardForm::DividersExit(TObject *Sender)
{
    ESTART
    UnicodeString aDividersLine = Dividers->Text;
    aDividersLine = StringReplace
        (aDividersLine, ",", " ", TReplaceFlags() << rfReplaceAll);
    aDividersLine = StringReplace
        (aDividersLine, ";", " ", TReplaceFlags() << rfReplaceAll);
    aDividersLine.Trim();
    std::vector<double> aDividers;
    while (aDividersLine.Length())
    {
        aDividersLine = aDividersLine.TrimLeft();
        int aIndex = aDividersLine.Pos(" ");
        if (!aIndex) aIndex = aDividersLine.Length();
        double aDivider = StrToFloat
            (aDividersLine.SubString(1, aIndex));
        aDividersLine = aDividersLine.SubString
            (aIndex + 1, aDividersLine.Length() - aIndex);
        aDividers.push_back(aDivider);
    }
    ionstd::GVParamsIter aVoice =
        aGWizard.GetGVParams(VoiceSelector->ItemIndex);
    aVoice->DividersCount(aDividers.size());
    for (unsigned int i = 0; i < aDividers.size(); i++)
    {
        aVoice->Divider(*(aDividers.begin() + i), i);
    }
    EEND
    Refresh();
}

void __fastcall TGWizardForm::FromOctaveChange(TObject *Sender)
{
    ESTART
    ionstd::GVParamsIter aVoice =
        aGWizard.GetGVParams(VoiceSelector->ItemIndex);
    aVoice->FromPitch(FromOctave->ItemIndex * aGWizard.Temperament());
    EEND
    Refresh();
}

void __fastcall TGWizardForm::FromNoteChange(TObject *Sender)
{
    ESTART
    ionstd::GVParamsIter aVoice =
        aGWizard.GetGVParams(VoiceSelector->ItemIndex);
    aVoice->FromPitch(FromOctave->ItemIndex * aGWizard.Temperament()
        + FromNote->ItemIndex);
    EEND
    Refresh();
}

void __fastcall TGWizardForm::ToOctaveChange(TObject *Sender)
{
    ESTART
    ionstd::GVParamsIter aVoice =
        aGWizard.GetGVParams(VoiceSelector->ItemIndex);
    aVoice->ToPitch(ToOctave->ItemIndex * aGWizard.Temperament());
    EEND
    Refresh();
}

void __fastcall TGWizardForm::ToNoteChange(TObject *Sender)
{
    ESTART
    ionstd::GVParamsIter aVoice =
        aGWizard.GetGVParams(VoiceSelector->ItemIndex);
    aVoice->ToPitch(ToOctave->ItemIndex * aGWizard.Temperament()
        + ToNote->ItemIndex);
    EEND
    Refresh();
}

void __fastcall TGWizardForm::RepetitivityExit(TObject *Sender)
{
    ESTART
    ionstd::GVParamsIter aVoice =
        aGWizard.GetGVParams(VoiceSelector->ItemIndex);
    aVoice->Repetitivity(StrToFloat(Repetitivity->Text));
    EEND

}

void __fastcall TGWizardForm::TonalIntensityExit(TObject *Sender)
{
    ESTART
    ionstd::GVParamsIter aVoice =
        aGWizard.GetGVParams(VoiceSelector->ItemIndex);
    aVoice->TonalIntensity(StrToFloat(TonalIntensity->Text));
    EEND
}

void __fastcall TGWizardForm::ModalIntensityExit(TObject *Sender)
{
    ESTART
    ionstd::GVParamsIter aVoice =
        aGWizard.GetGVParams(VoiceSelector->ItemIndex);
    aVoice->ModalIntensity(StrToFloat(ModalIntensity->Text));
    EEND
}

void __fastcall TGWizardForm::LowAddictionExit(TObject *Sender)
{
    ESTART
    ionstd::GVParamsIter aVoice =
        aGWizard.GetGVParams(VoiceSelector->ItemIndex);
    aVoice->LowAddiction(StrToFloat(LowAddiction->Text));
    EEND
}

void __fastcall TGWizardForm::HighAddictionExit(TObject *Sender)
{
    ESTART
    ionstd::GVParamsIter aVoice =
        aGWizard.GetGVParams(VoiceSelector->ItemIndex);
    aVoice->HighAddiction(StrToFloat(HighAddiction->Text));
    EEND
}

void __fastcall TGWizardForm::SmoothnessExit(TObject *Sender)
{
    ESTART
    ionstd::GVParamsIter aVoice =
        aGWizard.GetGVParams(VoiceSelector->ItemIndex);
    aVoice->Smoothness(StrToFloat(Smoothness->Text));
    EEND
}

void __fastcall TGWizardForm::LeapingExit(TObject *Sender)
{
    ESTART
    ionstd::GVParamsIter aVoice =
        aGWizard.GetGVParams(VoiceSelector->ItemIndex);
    aVoice->Leaping(StrToFloat(Leaping->Text));
    EEND
}

void __fastcall TGWizardForm::RetentionExit(TObject *Sender)
{
    ESTART
    ionstd::GVParamsIter aVoice =
        aGWizard.GetGVParams(VoiceSelector->ItemIndex);
    aVoice->Retention(StrToFloat(Retention->Text));
    EEND
}

void __fastcall TGWizardForm::VelocityExit(TObject *Sender)
{
    ESTART
    ionstd::GVParamsIter aVoice =
        aGWizard.GetGVParams(VoiceSelector->ItemIndex);
    aVoice->Velocity(StrToFloat(Velocity->Text));
    EEND
}

void __fastcall TGWizardForm::AlignmentExit(TObject *Sender)
{
    ESTART
    ionstd::GVParamsIter aVoice =
        aGWizard.GetGVParams(VoiceSelector->ItemIndex);
    aVoice->Alignment(StrToFloat(Alignment->Text));
    EEND
}

void __fastcall TGWizardForm::BeatDividerExit(TObject *Sender)
{
    ESTART
    ionstd::GVParamsIter aVoice =
        aGWizard.GetGVParams(VoiceSelector->ItemIndex);
    aVoice->BeatDivider(StrToFloat(BeatDivider->Text));
    EEND
}

void __fastcall TGWizardForm::SilenceShareExit(TObject *Sender)
{
    ESTART
    ionstd::GVParamsIter aVoice =
        aGWizard.GetGVParams(VoiceSelector->ItemIndex);
    aVoice->SilenceShare(StrToFloat(SilenceShare->Text));
    EEND
}

void __fastcall TGWizardForm::SilenceClusteringExit(TObject *Sender)
{
    ESTART
    ionstd::GVParamsIter aVoice =
        aGWizard.GetGVParams(VoiceSelector->ItemIndex);
    aVoice->SilenceClustering(StrToFloat(SilenceClustering->Text));
    EEND
}

void __fastcall TGWizardForm::PanoramButtonClick(TObject *Sender)
{
    ionstd::GVParamsIter aVoice =
        aGWizard.GetGVParams(VoiceSelector->ItemIndex);
    CieFncForm->Invoke("Common Panoram", aVoice->Panoram(), &PanoramFEP);
    if (CieFncForm->ModalResult != mrOk) return;
}

void __fastcall TGWizardForm::PanoramScallingClick(TObject *Sender)
{
    ESTART
    ionstd::GVParamsIter aVoice =
        aGWizard.GetGVParams(VoiceSelector->ItemIndex);
    aVoice->ScalePanoram(PanoramScalling->Checked);
    EEND
}

void __fastcall TGWizardForm::FrequencyButtonClick(TObject *Sender)
{
    ionstd::GVParamsIter aVoice =
        aGWizard.GetGVParams(VoiceSelector->ItemIndex);
    CieFncForm->Invoke("Common Frequency", aVoice->Frequency(), &FrequencyFEP);
    if (CieFncForm->ModalResult != mrOk) return;
}

void __fastcall TGWizardForm::FrequencyScallingClick(TObject *Sender)
{
    ESTART
    ionstd::GVParamsIter aVoice =
        aGWizard.GetGVParams(VoiceSelector->ItemIndex);
    aVoice->ScaleFrequency(FrequencyScalling->Checked);
    EEND
}

void __fastcall TGWizardForm::VolumeButtonClick(TObject *Sender)
{
    ionstd::GVParamsIter aVoice =
        aGWizard.GetGVParams(VoiceSelector->ItemIndex);
    CieFncForm->Invoke("Common Volume", aVoice->Volume(), &VolumeFEP);
    if (CieFncForm->ModalResult != mrOk) return;
}

void __fastcall TGWizardForm::VolumeScallingClick(TObject *Sender)
{
    ESTART
    ionstd::GVParamsIter aVoice =
        aGWizard.GetGVParams(VoiceSelector->ItemIndex);
    aVoice->ScaleVolume(VolumeScalling->Checked);
    EEND
}

void __fastcall TGWizardForm::SeedExit(TObject *Sender)
{
    ESTART
    aGWizard.Seed(StrToInt(Seed->Text));
    EEND
}

void __fastcall TGWizardForm::SaveButtonClick(TObject *Sender)
{
    if (!SaveDialog->Execute()) return;
    ESTART
    ionstd::Paramlist aList;
    aGWizard.Label(Adapt(aDefaultLabel));
    aGWizard.WriteTo(aList);
    aList.Save(Adapt(SaveDialog->FileName));
    EEND
}

void __fastcall TGWizardForm::LoadButtonClick(TObject *Sender)
{
    if (!OpenDialog->Execute()) return;
    ESTART
    ionstd::Paramlist aList;
    aList.Load(Adapt(OpenDialog->FileName));
    aGWizard.ReadFrom(aList, Adapt(aDefaultLabel));
    EEND
    Refresh();
}

void __fastcall TGWizardForm::ProcessButtonClick(TObject *Sender)
{
    ESTART
    aGWizard.ApplyWizardFor(aGenerator);
    EEND
}

void __fastcall TGWizardForm::FormMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y)
{
    long SC_DRAGMOVE = 0xf012;
    if(Button == mbLeft)
    {
        ReleaseCapture();
        SendMessage(Handle, WM_SYSCOMMAND, SC_DRAGMOVE, 0);
    }
}

void __fastcall TGWizardForm::FormPaint(TObject *Sender)
{
    Scale->Canvas->Brush->Color = clAppWorkSpace;
    Scale->Canvas->Brush->Style = bsSolid;
    Scale->Canvas->FloodFill
    (290, 142, Scale->Canvas->Pixels[290][142], fsSurface);
}
