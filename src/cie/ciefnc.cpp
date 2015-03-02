/***************************************************************************
 *  C++ source
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

#include <vcl.h>
#pragma hdrstop
#include "ciefnc.h"
#include "ciearg.h"
#include "ciechs.h"
#include "cieexc.h"

#pragma package(smart_init)
#pragma resource "*.dfm"

TCieFncForm *CieFncForm;

const FEP
DefaultPreferences  = { 0.,   1., 10, gmLinear,      2 },
PanoramDefaults     = { -1.,  2., 10, gmLinear,      2 },
FrequencyDefaults   = { 440., 2., 12, gmLogarithmic, 5 },
VolumeDefaults      = { 0.,   1., 10, gmLinear,      2 };

ionstd::String
fnceInvalidInLog  ("can't show non-positive value in log mode"),
fnceInvalidBottom ("invalid bottom value in log mode"),
fnceRangeError    ("min cannot be bigger than max"),
fnceCapacity      ("invalid vertical capacity"),
fnceLines         ("invalid number of horizontal lines"),
fncePrecision     ("invalid precision");

namespace ionstd { PLACEEXCEPTIONFNCHERE }

/***************************************************************************
 *  General
 **************************************************************************/

void __fastcall TCieFncForm::CalculateCost()
{
    aMin = iFnc()->Min();
    aMax = iFnc()->Max();
    // here we calculate cost of one pixel:
    if (iFnc()->SectionLength())
    {
        aXCost = aMax - aMin;
        aXCost /= Graph->Width - 2 * HPadding;
    }
    else aXCost = 1;
    // a little harder here.. we have two cases:
    if (iFEP.aGridMode == gmLinear) // linear:
    {
        aYCost = iFEP.aCapacity;
        aYCost /= Graph->Height - 2 * VPadding;
    }
    else // and logarithmic (count of pixels for 2x value)
    {
        aYCost = Graph->Height - 2 * VPadding;
    }
}

int __fastcall TCieFncForm::ArgToX(double Arg) const
{
    return HPadding + (Arg - aMin) / aXCost;
}

double __fastcall TCieFncForm::XToArg(int X) const
{
    return (X - HPadding) * aXCost + aMin;
}

int __fastcall TCieFncForm::ValueToY(double Value) const
{
    double aResult;
    if (iFEP.aGridMode == gmLinear)
    {
        aResult = iFEP.aCapacity - Value + iFEP.aBottom;
        aResult /= aYCost;
        aResult += VPadding;
    }
    else /* logarithmic mode */
    {
        aResult = Graph->Height - VPadding;
        aResult -= aYCost * (std::log10(Value / iFEP.aBottom)
            / std::log10(iFEP.aCapacity));
    }
    return aResult;
}

double __fastcall TCieFncForm::YToValue(int Y) const
{
    double aResult;
    if (Y > (Graph->Height - VPadding)) return iFEP.aBottom;
    if (iFEP.aGridMode == gmLinear)
    {
        if (Y < VPadding) return iFEP.aBottom + iFEP.aCapacity;
        aResult = (Y - VPadding) * aYCost;
        aResult -= iFEP.aCapacity + iFEP.aBottom;
        aResult *= -1;
    }
    else /* logarithmic mode */
    {
        if (Y < VPadding) return iFEP.aBottom * iFEP.aCapacity;
        aResult = iFEP.aBottom;
        double aY = - Y + Graph->Height - VPadding;
        aResult *= std::pow(iFEP.aCapacity, aY / aYCost);
    }
    return aResult;
}

void __fastcall TCieFncForm::RefreshControls()
{
    AKind->ImageIndex = iFnc()->Smooth() ? 3 : 4;
    AGridMode->ImageIndex = iFEP.aGridMode ? 10 : 11;
}

void __fastcall TCieFncForm::CheckForLog(ionstd::Function FunctionArg) const
{
    for (unsigned int i = 0; i < FunctionArg.PointsCount(); i++)
    if (FunctionArg.PointValue(i) <= 0) THROW_RE(fnceInvalidInLog);
}

const ionstd::Function *__fastcall TCieFncForm::iFnc() const
{
    return &iFunctions.top();
}

ionstd::Function *__fastcall TCieFncForm::iFnc()
{
    return &iFunctions.top();
}

