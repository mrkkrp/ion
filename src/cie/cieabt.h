/***************************************************************************
 *  C++ header
 *
 *  Common Interface Elements (CIE) for Interface of Nature - 'About' Form
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

#ifndef CIEABTH
#define CIEABTH

#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Graphics.hpp>

class PACKAGE TCieAbtForm : public TForm
{
    __published:    // IDE-managed Components
    TLabel *Title;
    TLabel *BSLabel;
    TLabel *Date;
    TLabel *VLabel;
    TLabel *Version;
    TLabel *UsesLabel;
    TLabel *Description;
    TLabel *CopyrightLabel;
    TLabel *Copyright;
    void __fastcall FormKeyPress(TObject *Sender, System::WideChar &Key);
    void __fastcall FormMouseDown(TObject *Sender, TMouseButton Button,
                                  TShiftState Shift, int X, int Y);

    public:     // User declarations
    __fastcall TCieAbtForm(TComponent* Owner);
    void __fastcall SetTitle(UnicodeString);
    void __fastcall SetDate(UnicodeString);
    void __fastcall SetVersion(UnicodeString);
    void __fastcall SetDescription(UnicodeString);
    void __fastcall SetCopyright(UnicodeString);
    void __fastcall Invoke();

}; // class PACKAGE TAboutForm

extern PACKAGE TCieAbtForm *CieAbtForm;

#endif // CIEABTH
