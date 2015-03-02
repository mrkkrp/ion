/***************************************************************************
 *  C++ header
 *
 *  ION Generator
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
#include <System.Actions.hpp>
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
#include <Vcl.ButtonGroup.hpp>
#include <ciefnc.h>
#include <mmsystem.h>
#include <prl.h>
#include <ion.h>

// constants

static const UINT UWM_AREYOUME =
    ::RegisterWindowMessage(L"GNR32-e4026f4f-92d9-44c0-bb44-24399d742094");

const UnicodeString
iDefaultName ("foo"),
iVersion     ("4.2.0"),
iCaption     ("ION Generator");

const int
GeneratorPC (9),
VoicePC     (4),
ElementPC   (6),
ElementTC   (5);

typedef std::vector<bool>               SelectionVector;

// tab-oriented structure

struct TabImage
{
    UnicodeString aFileName, aName, aAuthor, aComment, aCopyright;
    ionstd::Generator aGenerator;
    ionstd::ITP aITP;
    int aVoiceIndex, aGElementIndex;
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

class GeneratingThread : public TThread
{
    private:
    const ionstd::Generator *aGenerator;
    ionstd::ElementPointer aScore;

    protected:
    void __fastcall Execute();

    public:
    __fastcall GeneratingThread(const ionstd::Generator *);
    ionstd::ElementPointer __fastcall GetScore() const { return aScore; }

}; // class VoiceThread

typedef std::vector<TabImage>          TabImagesStore;
typedef std::vector<ionstd::GElement *> ClipboardStore;
typedef std::vector<ionstd::Sonority *> PRVSonorities;
typedef std::vector<VoiceThread *>     PRVThreads;

// some constants:

extern const UnicodeString GPropertyName[GeneratorPC];
extern const int           GPropertyImage[GeneratorPC];
extern const UnicodeString EPropertyName[ElementPC];
extern const int           EPropertyImage[ElementPC];
extern const UnicodeString ElementTypeName[ElementTC];

class TMainForm : public TForm
{
    __published:    // IDE-managed Components
    TActionManager *ActionManager;
    TImageList *ImageList;
    TPopupMenu *SelectorPopup;
    TActionMainMenuBar *MainMenu;
    TActionToolBar *ToolBar;
    TStatusBar *StatusBar;
    TSaveDialog *SaveDialog;
    TOpenDialog *OpenDialog;
    TAction *ANew;
    TAction *AOpen;
    TAction *ASave;
    TAction *ASaveAs;
    TAction *ASaveAll;
    TAction *AClose;
    TAction *ACloseAll;
    TAction *AProperties;
    TAction *AGenerate;
    TAction *AExit;
    TAction *ACut;
    TAction *ACopy;
    TAction *APaste;
    TAction *ANewElement;
    TAction *ARemoveElement;
    TAction *APreview;
    TAction *AStopPreview;
    TAction *AAbout;
    TMenuItem *New1;
    TMenuItem *N1;
    TMenuItem *Open1;
    TMenuItem *N2;
    TMenuItem *Close1;
    TMenuItem *CloseAll1;
    TAction *AReassign;
    TAction *ASaveSoundBank;
    TButtonGroup *Selector;
    TButtonGroup *TimeSettings;
    TButtonGroup *Sonorities;
    TButtonGroup *VoiceProperties;
    TButtonGroup *Voices;
    TButtonGroup *ElementProperties;
    TButtonGroup *Elements;
    TAction *ASelectAll;
    TAction *AClearSelection;
    TSaveDialog *SaveScoreDialog;
    TOpenDialog *OpenScoreDialog;
    TOpenDialog *OpenRecordDialog;
    TImageList *ElementType;
    TImageList *VoiceType;
    TImageList *ConditionAspect;
    TImageList *CreateGenerator;
    TImageList *CreateElements;
    TImageList *ReassignElement;
    void __fastcall ANewExecute(TObject *Sender);
    void __fastcall AOpenExecute(TObject *Sender);
    void __fastcall ASaveExecute(TObject *Sender);
    void __fastcall ASaveAsExecute(TObject *Sender);
    void __fastcall ASaveAllExecute(TObject *Sender);
    void __fastcall ACloseExecute(TObject *Sender);
    void __fastcall ACloseAllExecute(TObject *Sender);
    void __fastcall APropertiesExecute(TObject *Sender);
    void __fastcall AGenerateExecute(TObject *Sender);
    void __fastcall AExitExecute(TObject *Sender);
    void __fastcall ACutExecute(TObject *Sender);
    void __fastcall ACopyExecute(TObject *Sender);
    void __fastcall APasteExecute(TObject *Sender);
    void __fastcall ANewElementExecute(TObject *Sender);
    void __fastcall ARemoveElementExecute(TObject *Sender);
    void __fastcall APreviewExecute(TObject *Sender);
    void __fastcall AStopPreviewExecute(TObject *Sender);
    void __fastcall AAboutExecute(TObject *Sender);
    void __fastcall FormCloseQuery(TObject *Sender, bool &CanClose);
    void __fastcall FormResize(TObject *Sender);
    void __fastcall AReassignExecute(TObject *Sender);
    void __fastcall FormDestroy(TObject *Sender);
    void __fastcall ASaveSoundBankExecute(TObject *Sender);
    void __fastcall ASelectAllExecute(TObject *Sender);
    void __fastcall AClearSelectionExecute(TObject *Sender);
    void __fastcall SelectorButtonClicked(TObject *Sender, int Index);
    void __fastcall VoicePropertiesButtonClicked(TObject *Sender, int Index);
    void __fastcall VoicesButtonClicked(TObject *Sender, int Index);
    void __fastcall ElementsButtonClicked(TObject *Sender, int Index);
    void __fastcall TimeSettingsButtonClicked(TObject *Sender, int Index);
    void __fastcall SonoritiesButtonClicked(TObject *Sender, int Index);
    void __fastcall FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
    void __fastcall FormCreate(TObject *Sender);
    void __fastcall ElementPropertiesButtonClicked(TObject *Sender, int Index);

    private:    // User Declarations
    TabImagesStore aTabImages;
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
    FEP
        RatingFEP, PanoramFEP, VolumeFEP, FrequencyFEP,
        VoicePanoramFEP, VoiceVolumeFEP, VoiceFrequencyFEP;
    // next part is only for preview:
    PRVSonorities aPRVSonorities;
    PRVThreads aPRVThreads;
    HWAVEOUT aPRVHandle;
    WAVEHDR aPRVHeader;
    ionstd::Translator *aPRVTranslator;
    ionstd::WaveStruct *aPRVWaveStruct;
    ionstd::Element *aPRVElement;
    ionstd::numerator aPRVRate, aPRVChannels, aPRVDevice;
    ionstd::time aPRVRelease, aPRVFMargin, aPRVBMargin;
    public:
    ionstd::Generator *aGenerator;
    private:

    // smart access:

    ionstd::Generator *GetGenerator()
    { return &((aTabImages.begin() + aSelector)->aGenerator); }
    const ionstd::Generator *GetGenerator() const
    { return &((aTabImages.begin() + aSelector)->aGenerator); }
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
    int *GetVoiceIndex()
    { return &((aTabImages.begin() + aSelector)->aVoiceIndex); }
    const int *GetVoiceIndex() const
    { return &((aTabImages.begin() + aSelector)->aVoiceIndex); }
    void ClearVoiceIndex() { *GetVoiceIndex() = -1; }
    bool GeneratorSelected() const { return (*GetVoiceIndex() == -1); }
    int *GetElementIndex()
    { return &((aTabImages.begin() + aSelector)->aGElementIndex); }
    const int *GetElementIndex() const
    { return &((aTabImages.begin() + aSelector)->aGElementIndex); }
    ionstd::GElementIter GetAElement()
    { return GetGenerator()->GetGElement
        ((aTabImages.begin() + aSelector)->aGElementIndex); }
    ionstd::ConstGElementIter GetAElement() const
    { return GetGenerator()->GetGElement
        ((aTabImages.begin() + aSelector)->aGElementIndex); }

    // items:

    void __fastcall SelectVoice(int);
    void __fastcall SelectElement(int);
    void __fastcall EditVoiceProperty(unsigned int, bool = false);
    bool __fastcall EditElementProperty(unsigned int, bool = false);
    void __fastcall EditTimeSetting(unsigned int);
    void __fastcall EditSonority(unsigned int);

    // for interface:

    UnicodeString __fastcall MakeFncImage(ionstd::ConstFunctionPointer) const;
    UnicodeString __fastcall MakeHMaskImage(unsigned int) const;
    UnicodeString __fastcall MakeVoicePropertyD(unsigned int) const;
    UnicodeString __fastcall MakeElementPropertyD(unsigned int) const;
    UnicodeString __fastcall MakeElementD(unsigned int) const;
    UnicodeString __fastcall MakeTimeSettingD(unsigned int) const;
    int __fastcall MakeImageIndex(ionstd::ElementPointer, bool) const;

    // preparation for "Edit" section:

    bool __fastcall AnyElementSelected() const;
    void __fastcall ClearClipboard();
    void __fastcall SelectedToClipboard();
    void __fastcall DeleteSelected();

    // additional strings generation:

    UnicodeString __fastcall GetLastName(UnicodeString) const;
    UnicodeString __fastcall GenerateFormCaption() const;
    UnicodeString __fastcall GenerateStatisticText() const;

    // elementary actions:

    void __fastcall ApplyDefaults(ionstd::ITP *) const;
    void __fastcall AdjustFor(ionstd::Generator &, ionstd::ITP *) const;
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
