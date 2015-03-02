object CieCnfForm: TCieCnfForm
  Left = 0
  Top = 0
  BorderIcons = [biSystemMenu]
  BorderStyle = bsNone
  Caption = 'Confirm'
  ClientHeight = 98
  ClientWidth = 164
  Color = clAppWorkSpace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  Position = poOwnerFormCenter
  OnKeyPress = FormKeyPress
  OnMouseDown = FormMouseDown
  PixelsPerInch = 96
  TextHeight = 13
  object FileNameLabel: TLabel
    Left = 16
    Top = 8
    Width = 134
    Height = 13
    AutoSize = False
    Caption = 'FileNameLabel'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = [fsItalic]
    ParentFont = False
    OnMouseDown = FormMouseDown
  end
  object SaveImage: TImage
    Left = 16
    Top = 27
    Width = 64
    Height = 64
    Picture.Data = {
      07544269746D6170F6000000424DF60000000000000076000000280000001000
      0000100000000100040000000000800000000000000000000000100000000000
      0000000000000000800000800000008080008000000080008000808000008080
      8000C0C0C0000000FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFF
      FF009999999999999999990000000000000990BB000000FF0B0990BB000000FF
      0B0990BB000000FF0B0990BB000000000B0990BBBBBBBBBBBB0990BB00000000
      BB0990B0FFFFFFFF0B0990B0FFFFFFFF0B0990B0FFFFFFFF0B0990B0FFFFFFFF
      0B0990B0FFFFFFFF000990B0FFFFFFFF0F099000000000000009999999999999
      9999}
    Stretch = True
    Transparent = True
    OnClick = SaveImageClick
    OnMouseEnter = SaveImageMouseEnter
    OnMouseLeave = SaveImageMouseLeave
  end
  object DiscardImage: TImage
    Left = 86
    Top = 27
    Width = 64
    Height = 64
    Picture.Data = {
      07544269746D6170F6000000424DF60000000000000076000000280000001000
      000010000000010004000000000080000000120B0000120B0000100000000000
      0000000000000000800000800000008080008000000080008000808000008080
      8000C0C0C0000000FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFF
      FF0033300000000033333330F777777033333330F080807033333330F0807070
      33333330F080807033333330F080707033333330F080807033333030F0807070
      30333300F080807003333330F080709033333330709090903333330000000000
      0333330F88877777033333000000000003333333307770333333333330000033
      3333}
    Stretch = True
    Transparent = True
    OnClick = DiscardImageClick
    OnMouseEnter = DiscardImageMouseEnter
    OnMouseLeave = DiscardImageMouseLeave
  end
end
