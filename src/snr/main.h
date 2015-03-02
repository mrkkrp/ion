/***************************************************************************
 *  C++ header
 *
 *  ION Sonority Editor
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
#include <Vcl.ActnList.hpp>
#include <Vcl.ActnMan.hpp>
#include <Vcl.Dialogs.hpp>
#include <Vcl.PlatformDefaultStyleActnCtrls.hpp>
#include <Vcl.ActnCtrls.hpp>
#include <Vcl.ActnMenus.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.ImgList.hpp>
#include <Vcl.ToolWin.hpp>
#include <Vcl.Menus.hpp>
#include <Vcl.Tabs.hpp>
#include <System.Actions.hpp>
#include <Vcl.Grids.hpp>
#include <Vcl.ButtonGroup.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <mmsystem.h> // for preview..
#include <ciefnc.h>
#include <prl.h>
#include <ion.h>

// constants

static const UINT UWM_AREYOUME =
    ::RegisterWindowMessage(L"SNR32-0940e2a0-5bf6-4d0a-b0a6-4829d3c52524");

const UnicodeString
iDefaultName ("foo"),
iVersion     ("4.2.0"),
iCaption     ("ION Sonority Editor");

// tab-oriented structure

typedef std::vector<bool>               SelectionVector;

struct TabImage
{
    UnicodeString aFileName, aName, aAuthor, aComment, aCopyright;
    ionstd::Sonority aSonority;
    bool aModified;

}; // struct TabImage

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

typedef std::vector<TabImage>               TabImagesStore;

class TMainForm : public TForm
{
    __published:    // IDE-managed Components
    TOpenDialog *OpenDialog;
    TSaveDialog *SaveDialog;
    TImageList *ImageList;
    TActionToolBar *ToolBar;
    TStatusBar *StatusBar;
    TPopupMenu *SelectorPopup;
    TMenuItem *New1;
    TMenuItem *N2;
    TMenuItem *Open1;
    TMenuItem *N3;
    TMenuItem *Close1;
    TMenuItem *CloseAll1;
    TOpenDialog *ExportDialog;
    TButtonGroup *Selector;
    TPanel *PreviewPanel;
    TComboBox *PreviewCategory;
    TComboBox *PreviewProbe;
    TButtonGroup *Dynamic;
    TButtonGroup *Signal;
    TActionManager *ActionManager;
    TAction *ANew;
    TAction *AOpen;
    TAction *ASave;
    TAction *ASaveAs;
    TAction *ASaveAll;
    TAction *AClose;
    TAction *ACloseAll;
    TAction *AProperties;
    TAction *AExit;
    TAction *AAbout;
    TAction *APreview;
    TAction *AStopPreview;
    TActionMainMenuBar *MainMenu;
    TOpenDialog *OpenSample;
    void __fastcall ANewExecute(TObject *Sender);
    void __fastcall AOpenExecute(TObject *Sender);
    void __fastcall ASaveExecute(TObject *Sender);
    void __fastcall ASaveAsExecute(TObject *Sender);
    void __fastcall ASaveAllExecute(TObject *Sender);
    void __fastcall ACloseExecute(TObject *Sender);
    void __fastcall ACloseAllExecute(TObject *Sender);
    void __fastcall APropertiesExecute(TObject *Sender);
    void __fastcall AExitExecute(TObject *Sender);
    void __fastcall AAboutExecute(TObject *Sender);
    void __fastcall FormCloseQuery(TObject *Sender, bool &CanClose);
    void __fastcall FormResize(TObject *Sender);
    void __fastcall APreviewExecute(TObject *Sender);
    void __fastcall AStopPreviewExecute(TObject *Sender);
    void __fastcall FormDestroy(TObject *Sender);
    void __fastcall SelectorButtonClicked(TObject *Sender, int Index);
    void __fastcall DynamicButtonClicked(TObject *Sender, int Index);
    void __fastcall SignalButtonClicked(TObject *Sender, int Index);
    void __fastcall FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
    void __fastcall PreviewCategoryChange(TObject *Sender);
    void __fastcall PreviewProbeChange(TObject *Sender);
    void __fastcall FormCreate(TObject *Sender);

    private:    // User declarations
    TabImagesStore aTabImages;
    bool aClosingCancel, aSoundPreview;
    SelectionVector aSelection;
    unsigned int aSelector;
    // these are preferences for function editor
    FEP
    APShapeFEP, AFShapeFEP, AVShapeFEP,
    RPShapeFEP, RFShapeFEP, RVShapeFEP,
    PModFEP, FModFEP, VModFEP,
    SignalShapeFEP, PhaseAltFEP;
    // next part is only for sound preview:
    ionstd::ElementPointer aPRVElement;
    ionstd::Sonority *aPRVSonority;
    ionstd::String aPRVElementFile, aPRVSourceLabel;
    VoiceThread *aPRVThread;
    HWAVEOUT aPRVHandle;
    WAVEHDR aPRVHeader;
    ionstd::Translator *aTranslator;
    ionstd::WaveStruct *aWaveStruct;
    ionstd::numerator aPRVRate, aPRVChannels, aPRVDevice;
    ionstd::time aPRVRelease, aPRVFMargin, aPRVBMargin;
    UnicodeString aPRVCategory, aPRVProbe;

    // smart access:

    ionstd::Sonority *GetSonority()
    { return &((aTabImages.begin() + aSelector)->aSonority); }
    const ionstd::Sonority *GetSonority() const
    { return &((aTabImages.begin() + aSelector)->aSonority); }
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

    // items:

    void __fastcall EditDynamic(unsigned int);
    void __fastcall EditSignal(unsigned int);

    // for interface:

    UnicodeString __fastcall MakeFncImage(ionstd::ConstFunctionPointer) const;
    UnicodeString __fastcall MakeDynamicD(unsigned int) const;
    UnicodeString __fastcall MakeSignalD(unsigned int) const;

    // preparing for "Edit" section:

    bool __fastcall AnyoneSelected() const;
    void __fastcall ClearClipboard();
    void __fastcall SelectedToClipboard();
    void __fastcall DeleteSelected();

    // string generation:

    ionstd::String __fastcall GetPrediction(UnicodeString) const;
    UnicodeString __fastcall GetLastName(UnicodeString) const;
    UnicodeString __fastcall GenerateFormCaption() const;
    UnicodeString __fastcall GenerateStatisticText() const;

    // elementary actions:

    bool __fastcall eOpen(UnicodeString);
    void __fastcall eSave(UnicodeString);
    void __fastcall eClose();
    void __fastcall EnableControls(bool);

    // the methods for building lists of avaible preview probes:

    void __fastcall LoadPreviewDirectories();
    void __fastcall LoadPreviewProbes();
    void __fastcall LoadProbe();

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
