object CiePrgForm: TCiePrgForm
  Left = 0
  Top = 0
  BorderStyle = bsNone
  Caption = 'CiePrgForm'
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
  OnMouseDown = FormMouseDown
  PixelsPerInch = 96
  TextHeight = 13
  object PB: TImage
    Left = 8
    Top = 27
    Width = 433
    Height = 21
    OnMouseDown = FormMouseDown
  end
  object ProgressLabel: TLabel
    Left = 208
    Top = 30
    Width = 29
    Height = 13
    Caption = '100%'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = [fsItalic]
    ParentFont = False
    OnMouseDown = FormMouseDown
  end
  object DescriptionLabel: TLabel
    Left = 8
    Top = 8
    Width = 433
    Height = 13
    AutoSize = False
    Caption = 'DescriptionLabel'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = [fsItalic]
    ParentFont = False
    OnMouseDown = FormMouseDown
  end
  object Timer: TTimer
    Enabled = False
    Interval = 500
    OnTimer = TimerTimer
    Left = 376
    Top = 8
  end
end
