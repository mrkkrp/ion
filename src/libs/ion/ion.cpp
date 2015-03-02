/***************************************************************************
 *  C++ source
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

#define DLLBUILDING
#include <ion.h>

BEGINIONSTD

/***************************************************************************
 *  Data Section
 **************************************************************************/

const String
prlmkElement           ("-"),
prlpkElementType       ("$"),
prlpkDuration          ("D"),
prlpkRepeats           ("&"),
prlpkReversal          ("R"),
prlpkPanoram           ("P"),
prlpkVolume            ("V"),
prlpkFrequency         ("F"),
prlpkSegmentKind       ("*"),
prlpkElement           ("e"),
prlpxLink              ("^"),
prlpkDelta             ("Delta"),
prlpkLevel             ("Level"),
prlpkLeft              ("Left"),
prlpkRight             ("Right"),
prlpkStereo            ("Stereo"),
prlpkParam             ("Param"),
prlpxSignal            ("Signal"),
prlpxEcho              ("Echo"),
prlpkDelay             ("Delay"),
prlpkGain              ("Gain"),
prlmkSonority          ("Sonority"),
prlpkITP               ("itp"),
prlmkITP               ("itp"),
prlpkSonorityFile      ("sFile"),
prlpkSonorityLabel     ("sLabel"),
prlpkSonorityGain      ("sGain"),
prlpkFMargin           ("FMargin"),
prlpkBMargin           ("BMargin"),
prlpkStart             ("Start"),
prlpkEnd               ("End"),
prlpkCondition         ("c"),
prlpkRating            ("r"),
prlpkAffect            ("a"),
prlpkOverwrite         ("o"),
prlpkHarmonicMask      ("HarmonicMask"),
prlmkGenerator         ("Generator"),
prlpkGElement          ("g"),
prlpkCVectorSize       ("CVectorSize"),
prlpkHarmonicDepth     ("HarmonicDepth"),
prlpkSeed              ("Seed"),
prlpkVoiceLabel        ("L"),
prlmkGVoice            ("GVoice"),
prlpkDivider           ("d"),
prlmkGWizard           ("GWizard"),
prlpkGVoice            ("G"),
prlpkScale             ("s"),
prlmkSWizard           ("SWizard");

const real
PanoramLeft            (-1),
PanoramRight           (+1),
PanoramDefault         (0),
PanoramAltDefault      (0),
VolumeSilence          (0),
VolumeFull             (1),
VolumeDefault          (1),
VolumeAltDefault       (1),
FrequencyMin           (0),
FrequencyMax           (24e+3),
FrequencyDefault       (440),
FrequencyAltDefault    (1),
PhaseAltDefault        (0),
EchoGainDefault        (-10),
RatingDefault          (1),
AffectDefault          (0),
BasicC0                (16.3515978312874146),
SonorityCNumber        (8),
EchoIterLimit          (8);

const time
DurationDefault        (1.00),
AttackTimeDefault      (0.01),
ReleaseTimeDefault     (0.01),
DelayDefault           (0.10);

const numerator
TranslationBlockSize   (0x7fffu),
MusicBufferAllocated   (0x100000), // 1 Mb
GeneratorHarmonicDepth (32),
ProbesCount            (10);

const numerator Probes[ProbesCount] =
{ 2, 3, 5, 7, 11, 13, 17, 19, 23, 29 }; // prime numbers

const String       // exceptions wording
ioneElementLoading("cannot load element: unknown element type"),
ioneElementDuration("invalid duration of element"),
ioneSegmentDuration("cannot change segment duration explicitly"),
ioneSegmentAV("access violation in segment, position is beyond container"),
ioneSegmentUnknownType("unknown subelement type, impossible copying"),
ioneSegmentIndexation("fatal error of indexation subelements in segment"),
ionePoorLink("poor link is suddenly detected"),
ioneShortWave("the wave file contains too few samples to adapt the record"),
ioneSignalTime("invalid time setting for signal generator"),
ioneNominalFrequency("invalid nominal frequency"),
ioneSonorityCNumber("invalid components number, valid range: 0 - 8"),
ioneSonorityBadTime("invalid time setting"),
ioneSonorityBadBuffer("bad buffer is bad"),
ioneSonorityAV("access violation in sonority"),
ioneITPTime("invalid time settings"),
ioneITPAV("access violation in ITP"),
ioneTranslatorBadSource("source of translation is so bad"),
ioneTranslatorBadOut("wave structure is invalid (translation)"),
ioneTranslatorAV("access violation in translator"),
ioneTranslatorIPC("illegal parameter change (translator)"),
ioneTranslatorTime("invalid time settings (translator)"),
ioneTranslatorLETT("must be in preparing mode to go to translation"),
ioneTranslatorITRN("must be in translation mode to translate"),
ioneGElementAV("access violation in GElement"),
ioneGCVSize("condition vector of generative element has incorrect size"),
ioneGeneratorAV("access violation in generator"),
ionePoorVoice("poor voice is detected while generator's working"),
ioneGeneratorDuration("duration cannot be negative (generator)"),
ioneGeneratorHDG("unsupported harmonic depth of generative project"),
ioneGeneratorHDE("harmonic depth of generative element is greater than one"),
ioneGeneratorWEI("cannot import subelements, given element is not a segment"),
ioneGECreatorDuration("duration cannot be negative (creator)"),
ioneGECreatorAV("access violation in GElements creator"),
ioneGECreatorBD("zero cannot be a divider for GElements creator"),
ioneGECreatorBT("bad temperament for GElements creator"),
ioneGVoiceAV("access violation in generative voice parameters"),
ioneGVoiceBD("zero cannot be a divider for GVoice"),
ioneGVoiceBP("bad pitch diapason"),
ioneGVoiceNV("this value must be non-negative real"),
ioneGWizardBT("bad temperament for GWizard"),
ioneGWizardAV("access violation in GWizard"),
ioneGWizardDuration("duration cannot be negative (GWizard)");

/***************************************************************************
 *  Definitions of Inline Functions
 **************************************************************************/

inline double __CorrectExternalTime(time T, time D, bool R)
{
    T -= D * (SDWORD)(T / D);
    if (R) return (D - T);
    else return T;
}

template <typename data> inline void __CorrcetRequest
    (const IndexationMap &IM, data &R, numerator &i)
{
    while (!((R >= *(IM + i)) && (R < *(IM + i + 1))))
    {
        if (R >= *(IM + i + 1)) i++;
        if (R < *(IM + i)) i--;
    }
    R -= *(IM + i);
}

inline real __GetApproximatedValue
    (const BinaryChunk &C, numerator i, time D, real F)
{
    numerator s = C.Size() / sizeof(sample);
    real L = C.GetData<sample>(i % s);
    real R = C.GetData<sample>(++i % s);
    return (F * (R - L)) / D + L;
}

inline StereoPair __GetStereoPair
(time T, time D, bool StereoArg, const BinaryChunk &L, const BinaryChunk &R)
{
    numerator i = T / D;
    real F = T - i * D;
    StereoPair aResult;
    aResult.L = __GetApproximatedValue(L, i, D, F);
    aResult.R = StereoArg ? __GetApproximatedValue(R, i, D, F) : aResult.L;
    return aResult;
}

void __FFT(ComplexArray& x) // Cooley Tukey FFT (in-place)
{
    const numerator n = x.size();
    if (n <= 1) return;
    ComplexArray E = x[std::slice(0, n / 2, 2)];
    ComplexArray O = x[std::slice(1, n / 2, 2)];
    __FFT(E);
    __FFT(O);
    for (size_t k = 0; k < n / 2; k++)
    {
        Complex t = std::polar(1.0, -2 * M_PI * k / n) * O[k];
        x[k] = E[k] + t;
        x[k + n / 2] = E[k] - t;
    }
}

inline void __MultiplyFunction(FunctionPointer F, real x)
{
    for (numerator i = 0; i < F->PointsCount(); i++)
    {
        real aValue = F->PointValue(i);
        aValue *= x;
        F->PointValue(aValue, i);
    }
}

inline void __ScaleFunction(FunctionPointer F, real x)
{
    F->Min(0);
    F->Max(x);
}

inline void __GainPoint(FunctionPointer F, numerator i, real x)
{
    real aValue = F->PointValue(i);
    aValue *= std::exp(x);
    F->PointValue(aValue, i);
}

inline void __GainPointZ(FunctionPointer F, numerator i, real x)
{
    real aValue = F->PointValue(i);
    aValue *= std::exp(x) - 1;
    F->PointValue(aValue, i);
}

numerator __WeightedRandom(RealVector &RatingsArg)
{
    real aSum = 0.;
    RealVector::iterator i = RatingsArg.begin();
    while (i != RatingsArg.end()) aSum += *i, *i++ = aSum;
    real aRandom = std::rand() * (aSum / RAND_MAX);
    i = RatingsArg.begin();
    while (*i < aRandom) i++;
    return (i - RatingsArg.begin());
}

/***************************************************************************
 *  Definitions of Functions
 **************************************************************************/

PLACEEXCEPTIONFNCHERE

String GetIONDescription()
{
    return "Interface of Nature (ION) ("__DATE__")\n"
           "Copyright (c) 2014 Mark Karpov";
}

String GetIONVersion()
{
    return "4.2.0";
}

ElementPointer ReadElement(const Paramlist &SourceArg, String MapKeyArg)
{
    // here we have to find out type of the element, create it, and read it
    ElementPointer aElement;
    if (*(MapKeyArg.begin()) == *(prlpxLink.begin())) aElement = new Link();
    else
    {
        numerator aElementType =
            SourceArg.Data<numerator>(MapKeyArg, prlpkElementType);
        switch (aElementType)
        {
            case etPause    : aElement = new Pause; break;
            case etSound    : aElement = new Sound; break;
            case etSegment  : aElement = new Segment; break;
            case etRecord   : aElement = new Record; break;
            default: THROW_RE(ioneElementLoading);
        }
    } // else
    // we have standard copy of neccessary type, just read the element
    aElement->ReadFrom(SourceArg, MapKeyArg); // form Paramlist now
    return aElement;
}

ElementPointer CopyElement(ConstElementPointer ElementArg)
{
    // this function copies an element by its pointer and returns pointer
    // that references to new allocated element
    ElementPointer aElement; // it's our pointer
    switch (ElementArg->Type()) // look for type and create new the same
    {
        // now copy constructor helps
        case etPause    : aElement = new Pause
            (*dynamic_cast<const Pause *>(ElementArg)); break;
        case etSound    : aElement = new Sound
            (*dynamic_cast<const Sound *>(ElementArg)); break;
        case etSegment  : aElement = new Segment
            (*dynamic_cast<const Segment *>(ElementArg)); break;
        case etLink : aElement = new Link
            (*dynamic_cast<const Link *>(ElementArg)); break;
        case etRecord   : aElement = new Record
            (*dynamic_cast<const Record *>(ElementArg)); break;
        // defualt case: it throws a subtle exception
        default : THROW_LE(ioneSegmentUnknownType);
    }
    return aElement;
}

bool HasITP(const Paramlist &SourceArg, String MapKeyArg)
{
    return SourceArg.Exists(MapKeyArg, prlpkITP);
}

real ProduceFrequency(numerator PitchArg, numerator TemperamentArg)
{
    real aResult = PitchArg;
    aResult /= TemperamentArg;
    aResult = BasicC0 * std::pow(2., aResult);
    return aResult;
}

String FrequencyDescription(real FrequencyArg)
{
    // first: extract number of cents from basic C0 note
    numerator aCents = 1200; // rounding is possible here
    aCents *= std::log(FrequencyArg / BasicC0) / std::log(2.);
    // second: using the count of cents we are able to get octave
    numerator aOctave = aCents / 1200;
    // third: now we're about to find out nearest tone that corresponds to
    // the amount of cents
    numerator aNoteIndex = 0; // default value
    real aProgression[23] = // intervals in cents
    { 0, 71, 112, 204, 275, 316, 386, 427, 457, 498, 590, 631, 702,
    773, 814, 884, 977, 1018, 1088, 1129, 1159, 1200, 1200 };
    for (numerator i = 1; i < 22; i++)
    {
        real aLeft = *(aProgression + i - 1) + *(aProgression + i);
        real aRight = *(aProgression + i + 1) + *(aProgression + i);
        aLeft /= 2;
        aRight /= 2;
        if (((aCents % 1200) >= aLeft) && ((aCents % 1200) < aRight))
            { aNoteIndex = i; break; }
    }
    if (aNoteIndex == 21) aNoteIndex = 0, aOctave++;
    String aNoteName[21] = // s for sharp, f for flat and n for normal
    { "cn", "cs", "df", "dn", "ds", "ef", "en", "ff", "es", "fn", "fs",
    "gf", "gn", "gs", "af", "an", "as", "bf", "bn", "cf", "bs" };
    String aLabel(aNoteName[aNoteIndex]);
    // ..and now just add octave index to complete the operation
    CHAR aBuffer[SMALLBUFFERSIZE];
    std::itoa(aOctave, aBuffer, 10);
    aLabel += String(aBuffer);
    return aLabel; // that's it!
}

/***************************************************************************
 *  Definitions of non-inline Methods
 *  Section I - Score
 **************************************************************************/

// class Element

void Element::WriteTypeTo(Paramlist &TargetArg, String MapKeyArg) const
{
    TargetArg.Add<numerator>(MapKeyArg, prlpkElementType, aType);
}

Element::Element(ElementType TypeArg) :
    LabelledEssence(prlmkElement),
    aType(TypeArg)
{ }

