/***************************************************************************
 *  C++ header
 *
 *  ION Score Editor
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

#ifndef MAINH
#define MAINH

#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ActnCtrls.hpp>
#include <Vcl.ActnList.hpp>
#include <Vcl.ActnMan.hpp>
#include <Vcl.ActnMenus.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.Dialogs.hpp>
#include <Vcl.ImgList.hpp>
#include <Vcl.Menus.hpp>
#include <Vcl.PlatformDefaultStyleActnCtrls.hpp>
#include <Vcl.ToolWin.hpp>
#include <Clipbrd.hpp>
#include <mmsystem.h>
#include <Vcl.Grids.hpp>
#include <System.Actions.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Graphics.hpp>
#include <Vcl.CheckLst.hpp>
#include <Vcl.ButtonGroup.hpp>
#include <ciefnc.h>
#include <prl.h>
#include <ion.h>

// constants

static const UINT UWM_AREYOUME =
    ::RegisterWindowMessage(L"SCR32-6143a35a-2821-4ef2-95bc-8041957bcc8d");

const UnicodeString
iDefaultName ("foo"),
iVersion     ("4.2.0"),
iCaption     ("ION Score Editor");

const unsigned int
PausePC   (4),
SoundPC   (9),
SegmentPC (10),
LinkPC    (4),
RecordPC  (4),
ElementTC (5);

extern const UnicodeString PropertyName     [SegmentPC];
extern const int           PropertyTypeImage[SegmentPC];
extern const UnicodeString ElementTypeName  [ElementTC];

// tab-oriented structure

typedef std::vector<ionstd::ElementPointer> PathVector;
typedef std::vector<bool>                  SelectionVector;

struct TabImage
{
    UnicodeString aFileName, aName, aAuthor, aComment, aCopyright;
    PathVector aPath;
    ionstd::ElementPointer aScore;
    ionstd::ITP aITP;
    bool aModified;

}; // struct TabImage

// here we use separate thred for every voice

class VoiceThread : public TThread
{
    private:
    const ionstd::Translator *aTranslator;
    ionstd::numerator aVoice;

    protected:
    void __fastcall Execute();

    public:
    __fastcall VoiceThread(const ionstd::Translator *, ionstd::numerator);

}; // class VoiceThread

typedef std::vector<TabImage>              TabImagesStore;
typedef std::vector<ionstd::ElementPointer> ClipboardStore;
typedef std::vector<VoiceThread *>         PRVThreads;
typedef std::vector<ionstd::Sonority *>     PRVSonorities;

double TranslationProgress();
void AfterTranslation();

class TMainForm : public TForm
{
    __published:    // IDE-managed Components
    TStatusBar *StatusBar;
    TActionManager *ActionManager;
    TActionMainMenuBar *MainMenu;
    TActionToolBar *ToolBar;
    TSaveDialog *SaveDialog;
    TOpenDialog *OpenDialog;
    TImageList *ImageList;
    TPopupMenu *SelectorPopup;
    TAction *ANew;
    TAction *AOpen;
    TAction *ASave;
    TAction *ASaveAs;
    TAction *ASaveAll;
    TAction *AClose;
    TAction *ACloseAll;
    TAction *AProperties;
    TAction *AExit;
    TAction *AHelp;
    TAction *ACut;
    TAction *ACopy;
    TAction *APaste;
    TAction *ANewElement;
    TAction *ARemoveElement;
    TAction *AShiftUp;
    TAction *AShiftDown;
    TAction *ATranslate;
    TMenuItem *New1;
    TMenuItem *N1;
    TMenuItem *Open1;
    TMenuItem *N2;
    TMenuItem *Close1;
    TMenuItem *CloseAll1;
    TOpenDialog *ExportDialog;
    TAction *APreview;
    TAction *AStopPreview;
    TButtonGroup *Selector;
    TButtonGroup *Position;
    TButtonGroup *Properties;
    TButtonGroup *Subelements;
    TButtonGroup *Sonorities;
    TButtonGroup *TimeSettings;
    TAction *ASelectAll;
    TAction *AClearSelection;
    TOpenDialog *ImportPCM;
    TImageList *ElementType;
    TImageList *ElementTypeB;
    TSaveDialog *ExportPCM;
    void __fastcall FormResize(TObject *Sender);
    void __fastcall ANewExecute(TObject *Sender);
    void __fastcall AOpenExecute(TObject *Sender);
    void __fastcall ASaveExecute(TObject *Sender);
    void __fastcall ASaveAsExecute(TObject *Sender);
    void __fastcall ASaveAllExecute(TObject *Sender);
    void __fastcall ACloseExecute(TObject *Sender);
    void __fastcall ACloseAllExecute(TObject *Sender);
    void __fastcall APropertiesExecute(TObject *Sender);
    void __fastcall AExitExecute(TObject *Sender);
    void __fastcall ACutExecute(TObject *Sender);
    void __fastcall ACopyExecute(TObject *Sender);
    void __fastcall APasteExecute(TObject *Sender);
    void __fastcall ARemoveElementExecute(TObject *Sender);
    void __fastcall AShiftUpExecute(TObject *Sender);
    void __fastcall AShiftDownExecute(TObject *Sender);
    void __fastcall ATranslateExecute(TObject *Sender);
    void __fastcall AHelpExecute(TObject *Sender);
    void __fastcall FormCloseQuery(TObject *Sender, bool &CanClose);
    void __fastcall ANewElementExecute(TObject *Sender);
    void __fastcall APreviewExecute(TObject *Sender);
    void __fastcall AStopPreviewExecute(TObject *Sender);
    void __fastcall SelectorButtonClicked(TObject *Sender, int Index);
    void __fastcall PositionButtonClicked(TObject *Sender, int Index);
    void __fastcall PropertiesButtonClicked(TObject *Sender, int Index);
    void __fastcall SubelementsButtonClicked(TObject *Sender, int Index);
    void __fastcall TimeSettingsButtonClicked(TObject *Sender, int Index);
    void __fastcall SonoritiesButtonClicked(TObject *Sender, int Index);
    void __fastcall ASelectAllExecute(TObject *Sender);
    void __fastcall AClearSelectionExecute(TObject *Sender);
    void __fastcall FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
    void __fastcall SubelementsMouseDown(TObject *Sender, TMouseButton Button,
                                         TShiftState Shift, int X, int Y);
    void __fastcall FormDestroy(TObject *Sender);
    void __fastcall FormCreate(TObject *Sender);

    private:    // User declarations
    TabImagesStore aTabImages; // tabs
    ClipboardStore aClipboard;
    UnicodeString aDefaultLabel;
    bool aClosingCancel, aSoundPreview; // flags
    SelectionVector aSelection;
    unsigned int aSelector;
    // some defaults for ITP:
    ionstd::String dSonorityFile, dSonorityLabel;
    ionstd::real dSonorityGain;
    ionstd::time dFMargin, dBMargin, dStart;
    // these are preferences for function editor
    FEP sPanoramFEP, sFrequencyFEP, sVolumeFEP,
        aPanoramFEP, aFrequencyFEP, aVolumeFEP;
    // next part is for preview:
    PRVSonorities aPRVSonorities;
    PRVThreads aPRVThreads;
    HWAVEOUT aPRVHandle;
    WAVEHDR aPRVHeader;
    public:
    ionstd::Translator *aTranslator;
    ionstd::WaveStruct *aWaveStruct;
    ionstd::String aOut;
    bool tdNormalization, tdDither;
    private:
    ionstd::numerator aPRVRate, aPRVChannels, aPRVDevice;
    ionstd::time aPRVRelease, aPRVFMargin, aPRVBMargin;
    // defaults for translation:
    ionstd::numerator tdSampleRate, tdBitDepth, tdFormat;
    bool tdMono;

    // smart access:

    ionstd::ElementPointer GetScore()
    { return (aTabImages.begin() + aSelector)->aScore; }
    ionstd::ConstElementPointer GetScore() const
    { return (aTabImages.begin() + aSelector)->aScore; }
    ionstd::ITP *GetITP()
    { return &((aTabImages.begin() + aSelector)->aITP); }
    const ionstd::ITP *GetITP() const
    { return &((aTabImages.begin() + aSelector)->aITP); }
    UnicodeString *GetFileName()
    { return &((aTabImages.begin() + aSelector)->aFileName); }
    const UnicodeString *GetFileName() const
    { return &((aTabImages.begin() + aSelector)->aFileName); }
    UnicodeString *GetName()
    { return &((aTabImages.begin() + aSelector)->aName); }
    const UnicodeString *GetName() const
    { return &((aTabImages.begin() + aSelector)->aName); }
    UnicodeString *GetAuthor()
    { return &((aTabImages.begin() + aSelector)->aAuthor); }
    const UnicodeString *GetAuthor() const
    { return &((aTabImages.begin() + aSelector)->aAuthor); }
    UnicodeString *GetComment()
    { return &((aTabImages.begin() + aSelector)->aComment); }
    const UnicodeString *GetComment() const
    { return &((aTabImages.begin() + aSelector)->aComment); }
    UnicodeString *GetCopyright()
    { return &((aTabImages.begin() + aSelector)->aCopyright); }
    const UnicodeString *GetCopyright() const
    { return &((aTabImages.begin() + aSelector)->aCopyright); }
    bool *GetModified()
    { return &((aTabImages.begin() + aSelector)->aModified); }
    const bool *GetModified() const
    { return &((aTabImages.begin() + aSelector)->aModified); }
    ionstd::ElementPointer GetActual()
    { return (aTabImages.begin() + aSelector)->aPath.back(); }
    const ionstd::ElementPointer GetActual() const
    { return (aTabImages.begin() + aSelector)->aPath.back(); }
    const unsigned int GetDepth()
    { return (aTabImages.begin() + aSelector)->aPath.size(); }
    void PushToPath(ionstd::ElementPointer ElementArg)
    { (aTabImages.begin() + aSelector)->aPath.push_back(ElementArg); }
    void PopFromPath()
    { (aTabImages.begin() + aSelector)->aPath.pop_back(); }

    // items:
    
    void __fastcall GoIn(unsigned int);
    void __fastcall GoBack(unsigned int);
    void __fastcall EditProperty(unsigned int);
    void __fastcall EditTimeSetting(unsigned int);
    void __fastcall EditSonority(unsigned int);

    // for interface:

    UnicodeString __fastcall MakeFncImage(ionstd::ConstFunctionPointer) const;
    UnicodeString __fastcall MakePropertyD(unsigned int) const;
    UnicodeString __fastcall MakeElementD(unsigned int) const;
    UnicodeString __fastcall MakeTimeSettingD(unsigned int) const;
    int __fastcall MakeImageIndex(ionstd::ElementPointer, bool) const;

    // preparation for "Edit" section:

    bool __fastcall AnyoneSelected() const;
    void __fastcall ClearClipboard();
    void __fastcall SelectedToClipboard();
    void __fastcall DeleteSelected();

    // smart subelements detection:

    bool IsActualPause() const
    { return (GetActual()->Type() == ionstd::etPause); }
    bool IsActualSound() const
    { return (GetActual()->Type() == ionstd::etSound); }
    bool IsActualSegment() const
    { return (GetActual()->Type() == ionstd::etSegment); }
    bool IsActualLink() const
    { return (GetActual()->Type() == ionstd::etLink); }
    bool IsActualRecord() const
    { return (GetActual()->Type() == ionstd::etRecord); }

    ionstd::Pause *GetActualAsPause() const
    { return dynamic_cast<ionstd::Pause *>(GetActual()); }
    ionstd::Sound *GetActualAsSound() const
    { return dynamic_cast<ionstd::Sound *>(GetActual()); }
    ionstd::Segment *GetActualAsSegment() const
    { return dynamic_cast<ionstd::Segment *>(GetActual()); }
    ionstd::Link *GetActualAsLink() const
    { return dynamic_cast<ionstd::Link *>(GetActual()); }
    ionstd::Record *GetActualAsRecord() const
    { return dynamic_cast<ionstd::Record *>(GetActual()); }

    // string generation:

    UnicodeString __fastcall GetLastName(UnicodeString) const;
    UnicodeString __fastcall GenerateFormCaption() const;
    UnicodeString __fastcall GenerateStatisticText() const;

    // elementary actions:

    bool __fastcall AskForDuration(ionstd::ElementPointer);
    bool __fastcall AskForWave(ionstd::ElementPointer);
    void __fastcall ApplyDefaults(ionstd::ITP *) const;
    bool __fastcall eOpen(UnicodeString);
    void __fastcall eSave(UnicodeString);
    void __fastcall eClose();
    void __fastcall EnableControls(bool);

    // visual support:

    void __fastcall Refresh();

    // win messages processing:

    void __fastcall OnOpenOther(TWMCopyData &);
    void __fastcall WndProc(TMessage &);

    public:     // User declarations
    __fastcall TMainForm(TComponent* Owner);

    BEGIN_MESSAGE_MAP
        MESSAGE_HANDLER(WM_COPYDATA, TWMCopyData, OnOpenOther);
    END_MESSAGE_MAP(TComponent)

}; // class TMainForm

extern PACKAGE TMainForm *MainForm;

#endif // MAINH
