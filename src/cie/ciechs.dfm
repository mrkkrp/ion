object CieChsForm: TCieChsForm
  Left = 0
  Top = 0
  BorderStyle = bsNone
  Caption = 'Choise Form'
  ClientHeight = 96
  ClientWidth = 432
  Color = clAppWorkSpace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  Position = poOwnerFormCenter
  OnClick = FormClick
  OnHide = FormHide
  OnKeyPress = FormKeyPress
  OnMouseDown = FormMouseDown
  PixelsPerInch = 96
  TextHeight = 13
  object DescriptionLabel: TLabel
    Left = 8
    Top = 8
    Width = 417
    Height = 13
    AutoSize = False
    Caption = 'Description'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = [fsItalic]
    ParentFont = False
    OnMouseDown = FormMouseDown
  end
end
