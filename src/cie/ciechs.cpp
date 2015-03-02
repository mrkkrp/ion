/***************************************************************************
 *  C++ source
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

#include <vcl.h>
#pragma hdrstop
#include "ciechs.h"

#pragma package(smart_init)
#pragma resource "*.dfm"

TCieChsForm *CieChsForm;

void __fastcall TCieChsForm::FormKeyPress
    (TObject *Sender, System::WideChar &Key)
{
    if (Key == VK_ESCAPE) ModalResult = mrCancel, Hide();
    for (unsigned int i = 0; i < aCount; i++)
    {
        if ((Key == aKeys.LowerCase()[i + 1])
        || (Key == aKeys.UpperCase()[i + 1]))
        {
            ModalResult = mrOk;
            aIndex = i;
            Hide();
            break;
        }
    }
}

void __fastcall TCieChsForm::FormMouseDown
    (TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y)
{
    long SC_DRAGMOVE = 0xf012;
    if(Button == mbLeft)
    {
        ReleaseCapture();
        SendMessage(Handle, WM_SYSCOMMAND, SC_DRAGMOVE, 0);
    }
}

__fastcall TCieChsForm::TCieChsForm(TComponent* Owner) : TForm(Owner)
{ }

void __fastcall TCieChsForm::Invoke
    (UnicodeString CaptionArg, TImageList *ImagesArg, UnicodeString KeysArg)
{
    if (KeysArg.Length() > MaxSlotsChs)
        KeysArg = KeysArg.SubString(1, MaxSlotsChs);
    DescriptionLabel->Caption = CaptionArg;
    aKeys = KeysArg;
    aCount = aKeys.Length();
    int aWidth = 10 + 70 * aCount;
    DescriptionLabel->Width = aWidth;
    Width = aWidth;
    for (unsigned int i = 0; i < aCount; i++)
    {
        TBitmap *aBitmap = new TBitmap;
        TImage *aImage = new TImage(CieChsForm);
        aImage->Parent = CieChsForm;
        ImagesArg->GetBitmap(i, aBitmap);
        aImage->Stretch = true;
        aImage->Transparent = true;
        aImage->Width = ImageSize;
        aImage->Height = ImageSize;
        aImage->Left = ALeft + XStep * i;
        aImage->Top = ATop;
        aImage->Picture->Bitmap = aBitmap;
        aImage->OnClick = FormClick;
        aImage->OnMouseEnter = FormMouseEnter;
        aImage->OnMouseLeave = FormMouseLeave;
        delete aBitmap;
        *(aImages + i) = aImage;
        TLabel *aLabel = new TLabel(CieChsForm);
        aLabel->Parent = CieChsForm;
        aLabel->Caption = aKeys.SubString(i + 1, 1).UpperCase();
        aLabel->Font->Style = aLabel->Font->Style << fsItalic;
        aLabel->Width = LabelSize;
        aLabel->Height = LabelSize;
        aLabel->Left = ALeft + XStep * i;
        aLabel->Top = ATop;
        aLabel->OnClick = FormClick;
        *(aLabels + i) = aLabel;
    }
    Position = poOwnerFormCenter;
    ShowModal();
}

void __fastcall TCieChsForm::FormClick(TObject *Sender)
{
    for (unsigned int i = 0; i < aCount; i++)
    {
        if (Sender == *(aImages + i)) aIndex = i;
        if (Sender == *(aLabels + i)) aIndex = i;
    }
    ModalResult = mrOk;
    Hide();
}

unsigned int __fastcall TCieChsForm::GetIndex() const
{
    return aIndex;
}

void __fastcall TCieChsForm::FormHide(TObject *Sender)
{
    for (unsigned int i = 0; i < aCount; i++)
    {
        delete *(aImages + i);
        delete *(aLabels + i);
    }
}

void __fastcall TCieChsForm::Highlight(TObject *Sender, TColor ColorArg)
{
    TGraphicControl *aControl = dynamic_cast<TGraphicControl *>(Sender);
    Canvas->Pen->Color = ColorArg;
    Canvas->Brush->Style = bsClear;
    Canvas->Rectangle(aControl->Left - 1,
                      aControl->Top - 1,
                      aControl->Left + aControl->Width + 1,
                      aControl->Top + aControl->Height + 1);
}

void __fastcall TCieChsForm::FormMouseEnter(TObject *Sender)
{
    Highlight(Sender, clHighlightText);
}

void __fastcall TCieChsForm::FormMouseLeave(TObject *Sender)
{
    Highlight(Sender, clAppWorkSpace);
}
