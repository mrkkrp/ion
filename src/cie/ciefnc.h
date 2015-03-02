/***************************************************************************
 *  C++ header
 *
 *  Common Interface Elements (CIE) for Interface of Nature - Function
 *  Editor
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

#ifndef CIEFNCH
#define CIEFNCH

#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.ImgList.hpp>
#include <Math.hpp>
#include <Vcl.ActnList.hpp>
#include <Vcl.ActnMan.hpp>
#include <Vcl.PlatformDefaultStyleActnCtrls.hpp>
#include <Vcl.ActnCtrls.hpp>
#include <Vcl.ToolWin.hpp>
#include <System.Actions.hpp>
#include <Vcl.Graphics.hpp>
#include <stack>
#include <prl.h>

// constants:

const int
VPadding   (64),
HPadding   (32),
PRadius    (4),
SLineWidth (1),
GLineWidth (2);

const TColor
clrGrid          (clHighlightText),
clrBackground    (clAppWorkSpace),
clrFunction      (clHighlightText),
clrPoint         (clAppWorkSpace),
clrSelectedPoint (clActiveCaption);

enum GridMode { gmLinear, gmLogarithmic };

// this structure helps to save user preferences for every kind of
// function and load they when editor is invoked

struct FEP
{
    double aBottom, aCapacity;
    int aLines;
    GridMode aGridMode;
    int aPrecision;
}; // struct FEP

const PACKAGE FEP
DefaultPreferences,
PanoramDefaults,
FrequencyDefaults,
VolumeDefaults;

typedef std::stack<ionstd::Function> FunctionsStore;

class PACKAGE TCieFncForm : public TForm
{
    __published:    // IDE-managed Components
    TImageList *ImageList;
    TImage *Graph;
    TLabel *Caption;
    TActionManager *ActionManager;
    TActionToolBar *ToolBar;
    TAction *APointsCount;
    TAction *AMin;
    TAction *AMax;
    TAction *AKind;
    TAction *AUseRand;
    TAction *ARegular;
    TAction *ABottom;
    TAction *ACapacity;
    TAction *AVerticalCount;
    TAction *AGridMode;
    TAction *APrecision;
    TAction *AManually;
    TAction *AUndo;
    TAction *ASin;
    TAction *AMultiply;
    TAction *AAdd;
    TAction *AReverse;
    TAction *ANextPoint;
    TAction *AScreenUp;
    TAction *AScreenDown;
    TAction *AFinish;
    TAction *AEscape;
    TImageList *DistributionKind;
    void __fastcall FormResize(TObject *Sender);
    void __fastcall GraphMouseDown(TObject *Sender, TMouseButton Button,
                                   TShiftState Shift, int X, int Y);
    void __fastcall GraphMouseUp(TObject *Sender, TMouseButton Button,
                                 TShiftState Shift, int X, int Y);
    void __fastcall GraphMouseMove(TObject *Sender, TShiftState Shift,
                                   int X, int Y);
    void __fastcall APointsCountExecute(TObject *Sender);
    void __fastcall AMinExecute(TObject *Sender);
    void __fastcall AMaxExecute(TObject *Sender);
    void __fastcall AKindExecute(TObject *Sender);
    void __fastcall AUseRandExecute(TObject *Sender);
    void __fastcall ARegularExecute(TObject *Sender);
    void __fastcall ABottomExecute(TObject *Sender);
    void __fastcall ACapacityExecute(TObject *Sender);
    void __fastcall AVerticalCountExecute(TObject *Sender);
    void __fastcall AGridModeExecute(TObject *Sender);
    void __fastcall APrecisionExecute(TObject *Sender);
    void __fastcall AManuallyExecute(TObject *Sender);
    void __fastcall AUndoExecute(TObject *Sender);
    void __fastcall GraphMouseLeave(TObject *Sender);
    void __fastcall ASinExecute(TObject *Sender);
    void __fastcall AAddExecute(TObject *Sender);
    void __fastcall AMultiplyExecute(TObject *Sender);
    void __fastcall AReverseExecute(TObject *Sender);
    void __fastcall ANextPointExecute(TObject *Sender);
    void __fastcall AScreenUpExecute(TObject *Sender);
    void __fastcall AScreenDownExecute(TObject *Sender);
    void __fastcall AFinishExecute(TObject *Sender);
    void __fastcall AEscapeExecute(TObject *Sender);
    void __fastcall FormCreate(TObject *Sender);

    private:    // User Declarations
    TCanvas *aCanvas;
    ionstd::Function *eFunction; // external function
    FunctionsStore iFunctions;   // our internal functions
    // parameters of grid:
    double aMin, aMax, aXCost, aYCost;
    int aPointIndex; // actual position
    // these modes allow to edit a function with your mouse:
    bool aUserCatchPoint, aUserMoveGrid;
    FEP *eFEP, iFEP; // external preferences and internal

    void __fastcall CalculateCost();
    int __fastcall ArgToX(double) const;
    double __fastcall XToArg(int) const;
    int __fastcall ValueToY(double) const;
    double __fastcall YToValue(int) const;
    void __fastcall PaintBlock(int);
    void __fastcall PaintAll();
    void __fastcall RefreshControls();
    void __fastcall CheckForLog(ionstd::Function) const;
    const ionstd::Function *__fastcall iFnc() const;
    ionstd::Function *__fastcall iFnc();
    void __fastcall Register(ionstd::Function);
    void __fastcall Undo();

    public:     // User Declarations
    __fastcall TCieFncForm(TComponent* Owner);
        void __fastcall Invoke(UnicodeString, ionstd::Function *, FEP *);

}; // class PACKAGE TCieFncForm

extern PACKAGE TCieFncForm *CieFncForm;

#endif // CIEFNCH
