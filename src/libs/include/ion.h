/***************************************************************************
 *  C/C++ header
 *
 *  Interface of Nature (ION)
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

#ifndef INTERFACEOFNATURELIB
#define INTERFACEOFNATURELIB

#ifndef __cplusplus
#error This library use C++ syntax, you cannot use ION in C programs.
#endif // __cplusplus

#include <cmn.h> // common header
#include <prl.h> // parameter lists & functions
#include <wav.h> // for work with wave and sound

BEGINIONSTD

/***************************************************************************
 *  Type Definitions & Forward Declarations
 **************************************************************************/

// Section I - Score

bool const melodic(false), harmonic(true);
enum ElementType { etPause, etSound, etSegment, etLink, etRecord };

class InfluenceImage;
class Element;
class Pause;
class Sound;
class Segment;
class Link;
class Record;

typedef real                        time;
typedef Element *                   ElementPointer;
typedef const Element *             ConstElementPointer;
typedef std::vector<ElementPointer> ElementsStore;
typedef time *                      IndexationMap;
typedef Pause *                     PausePointer;
typedef Sound *                     SoundPointer;
typedef Segment *                   SegmentPointer;
typedef Link *                      LinkPointer;
typedef const Link *                ConstLinkPointer;
typedef std::vector<LinkPointer>    LinksStore;
typedef ConstElementPointer         Source;

// Section II - Timbre

enum SoundingStage { ssAttack, ssSustain, ssRelease, ssPause };

class StereoPair;
class Sonority;

typedef std::complex<real>          Complex;
typedef std::valarray<Complex>      ComplexArray;
typedef Function *                  FunctionPointer;
typedef const Function *            ConstFunctionPointer;
typedef const Sonority *            Timbre;

// Section III - Translation

enum TranslatorMode { tmPreparing, tmTranslation };

struct SonorityTP
{
    String aFile;
    String aLabel;
    real aGain;
}; // struct SonorityTP

struct VoiceKit
{
    Source aSource;
    Timbre aTimbre;
    numerator TSC;
    real Gain;
}; // struct VoiceKit

class ITP;
class Translator;

typedef std::vector<SonorityTP>     STPStore;
typedef WaveStruct *                Out;
typedef std::vector<VoiceKit>       VoiceKitsStore;

// Section IV - Generative Music

enum ToneType { ttPassing, ttScale, ttTonal };

class Condition;
class GElement;
class Generator;
class GElementsCreator;
class GVoiceParams;
class GWizard;

typedef std::vector<Function>           FunctionsStore;
typedef FunctionsStore::iterator        FunctionIter;
typedef FunctionsStore::const_iterator  ConstFunctionIter;
typedef std::vector<real>               RealVector;
typedef std::vector<Condition>          ConditionVector;
typedef ConditionVector::iterator       ConditionIter;
typedef ConditionVector::const_iterator ConstConditionIter;
typedef std::vector<GElement>           GElementsStore;
typedef GElementsStore::iterator        GElementIter;
typedef GElementsStore::const_iterator  ConstGElementIter;
typedef std::vector<SegmentPointer>     SegmentsStore;
typedef std::vector<String>             StringsStore;
typedef std::vector<bool>               FlagsStore;
typedef std::vector<time>               DurationsStore;
typedef std::vector<numerator>          IndexesStore;
typedef std::vector<ToneType>           ScaleStore;
typedef std::vector<GVoiceParams>       GVParamsStore;
typedef GVParamsStore::iterator         GVParamsIter;
typedef GVParamsStore::const_iterator   ConstGVParamsIter;

/***************************************************************************
 *  Declarations of Functions
 **************************************************************************/

DLLFNC String GetIONDescription();
DLLFNC String GetIONVersion();
DLLFNC ElementPointer ReadElement(const Paramlist &, String);
DLLFNC ElementPointer CopyElement(ConstElementPointer);
DLLFNC bool HasITP(const Paramlist &, String);
DLLFNC real ProduceFrequency(numerator, numerator);
DLLFNC String FrequencyDescription(real);