Element::Element(const Element &Another) :
    LabelledEssence(Another),
    aType(Another.aType)
{ }

// class Pause

Pause::Pause(time DurationArg) :
    Element(etPause),
    LabelledEssence(prlmkElement),
    aDuration(DurationArg)
{ }

Pause::Pause(const Pause &Another) : Element(etPause),
    LabelledEssence(Another),
    aDuration(Another.aDuration)
{ }

Pause Pause::operator = (const Pause &Another)
{
    LabelledEssence::operator = (Another),
    aDuration = Another.aDuration;
    return *this;
}

void Pause::SetDuration(time DurationArg)
{
    if (DurationArg < 0) THROW_RE(ioneElementDuration);
    aDuration = DurationArg;
}

String Pause::WriteTo(Paramlist &TargetArg) const
{
    String aMapKey = GenerateUniqueMapKey(TargetArg); // get new map key
    WriteTypeTo(TargetArg, aMapKey); // type of element (pause)
    WriteLabelTo(TargetArg, aMapKey); // label of element
    TargetArg.Add<time>(aMapKey, prlpkDuration, aDuration); // in secs
    return aMapKey; // every "WriteTo" function must return map key
}

void Pause::ReadFrom(const Paramlist &SourceArg, String MapKeyArg)
{
    ReadLabelFrom(SourceArg, MapKeyArg); // reading label if it exists
    aDuration = SourceArg.Data<time>(MapKeyArg, prlpkDuration);
}

// class Sound

Sound::Sound(time DurationArg) : Element(etSound),
    LabelledEssence(prlmkElement),
    Pause(DurationArg),
    aRepeats(1),
    aReversal(false),
    aPanoram(0, DurationArg, PanoramDefault),
    aFrequency(0, DurationArg, FrequencyDefault),
    aVolume(0, DurationArg, VolumeDefault)
{ }

Sound::Sound(const Sound &Another) : Element(etSound),
    LabelledEssence(Another),
    Pause(Another),
    aRepeats(Another.aRepeats),
    aReversal(Another.aReversal),
    aPanoram(Another.aPanoram),
    aFrequency(Another.aFrequency),
    aVolume(Another.aVolume)
{ }

Sound Sound::operator = (const Sound &Another)
{
    LabelledEssence::operator = (Another);
    SetDuration(Another.GetDuration());
    aRepeats = Another.aRepeats;
    aReversal = Another.aReversal;
    aPanoram = Another.aPanoram;
    aFrequency = Another.aFrequency;
    aVolume = Another.aVolume;
    return *this;
}

InfluenceImage Sound::GetInfluence(time TimeArg, numerator VoiceArg) const
{
    InfluenceImage aResult; // make clear image
    if (TimeArg >= Time()) return aResult; // beyond segment time
    if (VoiceArg >= VoicesCount()) return aResult; // beyond harmonic depth
    TimeArg = __CorrectExternalTime(TimeArg, GetDuration(), Reversal());
    // now we have to assign values from every function:
    aResult.P = aPanoram(TimeArg);
    aResult.F = aFrequency(TimeArg);
    aResult.aV = aVolume(TimeArg);
    return aResult; // and return the result
}

String Sound::WriteTo(Paramlist &TargetArg) const
{
    String aMapKey = GenerateUniqueMapKey(TargetArg);
    WriteTypeTo(TargetArg, aMapKey);
    WriteLabelTo(TargetArg, aMapKey);
    TargetArg.Add<time>(aMapKey, prlpkDuration, GetDuration());
    TargetArg.Add<numerator>(aMapKey, prlpkRepeats, aRepeats);
    TargetArg.Add<bool>(aMapKey, prlpkReversal, aReversal);
    TargetArg.Add<Function>(aMapKey, prlpkPanoram, aPanoram);
    TargetArg.Add<Function>(aMapKey, prlpkFrequency, aFrequency);
    TargetArg.Add<Function>(aMapKey, prlpkVolume, aVolume);
    return aMapKey;
}

void Sound::ReadFrom(const Paramlist &SourceArg, String MapKeyArg)
{
    ReadLabelFrom(SourceArg, MapKeyArg);
        SetDuration(SourceArg.Data<time>(MapKeyArg, prlpkDuration));
    aRepeats = SourceArg.Data<numerator>(MapKeyArg, prlpkRepeats);
    aReversal = SourceArg.Data<bool>(MapKeyArg, prlpkReversal);
    aPanoram = SourceArg.Data<Function>(MapKeyArg, prlpkPanoram);
    aFrequency = SourceArg.Data<Function>(MapKeyArg, prlpkFrequency);
    aVolume = SourceArg.Data<Function>(MapKeyArg, prlpkVolume);
}

// class Segment

void Segment::CopyElements(const Segment &Another)
{
    // it uses method CopyElement and copies all subelements of segment
    ElementsStore::const_iterator i = Another.aElements.begin();
    while (i != Another.aElements.end())
        aElements.push_back(CopyElement(*i++));
    Refresh(); // it refreshes duration and voices count
}

void Segment::GetLinks(LinksStore &LinksArg) const
{
    for (ElementsStore::const_iterator i = aElements.begin();
        i != aElements.end(); i++)
    {
        if ((**i).Type() == etLink)
            LinksArg.push_back(dynamic_cast<Link *>(*i));
        else if ((**i).Type() == etSegment)
            (dynamic_cast<Segment *>(*i))->GetLinks(LinksArg);
    }
}

void Segment::SolveLinks(LinksStore &LinksArg) const
{
    for (ElementsStore::const_iterator i = aElements.begin();
        i != aElements.end(); i++) // solving links
    {
        if ((**i).Type() == etLink) continue;
        LinksStore::iterator aLink = LinksArg.begin();
        while (aLink != LinksArg.end())
        {
            if ((**aLink).Label() == (**i).Label())
            {
                (**aLink).SetObject(*i);
                LinksArg.erase(aLink);
            }
            else aLink++;
        }
    }
    for (ElementsStore::const_iterator i = aElements.begin();
        i != aElements.end(); i++) // call the function for subelements
    {
        if ((**i).Type() == etSegment)
            (dynamic_cast<Segment *>(*i))->SolveLinks(LinksArg);
    }
}

void Segment::AssociateLinks() const
{
    // first of all, we're looking for all links and take them to store
    LinksStore aLinks;
    GetLinks(aLinks);
    // second: solve all these links
    SolveLinks(aLinks);
    // third: set NIL for unsolved links
    LinksStore::iterator aLink = aLinks.begin();
    while (aLink != aLinks.end())
        (**aLink++).SetObject(NIL);
}

void Segment::CalcDuration() const
{
    aDuration = 0;
    ElementsStore::const_iterator i = aElements.begin();
    if (aKind == melodic) // melodic order: amount of times
    {
        while (i != aElements.end()) aDuration += (**i++).Time();
    }
    else // harmonic order: longest time
    {
        while (i != aElements.end())
        {
            if ((**i).Time() > aDuration) aDuration = (**i).Time();
            i++;
        }
    }
}

void Segment::CalcVoicesCount() const
{
    aVoicesCount = 0; // this variable is able to be requested
    ElementsStore::const_iterator i = aElements.begin();
    if (aKind == melodic) // melodic order: most complex subelement
    {
        while (i != aElements.end())
        {
            if ((**i).VoicesCount() > aVoicesCount)
                aVoicesCount = (**i).VoicesCount();
            i++;
        }
    }
    else // else harmonic order: amount of voices numbers
    {
        while (i != aElements.end())
            aVoicesCount += (**i++).VoicesCount();
    }
}

void Segment::UpdateIndexationMap() const
{
    if (aIndexationMap) delete [] aIndexationMap;
    aPosition = 0; // get it to start
    if (!aElements.size()) return;
    time Accumulator = 0.;
    aIndexationMap = new time[aElements.size() + 1];
    *aIndexationMap = Accumulator;
    ElementsStore::const_iterator i = aElements.begin();
    while (i != aElements.end())
    {
        if (aKind == melodic) Accumulator += (**i).Time();
        else /* harmonic */ Accumulator += (**i).VoicesCount();
        *(aIndexationMap + (i++ - aElements.begin()) + 1) = Accumulator;
    }
}

void Segment::FastRefresh() const
{
    for (ElementsStore::const_iterator i = aElements.begin();
        i != aElements.end(); i++)
    {
        if ((**i).Type() == etSegment)
        {
            Segment *aSegment = dynamic_cast<Segment *>(*i);
            aSegment->FastRefresh();
        }
    }
    CalcDuration(); // second: calculate duration
    CalcVoicesCount(); // third: calcualte voices count
    UpdateIndexationMap(); // fourth: update indexation map
}

Segment::Segment() : Element(etSegment),
    LabelledEssence(prlmkElement),
    Sound(0.),
    aDuration(0.),
    aVoicesCount(0),
    aKind(melodic),
    aIndexationMap(NIL),
    aPosition(0)
{
    // reassign new values after Sound constructor
    Panoram()->PointValue(PanoramAltDefault, 0);
    Frequency()->PointValue(FrequencyAltDefault, 0);
    Volume()->PointValue(VolumeAltDefault, 0);
}

Segment::Segment(const Segment &Another) : Element(etSegment),
    LabelledEssence(Another),
    Sound(Another),
    aKind(Another.aKind),
    aIndexationMap(NIL),
    aPosition(0)
{
    CopyElements(Another);
}

Segment Segment::operator = (const Segment &Another)
{
    LabelledEssence::operator = (Another);
    Sound::operator = (Another);
    aKind = Another.aKind;
    Clear();
    CopyElements(Another);
    return *this;
}

void Segment::SetDuration(time)
{
    // duration of segment is defined by its content
    // everyone who dares to set duration with this function will fuck up
    THROW_LE(ioneSegmentDuration);
}

void Segment::Insert(ElementPointer ElementArg, numerator PositionArg)
{
    // this function inserts element before specified position
    if (PositionArg >= aElements.size()) THROW_LE(ioneSegmentAV);
    aElements.insert(aElements.begin() + PositionArg, ElementArg);
    Refresh();
}

void Segment::PushBack(ElementPointer ElementArg)
{
    aElements.push_back(ElementArg);
    Refresh();
}

void Segment::PushBackSuddenly(ElementPointer ElementArg)
{
    aElements.push_back(ElementArg);
}

void Segment::Erase(numerator PositionArg)
{
    // it carefully erases subelement from specified position
    if (PositionArg >= aElements.size()) THROW_LE(ioneSegmentAV);
    delete *(aElements.begin() + PositionArg);
    aElements.erase(aElements.begin() + PositionArg);
    Refresh();
}

void Segment::Clear()
{
    // all subelements will be erased
    if (aElements.empty()) return;
    ElementsStore::iterator i = aElements.begin();
    while (i != aElements.end()) delete *i++;
    aElements.clear();
    Refresh();
}

ElementPointer Segment::GetElement(numerator PositionArg)
{
    // ok, we just want to get pointer for specified subelement
    if (PositionArg >= aElements.size()) THROW_LE(ioneSegmentAV);
    return *(aElements.begin() + PositionArg);
}

ConstElementPointer Segment::GetElement(numerator PositionArg) const
{
    // this variant for constant fucntions
    if (PositionArg >= aElements.size()) THROW_LE(ioneSegmentAV);
    return *(aElements.begin() + PositionArg);
}

void Segment::Refresh() const
{
    AssociateLinks();
    FastRefresh();
}

InfluenceImage Segment::GetInfluence(time TimeArg, numerator VoiceArg) const
{
    // this code must be fast 'cause translator calls it for every sample
    InfluenceImage aResult; // make clear image
    if (TimeArg >= Time()) return aResult; // beyond segment time
    if (VoiceArg >= VoicesCount()) return aResult; // beyond harmonic depth
    time aSelfTime = TimeArg =
        __CorrectExternalTime(TimeArg, GetDuration(), Reversal());
    // now we should get influence, here are two cases:
    if (aKind == melodic) __CorrcetRequest(aIndexationMap, TimeArg, aPosition);
    else /* harmonic */ __CorrcetRequest(aIndexationMap, VoiceArg, aPosition);
    // now, just get influence image from selected position:
    aResult = (**(aElements.begin() + aPosition)).
        GetInfluence(TimeArg, VoiceArg);
    // we should apply segment laws
    aResult.P += Panoram()->operator()(aSelfTime);
    aResult.F *= Frequency()->operator()(aSelfTime);
    aResult.aV *= Volume()->operator()(aSelfTime);
    if (aResult.F < 0) aResult.bV *= Volume()->operator()(aSelfTime);
    return aResult;
}

String Segment::WriteTo(Paramlist &TargetArg) const
{
    String aMapKey = GenerateUniqueMapKey(TargetArg);
    WriteTypeTo(TargetArg, aMapKey);
    WriteLabelTo(TargetArg, aMapKey);
    TargetArg.Add<numerator>(aMapKey, prlpkRepeats, Repeats());
    TargetArg.Add<bool>(aMapKey, prlpkReversal, Reversal());
    TargetArg.Add<bool>(aMapKey, prlpkSegmentKind, aKind);
    TargetArg.Add<Function>(aMapKey, prlpkPanoram, *Panoram());
    TargetArg.Add<Function>(aMapKey, prlpkFrequency, *Frequency());
    TargetArg.Add<Function>(aMapKey, prlpkVolume, *Volume());
    // it's like all the others, but here we also have to save subelements
    numerator aCount = ElementsCount();
    TargetArg.AddAsCount(aMapKey, prlpkElement, aCount);
    for (numerator i = 0; i < aCount; i++)
    {
        TargetArg.Add<String>(aMapKey, prlpkElement, i,
            (**(aElements.begin() + i)).WriteTo(TargetArg));
    }
    return aMapKey;
}

