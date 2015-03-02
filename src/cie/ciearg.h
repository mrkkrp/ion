/***************************************************************************
 *  C++ header
 *
 *  Common Interface Elements (CIE) for Interface of Nature - Standard
 *  Argument Form
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

#ifndef CIEARGH
#define CIEARGH

#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>

class PACKAGE TCieArgForm : public TForm
{
    __published:    // IDE-managed Components
    TEdit *Field;
    TLabel *DescriptionLabel;
    void __fastcall FieldKeyPress(TObject *Sender, System::WideChar &Key);
    void __fastcall FormShow(TObject *Sender);
    void __fastcall FormMouseDown(TObject *Sender, TMouseButton Button,
                                  TShiftState Shift, int X, int Y);

    public:     // User Declarations
    __fastcall TCieArgForm(TComponent* Owner);
    void __fastcall Invoke(UnicodeString, bool);
    void __fastcall Invoke(UnicodeString, int);
    void __fastcall Invoke(UnicodeString, unsigned int);
    void __fastcall Invoke(UnicodeString, double);
    void __fastcall Invoke(UnicodeString, UnicodeString);
    bool __fastcall ResultAsBool() const;
    int __fastcall ResultAsInt() const;
    double __fastcall ResultAsDouble() const;
    UnicodeString __fastcall ResultAsString() const;

}; // class PACKAGE TCieArgForm

extern PACKAGE TCieArgForm *CieArgForm;

#endif // CIEARGH