/***************************************************************************
 *  Definitions of Classes
 *  Section I - Score
 **************************************************************************/

class InfluenceImage
{
    public:
    real P, F, aV, bV;

    explicit InfluenceImage() : P(0), F(0), aV(0), bV(0) { }

    // interface:

    void Clear() { P = F = aV = bV = 0; }

}; // class InfluenceImage

class Element : virtual public LabelledEssence
{
    private:
    ElementType aType;

    protected:
    DLLFNC void WriteTypeTo(Paramlist &, String) const;

    public:
    DLLFNC Element(ElementType);
    DLLFNC Element(const Element &);
    virtual ~Element() { }

    // interface:

    ElementType Type() const { return aType; }
    bool IsIt(ElementType Arg) const { return (aType == Arg); }
    virtual time Time() const = 0;
    virtual InfluenceImage GetInfluence(time, numerator) const = 0;
    virtual numerator VoicesCount() const { return 1; }
    virtual String WriteTo(Paramlist &) const = 0;
    virtual void ReadFrom(const Paramlist &, String) = 0;

}; // class Element

class Pause : virtual public Element
{
    private:
    time aDuration;

    public:
    DLLFNC Pause(time = 0.);
    DLLFNC Pause(const Pause &);
    DLLFNC Pause operator = (const Pause &);

    // interface:

    virtual DLLFNC void SetDuration(time);
    time GetDuration() const { return aDuration; }
    time Time() const { return aDuration; }
    InfluenceImage GetInfluence(time, numerator) const
    { return InfluenceImage(); }
    DLLFNC String WriteTo(Paramlist &) const;
    DLLFNC void ReadFrom(const Paramlist &, String);

}; // class Pause

class Sound : public Pause
{
    private:
    numerator aRepeats;
    bool aReversal;
    Function aPanoram, aFrequency, aVolume;

    public:
    DLLFNC Sound(time = 0.);
    DLLFNC Sound(const Sound &);
    DLLFNC Sound operator = (const Sound &);

    // interface:

    numerator Repeats() const { return aRepeats; }
    void Repeats(numerator Arg) { aRepeats = Arg; }
    bool Reversal() const { return aReversal; }
    void Reversal(bool Arg) { aReversal = Arg; }
    FunctionPointer Panoram() { return &aPanoram; }
    ConstFunctionPointer Panoram() const { return &aPanoram; }
    FunctionPointer Frequency() { return &aFrequency; }
    ConstFunctionPointer Frequency() const { return &aFrequency; }
    FunctionPointer Volume() { return &aVolume; }
    ConstFunctionPointer Volume() const { return &aVolume; }
    time Time() const { return (GetDuration() * Repeats()); }
    DLLFNC InfluenceImage GetInfluence(time, numerator) const;
    DLLFNC String WriteTo(Paramlist &) const;
    DLLFNC void ReadFrom(const Paramlist &, String);

}; // class Sound

class Segment : public Sound
{
    private:
    ElementsStore aElements;
    bool aKind; // melodic or harmonic

    mutable time aDuration;
    mutable numerator aVoicesCount;
    mutable IndexationMap aIndexationMap;
    mutable numerator aPosition;

    protected:

    DLLFNC void CopyElements(const Segment &);
    DLLFNC void GetLinks(LinksStore &) const;
    DLLFNC void SolveLinks(LinksStore &) const;
    DLLFNC void AssociateLinks() const;
    DLLFNC void CalcDuration() const;
    DLLFNC void CalcVoicesCount() const;
    DLLFNC void UpdateIndexationMap() const;
    DLLFNC void FastRefresh() const;

    public:
    DLLFNC Segment();
    DLLFNC Segment(const Segment &);
    DLLFNC Segment operator = (const Segment &);
    ~Segment() { Clear(); }

    // interface:

