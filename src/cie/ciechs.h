/***************************************************************************
 *  C++ header
 *
 *  Common Interface Elements (CIE) for Interface of Nature - Choice Form
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

#ifndef CIECHSH
#define CIECHSH

#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ImgList.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Graphics.hpp>

const int
MaxSlotsChs (6),
ImageSize   (64),
LabelSize   (16),
ALeft       (8),
ATop        (26),
XStep       (70);

class PACKAGE TCieChsForm : public TForm
{
    __published:    // IDE-managed Components
    TLabel *DescriptionLabel;
    void __fastcall FormKeyPress(TObject *Sender, System::WideChar &Key);
    void __fastcall FormMouseDown(TObject *Sender, TMouseButton Button,
                                  TShiftState Shift, int X, int Y);
    void __fastcall FormClick(TObject *Sender);
    void __fastcall FormHide(TObject *Sender);
    void __fastcall FormMouseEnter(TObject *Sender);
    void __fastcall FormMouseLeave(TObject *Sender);

    private:
    unsigned int aIndex, aCount;
    UnicodeString aKeys;
    TImage *aImages[MaxSlotsChs];
    TLabel *aLabels[MaxSlotsChs];

    void __fastcall Highlight(TObject *, TColor);

    public:     // User Declarations
    __fastcall TCieChsForm(TComponent* Owner);
    void __fastcall Invoke(UnicodeString, TImageList *, UnicodeString);
    unsigned int __fastcall GetIndex() const;

}; // class PACKAGE TCieChsForm

extern PACKAGE TCieChsForm *CieChsForm;

#endif // CIECHSH