void __fastcall TCieFncForm::Register(ionstd::Function FunctionArg)
{
    iFunctions.push(FunctionArg);
}

void __fastcall TCieFncForm::Undo()
{
    if (iFunctions.size() > 1) iFunctions.pop();
}

__fastcall TCieFncForm::TCieFncForm(TComponent* Owner) :
    TForm (Owner),
    iFEP  (DefaultPreferences)
{
    aCanvas = Graph->Canvas;
}

void __fastcall TCieFncForm::FormCreate(TObject *Sender)
{
    Randomize(); // for RandG
    randomize(); // for rand
}

void __fastcall TCieFncForm::PaintBlock(int BlockArg)
{
    int aCount = iFnc()->PointsCount();
    double HStep = iFnc()->SectionLength();
    if (!HStep) HStep = aMax - aMin;
    // paint background
    int x0, y0 = 0, x1, y1 = Graph->Height;
    if (BlockArg) x0 = ArgToX(aMin + HStep * (BlockArg - 1));
    else x0 = 0;
    if (BlockArg < (int)iFnc()->PointsCount() - 2)
        x1 = ArgToX(aMin + HStep * (BlockArg + 1));
    else x1 = Graph->Width;
    aCanvas->Brush->Color = clrBackground;
    aCanvas->Brush->Style = bsSolid;
    aCanvas->FillRect(Rect(x0, y0, x1, y1));
    // paint grid
    aCanvas->Pen->Color = clrGrid;
    aCanvas->Pen->Width = SLineWidth;
    aCanvas->Pen->Style = psSolid;
    // - vertical -
    int VStart = BlockArg ? BlockArg - 1 : 0;
    int VEnd = (BlockArg == aCount - 1) ? aCount - 1 : BlockArg + 1;
    for (int n = VStart; n <= VEnd; n++)
    {
        int X = ArgToX(aMin + n * HStep);
        aCanvas->MoveTo(X, VPadding);
        aCanvas->LineTo(X, Graph->Height - VPadding);
    }
    // - horizontal -
    double aStep, aTop;
    if (iFEP.aGridMode == gmLinear)
    {
        aStep = iFEP.aCapacity / iFEP.aLines;
        aTop = iFEP.aBottom + iFEP.aCapacity;
    }
    else // logarithmic grid
    {
        double Y = 1. / iFEP.aLines;
        aStep = std::pow(iFEP.aCapacity, Y);
        aTop = iFEP.aBottom * iFEP.aCapacity;
    }
    for (double Value = iFEP.aBottom; Value <= aTop;
        iFEP.aGridMode ? Value *= aStep : Value += aStep)
    {
        int Y = ValueToY(Value);
        aCanvas->MoveTo(HPadding, Y);
        aCanvas->LineTo(Graph->Width - HPadding, Y);
    }
    // paint function
    aCanvas->Pen->Color = clrFunction;
    aCanvas->Pen->Width = GLineWidth;
    for (int i = VStart; i <= VEnd; i++)
    {
    double Arg = aMin + i * HStep;
    double Value = iFnc()->PointValue(i);
    if (i == VStart) aCanvas->MoveTo(ArgToX(Arg), ValueToY(Value));
    else
    {
        if (iFnc()->Smooth())
            aCanvas->LineTo(ArgToX(Arg), ValueToY(Value));
        else
        {
            aCanvas->LineTo(ArgToX(Arg), aCanvas->PenPos.Y);
            aCanvas->LineTo(ArgToX(Arg), ValueToY(Value));
            if (i == aCount - 1)
            aCanvas->LineTo(ArgToX(aMax), aCanvas->PenPos.Y);
        }
    }
    if (VStart == VEnd) aCanvas->LineTo(ArgToX(aMax), ValueToY(Value));
    } // for
    aCanvas->FillRect(Rect(x0, y0, x1, VPadding - PRadius));
    aCanvas->FillRect(Rect(x0, y1 - VPadding + PRadius, x1, y1));
    // paint ponts and refresh info hint
    aCanvas->Pen->Color = clrFunction;
    aCanvas->Pen->Width = GLineWidth;
    for (int i = VStart; i <= VEnd; i++)
    {
        x0 = ArgToX(aMin + i * HStep) - PRadius;
        double Value = iFnc()->PointValue(i);
        y0 = ValueToY(Value) - PRadius;
        x1 = x0 + 2 * PRadius;
        y1 = y0 + 2 * PRadius;
        aCanvas->Brush->Color =
            (i == aPointIndex) ? clrSelectedPoint : clrPoint;
        if ((y0 > VPadding - PRadius - GLineWidth) &&
            (y1 < Graph->Height - VPadding + PRadius + GLineWidth))
                aCanvas->Ellipse(x0, y0, x1, y1);
    }
    if (aPointIndex >= 0)
    {
        Hint = "f(" + FloatToStrF(aMin + aPointIndex * HStep,
            ffGeneral, iFEP.aPrecision, 10) + ") = ";
        Hint += FloatToStrF(iFnc()->PointValue(aPointIndex),
            ffGeneral, iFEP.aPrecision, 10) + "; p = ";
        Hint += IntToStr(aPointIndex);
    }
    else Hint = "";
}