    DLLFNC void SetDuration(time);
    time GetDuration() const { return aDuration; }
    numerator ElementsCount() const { return aElements.size(); }
    void Kind(bool KindArg) { aKind = KindArg, Refresh(); }
    bool Kind() const { return aKind; }
    DLLFNC void Insert(ElementPointer, numerator = 0);
    DLLFNC void PushBack(ElementPointer);
    DLLFNC void PushBackSuddenly(ElementPointer);
    DLLFNC void Erase(numerator = 0);
    DLLFNC void Clear();
    DLLFNC ElementPointer GetElement(numerator = 0);
    DLLFNC ConstElementPointer GetElement(numerator = 0) const;
    DLLFNC void Refresh() const; // call whenever Segment is changing
    time Time() const { return (GetDuration() * Repeats()); }
    numerator VoicesCount() const { return aVoicesCount; }
    DLLFNC InfluenceImage GetInfluence(time, numerator) const;
    DLLFNC String WriteTo(Paramlist &) const;
    DLLFNC void ReadFrom(const Paramlist &, String);

}; // class Segment

class Link : public Element
{
    private:
    mutable ConstElementPointer aObject;

    public:
    DLLFNC Link();
    DLLFNC Link(String);
    DLLFNC Link(const Link &);
    DLLFNC Link operator = (const Link &);

    // interface:

    void SetObject(ConstElementPointer Arg) const { aObject = Arg; }
    DLLFNC time Time() const;
    DLLFNC InfluenceImage GetInfluence(time, numerator) const;
    DLLFNC numerator VoicesCount() const;
    DLLFNC String WriteTo(Paramlist &) const;
    DLLFNC void ReadFrom(const Paramlist &, String);

}; // class Link

class Record : public Element
{
    private:
    time aTime, aDelta;
    BinaryChunk aLeft, aRight;
    bool aStereo;

    public:
    DLLFNC Record();
    DLLFNC Record(const WaveStruct &);
    DLLFNC Record(const Record &);
    DLLFNC Record operator = (const Record &);
    ~Record() { aLeft.Clear(), aRight.Clear(); }

    // interface:

    DLLFNC void AdaptRecord(const WaveStruct &);
    time Time() const { return aTime; }
    DLLFNC InfluenceImage GetInfluence(time, numerator) const;
    DLLFNC StereoPair GetStereoPair(time) const;
    DLLFNC String WriteTo(Paramlist &) const;
    DLLFNC void ReadFrom(const Paramlist &, String);

}; // class Record

/***************************************************************************
 *  Definitions of Classes
 *  Section II - Timbre
 **************************************************************************/

class StereoPair
{
    public:
    sample L, R;

    StereoPair() : L(0), R(0) { }

    // interface:

    StereoPair operator += (StereoPair A)
    { L += A.L, R += A.R; return *this; }
    StereoPair operator += (real A)
    { L += A, R += A; return *this; }
    StereoPair operator *= (real A)
    { L *= A, R *= A; return *this; }
    StereoPair Gain(real A)
    { L *= __DbToFactor(A), R *= __DbToFactor(A); return *this; }
    StereoPair operator = (InfluenceImage A)
    { L = A.aV, R = A.bV; return *this; }
    StereoPair ApplyPan(real A)
    {
        real P = 0.5 * std::sin(A * (M_PI / 2.));
        L *= 0.5 - P, R *= 0.5 + P;
        return *this;
    }
    sample AsMono() { return L / 2 + R / 2; }

}; // class StereoPair

class Sonority : public LabelledEssence
{
    private:
    // dynamic parameters:
    time tA /* attack time */, tR /* release time */;
    Function sAP, sAF, sAV, sRP, sRF, sRV, aPMod, aFMod, aVMod;
    // sample parameters:
    Record nR; // nominal record
    real nF, rnI, anI, anP; // nominal frequency and noises levels
    // process variables:
    mutable SoundingStage SS;
    mutable InfluenceImage iLast, iAux;
    mutable time tQ /* guantum */, tS /* stage */, tP /* pointer */;
    mutable real frnI, fanI; // noises factors