void Segment::ReadFrom(const Paramlist &SourceArg, String MapKeyArg)
{
    ReadLabelFrom(SourceArg, MapKeyArg);
    Repeats(SourceArg.Data<numerator>(MapKeyArg, prlpkRepeats));
    Reversal(SourceArg.Data<bool>(MapKeyArg, prlpkReversal));
    aKind = SourceArg.Data<bool>(MapKeyArg, prlpkSegmentKind);
    *Panoram() = SourceArg.Data<Function>(MapKeyArg, prlpkPanoram);
    *Frequency() = SourceArg.Data<Function>(MapKeyArg, prlpkFrequency);
    *Volume() = SourceArg.Data<Function>(MapKeyArg, prlpkVolume);
    Clear(); // clear all old subelements and be ready for loading
    numerator aCount = SourceArg.DataAsCount(MapKeyArg, prlpkElement);
    ElementPointer aElement;
    String aMapKey;
    for (numerator i = 0; (i < aCount); i++)
    {
        // take key of map that contains subelement
        aMapKey = SourceArg.Data<String>(MapKeyArg, prlpkElement, i);
        // all right, just load an element, we have to use ReadElement
        aElement = ReadElement(SourceArg, aMapKey);
        aElements.push_back(aElement); // and push it back!
    }
    Refresh();
}

// class Link

Link::Link() : Element(etLink),
    LabelledEssence(prlmkElement),
    aObject(NIL)
{ }

Link::Link(String LabelArg) : Element(etLink),
    LabelledEssence(LabelArg),
    aObject(NIL)
{ }

Link::Link(const Link &Another) : Element(etLink),
    LabelledEssence(Another),
    aObject(NIL)
{ }

Link Link::operator = (const Link &Another)
{
    LabelledEssence::operator = (Another);
    aObject = NIL;
    return *this;
}

time Link::Time() const
{
    if (!aObject) return 0.;
    time aTime = aObject->Time();
    return aTime;
}

InfluenceImage Link::GetInfluence(time TimeArg, numerator VoiceArg) const
{
    if (!aObject) THROW_LE(ionePoorLink);
    InfluenceImage aImage = aObject->GetInfluence(TimeArg, VoiceArg);
    return aImage;
}

numerator Link::VoicesCount() const
{
    if (!aObject) return 0;
    numerator aVoicesCount = aObject->VoicesCount();
    return aVoicesCount;
}

String Link::WriteTo(Paramlist &) const
{
    return (prlpxLink + Label());
}

void Link::ReadFrom(const Paramlist &, String MapKeyArg)
{
    if (*(MapKeyArg.begin()) == *(prlpxLink.begin()))
        MapKeyArg.erase(MapKeyArg.begin());
    Label(MapKeyArg);
}

// class Record

Record::Record() : Element(etRecord),
    LabelledEssence(prlmkElement),
    aTime(0.)
{ }

Record::Record(const WaveStruct &WaveArg) : Element(etRecord),
    LabelledEssence(prlmkElement)
{
    AdaptRecord(WaveArg);
}

Record::Record(const Record &Another) : Element(etRecord),
    LabelledEssence(Another),
    aTime(Another.aTime),
    aDelta(Another.aDelta),
    aLeft(Another.aLeft),
    aRight(Another.aRight),
    aStereo(Another.aStereo)
{ }

Record Record::operator = (const Record &Another)
{
    LabelledEssence::operator = (Another);
    aTime = Another.aTime;
    aDelta = Another.aDelta;
    aLeft = Another.aLeft;
    aRight = Another.aRight;
    aStereo = Another.aStereo;
    return *this;
}

void Record::AdaptRecord(const WaveStruct &WaveArg)
{
    aTime = WaveArg.Duration();
    aStereo = (WaveArg.Channels() == stereo);
    numerator aSize = WaveArg.SamplesInData();
    if (aSize > 1) aDelta = aTime / (aSize - 1);
    else THROW_RE(ioneShortWave);
    if (aStereo)
    {
        aLeft.Size(aSize * sizeof(sample));
        aRight.Size(aSize * sizeof(sample));
        for (numerator i = 0; i < aSize; i++)
        {
            aLeft.SetData<sample>(WaveArg.Sample(2 * i), i);
            aRight.SetData<sample>(WaveArg.Sample(2 * i + 1), i);
        }
    }
    else
    {
        aLeft.Size(aSize * sizeof(sample));
        aRight.Clear();
        for (numerator i = 0; i < aSize; i++)
        {
            aLeft.SetData<sample>(WaveArg.Sample(i), i);
        }
    }
}

InfluenceImage Record::GetInfluence(time TimeArg, numerator VoiceArg) const
{
    InfluenceImage aResult;
    aResult.F = -1;
    if (TimeArg >= Time()) return aResult;
    if (VoiceArg >= VoicesCount()) return aResult;
    StereoPair aStereoPair = __GetStereoPair
        (TimeArg, aDelta, aStereo, aLeft, aRight);
    aResult.aV = aStereoPair.L;
    aResult.bV = aStereoPair.R;
    return aResult;
}

StereoPair Record::GetStereoPair(time TimeArg) const
{
    if (!Time()) return StereoPair();
    return __GetStereoPair(TimeArg, aDelta, aStereo, aLeft, aRight);
}

String Record::WriteTo(Paramlist &TargetArg) const
{
    String aMapKey = GenerateUniqueMapKey(TargetArg);
    WriteTypeTo(TargetArg, aMapKey);
    WriteLabelTo(TargetArg, aMapKey);
    TargetArg.Add<bool>(aMapKey, prlpkStereo, aStereo);
    TargetArg.Add<time>(aMapKey, prlpkDuration, aTime);
    TargetArg.Add<time>(aMapKey, prlpkDelta, aDelta);
    if (aStereo)
    {
        TargetArg.Add<BinaryChunk>(aMapKey, prlpkLeft, aLeft);
                TargetArg.Add<BinaryChunk>(aMapKey, prlpkRight, aRight);
    }
    else TargetArg.Add<BinaryChunk>(aMapKey, prlpkLevel, aLeft);
    return aMapKey;
}

void Record::ReadFrom(const Paramlist &SourceArg, String MapKeyArg)
{
    ReadLabelFrom(SourceArg, MapKeyArg);
    if (SourceArg.Exists(MapKeyArg, prlpkStereo))
        aStereo = SourceArg.Data<bool>(MapKeyArg, prlpkStereo);
    else aStereo = false;
    aTime = SourceArg.Data<time>(MapKeyArg, prlpkDuration);
    aDelta = SourceArg.Data<time>(MapKeyArg, prlpkDelta);
    if (aStereo)
    {
        aLeft = SourceArg.Data<BinaryChunk>(MapKeyArg, prlpkLeft);
        aRight = SourceArg.Data<BinaryChunk>(MapKeyArg, prlpkRight);
    }
    else aLeft = SourceArg.Data<BinaryChunk>(MapKeyArg, prlpkLevel);
}

/***************************************************************************
 *  Definitions of non-inline Methods
 *  Section II - Timbre
 **************************************************************************/

// class Sonority

void Sonority::SwitchStage(SoundingStage StageArg) const
{
    SS = StageArg;
    iAux = iLast;
    tS = 0;
    if (SS == ssAttack) tP = 0;
}

Sonority::Sonority() :
    LabelledEssence(prlmkSonority),
    tA(AttackTimeDefault),
    tR(ReleaseTimeDefault),
    sAP(0, 1, PanoramAltDefault),
    sAF(0, 1, FrequencyAltDefault),
    sAV(0, 1, VolumeFull),
    sRP(0, 1, PanoramAltDefault),
    sRF(0, 1, FrequencyAltDefault),
    sRV(0, 1, VolumeSilence),
    aPMod(0, 1, PanoramAltDefault),
    aFMod(0, 1, FrequencyAltDefault),
    aVMod(0, 1, VolumeAltDefault),
    nF(440   /* hz */),
    rnI(-100 /* db */),
    anI(0    /* db */),
    anP(0)
{
    Reset();
}

Sonority::Sonority(const Sonority &Another) :
    LabelledEssence(Another),
    tA(Another.tA),
    tR(Another.tR),
    sAP(Another.sAP),
    sAF(Another.sAF),
    sAV(Another.sAV),
    sRP(Another.sRP),
    sRF(Another.sRF),
    sRV(Another.sRV),
    aPMod(Another.aPMod),
    aFMod(Another.aFMod),
    aVMod(Another.aVMod),
    nR(Another.nR),
    nF(Another.nF),
    rnI(Another.rnI),
    anI(Another.anI),
    anP(Another.anP)
{
    Reset();
}

Sonority Sonority::operator = (const Sonority &Another)
{
    LabelledEssence::operator = (Another);
    tA = Another.tA;
    tR = Another.tR;
    sAP = Another.sAP;
    sAF = Another.sAF;
    sAV = Another.sAV;
    sRP = Another.sRP;
    sRF = Another.sRF;
    sRV = Another.sRV;
    aPMod = Another.aPMod;
    aFMod = Another.aFMod;
    aVMod = Another.aVMod;
    nR = Another.nR;
    nF = Another.nF;
    rnI = Another.rnI;
    anI = Another.anI;
    anP = Another.anP;
    Reset();
    return *this;
}

void Sonority::AttackTime(time TimeArg)
{
    if (TimeArg <= 0) THROW_RE(ioneSignalTime);
    tA = TimeArg;
}

void Sonority::ReleaseTime(time TimeArg)
{
    if (TimeArg <= 0) THROW_RE(ioneSignalTime);
    tR = TimeArg;
}

void Sonority::UseSample(const WaveStruct &WaveArg)
{
    nR.AdaptRecord(WaveArg);
    real A = WaveArg.SamplesInData();
    A = std::log(A) / std::log(2.);
    numerator aSize = std::pow(2, std::floor(A));
    real aIndexFactor = (real)WaveArg.SampleRate() / aSize;
    numerator
    aLow = 40 /* hz */ / aIndexFactor,
    aHigh = 8000 /* hz */ / aIndexFactor;
    if (aHigh >= aSize / 2) aHigh = aSize / 2 - 1;
    ComplexArray x(Complex(0, 0), aSize);
    for (numerator i = 0; i < aSize; i++) // get data, only left for stereo
        x[i].real(WaveArg.Sample(i * WaveArg.Channels()));
    __FFT(x); // oh it's too simple
    RealVector Mods(aSize / 2);
    for (numerator i = 0; i < aSize / 2; Mods[i] = std::abs(x[i]), i++);
    RealVector::const_iterator S = Mods.begin();
    nF = std::max_element(S + aLow, S + aHigh) - S;
    nF *= aIndexFactor;
}

void Sonority::NominalFrequency(real Arg)
{
    if (Arg <= 0) THROW_RE(ioneNominalFrequency);
    nF = Arg;
}

StereoPair Sonority::GetStereoPair(InfluenceImage ImageArg, time TimeArg) const
{
    StereoPair aResult;
    if (ImageArg.F < 0) aResult = ImageArg; // as is
    else // we have to generate sample first
    {
    // first: switching stages
    switch (SS)
    {
        case ssAttack : /* attack */
        if (!ImageArg.aV) SwitchStage(ssRelease);
        if (tS >= tA) SwitchStage(ssSustain);
        break; /* attack */
        case ssSustain : /* sustain */
        if (!ImageArg.aV) SwitchStage(ssRelease);
        break; /* sustain */
        case ssRelease : /* release */
        if (ImageArg.aV) SwitchStage(ssAttack);
        if (tS >= tR) SwitchStage(ssPause);
        break; /* release */
        case ssPause : /* pause */
        if (ImageArg.aV) SwitchStage(ssAttack);
        break; /* pause */
    }
    // second: process actual stage
    bool Attack = (SS == ssAttack), Release = (SS == ssRelease);
    if (Attack || Release)
    {
        real tE = tS / (Attack ? tA : tR);
        if (Release) ImageArg = iAux;
        ImageArg.P += (Attack ? sAP : sRP)(tE);
        ImageArg.F *= (Attack ? sAF : sRF)(tE);
        ImageArg.aV *= (Attack ? sAV : sRV)(tE);
        if (Attack) ImageArg.aV += iAux.aV * (1 - tE);
    }
    // third: apply modulations
    ImageArg.P += aPMod(tS);
    ImageArg.F *= aFMod(tS);
    ImageArg.aV *= aVMod(tS);
    // fourth: actually get the sample from record
    aResult = nR.GetStereoPair(tP);
    aResult *= ImageArg.aV;
    // fifth: apply noises
    real aRand = (real)std::rand() / RAND_MAX;
    aResult *= 1. - frnI;
    aResult += ImageArg.aV * frnI * (2. * aRand - 1.);
    if (anP > aRand) aResult *= fanI;
    // sixth: refresh some variables
    iLast = ImageArg;
    tS += tQ;
    tP += tQ * (ImageArg.F / nF);
    } // generation of sample
    return aResult.ApplyPan(ImageArg.P);
}

void Sonority::Reset() const
{
    SS = ssPause;
    iLast.Clear();
    iAux.Clear();
    frnI = __DbToFactor(rnI);
    fanI = __DbToFactor(anI);
}

