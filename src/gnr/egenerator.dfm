object EGeneratorForm: TEGeneratorForm
  Left = 0
  Top = 0
  BorderStyle = bsNone
  Caption = 'Element Generator'
  ClientHeight = 391
  ClientWidth = 390
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
  object DurationsBox: TGroupBox
    Left = 8
    Top = 8
    Width = 368
    Height = 105
    Caption = 'Durations'
    TabOrder = 0
    OnMouseDown = FormMouseDown
    object GeneratePauses: TCheckBox
      Left = 157
      Top = 34
      Width = 99
      Height = 17
      Caption = 'Pauses'
      Checked = True
      State = cbChecked
      TabOrder = 2
    end
    object GenerateSounds: TCheckBox
      Left = 252
      Top = 34
      Width = 99
      Height = 17
      Caption = 'Sounds'
      Checked = True
      State = cbChecked
      TabOrder = 3
    end
    object Dividers: TLabeledEdit
      Left = 16
      Top = 72
      Width = 335
      Height = 21
      EditLabel.Width = 38
      EditLabel.Height = 13
      EditLabel.Caption = 'Dividers'
      EditLabel.Font.Charset = DEFAULT_CHARSET
      EditLabel.Font.Color = clWindowText
      EditLabel.Font.Height = -11
      EditLabel.Font.Name = 'Tahoma'
      EditLabel.Font.Style = [fsItalic]
      EditLabel.ParentFont = False
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = [fsItalic]
      ParentFont = False
      TabOrder = 1
      Text = '8'
    end
    object QuarterTime: TLabeledEdit
      Left = 15
      Top = 32
      Width = 136
      Height = 21
      EditLabel.Width = 61
      EditLabel.Height = 13
      EditLabel.Caption = 'Quarter, sec'
      EditLabel.Font.Charset = DEFAULT_CHARSET
      EditLabel.Font.Color = clWindowText
      EditLabel.Font.Height = -11
      EditLabel.Font.Name = 'Tahoma'
      EditLabel.Font.Style = [fsItalic]
      EditLabel.ParentFont = False
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = [fsItalic]
      ParentFont = False
      TabOrder = 0
      Text = '0'
    end
  end
  object PitchBox: TGroupBox
    Left = 8
    Top = 119
    Width = 368
    Height = 146
    Caption = 'Scale'
    TabOrder = 1
    OnMouseDown = FormMouseDown
    object TemperamentLabel: TLabel
      Left = 243
      Top = 118
      Width = 21
      Height = 13
      Caption = 'EDO'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = [fsItalic]
      ParentFont = False
    end
    object FromPitchLabel: TLabel
      Left = 16
      Top = 118
      Width = 24
      Height = 13
      Caption = 'From'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = [fsItalic]
      ParentFont = False
    end
    object ToPitchLabel: TLabel
      Left = 130
      Top = 118
      Width = 12
      Height = 13
      Caption = 'To'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = [fsItalic]
      ParentFont = False
    end
    object Scale: TCheckListBox
      Left = 16
      Top = 20
      Width = 335
      Height = 88
      BorderStyle = bsNone
      Color = clAppWorkSpace
      Columns = 4
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = [fsItalic]
      HeaderBackgroundColor = clAppWorkSpace
      ItemHeight = 13
      ParentFont = False
      TabOrder = 0
    end
    object Temperament: TComboBox
      Left = 270
      Top = 114
      Width = 79
      Height = 21
      Style = csDropDownList
      ItemIndex = 0
      TabOrder = 1
      Text = '12'
      OnChange = TemperamentChange
      Items.Strings = (
        '12'
        '13'
        '14'
        '15'
        '16'
        '17'
        '18'
        '19'
        '20'
        '21'
        '22'
        '23'
        '24')
    end
    object FromOctave: TComboBox
      Left = 46
      Top = 114
      Width = 29
      Height = 21
      Style = csDropDownList
      ItemIndex = 0
      TabOrder = 2
      Text = '0'
      OnChange = FromOctaveChange
      Items.Strings = (
        '0'
        '1'
        '2'
        '3'
        '4'
        '5'
        '6'
        '7'
        '8')
    end
    object ToOctave: TComboBox
      Left = 159
      Top = 114
      Width = 29
      Height = 21
      Style = csDropDownList
      ItemIndex = 0
      TabOrder = 3
      Text = '0'
      OnChange = ToOctaveChange
      Items.Strings = (
        '0'
        '1'
        '2'
        '3'
        '4'
        '5'
        '6'
        '7'
        '8')
    end
    object FromNote: TComboBox
      Left = 81
      Top = 114
      Width = 43
      Height = 21
      Style = csDropDownList
      TabOrder = 4
    end
    object ToNote: TComboBox
      Left = 194
      Top = 114
      Width = 43
      Height = 21
      Style = csDropDownList
      TabOrder = 5
    end
  end
  object ProcessButton: TButton
    Left = 220
    Top = 359
    Width = 75
    Height = 25
    Caption = '&OK'
    ModalResult = 1
    TabOrder = 3
    OnClick = ProcessButtonClick
  end
  object CancelButton: TButton
    Left = 301
    Top = 359
    Width = 75
    Height = 25
    Cancel = True
    Caption = '&Cancel'
    ModalResult = 2
    TabOrder = 4
    OnClick = CancelButtonClick
  end
  object FunctionsBox: TGroupBox
    Left = 8
    Top = 271
    Width = 368
    Height = 82
    Caption = 'Functions'
    TabOrder = 2
    OnMouseDown = FormMouseDown
    object PanoramButton: TButton
      Left = 16
      Top = 20
      Width = 108
      Height = 25
      Caption = 'Panoram'
      ImageIndex = 0
      Images = ImageList
      TabOrder = 0
      OnClick = PanoramButtonClick
    end
    object PanoramScalling: TCheckBox
      Left = 16
      Top = 51
      Width = 56
      Height = 17
      Caption = 'Scalling'
      Checked = True
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = [fsItalic]
      ParentFont = False
      State = cbChecked
      TabOrder = 1
    end
    object VolumeButton: TButton
      Left = 241
      Top = 20
      Width = 108
      Height = 25
      Caption = 'Volume'
      ImageIndex = 0
      Images = ImageList
      TabOrder = 4
      OnClick = VolumeButtonClick
    end
    object VolumeScalling: TCheckBox
      Left = 243
      Top = 51
      Width = 56
      Height = 17
      Caption = 'Scalling'
      Checked = True
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = [fsItalic]
      ParentFont = False
      State = cbChecked
      TabOrder = 5
    end
    object FrequencyButton: TButton
      Left = 128
      Top = 20
      Width = 109
      Height = 25
      Caption = 'Frequency'
      ImageIndex = 0
      Images = ImageList
      TabOrder = 2
      OnClick = FrequencyButtonClick
    end
    object FrequencyScalling: TCheckBox
      Left = 130
      Top = 51
      Width = 56
      Height = 17
      Caption = 'Scalling'
      Checked = True
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = [fsItalic]
      ParentFont = False
      State = cbChecked
      TabOrder = 3
    end
  end
  object ImageList: TImageList
    Left = 272
    Top = 160
    Bitmap = {
      494C010101003000AC0210001000FFFFFFFFFF10FFFFFFFFFFFFFFFF424D3600
      0000000000003600000028000000400000001000000001002000000000000010
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000080
      80000000000000000000C0C0C000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000000000000000C0C0
      C000000000000000000080800000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000080000000000080000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000008080000000000000000000C0C0C0000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000C0C0C0000000000000000000808000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000800000000000800000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000008080000000000000000000C0C0C00000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000C0C0C00000000000000000008080000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000800000000000000000000000000000000000800000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000008080000000000000000000C0C0C000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000C0C0C000000000000000000080800000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000008080000000000000000000C0C0C0000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000C0C0C00000000000000000000000
      0000800000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000424D3E000000000000003E000000
      2800000040000000100000000100010000000000800000000000000000000000
      000000000000000000000000FFFFFF00FFFF000000000000FFFF000000000000
      E1FF000000000000E1FF000000000000F1FF000000000000F0FF000000000000
      F0FF000000000000F8FF000000000000F87F000000000000F87F000000000000
      F81F000000000000FC3F000000000000FC3F000000000000FE1F000000000000
      FF07000000000000FFFF00000000000000000000000000000000000000000000
      000000000000}
  end
end