    protected:
    DLLFNC void SwitchStage(SoundingStage) const;

    public:
    DLLFNC Sonority();
    DLLFNC Sonority(const Sonority &);
    DLLFNC Sonority operator = (const Sonority &);

    // interface:

    DLLFNC void AttackTime(time);
    time AttackTime() const { return tA; }
    DLLFNC void ReleaseTime(time);
    time ReleaseTime() const { return tR; }
    FunctionPointer APShape() { return &sAP; }
    ConstFunctionPointer APShape() const { return &sAP; }
    FunctionPointer AFShape() { return &sAF; }
    ConstFunctionPointer AFShape() const { return &sAF; }
    FunctionPointer AVShape() { return &sAV; }
    ConstFunctionPointer AVShape() const { return &sAV; }
    FunctionPointer RPShape() { return &sRP; }
    ConstFunctionPointer RPShape() const { return &sRP; }
    FunctionPointer RFShape() { return &sRF; }
    ConstFunctionPointer RFShape() const { return &sRF; }
    FunctionPointer RVShape() { return &sRV; }
    ConstFunctionPointer RVShape() const { return &sRV; }
    FunctionPointer PMod() { return &aPMod; }
    ConstFunctionPointer PMod() const { return &aPMod; }
    FunctionPointer FMod() { return &aFMod; }
    ConstFunctionPointer FMod() const { return &aFMod; }
    FunctionPointer VMod() { return &aVMod; }
    ConstFunctionPointer VMod() const { return &aVMod; }
    DLLFNC void UseSample(const WaveStruct &);
    DLLFNC void NominalFrequency(real);
    real NominalFrequency() const { return nF; }
    time SampleDuration() const { return nR.Time(); }
    void RNoise(real Arg) { rnI = Arg; }
    real RNoise() const { return rnI; }
    void ANoise(real Arg) { anI = Arg; }
    real ANoise() const { return anI; }
    void ANoiseP(real Arg) { anP = Arg; }
    real ANoiseP() const { return anP; }
    void SetQuantum(time Arg) const { tQ = Arg; }
    DLLFNC StereoPair GetStereoPair(InfluenceImage, time) const;
    DLLFNC void Reset() const;
    DLLFNC String WriteTo(Paramlist &) const;
    DLLFNC void ReadFrom(const Paramlist &, String);

}; // class Sonority

/***************************************************************************
 *  Definitions of Classes
 *  Section III - Translation
 **************************************************************************/

class ITP : public LabelledEssence
{
    private:
    STPStore aSTP;
    String dSonorityFile, dSonorityLabel;
    real dSonorityGain;
    bool aKeepEnd;
    time aFMargin, aBMargin, aStart, aEnd;

    public:
    DLLFNC ITP();
    DLLFNC ITP(const ITP &);
    DLLFNC ITP operator = (const ITP &);

    // interface:

    void DefaultSonorityFile(String Arg) { dSonorityFile = Arg; }
    String DefaultSonorityFile() const { return dSonorityFile; }
    void DefaultSonorityLabel(String Arg) { dSonorityLabel = Arg; }
    String DefaultSonorityLabel() const { return dSonorityLabel; }
    void DefaultSonorityGain(real Arg) { dSonorityGain = Arg; }
    real DefaultSonorityGain() const { return dSonorityGain; }
    DLLFNC void SonorityFile(String, numerator);
    DLLFNC String SonorityFile(numerator) const;
    DLLFNC void SonorityLabel(String, numerator);
    DLLFNC String SonorityLabel(numerator) const;
    DLLFNC void SonorityGain(real, numerator);
    DLLFNC real SonorityGain(numerator) const;
    numerator SonoritiesCount() const { return aSTP.size(); }
    DLLFNC void ForwardMargin(time);
    time ForwardMargin() const { return aFMargin; }
    DLLFNC void BackwardMargin(time);
    time BackwardMargin() const { return aBMargin; }
    DLLFNC void StartTime(time);
    time StartTime() const { return aStart; }
    DLLFNC void EndTime(time);
    time EndTime() const { return aEnd; }
    DLLFNC void AdjustFor(Element &);
    DLLFNC String WriteFor(Paramlist &, String) const;
    DLLFNC void ReadFor(const Paramlist &, String);

}; // class ITP

