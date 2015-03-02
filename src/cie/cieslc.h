/***************************************************************************
 *  C++ header
 *
 *  Common Interface Elements (CIE) for Interface of Nature -
 *  Selection Form
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

#ifndef CIESLCH
#define CIESLCH

#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>

const int MaxSlotsSlc(21);

class PACKAGE TCieSlcForm : public TForm
{
    __published:    // IDE-managed Components
    TLabel *DescriptionLabel;
    void __fastcall FormKeyPress(TObject *Sender, System::WideChar &Key);
    void __fastcall FormShow(TObject *Sender);
    void __fastcall FormHide(TObject *Sender);
    void __fastcall FormMouseDown(TObject *Sender, TMouseButton Button,
                                  TShiftState Shift, int X, int Y);

    private:    // User Declarations
    unsigned int aCount, aMask;
    TCheckBox *aBoxes[MaxSlotsSlc];

    public:     // User Declarations
    __fastcall TCieSlcForm(TComponent* Owner);
    void __fastcall Invoke(UnicodeString, unsigned int, unsigned int);
    unsigned int __fastcall GetMask() const;

}; // class PACKAGE TCieSlcForm

extern PACKAGE TCieSlcForm *CieSlcForm;

#endif // CIESLCH