String Sonority::WriteTo(Paramlist &TargetArg) const
{
    String aMapKey = GenerateUniqueMapKey(TargetArg);
    String aPrefix = prlpxSignal + prlpkParam;
    TargetArg.Add<time>(aMapKey, aPrefix, 0, tA);
    TargetArg.Add<time>(aMapKey, aPrefix, 1, tR);
    TargetArg.Add<Function>(aMapKey, aPrefix, 2, sAP);
    TargetArg.Add<Function>(aMapKey, aPrefix, 3, sAF);
    TargetArg.Add<Function>(aMapKey, aPrefix, 4, sAV);
    TargetArg.Add<Function>(aMapKey, aPrefix, 5, sRP);
    TargetArg.Add<Function>(aMapKey, aPrefix, 6, sRF);
    TargetArg.Add<Function>(aMapKey, aPrefix, 7, sRV);
    TargetArg.Add<Function>(aMapKey, aPrefix, 8, aPMod);
    TargetArg.Add<Function>(aMapKey, aPrefix, 9, aFMod);
    TargetArg.Add<Function>(aMapKey, aPrefix, 10, aVMod);
    TargetArg.Add<String>(aMapKey, aPrefix, 11, nR.WriteTo(TargetArg));
    TargetArg.Add<real>(aMapKey, aPrefix, 12, nF);
    TargetArg.Add<real>(aMapKey, aPrefix, 13, rnI);
    TargetArg.Add<real>(aMapKey, aPrefix, 14, anI);
    TargetArg.Add<real>(aMapKey, aPrefix, 15, anP);
    return aMapKey;
}

void Sonority::ReadFrom(const Paramlist &SourceArg, String MapKeyArg)
{
    String aPrefix = prlpxSignal + prlpkParam;
    AttackTime(SourceArg.Data<time>(MapKeyArg, aPrefix, 0));
    ReleaseTime(SourceArg.Data<time>(MapKeyArg, aPrefix, 1));
    sAP = SourceArg.Data<Function>(MapKeyArg, aPrefix, 2);
    sAF = SourceArg.Data<Function>(MapKeyArg, aPrefix, 3);
    sAV = SourceArg.Data<Function>(MapKeyArg, aPrefix, 4);
    sRP = SourceArg.Data<Function>(MapKeyArg, aPrefix, 5);
    sRF = SourceArg.Data<Function>(MapKeyArg, aPrefix, 6);
    sRV = SourceArg.Data<Function>(MapKeyArg, aPrefix, 7);
    aPMod = SourceArg.Data<Function>(MapKeyArg, aPrefix, 8);
    aFMod = SourceArg.Data<Function>(MapKeyArg, aPrefix, 9);
    aVMod = SourceArg.Data<Function>(MapKeyArg, aPrefix, 10);
    nR.ReadFrom(SourceArg, SourceArg.Data<String>(MapKeyArg, aPrefix, 11));
    NominalFrequency(SourceArg.Data<real>(MapKeyArg, aPrefix, 12));
    rnI = SourceArg.Data<real>(MapKeyArg, aPrefix, 13);
    anI = SourceArg.Data<real>(MapKeyArg, aPrefix, 14);
    anP = SourceArg.Data<real>(MapKeyArg, aPrefix, 15);
    Reset();
}

/***************************************************************************
 *  Definitions of non-inline Methods
 *  Section III - Translation
 **************************************************************************/

// class ITP

ITP::ITP() : LabelledEssence(prlmkITP),
    aSTP(),
    dSonorityFile(),
    dSonorityLabel(),
    dSonorityGain(0.),
    aKeepEnd(false),
    aFMargin(0.),
    aBMargin(0.),
    aStart(0.),
    aEnd(0.)
{ }

ITP::ITP(const ITP &Another) :
    LabelledEssence(Another),
    aSTP(Another.aSTP),
    dSonorityFile(Another.dSonorityFile),
    dSonorityLabel(Another.dSonorityLabel),
    dSonorityGain(Another.dSonorityGain),
    aKeepEnd(Another.aKeepEnd),
    aFMargin(Another.aFMargin),
    aBMargin(Another.aBMargin),
    aStart(Another.aStart),
    aEnd(Another.aEnd)
{ }

ITP ITP::operator = (const ITP &Another)
{
    LabelledEssence::operator = (Another);
    aSTP = Another.aSTP;
    dSonorityFile = Another.dSonorityFile;
    dSonorityLabel = Another.dSonorityLabel;
    dSonorityGain = Another.dSonorityGain;
    aKeepEnd = Another.aKeepEnd;
    aFMargin = Another.aFMargin;
    aBMargin = Another.aBMargin;
    aStart = Another.aStart;
    aEnd = Another.aEnd;
    return *this;
}

void ITP::SonorityFile(String FileArg, numerator IndexArg)
{
    if (IndexArg >= aSTP.size()) THROW_LE(ioneITPAV);
    (aSTP.begin() + IndexArg)->aFile = FileArg;
}

String ITP::SonorityFile(numerator IndexArg) const
{
    if (IndexArg >= aSTP.size()) THROW_LE(ioneITPAV);
    return (aSTP.begin() + IndexArg)->aFile;
}

void ITP::SonorityLabel(String LabelArg, numerator IndexArg)
{
    if (IndexArg >= aSTP.size()) THROW_LE(ioneITPAV);
        (aSTP.begin() + IndexArg)->aLabel = LabelArg;
}

String ITP::SonorityLabel(numerator IndexArg) const
{
    if (IndexArg >= aSTP.size()) THROW_LE(ioneITPAV);
    return (aSTP.begin() + IndexArg)->aLabel;
}

void ITP::SonorityGain(real GainArg, numerator IndexArg)
{
    if (IndexArg >= aSTP.size()) THROW_LE(ioneITPAV);
        (aSTP.begin() + IndexArg)->aGain = GainArg;
}

real ITP::SonorityGain(numerator IndexArg) const
{
    if (IndexArg >= aSTP.size()) THROW_LE(ioneITPAV);
    return (aSTP.begin() + IndexArg)->aGain;
}

void ITP::ForwardMargin(time Arg)
{
    if (Arg < 0) THROW_RE(ioneITPTime);
    aFMargin = Arg;
}

void ITP::BackwardMargin(time Arg)
{
    if (Arg < 0) THROW_RE(ioneITPTime);
    aBMargin = Arg;
}

void ITP::StartTime(time Arg)
{
    if (Arg < 0) THROW_RE(ioneITPTime);
    if (Arg > aEnd) THROW_RE(ioneITPTime);
    aStart = Arg;
}

void ITP::EndTime(time Arg)
{
    if (Arg < 0) THROW_RE(ioneITPTime);
    if (Arg < aStart) THROW_RE(ioneITPTime);
    if (Arg != aEnd) aKeepEnd = true;
    aEnd = Arg;
}

void ITP::AdjustFor(Element &TargetArg)
{
    SonorityTP aParams = { dSonorityFile, dSonorityLabel, dSonorityGain };
    aSTP.resize(TargetArg.VoicesCount(), aParams);
    if (!aKeepEnd) aEnd = TargetArg.Time();
}

String ITP::WriteFor(Paramlist &TargetArg, String MapKeyArg) const
{
    String aMapKey = GenerateUniqueMapKey(TargetArg);
    TargetArg.Add<String>(MapKeyArg, prlpkITP, aMapKey);
    TargetArg.Add<time>(aMapKey, prlpkFMargin, aFMargin);
    TargetArg.Add<time>(aMapKey, prlpkBMargin, aBMargin);
    TargetArg.Add<time>(aMapKey, prlpkStart, aStart);
    TargetArg.Add<time>(aMapKey, prlpkEnd, aEnd);
    numerator aCount = aSTP.size();
    TargetArg.AddAsCount(aMapKey, prlpkSonorityFile, aCount);
    for (numerator i = 0; i < aCount; i++)
    {
        TargetArg.Add<String>
        (aMapKey, prlpkSonorityFile, i, (aSTP.begin() + i)->aFile);
        TargetArg.Add<String>
        (aMapKey, prlpkSonorityLabel, i, (aSTP.begin() + i)->aLabel);
        TargetArg.Add<real>
        (aMapKey, prlpkSonorityGain, i, (aSTP.begin() + i)->aGain);
    }
    return aMapKey;
}

void ITP::ReadFor(const Paramlist &SourceArg, String MapKeyArg)
{
    String aMapKey = SourceArg.Data<String>(MapKeyArg, prlpkITP);
    ForwardMargin(SourceArg.Data<time>(aMapKey, prlpkFMargin));
    BackwardMargin(SourceArg.Data<time>(aMapKey, prlpkBMargin));
    StartTime(SourceArg.Data<time>(aMapKey, prlpkStart));
    EndTime(SourceArg.Data<time>(aMapKey, prlpkEnd));
    numerator aCount = SourceArg.DataAsCount(aMapKey, prlpkSonorityFile);
    for (numerator i = 0; i < aCount; i++)
    {
        SonorityTP aParams;
        aParams.aFile = SourceArg.Data<String>
            (aMapKey, prlpkSonorityFile, i);
        aParams.aLabel = SourceArg.Data<String>
            (aMapKey, prlpkSonorityLabel, i);
        aParams.aGain = SourceArg.Data<real>
            (aMapKey, prlpkSonorityGain, i);
        aSTP.push_back(aParams);
    }
}

// class Translator

Translator::Translator(Source SourceArg, Out OutArg) :
    aTranslatorMode(tmPreparing),
    aForwardMargin(0.),
    aBackwardMargin(0.),
    aStartTime(0.)
{
    SetSource(SourceArg); // set source with null-test
    SetOut(OutArg); // set output with null-test
    aEndTime = SourceArg->Time();
}

Translator::~Translator()
{
    Reset();
}

void Translator::ForwardMargin(time ForwardMarginArg)
{
    if (aTranslatorMode != tmPreparing) THROW_LE(ioneTranslatorIPC);
    aForwardMargin = ForwardMarginArg;
}

void Translator::BackwardMargin(time BackwardMarginArg)
{
    if (aTranslatorMode != tmPreparing) THROW_LE(ioneTranslatorIPC);
    aBackwardMargin = BackwardMarginArg;
}

void Translator::StartTime(time StartTimeArg)
{
    if (aTranslatorMode != tmPreparing) THROW_LE(ioneTranslatorIPC);
    aStartTime = StartTimeArg;
}

void Translator::EndTime(time EndTimeArg)
{
    if (aTranslatorMode != tmPreparing) THROW_LE(ioneTranslatorIPC);
    aEndTime = EndTimeArg;
}

void Translator::SetTimbre(Timbre TimbreArg, numerator PositionArg)
{
    if (aTranslatorMode != tmPreparing) THROW_LE(ioneTranslatorIPC);
    if (PositionArg >= TimbresCount()) THROW_LE(ioneTranslatorAV);
    (aVoiceKits.begin() + PositionArg)->aTimbre = TimbreArg;
}

void Translator::MuteTimbre(numerator PositionArg)
{
    if (aTranslatorMode != tmPreparing) THROW_LE(ioneTranslatorIPC);
    if (PositionArg >= TimbresCount()) THROW_LE(ioneTranslatorAV);
    (aVoiceKits.begin() + PositionArg)->aTimbre = NIL;
}

void Translator::SetGain(real GainArg, numerator PositionArg)
{
    if (aTranslatorMode != tmPreparing) THROW_LE(ioneTranslatorIPC);
    if (PositionArg >= TimbresCount()) THROW_LE(ioneTranslatorAV);
    (aVoiceKits.begin() + PositionArg)->Gain = GainArg;
}

void Translator::SetSource(Source SourceArg)
{
    if (aTranslatorMode != tmPreparing) THROW_LE(ioneTranslatorIPC);
    if (!SourceArg) THROW_LE(ioneTranslatorBadSource);
    aSource = SourceArg;
    VoiceKit aVoiceKit = { NIL, NIL, 0u, 0. };
    aVoiceKits.resize(aSource->VoicesCount(), aVoiceKit);
}

void Translator::SetOut(Out OutArg)
{
    if (aTranslatorMode != tmPreparing) THROW_LE(ioneTranslatorIPC);
    if (!OutArg) THROW_LE(ioneTranslatorBadOut);
    aOut = OutArg;
}

numerator Translator::SetTimbresCount() const
{
    numerator aResult = 0;
    VoiceKitsStore::const_iterator i = aVoiceKits.begin();
    while (i != aVoiceKits.end()) if (i++->aTimbre) aResult++;
    return aResult;
}

real Translator::GetGain(numerator PositionArg) const
{
    if (PositionArg >= TimbresCount()) THROW_LE(ioneTranslatorAV);
    return (aVoiceKits.begin() + PositionArg)->Gain;
}

numerator Translator::SignificantSC() const
{
    // first: verification
    if ((aStartTime < 0.) || (aStartTime > aSource->Time()))
        THROW_LE(ioneTranslatorTime);
    if (aEndTime < 0.) THROW_LE(ioneTranslatorTime);
    // second: calculation
    numerator aMSPS = aOut->SampleRate() * aOut->Channels();
    numerator tSSC = (aEndTime - aStartTime) * aMSPS;
    return (tSSC / aOut->Channels());
}

time Translator::TotalTime() const
{
    time aResult = aEndTime - aStartTime;
    aResult += aForwardMargin + aBackwardMargin;
    return aResult;
}