class Translator
{
    private:
    mutable TranslatorMode aTranslatorMode;
    mutable VoiceKitsStore aVoiceKits;
    Source aSource;
    Out aOut;
    time aForwardMargin, aBackwardMargin, aStartTime, aEndTime;
    numerator aMSC, aSSC, aTSC;

    Translator(const Translator &); // one cannot copy a Translator

    public:
    DLLFNC Translator(Source, Out);
    DLLFNC ~Translator();

    // interface:

    DLLFNC void ForwardMargin(time);
    DLLFNC void BackwardMargin(time);
    DLLFNC void StartTime(time);
    DLLFNC void EndTime(time);
    DLLFNC void SetTimbre(Timbre, numerator = 0);
    DLLFNC void MuteTimbre(numerator = 0);
    DLLFNC void SetGain(real, numerator = 0);
    DLLFNC void SetSource(Source SourceArg);
    DLLFNC void SetOut(Out OutArg);
    time ForwardMargin() const { return aForwardMargin; }
    time BackwardMargin() const { return aBackwardMargin; }
    time StartTime() const { return aStartTime; }
    time EndTime() const { return aEndTime; }
    numerator TimbresCount() const { return aVoiceKits.size(); }
    DLLFNC numerator SetTimbresCount() const;
    DLLFNC real GetGain(numerator = 0) const;
    DLLFNC numerator SignificantSC() const;
    DLLFNC time TotalTime() const;
    DLLFNC void GetReady();
    DLLFNC bool TranslateBlock(numerator = 0) const;
    DLLFNC void Reset() const;
    DLLFNC real Progress() const;

}; // class Translator

/***************************************************************************
 *  Definitions of Classes
 *  Section IV - Generative Music
 **************************************************************************/

class Condition
{
    private:
    Function aRating;
    real aAffect;
    bool aOverwrite;

    public:
    DLLFNC Condition();
    DLLFNC Condition(const Condition &);
    DLLFNC Condition operator = (const Condition &);

    // interface:

    FunctionPointer Rating() { return &aRating; }
    ConstFunctionPointer Rating() const { return &aRating; }
    void Affect(real Arg) { aAffect = Arg; }
    real Affect() const { return aAffect; }
    void Overwrite(bool Arg) { aOverwrite = Arg; }
    bool Overwrite() const { return aOverwrite; }

}; // class Condition

class GElement
{
    private:
    ElementPointer aElement;
    ConditionVector aCVector;
    numerator aHarmonicMask;
    mutable numerator aAssignedValue;

    public:
    DLLFNC GElement();
    DLLFNC GElement(ElementPointer);
    DLLFNC GElement(const GElement &);
    DLLFNC GElement operator = (const GElement &);
    DLLFNC ~GElement();

    // interface:

    ElementPointer GetElement() { return aElement; }
    ConstElementPointer GetElement() const { return aElement; }
    void SetElement(ElementPointer ElementArg) { aElement = ElementArg; }
    numerator CVectorSize() const { return aCVector.size(); }
    DLLFNC void CVectorSize(numerator);
    DLLFNC ConditionIter GetCondition(numerator);
    DLLFNC ConstConditionIter GetCondition(numerator) const;
    DLLFNC real GetRating(const RealVector &) const;
    void HarmonicMask(numerator Arg) { aHarmonicMask = Arg; }
    numerator HarmonicMask() const { return aHarmonicMask; }
    DLLFNC numerator GetFSV() const;
    void AssignedValue(numerator Arg) const { aAssignedValue = Arg; }
    numerator AssignedValue() const { return aAssignedValue; }
    DLLFNC String WriteTo(Paramlist &) const;
    DLLFNC void ReadFrom(const Paramlist &, String);

}; // class GElement