void __fastcall TCieFncForm::PaintAll()
{
    CalculateCost();
    int aCount = iFnc()->PointsCount();
    for (int i = 0; i < aCount; i += 2) PaintBlock(i);
}

void __fastcall TCieFncForm::Invoke
    (UnicodeString CaptionArg, ionstd::Function *aFncArg, FEP *aFEPArg)
{
    Caption->Caption = CaptionArg;
    eFunction = aFncArg;
    Register(*eFunction);
    eFEP = aFEPArg;
    iFEP = *eFEP;
    aUserCatchPoint = false;
    aUserMoveGrid = false;
    aPointIndex = -1;
    PaintAll();
    RefreshControls();
    ShowModal();
}

/***************************************************************************
 *  Interactive Elements
 **************************************************************************/

void __fastcall TCieFncForm::GraphMouseDown
(TObject *Sender, TMouseButton Button, TShiftState Shift, int XArg, int YArg)
{
    if (
        (XArg > HPadding - PRadius) &&
        (XArg < Graph->Width - HPadding + PRadius) &&
        (YArg > VPadding - PRadius) &&
        (YArg < Graph->Height - VPadding + PRadius))
    {
    if (Button == mbLeft) // left mouse button var
    {
    if (!aUserCatchPoint) Register(*iFnc());
    // here we have to check if user clicks on some 'point'
    // first, we have to make sure that user clicks on right vertical
    // position, also we should remember index of the point
    double aStep = iFnc()->SectionLength();
    int aPointsCount = iFnc()->PointsCount();
    for (aPointIndex = 0; aPointIndex < aPointsCount; aPointIndex++)
    {
        int aX = ArgToX(aMin + aPointIndex * aStep) - PRadius;
        if ((XArg >= aX) && (XArg <= aX + 2 * PRadius))
        { aUserCatchPoint = true; break; }
    }
    // second: we should test horizontal position for alleged point
    if (aUserCatchPoint)
    {
        int aY = ValueToY(iFnc()->PointValue(aPointIndex)) - PRadius;
        if (!((YArg >= aY) && (YArg <= aY + 2 * PRadius)))
        aPointIndex = -1, aUserCatchPoint = false;
    }
    else aPointIndex = -1; // if user didn't catch a point, selection
                           // should be removed
    if (aPointIndex != -1) PaintBlock(aPointIndex); else PaintAll();
    }
    else if (Button == mbRight) aUserMoveGrid = true;
    }
    else
    {
        long SC_DRAGMOVE = 0xf012;
        if(Button == mbLeft)
        {
            ReleaseCapture();
            SendMessage(Handle, WM_SYSCOMMAND, SC_DRAGMOVE, 0);
        }
        }
}

void __fastcall TCieFncForm::GraphMouseUp
    (TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y)
{
    if (aUserCatchPoint) aUserCatchPoint = false;
    if (aUserMoveGrid) aUserMoveGrid = false;
}

void __fastcall TCieFncForm::GraphMouseMove
    (TObject *Sender, TShiftState Shift, int, int YArg)
{
    static double Value = YToValue(YArg);
    if (aUserCatchPoint)
    {
        iFnc()->PointValue(YToValue(YArg), aPointIndex);
        PaintBlock(aPointIndex);
    }
    if (aUserMoveGrid)
    {
        double Temp = YToValue(YArg) - Value;
        if ((iFEP.aGridMode != gmLogarithmic) ||
            (iFEP.aBottom > Temp)) iFEP.aBottom -= Temp;
        PaintAll();
    }
    Value = YToValue(YArg);
}