void Translator::GetReady()
{
    if (aTranslatorMode != tmPreparing) THROW_LE(ioneTranslatorLETT);
    // first: verification of all parameters
    if (aForwardMargin < 0.) THROW_LE(ioneTranslatorTime);
    if (aBackwardMargin < 0.) THROW_LE(ioneTranslatorTime);
    if ((aStartTime < 0.) || (aStartTime > aSource->Time()))
        THROW_LE(ioneTranslatorTime);
    if (aEndTime < 0.) THROW_LE(ioneTranslatorTime);
    // second: calculate samples count for several cases
    numerator aMSPS = aOut->SampleRate() * aOut->Channels(); // ms per second
    aMSC = aForwardMargin * aMSPS; // forward margin
    aSSC = (aEndTime - aStartTime) * aMSPS; // number of significant samples
    aTSC = aSSC + (aForwardMargin + aBackwardMargin) * aMSPS; // total number
    // third: resize WAVE file and prepare it
    aOut->SamplesInData(aTSC); // init by total samples count
    // fourth: prepare voice kits
    time aQuantum = 1 / (time)aOut->SampleRate();
    for (VoiceKitsStore::iterator i = aVoiceKits.begin();
        i != aVoiceKits.end(); i++)
    if (i->aTimbre)
    {
        i->aTimbre->Reset(); // reset sonority
        i->aTimbre->SetQuantum(aQuantum);
        i->aSource = CopyElement(aSource);
    }
    // fifth: set new translator mode
    aTranslatorMode = tmTranslation;
}

bool Translator::TranslateBlock(numerator VoiceArg) const
{
    // first: check some conditions
    if (aTranslatorMode != tmTranslation) THROW_LE(ioneTranslatorITRN);
    if (VoiceArg >= TimbresCount()) THROW_LE(ioneTranslatorAV);
    if (!(aVoiceKits.begin() + VoiceArg)->aTimbre) return false; // muted
    // second: calculate some variables
    StereoPair aStereoPair;
    real aTimeFactor = 1. / (aOut->SampleRate() * aOut->Channels());
    Timbre aTimbre = (aVoiceKits.begin() + VoiceArg)->aTimbre;
    Source aSource = (aVoiceKits.begin() + VoiceArg)->aSource;
    real aGain = (aVoiceKits.begin() + VoiceArg)->Gain;
    numerator *aPosition = &((aVoiceKits.begin() + VoiceArg)->TSC);
    numerator aCycles = 0;
    // third: main loop
    while ((*aPosition < aSSC) && (aCycles++ < TranslationBlockSize))
    {
        // 3.1 calculate actual time
        time aTime = *aPosition * aTimeFactor + aStartTime;
        // 3.2 get stereo pair for actual time and voice
        aStereoPair = aTimbre->GetStereoPair
            (aSource->GetInfluence(aTime, VoiceArg), aTime);
        // 3.3 use gain
        if (aGain) aStereoPair.Gain(aGain);
        // 3.4 assign stereo pair to data cell
        if (aOut->Channels() == mono) // mono variant
        {
            aOut->Superposition
                (aStereoPair.AsMono(), aMSC + (*aPosition)++);
        }
        else // ..and stereo variant
        {
            aOut->Superposition
                (aStereoPair.L, aMSC + (*aPosition)++);
            aOut->Superposition
                (aStereoPair.R, aMSC + (*aPosition)++);
        }
    }
    return (*aPosition < aSSC);
}

void Translator::Reset() const
{
    if (aTranslatorMode == tmPreparing) return;
    VoiceKitsStore::const_iterator i = aVoiceKits.begin();
    while (i != aVoiceKits.end())
    {
        if (i->aTimbre)
        {
            i->aTimbre->Reset(); // reset sonority
            delete i->aSource; // reset private source
        }
        i++;
    }
    aTranslatorMode = tmPreparing;
}

real Translator::Progress() const
{
    if (aTranslatorMode < tmTranslation) return 0.;
    numerator aSetTimbresCount = SetTimbresCount();
    if (!aSetTimbresCount || !aSSC) return 100.;
    real aResult = 0.;
    VoiceKitsStore::const_iterator i = aVoiceKits.begin();
    while (i != aVoiceKits.end())
    {
        if (i->aTimbre) aResult += i->TSC;
        i++;
    }
    aResult /= (real)aSSC * aSetTimbresCount;
    return aResult * 100.; // it returns its result in percentage
}

/***************************************************************************
 *  Definitions of non-inline Methods
 *  Section IV - Generative Music
 **************************************************************************/

// class Condition

Condition::Condition() :
    aRating(0, 0, RatingDefault),
    aAffect(AffectDefault),
    aOverwrite(false)
{ }

Condition::Condition(const Condition &Another) :
    aRating(Another.aRating),
    aAffect(Another.aAffect),
    aOverwrite(Another.aOverwrite)
{ }

Condition Condition::operator = (const Condition &Another)
{
    aRating = Another.aRating;
    aAffect = Another.aAffect;
    aOverwrite = Another.aOverwrite;
    return *this;
}

// class GElement

GElement::GElement() :
    aElement(NIL),
    aHarmonicMask(0),
    aAssignedValue(0)
{ }

GElement::GElement(ElementPointer ElementArg) :
    aElement(ElementArg),
    aHarmonicMask(0),
    aAssignedValue(0)
{ }

GElement::GElement(const GElement &Another) :
    aCVector(Another.aCVector),
    aHarmonicMask(Another.aHarmonicMask),
    aAssignedValue(Another.aAssignedValue)
{
    aElement = CopyElement(Another.aElement);
}

GElement GElement::operator = (const GElement &Another)
{
    if (aElement) delete aElement;
    aElement = CopyElement(Another.aElement);
    aCVector = Another.aCVector;
    aHarmonicMask = Another.aHarmonicMask;
    aAssignedValue = Another.aAssignedValue;
    return *this;
}

GElement::~GElement()
{
    if (aElement) delete aElement;
}

void GElement::CVectorSize(numerator SizeArg)
{
    aCVector.resize(SizeArg);
}

ConditionIter GElement::GetCondition(numerator PositionArg)
{
    if (PositionArg >= aCVector.size()) THROW_LE(ioneGElementAV);
    return (aCVector.begin() + PositionArg);
}

ConstConditionIter GElement::GetCondition(numerator PositionArg) const
{
    if (PositionArg >= aCVector.size()) THROW_LE(ioneGElementAV);
    return (aCVector.begin() + PositionArg);
}

real GElement::GetRating(const RealVector &CVectorArg) const
{
    if (CVectorArg.size() > CVectorSize()) THROW_LE(ioneGCVSize);
    real aRating = 1;
    numerator aCVectorSize = CVectorSize();
    for (numerator i = 0; i < aCVectorSize; i++)
    {
        aRating *= (aCVector.begin() + i)->Rating()->
            operator()(*(CVectorArg.begin() + i));
    }
    return aRating;
}

numerator GElement::GetFSV() const
{
    for (numerator i = 0; i < (sizeof(numerator) << 3); i++)
        if (aHarmonicMask & (1 << i)) return i;
    return (sizeof(numerator) << 3);
}

String GElement::WriteTo(Paramlist &TargetArg) const
{
    // first: just save the element and get its map key
    String aMapKey = aElement->WriteTo(TargetArg);
    // second: we also have to save rating functions and condition vector
    numerator aSize = CVectorSize();
    TargetArg.AddAsCount(aMapKey, prlpkCondition, aSize);
    for (numerator i = 0; i < aSize; i++)
    {
                // writing a condition:
        ConstConditionIter aCondition = aCVector.begin() + i;
        TargetArg.Add<Function>
            (aMapKey, prlpkRating, i, *(aCondition->Rating()));
        TargetArg.Add<real>
            (aMapKey, prlpkAffect, i, aCondition->Affect());
        TargetArg.Add<bool>
            (aMapKey, prlpkOverwrite, i, aCondition->Overwrite());
    }
    // third: save harmonic index
    TargetArg.Add<numerator>(aMapKey, prlpkHarmonicMask, aHarmonicMask);
    return aMapKey;
}

void GElement::ReadFrom(const Paramlist &SourceArg, String MapKeyArg)
{
    // first: load our element and allocate it with ReadElement method
    aElement = ReadElement(SourceArg, MapKeyArg);
    // second: set size of condition vector and load rating functions + CV
    numerator aSize = SourceArg.DataAsCount(MapKeyArg, prlpkCondition);
    CVectorSize(aSize);
    for (numerator i = 0; i < aSize; i++)
    {
        // reading a condition:
        ConditionIter aCondition = aCVector.begin() + i;
        *(aCondition->Rating()) =
            SourceArg.Data<Function>(MapKeyArg, prlpkRating, i);
        aCondition->Affect
            (SourceArg.Data<real>(MapKeyArg, prlpkAffect, i));
        aCondition->Overwrite
            (SourceArg.Data<bool>(MapKeyArg, prlpkOverwrite, i));
    }
    // third: load harmonic index
    aHarmonicMask = SourceArg.Data<numerator>(MapKeyArg, prlpkHarmonicMask);
}

// class Generator

Generator::Generator() : LabelledEssence(prlmkGenerator),
    aCVectorSize(0),
    aHarmonicDepth(0),
    aSeed(0),
    aPanoram(0, 0, PanoramAltDefault),
    aFrequency(0, 0, FrequencyAltDefault),
    aVolume(0, 0, VolumeAltDefault),
    aLesserDuration(0.),
    aRequestedDuration(0.)
{ }

Generator::Generator(const Generator &Another) : LabelledEssence(Another),
    aCVectorSize(Another.aCVectorSize),
    aHarmonicDepth(Another.aHarmonicDepth),
    aSeed(Another.aSeed),
    aPanoram(Another.aPanoram),
    aFrequency(Another.aFrequency),
    aVolume(Another.aVolume),
    aGElements(Another.aGElements),
    aInitCVector(Another.aInitCVector),
    aVoicesLabels(Another.aVoicesLabels),
    aVoicesPanoram(Another.aVoicesPanoram),
    aVoicesFrequency(Another.aVoicesFrequency),
    aVoicesVolume(Another.aVoicesVolume),
    aRequestedDuration(Another.aRequestedDuration)
{ }

Generator Generator::operator = (const Generator &Another)
{
    LabelledEssence::operator = (Another);
    aCVectorSize = Another.aCVectorSize;
    aHarmonicDepth = Another.aHarmonicDepth;
    aSeed = Another.aSeed;
    aPanoram = Another.aPanoram;
    aFrequency = Another.aFrequency;
    aVolume = Another.aVolume;
    aGElements = Another.aGElements;
    aInitCVector = Another.aInitCVector;
    aVoicesLabels = Another.aVoicesLabels;
    aVoicesPanoram = Another.aVoicesPanoram;
    aVoicesFrequency = Another.aVoicesFrequency;
    aVoicesVolume = Another.aVoicesVolume;
    aRequestedDuration = Another.aRequestedDuration;
    return *this;
}

void Generator::CVectorSize(numerator SizeArg)
{
    aInitCVector.resize(SizeArg);
    aCVectorSize = SizeArg;
    GElementsStore::iterator aGElement = aGElements.begin();
    while (aGElement != aGElements.end())
        aGElement++->CVectorSize(SizeArg);
}

void Generator::HarmonicDepth(numerator HarmonicDepthArg)
{
    if (HarmonicDepthArg >= GeneratorHarmonicDepth)
        THROW_LE(ioneGeneratorHDG);
    aHarmonicDepth = HarmonicDepthArg;
    aVoicesLabels.resize(aHarmonicDepth, prlmkElement);
    aVoicesPanoram.resize
        (aHarmonicDepth, Function(0, 0, PanoramAltDefault));
    aVoicesVolume.resize
        (aHarmonicDepth, Function(0, 0, VolumeAltDefault));
    aVoicesFrequency.resize
        (aHarmonicDepth, Function(0, 0, FrequencyAltDefault));
}

void Generator::InitCondition(real ValueArg, numerator PositionArg)
{
    if (PositionArg >= aCVectorSize) THROW_LE(ioneGeneratorAV);
    *(aInitCVector.begin() + PositionArg) = ValueArg;
}

real Generator::InitCondition(numerator PositionArg) const
{
    if (PositionArg >= aCVectorSize) THROW_LE(ioneGeneratorAV);
    return *(aInitCVector.begin() + PositionArg);
}

void Generator::VoiceLabel(String VoiceLabelArg, numerator IndexArg)
{
    if (IndexArg >= aHarmonicDepth) THROW_LE(ioneGeneratorAV);
    *(aVoicesLabels.begin() + IndexArg) = VoiceLabelArg;
}

String Generator::VoiceLabel(numerator IndexArg) const
{
    if (IndexArg >= aHarmonicDepth) THROW_LE(ioneGeneratorAV);
    return *(aVoicesLabels.begin() + IndexArg);
}

FunctionIter Generator::VoicePanoram(numerator IndexArg)
{
    if (IndexArg >= aHarmonicDepth) THROW_LE(ioneGeneratorAV);
    return (aVoicesPanoram.begin() + IndexArg);
}

ConstFunctionIter Generator::VoicePanoram(numerator IndexArg) const
{
    if (IndexArg >= aHarmonicDepth) THROW_LE(ioneGeneratorAV);
    return (aVoicesPanoram.begin() + IndexArg);
}

FunctionIter Generator::VoiceFrequency(numerator IndexArg)
{
    if (IndexArg >= aHarmonicDepth) THROW_LE(ioneGeneratorAV);
    return (aVoicesFrequency.begin() + IndexArg);
}

