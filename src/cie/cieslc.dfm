object CieSlcForm: TCieSlcForm
  Left = 0
  Top = 0
  BorderStyle = bsNone
  Caption = 'Selection Form'
  ClientHeight = 56
  ClientWidth = 448
  Color = clAppWorkSpace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  Position = poOwnerFormCenter
  OnHide = FormHide
  OnKeyPress = FormKeyPress
  OnMouseDown = FormMouseDown
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object DescriptionLabel: TLabel
    Left = 8
    Top = 8
    Width = 433
    Height = 13
    AutoSize = False
    Caption = 'Enter your value'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = [fsItalic]
    ParentFont = False
    OnMouseDown = FormMouseDown
  end
end