void __fastcall TCieFncForm::GraphMouseLeave(TObject *Sender)
{
    if (aUserCatchPoint) aUserCatchPoint = false;
    if (aUserMoveGrid) aUserMoveGrid = false;
}

void __fastcall TCieFncForm::FormResize(TObject *Sender)
{
    Graph->Picture->Bitmap->Width = Graph->Width;
        Graph->Picture->Bitmap->Height = Graph->Height;
    PaintAll();
}

/***************************************************************************
 *  Actions
 **************************************************************************/

void __fastcall TCieFncForm::APointsCountExecute(TObject *Sender)
{
    ESTART
    ionstd::Function iFunction(*iFnc());
    CieArgForm->Invoke("Number of Points", iFunction.PointsCount());
    if (CieArgForm->ModalResult != mrOk) return;
    int aPointsCount = CieArgForm->ResultAsInt();
    if (aPointsCount < 0) aPointsCount = 0;
    iFunction.PointsCount(aPointsCount);
    if (iFEP.aGridMode == gmLogarithmic) CheckForLog(iFunction);
    Register(iFunction);
    if (aPointIndex >= (int)aPointsCount) aPointIndex = -1;
    EEND
    PaintAll();
}

void __fastcall TCieFncForm::AMinExecute(TObject *Sender)
{
    ESTART
    ionstd::Function iFunction(*iFnc());
    CieArgForm->Invoke("Min Argument", aMin);
    if (CieArgForm->ModalResult != mrOk) return;
    if (CieArgForm->ResultAsDouble() > aMax) THROW_RE(fnceRangeError);
    iFunction.Min(CieArgForm->ResultAsDouble());
    Register(iFunction);
    EEND
    PaintAll();
}

void __fastcall TCieFncForm::AMaxExecute(TObject *Sender)
{
    ESTART
    ionstd::Function iFunction(*iFnc());
    CieArgForm->Invoke("Max Argument", aMax);
    if (CieArgForm->ModalResult != mrOk) return;
    if (CieArgForm->ResultAsDouble() < aMin) THROW_RE(fnceRangeError);
    iFunction.Max(CieArgForm->ResultAsDouble());
    Register(iFunction);
    EEND
    PaintAll();
}

void __fastcall TCieFncForm::AKindExecute(TObject *Sender)
{
    ESTART
    ionstd::Function iFunction(*iFnc());
    bool aKind = !iFunction.Smooth();
    iFunction.Smooth(aKind);
    Register(iFunction);
    EEND
    PaintAll();
    RefreshControls();
}

void __fastcall TCieFncForm::AManuallyExecute(TObject *Sender)
{
    ESTART
    if (aPointIndex == -1) aPointIndex = 0;
    else
    {
	ionstd::Function iFunction(*iFnc());
        CieArgForm->Invoke("Set Value", iFunction.PointValue(aPointIndex));
        if (CieArgForm->ModalResult != mrOk) return;
        iFunction.PointValue(CieArgForm->ResultAsDouble(), aPointIndex);
        if (iFEP.aGridMode == gmLogarithmic) CheckForLog(iFunction);
        Register(iFunction);
    }
    EEND
    PaintAll();
}

void __fastcall TCieFncForm::AAddExecute(TObject *Sender)
{
    ESTART
    ionstd::Function iFunction(*iFnc());
    CieArgForm->Invoke("Add Value to All", 0.);
    if (CieArgForm->ModalResult != mrOk) return;
    double aValue = CieArgForm->ResultAsDouble();
    int aPointsCount = iFunction.PointsCount();
    for (int i = 0; i < aPointsCount; iFunction.PointValue
        (iFunction.PointValue(i) + aValue, i), i++);
    if (iFEP.aGridMode == gmLogarithmic) CheckForLog(iFunction);
    Register(iFunction);
    EEND
    PaintAll();
}