ConstFunctionIter Generator::VoiceFrequency(numerator IndexArg) const
{
    if (IndexArg >= aHarmonicDepth) THROW_LE(ioneGeneratorAV);
    return (aVoicesFrequency.begin() + IndexArg);
}

FunctionIter Generator::VoiceVolume(numerator IndexArg)
{
    if (IndexArg >= aHarmonicDepth) THROW_LE(ioneGeneratorAV);
    return (aVoicesVolume.begin() + IndexArg);
}

ConstFunctionIter Generator::VoiceVolume(numerator IndexArg) const
{
    if (IndexArg >= aHarmonicDepth) THROW_LE(ioneGeneratorAV);
    return (aVoicesVolume.begin() + IndexArg);
}

void Generator::RequestedDuration(time Arg)
{
    if (Arg < 0) THROW_LE(ioneGeneratorDuration);
    aRequestedDuration = Arg;
}

void Generator::Insert(GElement GElementArg, numerator PositionArg)
{
    if (PositionArg >= GElementsCount()) THROW_LE(ioneGeneratorAV);
    GElementArg.CVectorSize(aCVectorSize);
    aGElements.insert(aGElements.begin() + PositionArg, GElementArg);
}

void Generator::PushBack(GElement GElementArg)
{
    GElementArg.CVectorSize(aCVectorSize);
    aGElements.push_back(GElementArg);
}

void Generator::Erase(numerator PositionArg)
{
    if (PositionArg >= GElementsCount()) THROW_LE(ioneGeneratorAV);
    aGElements.erase(aGElements.begin() + PositionArg);
}

void Generator::Clear()
{
    aGElements.clear();
}

GElementIter Generator::GetGElement(numerator PositionArg)
{
    if (PositionArg >= GElementsCount()) THROW_LE(ioneGeneratorAV);
    return (aGElements.begin() + PositionArg);
}

ConstGElementIter Generator::GetGElement(numerator PositionArg) const
{
    if (PositionArg >= GElementsCount()) THROW_LE(ioneGeneratorAV);
    return (aGElements.begin() + PositionArg);
}

void Generator::ImportElement(const Paramlist &SourceArg, String MapArg)
{
    ElementPointer aElement = ReadElement(SourceArg, MapArg);
    PushBack(GElement(aElement));
}

void Generator::ImportSubelements(const Paramlist &SourceArg, String MapArg)
{
    ElementPointer aElement = ReadElement(SourceArg, MapArg);
    if (!aElement->IsIt(etSegment)) THROW_LE(ioneGeneratorWEI);
    SegmentPointer aSegment = dynamic_cast<SegmentPointer>(aElement);
    numerator aCount = aSegment->ElementsCount();
    for (numerator i = 0; i < aCount;
        PushBack(GElement(CopyElement(aSegment->GetElement(i++)))));
    delete aSegment;
}

ElementPointer Generator::GenerateScore() const
{
    // first: preparing
    SegmentPointer aScore = new Segment; // create empty score
    aScore->Kind(harmonic); // always harmonic
    aScore->Label(Label()); // the same label as generator's label
    *(aScore->Panoram()) = aPanoram;
    *(aScore->Frequency()) = aFrequency;
    *(aScore->Volume()) = aVolume;
    if (!aHarmonicDepth) return aScore; // a little check
    // it's time to write all melodic segments into the score
    SegmentsStore aVoices;
    for (numerator i = 0; i < aHarmonicDepth; i++)
    {
        SegmentPointer aSegment = new Segment;
        // now just apply some adjustments
        aSegment->Kind(melodic); // always melodic
        aSegment->Label(*(aVoicesLabels.begin() + i)); // label
        *(aSegment->Panoram()) = *(aVoicesPanoram.begin() + i);
        *(aSegment->Frequency()) = *(aVoicesFrequency.begin() + i);
        *(aSegment->Volume()) = *(aVoicesVolume.begin() + i);
        aScore->PushBack(aSegment); // now push it back into the score
        aVoices.push_back(aSegment); // ..and into vector of voices
    }
    // set some parameters..
    std::srand(aSeed); // set the seed for randomator
    RealVector aCVector = aInitCVector; // initialize condition vector
    FlagsStore aFlags(GElementsCount(), false); // usage of GElements
    DurationsStore aDrt(aHarmonicDepth, 0.); // durations of voices
    // second: process
    aLesserDuration = 0.;
    while ((aLesserDuration =
        *std::min_element<DurationsStore::const_iterator>
        (aDrt.begin(), aDrt.end())) < aRequestedDuration)
    {
        // find voice with minimum duration:
        numerator aHarmonicIndex =
        std::min_element<DurationsStore::const_iterator>
            (aDrt.begin(), aDrt.end()) - aDrt.begin();
        // now we need to select the next element
        // for this: find elements with sufficient harmonic indexes
        IndexesStore aIndexes; // indexes from begin of vector
        RealVector aRatings; // ratings scale (probabilities)
        for (ConstGElementIter i = aGElements.begin();
            i != aGElements.end(); i++)
        {
            // now we have to take a look at the bit mask:
            if (i->HarmonicMask() & (1 << aHarmonicIndex))
            {
                if (i->CVectorSize() < aCVectorSize)
                    THROW_LE(ioneGCVSize);
                aIndexes.push_back(i - aGElements.begin());
                aRatings.push_back(i->GetRating(aCVector));
            }
        }
        // if here is no elements for the voice, it won't work..
        if (aIndexes.empty()) THROW_LE(ionePoorVoice);
        numerator aElementIndex = // just take one randomly
            *(aIndexes.begin() + __WeightedRandom(aRatings));
        ConstGElementIter aGElement =
            aGElements.begin() + aElementIndex;
        ConstElementPointer aPretender = // access the element
            aGElement->GetElement();
        // a little check for our pretender:
        if (aPretender->VoicesCount() > 1) THROW_LE(ioneGeneratorHDE);
        // now we have the element index and we're ready to process it
        // and that's how: we add the element to score
        ElementPointer aElement;
        if (*(aFlags.begin() + aElementIndex)) // create link
            aElement = new Link(aPretender->Label());
        else // ..or create element
        {
            aElement = CopyElement(aPretender);
            *(aFlags.begin() + aElementIndex) = true;
        }
        // push it into the score
        (**(aVoices.begin() + aHarmonicIndex)).PushBackSuddenly(aElement);
        // increment duration
        *(aDrt.begin() + aHarmonicIndex) += aPretender->Time();
        // ..and at last change condition vector
        for (numerator i = 0; i < aCVectorSize; i++)
        {
            ConstConditionIter aCondition = aGElement->GetCondition(i);
            if (aCondition->Overwrite())
                *(aCVector.begin() + i) = aCondition->Affect();
            else *(aCVector.begin() + i) += aCondition->Affect();
        }
    } // while
    aScore->Refresh(); // third: refresh the score
    return aScore;
}

real Generator::Progress() const
{
    real aResult = aRequestedDuration ?
        aLesserDuration / aRequestedDuration : 1.;
    if (aResult > 1.) aResult = 1.;
    return aResult * 100.; // in percenage
}

String Generator::WriteTo(Paramlist &TargetArg) const
{
    // first: generate unique map key
    String aMapKey = GenerateUniqueMapKey(TargetArg);
    WriteLabelTo(TargetArg, aMapKey); // ..and write label
    // second: write simple parameters
    TargetArg.Add<numerator>(aMapKey, prlpkCVectorSize, CVectorSize());
    TargetArg.Add<numerator>(aMapKey, prlpkHarmonicDepth, HarmonicDepth());
    TargetArg.Add<numerator>(aMapKey, prlpkSeed, Seed());
    TargetArg.Add<time>(aMapKey, prlpkDuration, RequestedDuration());
    // thrid: write all our generative elements
    numerator aCount = GElementsCount();
    TargetArg.AddAsCount(aMapKey, prlpkGElement, aCount);
    for (numerator i = 0; i < aCount; i++)
    {
        TargetArg.Add<String>(aMapKey, prlpkGElement, i,
            (aGElements.begin() + i)->WriteTo(TargetArg));
    }
    // fourth: write initial condition vector
    numerator aSize = CVectorSize();
    TargetArg.AddAsCount(aMapKey, prlpkCondition, aSize);
    for (numerator i = 0; i < aSize; i++)
    {
        TargetArg.Add<real>(aMapKey,
            prlpkCondition, i, *(aInitCVector.begin() + i));
    }
    // fifth: wirte parameters of voices as well
    aSize = HarmonicDepth();
    for (numerator i = 0; i < aSize; i++)
    {
        TargetArg.Add<String>(aMapKey,
            prlpkVoiceLabel, i, *(aVoicesLabels.begin() + i));
        TargetArg.Add(aMapKey,
            prlpkPanoram, i, *(aVoicesPanoram.begin() + i));
        TargetArg.Add(aMapKey,
            prlpkVolume, i, *(aVoicesVolume.begin() + i));
        TargetArg.Add(aMapKey,
            prlpkFrequency, i, *(aVoicesFrequency.begin() + i));
    }
    // sixth: write params of score
    TargetArg.Add(aMapKey, prlpkPanoram, aPanoram);
    TargetArg.Add(aMapKey, prlpkFrequency, aFrequency);
    TargetArg.Add(aMapKey, prlpkVolume, aVolume);
    return aMapKey;
}

void Generator::ReadFrom(const Paramlist &SourceArg, String MapKeyArg)
{
    // first: read label and assign it
    ReadLabelFrom(SourceArg, MapKeyArg);
    // second: read all simple parameters
    CVectorSize(SourceArg.Data<numerator>(MapKeyArg, prlpkCVectorSize));
    HarmonicDepth(SourceArg.Data<numerator>(MapKeyArg, prlpkHarmonicDepth));
    Seed(SourceArg.Data<numerator>(MapKeyArg, prlpkSeed));
    RequestedDuration(SourceArg.Data<time>(MapKeyArg, prlpkDuration));
    // third: read our generative elements, one by one
    Clear();
    numerator aCount = SourceArg.DataAsCount(MapKeyArg, prlpkGElement);
    String aMapKey;
    GElement aGElement;
    for (numerator i = 0; i < aCount; i++)
    {
        aMapKey = SourceArg.Data<String>(MapKeyArg, prlpkGElement, i);
        aGElement.ReadFrom(SourceArg, aMapKey);
        aGElements.push_back(aGElement);
    }
    // fourth: read inital condition vector
    numerator aSize = SourceArg.DataAsCount(MapKeyArg, prlpkCondition);
    CVectorSize(aSize);
    for (numerator i = 0; i < aSize; i++)
    {
        *(aInitCVector.begin() + i) = SourceArg.Data<real>
            (MapKeyArg, prlpkCondition, i);
    }
    // fifth: read parameters of voices
    aSize = HarmonicDepth();
    for (numerator i = 0; i < aSize; i++)
    {
        *(aVoicesLabels.begin() + i) = SourceArg.Data<String>
            (MapKeyArg, prlpkVoiceLabel, i);
        *(aVoicesPanoram.begin() + i) = SourceArg.Data<Function>
            (MapKeyArg, prlpkPanoram, i);
        *(aVoicesVolume.begin() + i) = SourceArg.Data<Function>
            (MapKeyArg, prlpkVolume, i);
        *(aVoicesFrequency.begin() + i) = SourceArg.Data<Function>
            (MapKeyArg, prlpkFrequency, i);
    }
    // sixth: read parameters of score
    aPanoram = SourceArg.Data<Function>(MapKeyArg, prlpkPanoram);
    aFrequency = SourceArg.Data<Function>(MapKeyArg, prlpkFrequency);
    aVolume = SourceArg.Data<Function>(MapKeyArg, prlpkVolume);
}

// class GElementsCreator

String GElementsCreator::ApplyMultivariant
    (Generator &TargetArg, String LabelArg) const
{
    String aPostfix;
    ElementPointer aLastOne = NIL;
    for (numerator i = 0; i < TargetArg.GElementsCount(); i++)
    {
        GElementIter aGElement = TargetArg.GetGElement(i);
        String aLabel = aGElement->GetElement()->Label();
        String aBase = aLabel.substr(0, LabelArg.size());
        if (aBase == LabelArg)
        {
            aPostfix = aLabel.substr
            (LabelArg.size(), aLabel.size() - LabelArg.size());
            aLastOne = aGElement->GetElement();
        }
    }
    if (!aLastOne) return LabelArg;
    if (aPostfix.empty())
    {
        aLastOne->Label(LabelArg + 'a');
        return (LabelArg + 'b');
    }
    else return (LabelArg + (CHAR)(*(aPostfix.begin()) + 1));
}

GElementsCreator::GElementsCreator() :
    aGeneratePauses(true),
    aGenerateSounds(true),
    aWholeNote(2),
    aDividers(1, 8),
    aFromPitch(0),
    aToPitch(0),
    aScale(12, ttPassing),
    aPanoram(0, 1, PanoramAltDefault),
    aFrequency(0, 1, FrequencyAltDefault),
    aVolume(0, 1, VolumeAltDefault),
    aScalePanoram(true),
    aScaleFrequency(true),
    aScaleVolume(true)
{ }

void GElementsCreator::WholeNote(time Arg)
{
    if (Arg < 0) THROW_LE(ioneGECreatorDuration);
    aWholeNote = Arg;
}

void GElementsCreator::DividersCount(numerator CountArg)
{
    aDividers.resize(CountArg, 8);
}

void GElementsCreator::Divider(real DividerArg, numerator PositionArg)
{
    if (PositionArg >= aDividers.size()) THROW_LE(ioneGECreatorAV);
    if (!DividerArg) THROW_LE(ioneGECreatorBD);
    *(aDividers.begin() + PositionArg) = DividerArg;
}

