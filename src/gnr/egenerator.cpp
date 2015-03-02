/***************************************************************************
 *  C++ source
 *
 *  Element Generator for ION Generator
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
#include "egenerator.h"
#include <ciearg.h>
#include <cieexc.h>

#pragma package(smart_init)
#pragma resource "*.dfm"

TEGeneratorForm *EGeneratorForm;

__fastcall TEGeneratorForm::TEGeneratorForm(TComponent* Owner) :
    TForm        (Owner),
    PanoramFEP   (PanoramDefaults),
    FrequencyFEP (DefaultPreferences),
    VolumeFEP    (DefaultPreferences),
    aPanoram     (0, 1, 0),
    aFrequency   (0, 1, 1),
    aVolume      (0, 1, 1)
{
    Temperament->OnChange(this);
}

void __fastcall TEGeneratorForm::PanoramButtonClick(TObject *Sender)
{
    CieFncForm->Invoke("Common Panoram", &aPanoram, &PanoramFEP);
    if (CieFncForm->ModalResult != mrOk) return;
}

void __fastcall TEGeneratorForm::FrequencyButtonClick(TObject *Sender)
{
    CieFncForm->Invoke("Common Frequency", &aFrequency, &FrequencyFEP);
    if (CieFncForm->ModalResult != mrOk) return;
}

void __fastcall TEGeneratorForm::VolumeButtonClick(TObject *Sender)
{
    CieFncForm->Invoke("Common Volume", &aVolume, &VolumeFEP);
    if (CieFncForm->ModalResult != mrOk) return;
}

void __fastcall TEGeneratorForm::FromOctaveChange(TObject *Sender)
{
    int aTemperament = StrToInt(Temperament->Text);
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
    FromNote->ItemIndex = 0;
}

void __fastcall TEGeneratorForm::ToOctaveChange(TObject *Sender)
{
    int aTemperament = StrToInt(Temperament->Text);
    int aStart = ToOctave->ItemIndex * aTemperament;
    int aEnd = aStart + aTemperament;
    ToNote->Clear();
    for (int i = aStart; i < aEnd; i++)
    {
        double aFrequency = ionstd::ProduceFrequency(i, aTemperament);
        UnicodeString aItemName =
            Adapt(ionstd::FrequencyDescription(aFrequency));
        ToNote->Items->Add(aItemName);
    }
    ToNote->ItemIndex = 0;
}

void __fastcall TEGeneratorForm::TemperamentChange(TObject *Sender)
{
    int aTemperament = StrToInt(Temperament->Text);
    Scale->Clear();
    for (int i = 0; i < aTemperament; i++)
    {
        double aFrequency = ionstd::ProduceFrequency(i, aTemperament);
        UnicodeString aItemName =
            Adapt(ionstd::FrequencyDescription(aFrequency));
        Scale->Items->Add(aItemName);
        Scale->Checked[i] = true;
    }
    FromOctave->OnChange(this);
    ToOctave->OnChange(this);
}

void __fastcall TEGeneratorForm::ProcessButtonClick(TObject *Sender)
{
    ESTART
    // first, calculate basic values
    // - duration of whole note -
    double aWholeNote = StrToFloat(QuarterTime->Text) * 4;
    // - and collection of dividers -
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
    // second: it's time to create sounds and pauses
    ionstd::GElementsCreator aCreator;
    aCreator.GeneratePauses(GeneratePauses->Checked);
    aCreator.GenerateSounds(GenerateSounds->Checked);
    aCreator.WholeNote(aWholeNote);
    aCreator.DividersCount(aDividers.size());
    std::vector<double>::const_iterator i = aDividers.begin() - 1;
    while (++i != aDividers.end())
    {
        aCreator.Divider(*i, i - aDividers.begin());
    }
    unsigned int aTemperament = StrToInt(Temperament->Text);
    aCreator.FromPitch
        (FromOctave->ItemIndex * aTemperament + FromNote->ItemIndex);
    aCreator.ToPitch
        (ToOctave->ItemIndex * aTemperament + ToNote->ItemIndex);
    aCreator.Temperament(aTemperament);
    for (unsigned int i = 0; i < aTemperament; i++)
    {
            aCreator.Scale(Scale->Checked[i], i);
    }
    *(aCreator.Panoram()) = aPanoram;
    *(aCreator.Frequency()) = aFrequency;
    *(aCreator.Volume()) = aVolume;
    aCreator.ScalePanoram(PanoramScalling->Checked);
    aCreator.ScaleFrequency(FrequencyScalling->Checked);
    aCreator.ScaleVolume(VolumeScalling->Checked);
    aCreator.CreateGElementsFor(*aGenerator, 0);
    EEND
    ModalResult = mrOk;
    Hide();
}

void __fastcall TEGeneratorForm::CancelButtonClick(TObject *Sender)
{
    Close();
}

void __fastcall TEGeneratorForm::FormMouseDown
    (TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y)
{
    long SC_DRAGMOVE = 0xf012;
    if(Button == mbLeft)
    {
        ReleaseCapture();
        SendMessage(Handle, WM_SYSCOMMAND, SC_DRAGMOVE, 0);
    }
}
