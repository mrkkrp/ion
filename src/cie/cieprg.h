/***************************************************************************
 *  C++ header
 *
 *  Common Interface Elements (CIE) for Interface of Nature -
 *  Alternative Progress Bar
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

#ifndef CIEPRGH
#define CIEPRGH

#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>

typedef double (* ProgressFnc)(void);
typedef void   (* AfterFnc)   (void);

const TColor
clrBorder (clBtnHighlight),
clrBack   (clAppWorkSpace),
clrFill   (clActiveCaption);

class PACKAGE TCiePrgForm : public TForm
{
    __published:    // IDE-managed Components
    TImage *PB;
    TLabel *ProgressLabel;
    TTimer *Timer;
    TLabel *DescriptionLabel;
    void __fastcall TimerTimer(TObject *Sender);
    void __fastcall FormMouseDown(TObject *Sender, TMouseButton Button,
                                  TShiftState Shift, int X, int Y);

    private:    // User Declarations
    ProgressFnc aPF;
    AfterFnc aAF;

    void __fastcall UpdateBar(double);

    public:     // User Declarations
    __fastcall TCiePrgForm(TComponent* Owner);
    void __fastcall Invoke(UnicodeString, ProgressFnc, AfterFnc);

}; // class PAKCAGE TCiePrgForm

extern PACKAGE TCiePrgForm *CiePrgForm;

#endif // CIEPRGH