real GElementsCreator::Divider(numerator PositionArg) const
{
    if (PositionArg >= aDividers.size()) THROW_LE(ioneGECreatorAV);
    return *(aDividers.begin() + PositionArg);
}

void GElementsCreator::Temperament(numerator TemperamentArg)
{
    if (!TemperamentArg) THROW_LE(ioneGECreatorBT);
    aScale.resize(TemperamentArg, ttPassing);
}

void GElementsCreator::Scale(bool ExistsArg, numerator PositionArg)
{
    if (PositionArg >= Temperament()) THROW_LE(ioneGECreatorAV);
    *(aScale.begin() + PositionArg) = ExistsArg;
}

bool GElementsCreator::Scale(numerator PositionArg) const
{
    if (PositionArg >= aScale.size()) THROW_LE(ioneGECreatorAV);
    return *(aScale.begin() + PositionArg);
}

void GElementsCreator::CreateGElementsFor
    (Generator &TargetArg, numerator HarmonicMaskArg) const
{
    CHAR aBuffer[SMALLBUFFERSIZE];
    for (RealVector::const_iterator aDivider = aDividers.begin();
        aDivider != aDividers.end(); aDivider++)
    {
        time aDuration = aWholeNote / *aDivider;
        std::gcvt(*aDivider, DoubleNIN, aBuffer);
        if (aGenerateSounds)
        for (numerator aPitch = aFromPitch; aPitch <= aToPitch; aPitch++)
        {
            if (!*(aScale.begin() + (aPitch % aScale.size()))) continue;
            SoundPointer aSound = new Sound(aDuration);
            *(aSound->Panoram()) = aPanoram;
            __ScaleFunction(aSound->Panoram(), aDuration);
            *(aSound->Frequency()) = aFrequency;
            real vFrequency = ProduceFrequency(aPitch, aScale.size());
            __MultiplyFunction(aSound->Frequency(), vFrequency);
            __ScaleFunction(aSound->Frequency(), aDuration);
            *(aSound->Volume()) = aVolume;
            __ScaleFunction(aSound->Volume(), aDuration);
            String aLabel = FrequencyDescription(vFrequency) + '-';
            aSound->Label
                (ApplyMultivariant(TargetArg, aLabel + aBuffer));
            GElement aGElement(aSound);
            aGElement.HarmonicMask(HarmonicMaskArg);
            aGElement.AssignedValue(aPitch);
            TargetArg.PushBack(aGElement);
        } // for pitch
        if (aGeneratePauses) // and now we add pause
        {
            ElementPointer aPause = new Pause(aDuration);
            // - label -
            String aLabel("p-");
            aPause->Label
                (ApplyMultivariant(TargetArg, aLabel + aBuffer));
            GElement aGElement(aPause);
            aGElement.HarmonicMask(HarmonicMaskArg);
            TargetArg.PushBack(aGElement);
        }
    }
}

// class GVoiceParams

GVoiceParams::GVoiceParams() :
    LabelledEssence(prlmkGVoice),
    aDividers(1, 8),
    gRepetitivity(0),
    mTonalIntensity(0),
    mModalIntensity(0),
    mLowAddiction(0),
    mHighAddiction(0),
    mSmoothness(0),
    mLeaping(0),
    mRetention(0),
    rVelocity(0),
    rAlignment(0),
    rBeatDivider(4),
    rSilenceShare(0),
    rSilenceClustering(0),
    aFromPitch(0),
    aToPitch(0),
    aPanoram(0, 1, PanoramAltDefault),
    aFrequency(0, 1, FrequencyAltDefault),
    aVolume(0, 1, VolumeAltDefault),
    aScalePanoram(true),
    aScaleFrequency(true),
    aScaleVolume(true)
{ }

GVoiceParams::GVoiceParams(const GVoiceParams &Another) :
    LabelledEssence(prlmkGVoice),
    aDividers(Another.aDividers),
    gRepetitivity(Another.gRepetitivity),
    mTonalIntensity(Another.mTonalIntensity),
    mModalIntensity(Another.mModalIntensity),
    mLowAddiction(Another.mLowAddiction),
    mHighAddiction(Another.mHighAddiction),
    mSmoothness(Another.mSmoothness),
    mLeaping(Another.mLeaping),
    mRetention(Another.mRetention),
    rVelocity(Another.rVelocity),
    rAlignment(Another.rAlignment),
    rBeatDivider(Another.rBeatDivider),
    rSilenceShare(Another.rSilenceShare),
    rSilenceClustering(Another.rSilenceClustering),
    aFromPitch(Another.aFromPitch),
    aToPitch(Another.aToPitch),
    aPanoram(Another.aPanoram),
    aFrequency(Another.aFrequency),
    aVolume(Another.aVolume),
    aScalePanoram(Another.aScalePanoram),
    aScaleFrequency(Another.aScaleFrequency),
    aScaleVolume(Another.aScaleVolume)
{ }

GVoiceParams GVoiceParams::operator = (const GVoiceParams &Another)
{
    aDividers = Another.aDividers;
    gRepetitivity = Another.gRepetitivity;
    mTonalIntensity = Another.mTonalIntensity;
    mModalIntensity = Another.mModalIntensity;
    mLowAddiction = Another.mLowAddiction;
    mHighAddiction = Another.mHighAddiction;
    mSmoothness = Another.mSmoothness;
    mLeaping = Another.mLeaping;
    mRetention = Another.mRetention;
    rVelocity = Another.rVelocity;
    rAlignment = Another.rAlignment;
    rBeatDivider = Another.rBeatDivider;
    rSilenceShare = Another.rSilenceShare;
    rSilenceClustering = Another.rSilenceClustering;
    aFromPitch = Another.aFromPitch;
    aToPitch = Another.aToPitch;
    aPanoram = Another.aPanoram;
    aFrequency = Another.aFrequency;
    aVolume = Another.aVolume;
    aScalePanoram = Another.aScalePanoram;
    aScaleFrequency = Another.aScaleFrequency;
    aScaleVolume = Another.aScaleVolume;
    return *this;
}

void GVoiceParams::DividersCount(numerator CountArg)
{
    aDividers.resize(CountArg, 8);
}

void GVoiceParams::Divider(real DividerArg, numerator PositionArg)
{
    if (PositionArg >= aDividers.size()) THROW_LE(ioneGVoiceAV);
    if (!DividerArg) THROW_LE(ioneGVoiceBD);
    *(aDividers.begin() + PositionArg) = DividerArg;
}

real GVoiceParams::Divider(numerator PositionArg) const
{
    if (PositionArg >= aDividers.size()) THROW_LE(ioneGVoiceAV);
    return *(aDividers.begin() + PositionArg);
}

#define GVOICEPARAMS_ACCESSOR(NAME, TARGET) \
void GVoiceParams::NAME(real Arg) \
{ \
    if (Arg < 0) THROW_LE(ioneGVoiceNV); \
    TARGET = Arg; \
}

GVOICEPARAMS_ACCESSOR(Repetitivity,      gRepetitivity)
GVOICEPARAMS_ACCESSOR(TonalIntensity,    mTonalIntensity)
GVOICEPARAMS_ACCESSOR(ModalIntensity,    mModalIntensity)
GVOICEPARAMS_ACCESSOR(LowAddiction,      mLowAddiction)
GVOICEPARAMS_ACCESSOR(HighAddiction,     mHighAddiction)
GVOICEPARAMS_ACCESSOR(Smoothness,        mSmoothness)
GVOICEPARAMS_ACCESSOR(Leaping,           mLeaping)
GVOICEPARAMS_ACCESSOR(Retention,         mRetention)
GVOICEPARAMS_ACCESSOR(Velocity,          rVelocity)
GVOICEPARAMS_ACCESSOR(Alignment,         rAlignment)
GVOICEPARAMS_ACCESSOR(SilenceShare,      rSilenceShare)
GVOICEPARAMS_ACCESSOR(SilenceClustering, rSilenceClustering)

void GVoiceParams::BeatDivider(real Arg)
{
    if (Arg < 0) THROW_LE(ioneGVoiceNV);
    if (!Arg) THROW_LE(ioneGVoiceBD);
    rBeatDivider = Arg;
}

numerator GVoiceParams::PitchDiapason() const
{
    return 1 + aToPitch - aFromPitch;
}

real GVoiceParams::BackgroundForPitch
    (numerator PitchArg, ToneType ToneTypeArg, bool IsSoundArg) const
{
    real aBackground = 1;
    if (IsSoundArg)
    {
        if ((ToneTypeArg == ttScale) || (ToneTypeArg == ttTonal))
            aBackground *= std::exp(mModalIntensity);
        if (ToneTypeArg == ttTonal)
            aBackground *= std::exp(mTonalIntensity);
        real aLowFactor = PitchDiapason() - (PitchArg + 1);
        aLowFactor /= PitchDiapason();
        aBackground *= std::exp(aLowFactor * mLowAddiction);
        real aHighFactor = (PitchArg + 1) / PitchDiapason();
        aBackground *= std::exp(aHighFactor * mHighAddiction);
    }
    else aBackground *= std::exp(rSilenceShare);
    return aBackground;
}

numerator GVoiceParams::WeighedRandomPitch(const ScaleStore &ScaleArg) const
{
    RealVector aRatings;
    for (numerator i = 0; i < PitchDiapason(); i++)
    {
        ToneType aToneType = *(ScaleArg.begin() +
            ((i + aFromPitch) % ScaleArg.size()));
        real aPretender = BackgroundForPitch(i, aToneType, true);
        aRatings.push_back(aPretender);
    }
    return __WeightedRandom(aRatings);
}

real GVoiceParams::BackgroundForTime
    (time TimeArg, time WholeNoteArg, bool IsSoundArg) const
{
        real aBackground = 1.;
    real aRelativeSpeed = WholeNoteArg / (TimeArg * rBeatDivider);
    if (IsSoundArg)
    {
        aBackground /= std::exp(rSilenceShare);
        aBackground *= std::exp(rVelocity * (aRelativeSpeed - 1));
    }
    else
    {
        aBackground *= std::exp(rSilenceShare);
        aBackground *= std::exp((1 / aRelativeSpeed) - 1);
    }
    return aBackground;
}

numerator GVoiceParams::OptimalDivider() const
{
    RealVector tDividers(aDividers);
    for (numerator i = 0; i < ProbesCount; i++)
    while (true)
    {
        bool aKeen = true;
        RealVector::iterator tDivider = tDividers.begin();
        while (tDivider != tDividers.end())
            aKeen &= !((int)(*tDivider++) % *(Probes + i));
        if (aKeen)
        {
            tDivider = tDividers.begin();
            while (tDivider != tDividers.end())
                *tDivider++ /= *(Probes + i);
        }
        else break;
    }
    numerator aOptimalDivider = 1;
    RealVector::iterator tDivider = tDividers.begin();
    while (tDivider != tDividers.end())
        aOptimalDivider *= *tDivider++;
    return aOptimalDivider;
}

real GVoiceParams::AffectByTime(time TimeArg, time WholeNoteArg) const
{
    real aRelativeSpeed = WholeNoteArg / (TimeArg * rBeatDivider);
    return OptimalDivider() / aRelativeSpeed;
}

real GVoiceParams::MobilityFactor(numerator PitchArg, numerator CenterArg) const
{
    if (PitchArg == CenterArg) return std::log(2.);
    real x0 = CenterArg;
    if (PitchArg > CenterArg) x0 += mLeaping + 1;
    else x0 -= mLeaping + 1;
    real aMobilityFactor = (PitchArg - x0) / (CenterArg - x0);
    aMobilityFactor *= mSmoothness;
    if (aMobilityFactor < 0) aMobilityFactor = 0;
    return aMobilityFactor;
}

String GVoiceParams::WriteTo(Paramlist &TargetArg) const
{
    String aMapKey = GenerateUniqueMapKey(TargetArg);
    TargetArg.AddAsCount(aMapKey, prlpkDivider, aDividers.size());
    for (numerator i = 0; i < aDividers.size(); i++)
    {
        TargetArg.Add<real>
            (aMapKey, prlpkDivider, i, Divider(i));
    }
    TargetArg.Add<real>(aMapKey, prlpkParam, 0, gRepetitivity);
    TargetArg.Add<real>(aMapKey, prlpkParam, 1, mTonalIntensity);
    TargetArg.Add<real>(aMapKey, prlpkParam, 2, mModalIntensity);
    TargetArg.Add<real>(aMapKey, prlpkParam, 3, mLowAddiction);
    TargetArg.Add<real>(aMapKey, prlpkParam, 4, mHighAddiction);
    TargetArg.Add<real>(aMapKey, prlpkParam, 5, mSmoothness);
    TargetArg.Add<real>(aMapKey, prlpkParam, 6, mLeaping);
    TargetArg.Add<real>(aMapKey, prlpkParam, 7, mRetention);
    TargetArg.Add<real>(aMapKey, prlpkParam, 8, rVelocity);
    TargetArg.Add<real>(aMapKey, prlpkParam, 9, rAlignment);
    TargetArg.Add<real>(aMapKey, prlpkParam, 10, rBeatDivider);
    TargetArg.Add<real>(aMapKey, prlpkParam, 11, rSilenceShare);
    TargetArg.Add<real>(aMapKey, prlpkParam, 12, rSilenceClustering);
    TargetArg.Add<numerator>(aMapKey, prlpkParam, 13, aFromPitch);
    TargetArg.Add<numerator>(aMapKey, prlpkParam, 14, aToPitch);
    TargetArg.Add<Function>(aMapKey, prlpkParam, 15, aPanoram);
    TargetArg.Add<Function>(aMapKey, prlpkParam, 16, aFrequency);
    TargetArg.Add<Function>(aMapKey, prlpkParam, 17, aVolume);
    TargetArg.Add<bool>(aMapKey, prlpkParam, 18, aScalePanoram);
    TargetArg.Add<bool>(aMapKey, prlpkParam, 19, aScaleFrequency);
    TargetArg.Add<bool>(aMapKey, prlpkParam, 20, aScaleVolume);
    return aMapKey;
}