class Generator : public LabelledEssence
{
    private:
    numerator aCVectorSize, aAVectorSize, aHarmonicDepth, aSeed;
    GElementsStore aGElements;
    RealVector aInitCVector;
    StringsStore aVoicesLabels;
    FunctionsStore aVoicesPanoram, aVoicesFrequency, aVoicesVolume;
    Function aPanoram, aFrequency, aVolume;
    mutable time aLesserDuration;
    time aRequestedDuration;

    public:
    DLLFNC Generator();
    DLLFNC Generator(const Generator &);
    DLLFNC Generator operator = (const Generator &);

    // interface:

    numerator GElementsCount() const { return aGElements.size(); }
    DLLFNC void CVectorSize(numerator);
    numerator CVectorSize() const { return aCVectorSize; }
    DLLFNC void AVectorSize(numerator);
    numerator AVectorSize() const { return aAVectorSize; }
    numerator HarmonicDepth() const { return aHarmonicDepth; }
    DLLFNC void HarmonicDepth(numerator);
    void Seed(numerator SeedArg) { aSeed = SeedArg; }
    numerator Seed() const { return aSeed; }
    DLLFNC void InitCondition(real, numerator);
    DLLFNC real InitCondition(numerator) const;
    FunctionPointer Panoram() { return &aPanoram; }
    ConstFunctionPointer Panoram() const { return &aPanoram; }
    FunctionPointer Frequency() { return &aFrequency; }
    ConstFunctionPointer Frequency() const { return &aFrequency; }
    FunctionPointer Volume() { return &aVolume; }
    ConstFunctionPointer Volume() const { return &aVolume; }
    DLLFNC void VoiceLabel(String, numerator);
    DLLFNC String VoiceLabel(numerator) const;
    DLLFNC FunctionIter VoicePanoram(numerator);
    DLLFNC ConstFunctionIter VoicePanoram(numerator) const;
    DLLFNC FunctionIter VoiceFrequency(numerator);
    DLLFNC ConstFunctionIter VoiceFrequency(numerator) const;
    DLLFNC FunctionIter VoiceVolume(numerator);
    DLLFNC ConstFunctionIter VoiceVolume(numerator) const;
    DLLFNC void RequestedDuration(time);
    time RequestedDuration() const { return aRequestedDuration; }
    DLLFNC void Insert(GElement, numerator = 0);
    DLLFNC void PushBack(GElement);
    DLLFNC void Erase(numerator = 0);
    DLLFNC void Clear();
    DLLFNC GElementIter GetGElement(numerator = 0);
    DLLFNC ConstGElementIter GetGElement(numerator = 0) const;
    DLLFNC void ImportElement(const Paramlist &, String);
    DLLFNC void ImportSubelements(const Paramlist &, String);
    DLLFNC ElementPointer GenerateScore() const;
    DLLFNC real Progress() const;
    DLLFNC String WriteTo(Paramlist &) const;
    DLLFNC void ReadFrom(const Paramlist &, String);

}; // class Generator

class GElementsCreator
{
    private:
    bool aGeneratePauses, aGenerateSounds;
    time aWholeNote;
    RealVector aDividers;
    numerator aFromPitch, aToPitch;
    FlagsStore aScale;
    Function aPanoram, aFrequency, aVolume;
    bool aScalePanoram, aScaleFrequency, aScaleVolume;

    DLLFNC String ApplyMultivariant(Generator &, String) const;
    GElementsCreator(const GElementsCreator &); // one cannot copy g creator

    public:
    DLLFNC GElementsCreator();

    // interface:

