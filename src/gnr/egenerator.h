/***************************************************************************
 *  C++ header
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

#ifndef ELEMENTGENERATORH
#define ELEMENTGENERATORH

#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.CheckLst.hpp>
#include <Vcl.Dialogs.hpp>
#include <Vcl.ImgList.hpp>
#include <ciefnc.h>
#include <ion.h>

class TEGeneratorForm : public TForm
{
    __published:    // IDE-managed Components
    TGroupBox *DurationsBox;
    TGroupBox *PitchBox;
    TButton *ProcessButton;
    TButton *CancelButton;
    TCheckBox *GeneratePauses;
    TCheckBox *GenerateSounds;
    TCheckListBox *Scale;
    TLabeledEdit *Dividers;
    TImageList *ImageList;
    TGroupBox *FunctionsBox;
    TButton *PanoramButton;
    TCheckBox *PanoramScalling;
    TButton *VolumeButton;
    TCheckBox *VolumeScalling;
    TButton *FrequencyButton;
    TCheckBox *FrequencyScalling;
    TComboBox *Temperament;
    TLabel *TemperamentLabel;
    TLabel *FromPitchLabel;
    TLabel *ToPitchLabel;
    TComboBox *FromOctave;
    TComboBox *ToOctave;
    TComboBox *FromNote;
    TComboBox *ToNote;
    TLabeledEdit *QuarterTime;
    void __fastcall ProcessButtonClick(TObject *Sender);
    void __fastcall CancelButtonClick(TObject *Sender);
    void __fastcall PanoramButtonClick(TObject *Sender);
    void __fastcall VolumeButtonClick(TObject *Sender);
    void __fastcall FrequencyButtonClick(TObject *Sender);
    void __fastcall TemperamentChange(TObject *Sender);
    void __fastcall FromOctaveChange(TObject *Sender);
    void __fastcall ToOctaveChange(TObject *Sender);
    void __fastcall FormMouseDown(TObject *Sender, TMouseButton Button,
                                  TShiftState Shift, int X, int Y);
    private:    // User declarations
    ionstd::Generator *aGenerator;
    FEP PanoramFEP, FrequencyFEP, VolumeFEP;
    ionstd::Function aPanoram, aFrequency, aVolume;

    public:     // User declarations
    __fastcall TEGeneratorForm(TComponent* Owner);
    void DoForGenerator(ionstd::Generator *Arg) { aGenerator = Arg; }

}; // class TEGeneratorForm

extern PACKAGE TEGeneratorForm *EGeneratorForm;

#endif // ELEMENTGENERATORH