void __fastcall TCieFncForm::AMultiplyExecute(TObject *Sender)
{
    ESTART
    ionstd::Function iFunction(*iFnc());
    CieArgForm->Invoke("Multiply Value to All", 1.);
    if (CieArgForm->ModalResult != mrOk) return;
    double aValue = CieArgForm->ResultAsDouble();
    int aPointsCount = iFunction.PointsCount();
    for (int i = 0; i < aPointsCount; iFunction.PointValue
        (iFunction.PointValue(i) * aValue, i), i++);
    if (iFEP.aGridMode == gmLogarithmic) CheckForLog(iFunction);
    Register(iFunction);
    EEND
    PaintAll();
}

void __fastcall TCieFncForm::AReverseExecute(TObject *Sender)
{
    ESTART
    ionstd::Function iFunction(*iFnc());
    ionstd::Function tFunction(iFunction);
    int aPointsCount = iFunction.PointsCount();
    int tShift = aPointsCount - 1;
    for (int i = 0; i < aPointsCount; iFunction.PointValue
        (tFunction.PointValue(tShift - i), i), i++);
    if (iFEP.aGridMode == gmLogarithmic) CheckForLog(iFunction);
    Register(iFunction);
    EEND
    PaintAll();
}

void __fastcall TCieFncForm::AUseRandExecute(TObject *Sender)
{
    ESTART
    ionstd::Function iFunction(*iFnc());
    CieChsForm->Invoke("Distribution", DistributionKind, "ug");
    if (CieChsForm->ModalResult != mrOk) return;
    bool aGaussian = CieChsForm->GetIndex();
    CieArgForm->Invoke(aGaussian ? "Mean" : "Left Bound", 0);
    if (CieArgForm->ModalResult != mrOk) return;
    double A = CieArgForm->ResultAsDouble();
    CieArgForm->Invoke(aGaussian ? "Variance" : "Right Bound", 0);
    if (CieArgForm->ModalResult != mrOk) return;
    double B = CieArgForm->ResultAsDouble();
    int aCount = iFunction.PointsCount();
    for (int i = 0; i < aCount; iFunction.PointValue
    ((aGaussian ? RandG(A, B) : A + ((B - A) * rand()) / RAND_MAX), i++));
    if (iFEP.aGridMode == gmLogarithmic) CheckForLog(iFunction);
    Register(iFunction);
    EEND
    PaintAll();
}

void __fastcall TCieFncForm::ARegularExecute(TObject *Sender)
{
    ESTART
    ionstd::Function iFunction(*iFnc());
    CieArgForm->Invoke("Process Every Nth", 1);
    if (CieArgForm->ModalResult != mrOk) return;
    int aPeriod = CieArgForm->ResultAsInt();
    if (!aPeriod) aPeriod = 1;
    CieArgForm->Invoke("Initial Shift", 0);
    if (CieArgForm->ModalResult != mrOk) return;
    int aShift = CieArgForm->ResultAsInt();
    if (aShift < 0) aShift = 0;
    CieArgForm->Invoke("Value", 0);
    if (CieArgForm->ModalResult != mrOk) return;
    double aValue = CieArgForm->ResultAsDouble();
    int aCount = iFunction.PointsCount();
    if ((aShift >= aCount) || (aPeriod > aCount)) return;
    for (int i = aShift; i < aCount; i++)
    if (!((i - aShift) % aPeriod)) iFunction.PointValue(aValue, i);
    if (iFEP.aGridMode == gmLogarithmic) CheckForLog(iFunction);
    Register(iFunction);
    EEND
    PaintAll();
}

void __fastcall TCieFncForm::ASinExecute(TObject *Sender)
{
    ESTART
    ionstd::Function iFunction(*iFnc());
    int aPointsCount = iFunction.PointsCount();
    int tPointsCount = iFunction.Smooth() ?
        (aPointsCount - 1) : (aPointsCount);
    if (!tPointsCount) tPointsCount = 1;
    double aStep = 2. * M_PI / tPointsCount;
    for (int i = 0; i < aPointsCount;
        iFunction.PointValue(std::sin(i * aStep), i), i++);
    if (iFEP.aGridMode == gmLogarithmic) CheckForLog(iFunction);
    Register(iFunction);
    EEND
    PaintAll();
}