    void GeneratePauses(bool Arg) { aGeneratePauses = Arg; }
    bool GeneratePauses() const { return aGeneratePauses; }
    void GenerateSounds(bool Arg) { aGenerateSounds = Arg; }
    bool GenerateSounds() const { return aGenerateSounds; }
    DLLFNC void WholeNote(time);
    time WholeNote() const { return aWholeNote; }
    DLLFNC void DividersCount(numerator);
    numerator DividersCount() const { return aDividers.size(); }
    DLLFNC void Divider(real, numerator);
    DLLFNC real Divider(numerator) const;
    void FromPitch(numerator Arg) { aFromPitch = Arg; }
    numerator FromPitch() const { return aFromPitch; }
    void ToPitch(numerator Arg) { aToPitch = Arg; }
    numerator ToPitch() const { return aToPitch; }
    DLLFNC void Temperament(numerator);
    numerator Temperament() const { return aScale.size(); }
    DLLFNC void Scale(bool, numerator);
    DLLFNC bool Scale(numerator) const;
    FunctionPointer Panoram() { return &aPanoram; }
    ConstFunctionPointer Panoram() const { return &aPanoram; }
    FunctionPointer Frequency() { return &aFrequency; }
    ConstFunctionPointer Frequency() const { return & aFrequency; }
    FunctionPointer Volume() { return &aVolume; }
    ConstFunctionPointer Volume() const { return &aVolume; }
    void ScalePanoram(bool Arg) { aScalePanoram = Arg; }
    bool ScalePanoram() const { return aScalePanoram; }
    void ScaleFrequency(bool Arg) { aScaleFrequency = Arg; }
    bool ScaleFrequency() const { return aScaleFrequency; }
    void ScaleVolume(bool Arg) { aScaleVolume = Arg; }
    bool ScaleVolume() const { return aScaleVolume; }
    DLLFNC void CreateGElementsFor(Generator &, numerator) const;

}; // class GElementsCreator

class GVoiceParams : public LabelledEssence
{
    private:
    RealVector aDividers;    // variety of notes
    real gRepetitivity,      // general: repetitivity
         mTonalIntensity,    // melody: tonal intensity
         mModalIntensity,    // melody: modal intensity
         mLowAddiction,      // melody: low addiction
         mHighAddiction,     // melody: high addiction
         mSmoothness,        // melody: smoothness
         mLeaping,           // melody: leaping
         mRetention,         // melody: retention (of tone)
         rVelocity,          // rhythm: velocity
         rAlignment,         // rhythm: alignment (to fit beat)
         rBeatDivider,       // rhythm: beat divider (defines beat)
         rSilenceShare,      // rhythm: silence share
         rSilenceClustering; // rhythm: slience clustering
    numerator aFromPitch, aToPitch; // sonic boundaries
    Function aPanoram, aFrequency, aVolume; // for every sound
    bool aScalePanoram, aScaleFrequency, aScaleVolume; // for element creator

    public:
    DLLFNC GVoiceParams();
    DLLFNC GVoiceParams(const GVoiceParams &);
    DLLFNC GVoiceParams operator = (const GVoiceParams &);

    // interface:

    DLLFNC void DividersCount(numerator);
    numerator DividersCount() const { return aDividers.size(); }
    DLLFNC void Divider(real, numerator);
    DLLFNC real Divider(numerator) const;
    DLLFNC void Repetitivity(real);
    real Repetitivity() const { return gRepetitivity; }
    DLLFNC void TonalIntensity(real);
    real TonalIntensity() const { return mTonalIntensity; }
    DLLFNC void ModalIntensity(real);
    real ModalIntensity() const { return mModalIntensity; }
    DLLFNC void LowAddiction(real);
    real LowAddiction() const { return mLowAddiction; }
    DLLFNC void HighAddiction(real);
    real HighAddiction() const { return mHighAddiction; }
    DLLFNC void Smoothness(real);
    real Smoothness() const { return mSmoothness; }
    DLLFNC void Leaping(real);
    real Leaping() const { return mLeaping; }
    DLLFNC void Retention(real);
    real Retention() const { return mRetention; }
    DLLFNC void Velocity(real);
    real Velocity() const { return rVelocity; }
    DLLFNC void Alignment(real);
    real Alignment() const { return rAlignment; }
    DLLFNC void BeatDivider(real);
    real BeatDivider() const { return rBeatDivider; }
    DLLFNC void SilenceShare(real);
    real SilenceShare() const { return rSilenceShare; }
    DLLFNC void SilenceClustering(real);
    real SilenceClustering() const { return rSilenceClustering; }
    void FromPitch(numerator Arg) { aFromPitch = Arg; }
    numerator FromPitch() const { return aFromPitch; }
    void ToPitch(numerator Arg) { aToPitch = Arg; }
    numerator ToPitch() const { return aToPitch; }
    FunctionPointer Panoram() { return &aPanoram; }
    ConstFunctionPointer Panoram() const { return &aPanoram; }
    FunctionPointer Frequency() { return &aFrequency; }
    ConstFunctionPointer Frequency() const { return & aFrequency; }
    FunctionPointer Volume() { return &aVolume; }
    ConstFunctionPointer Volume() const { return &aVolume; }
    void ScalePanoram(bool Arg) { aScalePanoram = Arg; }
    bool ScalePanoram() const { return aScalePanoram; }
    void ScaleFrequency(bool Arg) { aScaleFrequency = Arg; }
    bool ScaleFrequency() const { return aScaleFrequency; }
    void ScaleVolume(bool Arg) { aScaleVolume = Arg; }
    bool ScaleVolume() const { return aScaleVolume; }
    DLLFNC numerator PitchDiapason() const;
    DLLFNC real BackgroundForPitch(numerator, ToneType, bool) const;
    DLLFNC numerator WeighedRandomPitch(const ScaleStore &) const;
    DLLFNC real BackgroundForTime(time, time, bool) const;
    DLLFNC numerator OptimalDivider() const;
    DLLFNC real AffectByTime(time, time) const;
    DLLFNC real MobilityFactor(numerator, numerator) const;
    DLLFNC String WriteTo(Paramlist &) const;
    DLLFNC void ReadFrom(const Paramlist &, String);

}; // class GVoiceParams

class GWizard : public LabelledEssence
{
    private:
    GVParamsStore aVoicesParams;
    time aWholeNote, aRequestedDuration;
    numerator aSeed;
    ScaleStore aScale; // differential scale

    DLLFNC void AdjustPanoram(FunctionIter, numerator) const;
    DLLFNC void AdjustVolume(FunctionIter, numerator) const;

    public:
    DLLFNC GWizard();
    DLLFNC GWizard(const GWizard &);
    DLLFNC GWizard operator = (const GWizard &);

    // interface:

    DLLFNC void VoicesCount(numerator);
    numerator VoicesCount() const { return aVoicesParams.size(); }
    DLLFNC GVParamsIter GetGVParams(numerator);
    DLLFNC ConstGVParamsIter GetGVParams(numerator) const;
    DLLFNC void WholeNote(time);
    time WholeNote() const { return aWholeNote; }
    DLLFNC void RequestedDuration(time);
    time RequestedDuration() const { return aRequestedDuration; }
    void Seed(numerator Arg) { aSeed = Arg; }
    numerator Seed() const { return aSeed; }
    DLLFNC void Temperament(numerator);
    numerator Temperament() const { return aScale.size(); }
    DLLFNC void Scale(ToneType, numerator);
    DLLFNC ToneType Scale(numerator) const;
    DLLFNC void ApplyWizardFor(Generator &) const;
    DLLFNC String WriteTo(Paramlist &) const;
    DLLFNC void ReadFrom(const Paramlist &, String);

}; // class GWizard

ENDIONSTD

#endif // INTERFACEOFNATURELIB
