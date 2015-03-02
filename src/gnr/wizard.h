/***************************************************************************
 *  C++ header
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

#ifndef GENERATIVEWIZARDH
#define GENERATIVEWIZARDH

#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.ButtonGroup.hpp>
#include <Vcl.ImgList.hpp>
#include <Vcl.Dialogs.hpp>
#include <ciefnc.h>
#include <ion.h>

class TGWizardForm : public TForm
{
    __published:    // IDE-managed Components
    TGroupBox *VoiceSetupBox;
    TButton *ProcessButton;
    TButton *CancelButton;
    TGroupBox *GeneralSetupBox;
    TLabeledEdit *Duration;
    TLabel *TemperamentLabel;
    TComboBox *Temperament;
    TButtonGroup *Scale;
    TLabeledEdit *Dividers;
    TLabel *FromPitchLabel;
    TComboBox *FromOctave;
    TComboBox *FromNote;
    TLabel *ToPitchLabel;
    TComboBox *ToOctave;
    TComboBox *ToNote;
    TButton *PanoramButton;
    TCheckBox *PanoramScalling;
    TButton *FrequencyButton;
    TCheckBox *FrequencyScalling;
    TButton *VolumeButton;
    TCheckBox *VolumeScalling;
    TButton *SaveButton;
    TButton *LoadButton;
    TComboBox *VoiceSelector;
    TLabel *VoicesCountLabel;
    TComboBox *VoicesCount;
    TImageList *ImageList;
    TLabeledEdit *Repetitivity;
    TLabeledEdit *TonalIntensity;
    TLabeledEdit *ModalIntensity;
    TLabeledEdit *Smoothness;
    TLabeledEdit *Retention;
    TLabeledEdit *Velocity;
    TLabeledEdit *Alignment;
    TLabeledEdit *BeatDivider;
    TLabeledEdit *SilenceShare;
    TLabeledEdit *SilenceClustering;
    TOpenDialog *OpenDialog;
    TSaveDialog *SaveDialog;
    TLabeledEdit *Leaping;
    TLabeledEdit *HighAddiction;
    TLabeledEdit *LowAddiction;
    TLabeledEdit *QuarterTime;
    TLabeledEdit *Seed;
    void __fastcall FormShow(TObject *Sender);
    void __fastcall UseDurationClick(TObject *Sender);
    void __fastcall UseTempoClick(TObject *Sender);
    void __fastcall DurationExit(TObject *Sender);
    void __fastcall TemperamentChange(TObject *Sender);
    void __fastcall VoicesCountChange(TObject *Sender);
    void __fastcall ScaleButtonClicked(TObject *Sender, int Index);
    void __fastcall VoiceSelectorChange(TObject *Sender);
    void __fastcall DividersExit(TObject *Sender);
    void __fastcall FromOctaveChange(TObject *Sender);
    void __fastcall FromNoteChange(TObject *Sender);
    void __fastcall ToOctaveChange(TObject *Sender);
    void __fastcall ToNoteChange(TObject *Sender);
    void __fastcall RepetitivityExit(TObject *Sender);
    void __fastcall TonalIntensityExit(TObject *Sender);
    void __fastcall ModalIntensityExit(TObject *Sender);
    void __fastcall SmoothnessExit(TObject *Sender);
    void __fastcall RetentionExit(TObject *Sender);
    void __fastcall VelocityExit(TObject *Sender);
    void __fastcall AlignmentExit(TObject *Sender);
    void __fastcall BeatDividerExit(TObject *Sender);
    void __fastcall SilenceShareExit(TObject *Sender);
    void __fastcall SilenceClusteringExit(TObject *Sender);
    void __fastcall PanoramButtonClick(TObject *Sender);
    void __fastcall FrequencyButtonClick(TObject *Sender);
    void __fastcall PanoramScallingClick(TObject *Sender);
    void __fastcall FrequencyScallingClick(TObject *Sender);
    void __fastcall VolumeButtonClick(TObject *Sender);
    void __fastcall VolumeScallingClick(TObject *Sender);
    void __fastcall SeedExit(TObject *Sender);
    void __fastcall SaveButtonClick(TObject *Sender);
    void __fastcall LoadButtonClick(TObject *Sender);
    void __fastcall ProcessButtonClick(TObject *Sender);
    void __fastcall LowAddictionExit(TObject *Sender);
    void __fastcall HighAddictionExit(TObject *Sender);
    void __fastcall LeapingExit(TObject *Sender);
    void __fastcall QuarterTimeExit(TObject *Sender);
    void __fastcall FormMouseDown(TObject *Sender, TMouseButton Button,
                                  TShiftState Shift, int X, int Y);
    void __fastcall FormPaint(TObject *Sender);
    private:    // User declarations
    ionstd::GWizard aGWizard;
    ionstd::Generator aGenerator;
    FEP PanoramFEP, FrequencyFEP, VolumeFEP;
    UnicodeString aDefaultLabel;

    public:     // User declarations
    __fastcall TGWizardForm(TComponent* Owner);
    void __fastcall Refresh();
    double __fastcall CalculateWholeNote();
    ionstd::Generator GetGenerator() const { return aGenerator; }
    void UseDefaultLabel(UnicodeString Arg) { aDefaultLabel = Arg; }

}; // class TGWizardFrom

extern PACKAGE TGWizardForm *GWizardForm;

#endif // GENERATIVEWIZARDH