void GVoiceParams::ReadFrom(const Paramlist &SourceArg, String MapKeyArg)
{
    DividersCount(SourceArg.DataAsCount(MapKeyArg, prlpkDivider));
    RealVector::const_iterator aDivider = aDividers.begin();
    for (numerator i = 0; i < aDividers.size(); i++)
    {
        Divider(SourceArg.Data<real>(MapKeyArg, prlpkDivider, i), i);
    }
    Repetitivity(SourceArg.Data<real>(MapKeyArg, prlpkParam, 0));
    TonalIntensity(SourceArg.Data<real>(MapKeyArg, prlpkParam, 1));
    ModalIntensity(SourceArg.Data<real>(MapKeyArg, prlpkParam, 2));
    LowAddiction(SourceArg.Data<real>(MapKeyArg, prlpkParam, 3));
    HighAddiction(SourceArg.Data<real>(MapKeyArg, prlpkParam, 4));
    Smoothness(SourceArg.Data<real>(MapKeyArg, prlpkParam, 5));
    Leaping(SourceArg.Data<real>(MapKeyArg, prlpkParam, 6));
    Retention(SourceArg.Data<real>(MapKeyArg, prlpkParam, 7));
    Velocity(SourceArg.Data<real>(MapKeyArg, prlpkParam, 8));
    Alignment(SourceArg.Data<real>(MapKeyArg, prlpkParam, 9));
    BeatDivider(SourceArg.Data<real>(MapKeyArg, prlpkParam, 10));
    SilenceShare(SourceArg.Data<real>(MapKeyArg, prlpkParam, 11));
    SilenceClustering(SourceArg.Data<real>(MapKeyArg, prlpkParam, 12));
    FromPitch(SourceArg.Data<numerator>(MapKeyArg, prlpkParam, 13));
    ToPitch(SourceArg.Data<numerator>(MapKeyArg, prlpkParam, 14));
    aPanoram = SourceArg.Data<Function>(MapKeyArg, prlpkParam, 15);
    aFrequency = SourceArg.Data<Function>(MapKeyArg, prlpkParam, 16);
    aVolume = SourceArg.Data<Function>(MapKeyArg, prlpkParam, 17);
    ScalePanoram(SourceArg.Data<bool>(MapKeyArg, prlpkParam, 18));
    ScaleFrequency(SourceArg.Data<bool>(MapKeyArg, prlpkParam, 19));
    ScaleVolume(SourceArg.Data<bool>(MapKeyArg, prlpkParam, 20));
}

// class GWizard

void GWizard::AdjustPanoram(FunctionIter TargetArg, numerator Arg) const
{
    real aPanoram;
    if ((Arg == (VoicesCount() - 1)) && !(Arg % 2)) aPanoram = 0;
    else
    {
        aPanoram = (Arg % 2) ? +1 : -1;
        aPanoram *=
        (std::exp(1. / VoicesCount()) / (2. * M_E)) * (1 + Arg / 2);
    }
    TargetArg->PointValue(aPanoram, 0);
}

void GWizard::AdjustVolume(FunctionIter TargetArg, numerator Arg) const
{
    real aVolume = (VoicesCount() == 1) ? 1. : 2. / VoicesCount();
    TargetArg->PointValue(aVolume, 0);
}

GWizard::GWizard() :
    LabelledEssence(prlmkGWizard),
    aVoicesParams(1),
    aWholeNote(2),
    aRequestedDuration(60),
    aSeed(0),
    aScale(12, ttPassing)
{ }

GWizard::GWizard(const GWizard &Another) :
    LabelledEssence(prlmkGWizard),
    aVoicesParams(Another.aVoicesParams),
    aWholeNote(Another.aWholeNote),
    aRequestedDuration(Another.aRequestedDuration),
    aSeed(Another.aSeed),
    aScale(Another.aScale)
{ }

GWizard GWizard::operator = (const GWizard &Another)
{
    aVoicesParams = Another.aVoicesParams;
    aWholeNote = Another.aWholeNote;
    aRequestedDuration = Another.aRequestedDuration;
    aSeed = Another.aSeed;
    aScale = Another.aScale;
    return *this;
}

void GWizard::VoicesCount(numerator CountArg)
{
    aVoicesParams.resize(CountArg, GVoiceParams());
}

GVParamsIter GWizard::GetGVParams(numerator PositionArg)
{
    if (PositionArg >= aVoicesParams.size()) THROW_LE(ioneGWizardAV);
    return (aVoicesParams.begin() + PositionArg);
}

ConstGVParamsIter GWizard::GetGVParams(numerator PositionArg) const
{
    if (PositionArg >= aVoicesParams.size()) THROW_LE(ioneGWizardAV);
    return (aVoicesParams.begin() + PositionArg);
}

void GWizard::WholeNote(time Arg)
{
    if (Arg < 0) THROW_LE(ioneGWizardDuration);
    aWholeNote = Arg;
}

void GWizard::RequestedDuration(time Arg)
{
    if (Arg < 0) THROW_LE(ioneGWizardDuration);
    aRequestedDuration = Arg;
}

void GWizard::Temperament(numerator TemperamentArg)
{
    if (!TemperamentArg) THROW_LE(ioneGWizardBT);
    aScale.resize(TemperamentArg, ttPassing);
}

void GWizard::Scale(ToneType TypeArg, numerator PositionArg)
{
    if (PositionArg >= Temperament()) THROW_LE(ioneGWizardAV);
    *(aScale.begin() + PositionArg) = TypeArg;
}

ToneType GWizard::Scale(numerator PositionArg) const
{
    if (PositionArg >= Temperament()) THROW_LE(ioneGWizardAV);
    return *(aScale.begin() + PositionArg);
}

void GWizard::ApplyWizardFor(Generator &TargetArg) const
{
    // first: setting parameters of the generative project
    CHAR aBuffer[SMALLBUFFERSIZE];
    TargetArg.HarmonicDepth(VoicesCount());
    TargetArg.CVectorSize(2 * VoicesCount());
    TargetArg.Seed(0);
    TargetArg.RequestedDuration(aRequestedDuration);
    *(TargetArg.Panoram()) = Function(0, 0, 0);
    *(TargetArg.Frequency()) = Function(0, 0, 1);
    *(TargetArg.Volume()) = Function(0, 0, 1);
    TargetArg.Clear();
    std::srand(aSeed);
    // second: creating generative elements
    for (numerator v = 0; v < VoicesCount(); v++)
    {
        GVParamsStore::const_iterator aVoice = aVoicesParams.begin() + v;
        // - pitch diapason check -
        if (aVoice->FromPitch() > aVoice->ToPitch())
            THROW_LE(ioneGVoiceBP);
        // - setting initial conditions -
        TargetArg.InitCondition(0, 2 * v);
        TargetArg.InitCondition(aVoice->
            WeighedRandomPitch(aScale), 2 * v + 1);
        String aLabel("voice-");
        std::itoa(v, aBuffer, NumericRadix);
        TargetArg.VoiceLabel(aLabel + aBuffer, v);
        // - adjusting panoram & volume -
        AdjustPanoram(TargetArg.VoicePanoram(v), v);
        AdjustVolume(TargetArg.VoiceVolume(v), v);
                // - create elements with GElementsCreator -
        GElementsCreator aGECreator;
        aGECreator.GeneratePauses(true);
        aGECreator.GenerateSounds(true);
        aGECreator.WholeNote(aWholeNote);
        aGECreator.DividersCount(aVoice->DividersCount());
        for (numerator i = 0; i < aVoice->DividersCount(); i++)
            aGECreator.Divider(aVoice->Divider(i), i);
        aGECreator.FromPitch(aVoice->FromPitch());
        aGECreator.ToPitch(aVoice->ToPitch());
        aGECreator.Temperament(aScale.size());
        for (numerator i = 0; i < aScale.size();
            aGECreator.Scale(true, i++));
        *(aGECreator.Panoram()) = *(aVoice->Panoram());
        *(aGECreator.Frequency()) = *(aVoice->Frequency());
        *(aGECreator.Volume()) = *(aVoice->Volume());
        aGECreator.ScalePanoram(aVoice->ScalePanoram());
        aGECreator.ScaleFrequency(aVoice->ScaleFrequency());
        aGECreator.ScaleVolume(aVoice->ScaleVolume());
        aGECreator.CreateGElementsFor(TargetArg, (1 << v));
    }
    // third: adjusting generative elements
    for (numerator i = 0; i < TargetArg.GElementsCount(); i++)
    {
        // - getting some parameters -
        GElementIter aGElement = TargetArg.GetGElement(i);
        numerator v = aGElement->GetFSV();
        GVParamsStore::const_iterator V = aVoicesParams.begin() + v;
        bool S = aGElement->GetElement()->IsIt(etSound);
        numerator aPitch = aGElement->AssignedValue() - V->FromPitch();
        ToneType aToneType = *(aScale.begin() +
            (aGElement->AssignedValue() % Temperament()));
        time aTime = aGElement->GetElement()->Time();
        numerator aODivider = V->OptimalDivider();
        // - setting rhythmic system -
        ConditionIter aRC = aGElement->GetCondition(2 * v);
        aRC->Affect(V->AffectByTime(aTime, aWholeNote));
        aRC->Rating()->PointValue(V->BackgroundForTime
            (aTime, aWholeNote, S), 0);
        aRC->Rating()->Max(aODivider);
        aRC->Rating()->PointsCount(aODivider);
        for (numerator i = aRC->Affect() - 1; i < aODivider - 1;
            __GainPoint(aRC->Rating(), i++, V->Alignment()));
        __GainPoint(aRC->Rating(),
            std::rand() % aODivider, V->Repetitivity());
        aRC->Overwrite(false);
        // - setting melodic / harmonic system -
        ConditionIter aMC = aGElement->GetCondition(2 * v + 1);
        if (S) aMC->Affect(aPitch); else aMC->Affect(0);
        aMC->Rating()->PointValue(V->BackgroundForPitch
            (aPitch, aToneType, S), 0);
        aMC->Rating()->Max(V->PitchDiapason());
        aMC->Rating()->PointsCount(V->PitchDiapason());
        aMC->Overwrite(true);
        if (!S) continue;
        for (numerator i = 0; i < V->PitchDiapason(); i++)
            __GainPointZ(aMC->Rating(), i,
                V->MobilityFactor(i, aPitch));
        __GainPointZ(aMC->Rating(), aPitch, V->Retention());
        __GainPoint(aMC->Rating(),
            V->WeighedRandomPitch(aScale), V->Repetitivity());
    }
}

String GWizard::WriteTo(Paramlist &TargetArg) const
{
    String aMapKey = GenerateUniqueMapKey(TargetArg);
    WriteLabelTo(TargetArg, aMapKey);
    TargetArg.AddAsCount(aMapKey, prlpkGVoice, aVoicesParams.size());
    for (numerator i = 0; i < aVoicesParams.size(); i++)
    {
        TargetArg.Add<String>(aMapKey, prlpkGVoice, i,
            (aVoicesParams.begin() + i)->WriteTo(TargetArg));
    }
    TargetArg.AddAsCount(aMapKey, prlpkScale, aScale.size());
    for (numerator i = 0; i < aScale.size(); i++)
    {
        TargetArg.Add<numerator>(aMapKey, prlpkScale, i, Scale(i));
    }
    TargetArg.Add<real>(aMapKey, prlpkParam, 0, aWholeNote);
    TargetArg.Add<real>(aMapKey, prlpkParam, 1, aRequestedDuration);
    TargetArg.Add<numerator>(aMapKey, prlpkParam, 2, aSeed);
    return aMapKey;
}

void GWizard::ReadFrom(const Paramlist &SourceArg, String MapKeyArg)
{
    ReadLabelFrom(SourceArg, MapKeyArg);
    aVoicesParams.clear();
    numerator aSize = SourceArg.DataAsCount(MapKeyArg, prlpkGVoice);
    for(numerator i = 0; i < aSize; i++)
    {
        String aMapKey = SourceArg.
            Data<String>(MapKeyArg, prlpkGVoice, i);
        GVoiceParams aGVoiceParams;
        aGVoiceParams.ReadFrom(SourceArg, aMapKey);
        aVoicesParams.push_back(aGVoiceParams);
    }
    aScale.clear();
    aSize = SourceArg.DataAsCount(MapKeyArg, prlpkScale);
    for (numerator i = 0; i < aSize; i++)
    {
        aScale.push_back((ToneType)SourceArg.Data
            <numerator>(MapKeyArg, prlpkScale, i));
    }
    WholeNote(SourceArg.Data<real>(MapKeyArg, prlpkParam, 0));
    RequestedDuration(SourceArg.Data<real>(MapKeyArg, prlpkParam, 1));
    Seed(SourceArg.Data<numerator>(MapKeyArg, prlpkParam, 2));
}

ENDIONSTD