void __fastcall TCieFncForm::ABottomExecute(TObject *Sender)
{
    ESTART
    CieArgForm->Invoke("Bottom Value", iFEP.aBottom);
    if (CieArgForm->ModalResult != mrOk) return;
    double aBottom = CieArgForm->ResultAsDouble();
    if ((iFEP.aGridMode == gmLogarithmic) && (aBottom <= 0))
        THROW_RE(fnceInvalidBottom);
    iFEP.aBottom = aBottom;
    EEND
    PaintAll();
}

void __fastcall TCieFncForm::ACapacityExecute(TObject *Sender)
{
    ESTART
    CieArgForm->Invoke("Vertical Capacity", iFEP.aCapacity);
    if (CieArgForm->ModalResult != mrOk) return;
    double aCapacity = CieArgForm->ResultAsDouble();
    if (iFEP.aGridMode == gmLinear)
    {
        if (aCapacity <= 0) THROW_RE(fnceCapacity);
    }
    else /* log var */
    {
        if (aCapacity <= 1) THROW_RE(fnceCapacity);
    }
    iFEP.aCapacity = aCapacity;
    EEND
    PaintAll();
}

void __fastcall TCieFncForm::AVerticalCountExecute(TObject *Sender)
{
    ESTART
    CieArgForm->Invoke("Number of Horizontal Lines", iFEP.aLines);
    if (CieArgForm->ModalResult != mrOk) return;
    int aLines = CieArgForm->ResultAsInt();
    if (aLines <= 0) THROW_RE(fnceLines);
    iFEP.aLines = aLines;
    EEND
    PaintAll();
}

void __fastcall TCieFncForm::AGridModeExecute(TObject *Sender)
{
    ESTART
    GridMode aMode = iFEP.aGridMode;
    if (aMode == gmLinear) // from linear to log mode:
    {
        CheckForLog(*iFnc());
        if (iFEP.aBottom <= 0) THROW_RE(fnceInvalidBottom);
        iFEP.aCapacity += iFEP.aBottom;
        iFEP.aCapacity /= iFEP.aBottom;
        iFEP.aGridMode = gmLogarithmic;
    }
    else // from log to linear mode:
    {
        iFEP.aCapacity *= iFEP.aBottom;
        iFEP.aCapacity -= iFEP.aBottom;
        iFEP.aGridMode = gmLinear;
    }
    EEND
    PaintAll();
    RefreshControls();
}

void __fastcall TCieFncForm::APrecisionExecute(TObject *Sender)
{
    ESTART
    CieArgForm->Invoke("Precision", iFEP.aPrecision);
    if (CieArgForm->ModalResult != mrOk) return;
    int aPrecision = CieArgForm->ResultAsInt();
    if (aPrecision < 0) THROW_RE(fncePrecision);
    iFEP.aPrecision = aPrecision;
    EEND
    PaintAll();
}

void __fastcall TCieFncForm::AUndoExecute(TObject *Sender)
{
    Undo();
    if (aPointIndex >= (int)iFnc()->PointsCount()) aPointIndex = -1;
    PaintAll();
    RefreshControls();
}

void __fastcall TCieFncForm::ANextPointExecute(TObject *Sender)
{
    aPointIndex = (aPointIndex + 1) % iFnc()->PointsCount();
    PaintAll();
}

void __fastcall TCieFncForm::AScreenUpExecute(TObject *Sender)
{
    if (iFEP.aGridMode == gmLinear) iFEP.aBottom += iFEP.aCapacity;
    else iFEP.aBottom *= iFEP.aCapacity;
    PaintAll();
}

void __fastcall TCieFncForm::AScreenDownExecute(TObject *Sender)
{
    if (iFEP.aGridMode == gmLinear) iFEP.aBottom -= iFEP.aCapacity;
    else iFEP.aBottom /= iFEP.aCapacity;
    PaintAll();
}

void __fastcall TCieFncForm::AFinishExecute(TObject *Sender)
{
    ModalResult = mrOk;
    *eFunction = *iFnc();
    *eFEP = iFEP;
        while (!iFunctions.empty()) iFunctions.pop();
    Hide();
}

void __fastcall TCieFncForm::AEscapeExecute(TObject *Sender)
{
    ModalResult = mrCancel;
    while (!iFunctions.empty()) iFunctions.pop();
    Hide();
}
