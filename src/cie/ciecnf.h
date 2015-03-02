/***************************************************************************
 *  C++ header
 *
 *  Common Interface Elements (CIE) for Interface of Nature -
 *  Confirmation Form
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

#ifndef CIECNFH
#define CIECNFH

#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Graphics.hpp>

class PACKAGE TCieCnfForm : public TForm
{
    __published:    // IDE-managed Components
    TLabel *FileNameLabel;
    TImage *SaveImage;
    TImage *DiscardImage;
    void __fastcall FormKeyPress(TObject *Sender, System::WideChar &Key);
    void __fastcall SaveImageClick(TObject *Sender);
    void __fastcall DiscardImageClick(TObject *Sender);
    void __fastcall FormMouseDown(TObject *Sender, TMouseButton Button,
                                  TShiftState Shift, int X, int Y);
    void __fastcall SaveImageMouseEnter(TObject *Sender);
    void __fastcall SaveImageMouseLeave(TObject *Sender);
    void __fastcall DiscardImageMouseEnter(TObject *Sender);
    void __fastcall DiscardImageMouseLeave(TObject *Sender);

    private:    // User Declarations
    void __fastcall Highlight(TObject *, TColor);

    public:     // User Declarations
    __fastcall TCieCnfForm(TComponent* Owner);
    void __fastcall Invoke(UnicodeString);

}; // class PACKAGE TCieCnfForm

extern PACKAGE TCieCnfForm *CieCnfForm;

#endif // CIECNFH
