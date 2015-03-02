/***************************************************************************
 *  C++ source
 *
 *  Parameter Lists library
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
#include <prl.h>

BEGINIONSTD

/***************************************************************************
 *  Data Section
 **************************************************************************/

const BYTE     // status bytes:
sbNil          (0x00),
sbPrlName      (0x01),
sbMapKey       (0x02),
sbPairKey      (0x03),
sbTerminator   (0x05),
sbPrlAuthor    (0x06),
sbPrlComment   (0x07),
sbPrlCopyright (0x08),
sbTypeUnknown  (0x0f),
sbTypeBool     (0x10),
sbTypeChar     (0x11),
sbTypeUChar    (0x12),
sbTypeSChar    (0x13),
sbTypeInt      (0x14),
sbTypeUInt     (0x15),
sbTypeSInt     (0x16),
sbTypeShInt    (0x17),
sbTypeUShInt   (0x18),
sbTypeSShInt   (0x19),
sbTypeLInt     (0x1a),
sbTypeULInt    (0x1b),
sbTypeSLInt    (0x1c),
sbTypeFloat    (0x1d),
sbTypeDouble   (0x1e),
sbTypeLDouble  (0x1f),
sbTypeString   (0x20),
sbTypeBChunk   (0x21);

DLLVAL const String
prlpkLabel         ("Label"),
prlpkCount         ("?"),
prlmkFunction      ("f"),
prlpkFncMin        ("m"),
prlpkFncMax        ("M"),
prlpkFncSmooth     ("s"),
prlpkFncPoint      ("p"),
prlpkReservedChars ("~#$%^&?!");

const String      // exceptions wording
prleBinaryChunkAV ("access violation in binaray chunk"),
prlePairNotFound  ("requested pair is not found"),
prleMapNotFound   ("requested map is not found"),
prleUnknownSBC    ("unknown status byte of pair, impossible copying"),
prleUnknownSBL    ("unknown status byte in input stream, loading has fault"),
prleUnknownSBS    ("unknown status byte of pair, saving has fault"),
prlePairTypeMiss  ("cannot get data because requested type is invalid"),
prleSaving        ("saving has fault, cannot create stream"),
prleLoading       ("loading has fault, cannot create stream"),
fnceBadBounds     ("lower bound of function is above than upper"),
fnceBadCount      ("number of function's significant points is invalid"),
fnceAV            ("access violation in method of class Function");

/***************************************************************************
 *  Definitions of Inline Functions
 **************************************************************************/

inline real __WithinPeriod(real Arg, real PeriodArg, real MinArg)
{
    Arg -= MinArg;
    Arg -= PeriodArg * (SDWORD)((Arg < 0.) ?
        (Arg / PeriodArg - 1.) : (Arg/ PeriodArg));
    return (Arg + MinArg);
}

inline double __SectionLength
    (real PeriodArg, numerator PointsCountArg, bool SmoothArg)
{
    if (PointsCountArg == 1) return PeriodArg;
    if (SmoothArg) return (PeriodArg / (PointsCountArg - 1));
    else return (PeriodArg / PointsCountArg);
}

/***************************************************************************
 *  Definitions of Functions
 **************************************************************************/

PLACEEXCEPTIONFNCHERE

String GetPRLDescription()
{
    return "Parameter Lists library ("__DATE__")\n"
           "Copyright (c) 2014 Mark Karpov";
}

String GetPRLVersion()
{
    return "4.2.0";
}

void ThrowPairTypeMiss()
{
    THROW_LE(prlePairTypeMiss);
}

void ThrowBinaryChunkAV()
{
    THROW_LE(prleBinaryChunkAV);
}

void SaveString(const String &StringArg, std::ofstream &S)
{
    S.put(sbTypeString);
    S << StringArg << std::ends;
}

void ParamlistAddFunction
(Paramlist *ThisArg, String MapKeyArg, String PairKeyArg, Function DataArg)
{
    String aMapKey = ThisArg->GenerateMapKey(prlmkFunction);
    real aPoint, aLastPoint = 0;
    numerator aPointsCount = DataArg.PointsCount();
    ThisArg->Add<real>(aMapKey, prlpkFncMin, DataArg.Min());
    ThisArg->Add<real>(aMapKey, prlpkFncMax, DataArg.Max());
    ThisArg->Add<bool>(aMapKey, prlpkFncSmooth, DataArg.Smooth());
    ThisArg->AddAsCount(aMapKey, prlpkFncPoint, aPointsCount);
    for (numerator i = 0; i < aPointsCount; i++)
    {
        aPoint = DataArg.PointValue(i);
        if (aPoint == aLastPoint) continue;
        ThisArg->Add(aMapKey, prlpkFncPoint, i, aPoint);
        aLastPoint = aPoint;
    }
    ThisArg->Add<String>(MapKeyArg, PairKeyArg, aMapKey);
}

Function ParamlistDataFunction
    (const Paramlist *ThisArg, String MapKeyArg, String PairKeyArg)
{
    if (!ThisArg->Exists(MapKeyArg)) THROW_RE(prleMapNotFound);
    Function aData(0., 0., 0.);
    real aLastPoint = 0.;
    String aMapKey = ThisArg->Data<String>(MapKeyArg, PairKeyArg);
    aData.Min(ThisArg->Data<real>(aMapKey, prlpkFncMin));
    aData.Max(ThisArg->Data<real>(aMapKey, prlpkFncMax));
    aData.Smooth(ThisArg->Data<bool>(aMapKey, prlpkFncSmooth));
    aData.PointsCount(ThisArg->DataAsCount(aMapKey, prlpkFncPoint));
    numerator aPointsCount = aData.PointsCount();
    for (numerator i = 0; i < aPointsCount;)
    {
        if (ThisArg->Exists(aMapKey, prlpkFncPoint, i))
        aLastPoint = ThisArg->Data<real>(aMapKey, prlpkFncPoint, i);
        aData.PointValue(aLastPoint, i++);
    }
    return aData;
}

/***************************************************************************
 *  Definitions of non-inline Methods
 **************************************************************************/

// class BinaryChunk

void BinaryChunk::CopyData(const BinaryChunk &Another)
{
    Clear();
    Size(Another.aSize);
    for (numerator i = 0; i < aSize; i++)
        *(aData + i) = *(Another.aData + i);
}

void BinaryChunk::Size(numerator SizeArg)
{
    if (SizeArg == aSize) return;
    if (aSize) delete [] aData;
    aSize = SizeArg;
    if (aSize) aData = new BYTE[aSize];
    else aData = NIL;
}

void BinaryChunk::Save(std::ofstream &StreamArg) const
{
    StreamArg.put(sbTypeBChunk);
    StreamArg.write((BYTE *)&aSize, sizeof(numerator));
    StreamArg.write(aData, aSize);
}

void BinaryChunk::Load(std::ifstream &StreamArg)
{
    numerator aNewSize;
    StreamArg.read((BYTE *)&aNewSize, sizeof(numerator));
    Size(aNewSize);
    StreamArg.read(aData, aSize);
}

// class Field

template <> BYTE Field<bool>::SB()               const { return sbTypeBool; }
template <> BYTE Field<char>::SB()               const { return sbTypeChar; }
template <> BYTE Field<unsigned char>::SB()      const { return sbTypeUChar; }
template <> BYTE Field<signed char>::SB()        const { return sbTypeSChar; }
template <> BYTE Field<unsigned int>::SB()       const { return sbTypeUInt; }
template <> BYTE Field<signed int>::SB()         const { return sbTypeSInt; }
template <> BYTE Field<unsigned short int>::SB() const { return sbTypeUShInt; }
template <> BYTE Field<signed short int>::SB()   const { return sbTypeSShInt; }
template <> BYTE Field<unsigned long int>::SB()  const { return sbTypeULInt; }
template <> BYTE Field<signed long int>::SB()    const { return sbTypeSLInt; }
template <> BYTE Field<float>::SB()              const { return sbTypeFloat; }
template <> BYTE Field<double>::SB()             const { return sbTypeDouble; }
template <> BYTE Field<long double>::SB()        const { return sbTypeLDouble; }
template <> BYTE Field<String>::SB()             const { return sbTypeString; }
template <> BYTE Field<BinaryChunk>::SB()        const { return sbTypeBChunk; }

// text images for every data type

template <> String Field<bool>::ShowData() const
{ return (Data() ? "true" : "false"); }
template <> String Field<char>::ShowData() const
{ String R; R += '\'', R += Data(), R += '\''; return R; }
template <> String Field<unsigned char>::ShowData() const
{ String R; R += '\'', R += Data(), R += "\'u"; return R; }
template <> String Field<signed char>::ShowData() const
{ String R; R += '\'', R += Data(), R += '\''; return R; }
template <> String Field<unsigned int>::ShowData() const
{ CHAR R[SMALLBUFFERSIZE]; std::itoa(Data(), R, NumericRadix);
  return String(R) + "u"; }
template <> String Field<signed int>::ShowData() const
{ CHAR R[SMALLBUFFERSIZE]; std::itoa(Data(), R, NumericRadix);
  return String(R); }
template <> String Field<unsigned short int>::ShowData() const
{ CHAR R[SMALLBUFFERSIZE]; std::itoa(Data(), R, NumericRadix);
  return String(R) + "us"; }
template <> String Field<signed short int>::ShowData() const
{ CHAR R[SMALLBUFFERSIZE]; std::itoa(Data(), R, NumericRadix);
  return String(R) + "s"; }
template <> String Field<unsigned long int>::ShowData() const
{ CHAR R[SMALLBUFFERSIZE]; std::ltoa(Data(), R, NumericRadix);
  return String(R) + "ul"; }
template <> String Field<signed long int>::ShowData() const
{ CHAR R[SMALLBUFFERSIZE]; std::ltoa(Data(), R, NumericRadix);
  return String(R) + "l"; }
template <> String Field<float>::ShowData() const
{ CHAR R[SMALLBUFFERSIZE]; std::gcvt(Data(), FloatNIN, R);
  return String(R) + "f"; }
template <> String Field<double>::ShowData() const
{ CHAR R[SMALLBUFFERSIZE]; std::gcvt(Data(), DoubleNIN, R);
  return String(R) + "d"; }
template <> String Field<long double>::ShowData() const
{ CHAR R[SMALLBUFFERSIZE]; std::gcvt(Data(), DoubleNIN, R);
  return String(R) + "ld"; }
template <> String Field<String>::ShowData() const
{ String R; R += '"', R += Data(), R += '"'; return R; }
template <> String Field<BinaryChunk>::ShowData() const
{ CHAR R[SMALLBUFFERSIZE]; std::itoa(Data().Size(), R, NumericRadix);
  return String(R) + "bBD"; }

// class Map

ConstFieldIter Map::Ask(String KeyArg) const
{
    ConstFieldIter aPretender = aFields.find(KeyArg);
    if (aPretender == aFields.end()) THROW_RE(prlePairNotFound);
    return aPretender;
}

FieldIter Map::Select(String KeyArg)
{
    FieldIter aPretender = aFields.find(KeyArg);
    if (aPretender == aFields.end()) THROW_RE(prlePairNotFound);
    return aPretender;
}

void Map::CopyFields(const FieldsStore &TargetArg)
{
    FieldsStore::const_iterator i = TargetArg.begin();
    while (i != TargetArg.end()) CopyOne(i++);
}

String Map::ShowFields() const
{
    String aResult;
    if (aFields.empty()) return aResult;
    FieldsStore::const_iterator i = aFields.begin();
    while (true)
    {
        aResult += "\t[" + i->first + "] -> " + i->second->ShowData();
        if (++i == aFields.end()) { aResult += ";\n"; break; }
        else aResult += ",\n";
    }
    return aResult;
}

#define __PSPOC(TYPE) \
aPretender = new Field<TYPE>(*dynamic_cast<Field<TYPE> *>(PretenderArg->second))

void Map::CopyOne(ConstFieldIter PretenderArg)
{
    FieldPointer aPretender = NIL;
    switch (PretenderArg->second->SB())
    {
    case sbTypeBool    : __PSPOC(bool);               break;
    case sbTypeChar    : __PSPOC(char);               break;
    case sbTypeUChar   : __PSPOC(unsigned char);      break;
    case sbTypeSChar   : __PSPOC(signed char);        break;
    case sbTypeInt     : __PSPOC(int);                break;
    case sbTypeUInt    : __PSPOC(unsigned int);       break;
    case sbTypeSInt    : __PSPOC(signed int);         break;
    case sbTypeShInt   : __PSPOC(short int);          break;
    case sbTypeUShInt  : __PSPOC(unsigned short int); break;
    case sbTypeSShInt  : __PSPOC(signed short int);   break;
    case sbTypeLInt    : __PSPOC(long int);           break;
    case sbTypeULInt   : __PSPOC(unsigned long int);  break;
    case sbTypeSLInt   : __PSPOC(signed long int);    break;
    case sbTypeFloat   : __PSPOC(float);              break;
    case sbTypeDouble  : __PSPOC(double);             break;
    case sbTypeLDouble : __PSPOC(long double);        break;
    case sbTypeString  : __PSPOC(String);             break;
    case sbTypeBChunk  : __PSPOC(BinaryChunk);        break;
    default : THROW_LE(prleUnknownSBC);
    } // switch
    if (Exists(PretenderArg->first))
    {
        FieldIter aField = Select(PretenderArg->first);
        delete aField->second;
        aField->second = aPretender;
    }
    else aFields.insert(FieldPair(PretenderArg->first, aPretender));
}

void Map::PairKey(String KeyArg, String NewKeyArg)
{
    if (!Exists(KeyArg)) THROW_RE(prlePairNotFound);
    if (Exists(NewKeyArg)) Remove(NewKeyArg);
    FieldPointer aTarget = Select(KeyArg)->second;
    aFields.erase(Select(KeyArg));
    aFields.insert(FieldPair(NewKeyArg, aTarget));
}

void Map::Fuse(const Map &Another)
{
    FieldsStore aTarget = Another.aFields;
    FieldIter i = aTarget.begin();
    while (i != aTarget.end()) CopyOne(i++);
}

void Map::Remove(String KeyArg)
{
    if (!Exists(KeyArg)) THROW_RE(prlePairNotFound);
    FieldIter aTarget = Select(KeyArg);
    delete aTarget->second;
    aFields.erase(aTarget);
}

void Map::Clear()
{
    if (aFields.empty()) return;
    FieldIter i = aFields.begin();
    while (i != aFields.end()) delete i++->second;
    aFields.clear();
}

bool Map::Exists(String KeyArg) const
{
    return (aFields.find(KeyArg) != aFields.end());
}

void Map::Save(std::ofstream &StreamArg) const
{
    ConstFieldIter i = aFields.begin();
    while (i != aFields.end())
    {
        StreamArg.put(sbPairKey);
        StreamArg << i->first << std::ends;
        i++->second->Save(StreamArg);
    }
}

//class Paramlist

ConstMapIter Paramlist::Ask(String KeyArg) const
{
    ConstMapIter aPretender = aMaps.find(KeyArg);
    if (aPretender == aMaps.end()) THROW_RE(prlePairNotFound);
    return aPretender;
}

MapIter Paramlist::Select(String KeyArg)
{
    MapIter aPretender = aMaps.find(KeyArg);
    if (aPretender == aMaps.end()) THROW_RE(prlePairNotFound);
    return aPretender;
}

void Paramlist::PairKey(String MapKeyArg, String PairKeyArg, String NewKeyArg)
{
    if (!Exists(MapKeyArg, PairKeyArg)) THROW_RE(prlePairNotFound);
    Select(MapKeyArg)->second.PairKey(PairKeyArg, NewKeyArg);
}

void Paramlist::CopyPair(String MapKeyArg, String PairKeyArg, String NewKeyArg)
{
    if (!Exists(MapKeyArg, PairKeyArg)) THROW_RE(prlePairNotFound);
    if (MapKeyArg == NewKeyArg) return;
    if (!Exists(NewKeyArg)) aMaps.insert(MapPair(NewKeyArg, Map()));
    Select(NewKeyArg)->second.CopyOne
        (Select(MapKeyArg)->second.Get(PairKeyArg));
}

void Paramlist::SendPair(String MapKeyArg, String PairKeyArg, String NewKeyArg)
{
    if (!Exists(MapKeyArg, PairKeyArg)) THROW_RE(prlePairNotFound);
    if (MapKeyArg == NewKeyArg) return;
    if (!Exists(NewKeyArg)) aMaps.insert(MapPair(NewKeyArg, Map()));
    Select(NewKeyArg)->second.CopyOne
        (Select(MapKeyArg)->second.Get(PairKeyArg));
    Remove(MapKeyArg, PairKeyArg);
}

void Paramlist::Remove(String MapKeyArg, String PairKeyArg)
{
    if (!Exists(MapKeyArg, PairKeyArg)) THROW_RE(prlePairNotFound);
    Select(MapKeyArg)->second.Remove(PairKeyArg);
    if (Ask(MapKeyArg)->second.Empty()) aMaps.erase(Select(MapKeyArg));
}

void Paramlist::MapKey(String MapKeyArg, String NewKeyArg)
{
    if (!Exists(MapKeyArg)) THROW_RE(prleMapNotFound);
    if (Exists(NewKeyArg))
    {
        Select(NewKeyArg)->second.Fuse(Select(MapKeyArg)->second);
        Clear(MapKeyArg);
    }
    else
    {
        Map aMap = Select(MapKeyArg)->second;
        aMaps.erase(Select(MapKeyArg));
        aMaps.insert(MapPair(NewKeyArg, aMap));
    }
}

void Paramlist::DuplicateMap(String MapKeyArg, String NewKeyArg)
{
    if (!Exists(MapKeyArg)) THROW_RE(prleMapNotFound);
    if (MapKeyArg == NewKeyArg) return;
    if (!Exists(NewKeyArg)) aMaps.insert(MapPair(NewKeyArg, Map()));
    Select(NewKeyArg)->second.Fuse(Ask(MapKeyArg)->second);
}

void Paramlist::Clear(String MapKeyArg)
{
    if (!Exists(MapKeyArg)) THROW_RE(prleMapNotFound);
    aMaps.erase(Select(MapKeyArg));
}

void Paramlist::Clear()
{
    aMaps.clear();
}

void Paramlist::Fuse(const Paramlist &Another)
{
    MapsStore aTarget = Another.aMaps;
    for (MapsStore::iterator i = aTarget.begin(); i != aTarget.end(); i++)
    {
        if (!Exists(i->first)) aMaps.insert(MapPair(i->first, Map()));
        Select(i->first)->second.Fuse(i->second);
    }
}

void Paramlist::AddAsCount
    (String MapKeyArg, String PairKeyArg, numerator IndexArg)
{
    Add<numerator>(MapKeyArg, PairKeyArg + prlpkCount, IndexArg);
}

numerator Paramlist::DataAsCount(String MapKeyArg, String PairKeyArg) const
{
    return Data<numerator>(MapKeyArg, PairKeyArg + prlpkCount);
}

numerator Paramlist::PairsCount(String MapKeyArg) const
{
    if (!Exists(MapKeyArg)) THROW_RE(prleMapNotFound);
    return Ask(MapKeyArg)->second.PairsCount();
}

numerator Paramlist::PairsCount() const
{
    numerator aCount = 0;
    MapsStore::const_iterator i = aMaps.begin();
    while (i != aMaps.end()) aCount += i++->second.PairsCount();
    return aCount;
}

bool Paramlist::Exists(String MapKeyArg) const
{
    return (aMaps.find(MapKeyArg) != aMaps.end());
}

bool Paramlist::Exists(String MapKeyArg, numerator IndexArg) const
{
    CHAR aIndexImage[SMALLBUFFERSIZE];
    std::itoa(IndexArg, aIndexImage, IndexationRadix);
    return Exists(MapKeyArg + aIndexImage);
}

bool Paramlist::Exists(String MapKeyArg, String PairKeyArg) const
{
    if (!Exists(MapKeyArg)) return false;
    return Ask(MapKeyArg)->second.Exists(PairKeyArg);
}

bool Paramlist::Exists
    (String MapKeyArg, String PairKeyArg, numerator IndexArg) const
{
    CHAR aIndexImage[SMALLBUFFERSIZE];
    std::itoa(IndexArg, aIndexImage, IndexationRadix);
    return Exists(MapKeyArg, PairKeyArg + aIndexImage);
}

String Paramlist::GenerateMapKey(String BaseKeyArg) const
{
    CHAR aIndexImage[SMALLBUFFERSIZE];
    do
    {
        std::itoa(std::rand(), aIndexImage, IndexationRadix);
        Exists(BaseKeyArg + aIndexImage);
    }
    while (Exists(BaseKeyArg + aIndexImage));
    return (BaseKeyArg + aIndexImage);
}

String Paramlist::AsText() const
{
    String aResult;
    aResult += "PRL " + GetPRLVersion() + " [" + Name() + "]\n";
    if (AuthorExists()) aResult += "\tby " + Author() + "\n";
    aResult += "{\n";
    MapsStore::const_iterator i = aMaps.begin();
    while (i != aMaps.end())
    {
        aResult += "\t[" + i->first + "] ~\n";
        aResult += i++->second.AsText();
    }
    aResult += "}.\n\n";
    if (CommentExists()) aResult += Comment() + "\n";
    if (CopyrightExists()) aResult += Copyright() + "\n";
    return aResult;
}

bool Paramlist::Save(String FileNameArg) const
{
    std::ofstream aFileOut // try to open stream
        (FileNameArg.c_str(), std::ios::binary | std::ios::out);
    if (!aFileOut) THROW_RE(prleSaving);
    // now we have to save all info:
    aFileOut.put(sbPrlName), aFileOut << Name() << std::ends;
    aFileOut.put(sbPrlAuthor), aFileOut << Author() << std::ends;
    aFileOut.put(sbPrlComment), aFileOut << Comment() << std::ends;
    aFileOut.put(sbPrlCopyright), aFileOut << Copyright() << std::ends;
    // save all data as well
    MapsStore::const_iterator i = aMaps.begin();
    while (i != aMaps.end())
    {
        aFileOut.put(sbMapKey);
        aFileOut << i->first << std::ends;
        i++->second.Save(aFileOut);
    }
    aFileOut.put(sbTerminator); // end of file
    aFileOut.close();
    return !aFileOut.good();
}

#define __PSPOL(TYPE) \
{ \
    TYPE E; \
    aFileIn.read((BYTE *)&E, sizeof(TYPE)); \
    Add(tMapKey, tPairKey, E); \
}

bool Paramlist::Load(String FileNameArg)
{
    std::ifstream aFileIn
        (FileNameArg.c_str(), std::ios::binary | std::ios::in);
    if (!aFileIn) THROW_RE(prleLoading); else Clear();
    CHAR
    tPrlName[MEDIUMBUFFERSIZE] = { NIL },
    tPrlAuthor[MEDIUMBUFFERSIZE] = { NIL },
    tPrlComment[HUGEBUFFERSIZE] = { NIL },
    tPrlCopyright[MEDIUMBUFFERSIZE] = { NIL },
    tMapKey[MEDIUMBUFFERSIZE],
    tPairKey[MEDIUMBUFFERSIZE],
    tData[HUGEBUFFERSIZE];
    register CHAR asb = NIL;
    while ((aFileIn.get(asb)) && (asb != sbTerminator))
    switch (asb)
    {
    case sbNil : break;
    case sbPrlName      :
        aFileIn.getline(tPrlName, MEDIUMBUFFERSIZE, NIL);      break;
    case sbPrlAuthor    :
        aFileIn.getline(tPrlAuthor, MEDIUMBUFFERSIZE, NIL);    break;
    case sbPrlComment   :
        aFileIn.getline(tPrlComment, HUGEBUFFERSIZE, NIL);     break;
    case sbPrlCopyright :
        aFileIn.getline(tPrlCopyright, MEDIUMBUFFERSIZE, NIL); break;
    case sbMapKey       :
        aFileIn.getline(tMapKey, MEDIUMBUFFERSIZE, NIL);       break;
    case sbPairKey      :
        aFileIn.getline(tPairKey, MEDIUMBUFFERSIZE, NIL);      break;
    case sbTypeBool     : __PSPOL(bool);                       break;
    case sbTypeChar     : __PSPOL(char);                       break;
    case sbTypeUChar    : __PSPOL(unsigned char);              break;
    case sbTypeSChar    : __PSPOL(signed char);                break;
    case sbTypeInt      : __PSPOL(int);                        break;
    case sbTypeUInt     : __PSPOL(unsigned int);               break;
    case sbTypeSInt     : __PSPOL(signed int);                 break;
    case sbTypeShInt    : __PSPOL(short int);                  break;
    case sbTypeUShInt   : __PSPOL(unsigned short int);         break;
    case sbTypeSShInt   : __PSPOL(signed short int);           break;
    case sbTypeLInt     : __PSPOL(long int);                   break;
    case sbTypeULInt    : __PSPOL(unsigned long int);          break;
    case sbTypeSLInt    : __PSPOL(signed long int);            break;
    case sbTypeFloat    : __PSPOL(float);                      break;
    case sbTypeDouble   : __PSPOL(double);                     break;
    case sbTypeLDouble  : __PSPOL(long double);                break;
    case sbTypeString   :
    {
        aFileIn.getline(tData, HUGEBUFFERSIZE, NIL);
        Add(tMapKey, tPairKey, String(tData));
    }                                                          break;
    case sbTypeBChunk  :
    {
        BinaryChunk aBinaryChunk;
        aBinaryChunk.Load(aFileIn);
        Add(tMapKey, tPairKey, aBinaryChunk);
    }                                                          break;
    default : THROW_RE(prleUnknownSBL);
    } // while
    Name(tPrlName);
    Author(tPrlAuthor);
    Comment(tPrlComment);
    Copyright(tPrlCopyright);
    aFileIn.close();
    return !aFileIn.good();
}

// class LabelledEssence

String LabelledEssence::UseFilter(String LabelArg) const
{
    for (String::const_iterator i = prlpkReservedChars.begin();
        i != prlpkReservedChars.end(); i++)
    {
        numerator aBadChar;
        while ((aBadChar = LabelArg.find(*i)) != String::npos)
            LabelArg.erase(LabelArg.begin() + aBadChar);
    }
    return LabelArg;
}

void LabelledEssence::WriteLabelTo
    (Paramlist &TargetArg, String MapKeyArg) const
{
    if (aLabel != aDefaultLabel)
        TargetArg.Add<String>(MapKeyArg, prlpkLabel, aLabel);
}

void LabelledEssence::ReadLabelFrom(Paramlist SourceArg, String MapKeyArg)
{
    if (SourceArg.Exists(MapKeyArg, prlpkLabel))
        aLabel = SourceArg.Data<String>(MapKeyArg, prlpkLabel);
    else aLabel = aDefaultLabel;
}

String LabelledEssence::GenerateUniqueMapKey(Paramlist SourceArg) const
{
    String aResult;
    if (aLabel != aDefaultLabel) aResult =
    (SourceArg.Exists(aLabel)) ? SourceArg.GenerateMapKey(aLabel) : aLabel;
    else aResult = SourceArg.GenerateMapKey(aDefaultLabel);
    return aResult;
}

// class Function

Function::Function
    (real MinArg, real MaxArg, real PointValueArg, numerator CountArg) :
    aMin(MinArg),
    aMax(MaxArg),
    aSmooth(false),
    aPointsCount(1)
{
    aPoints = new real(PointValueArg);
    PointsCount(CountArg);
}

Function::Function(const Function &Another) :
    aMin(Another.aMin),
    aMax(Another.aMax),
    aSmooth(Another.aSmooth),
    aPointsCount(Another.aPointsCount)
{
    aPoints = new real[aPointsCount];
    for (numerator i = 0; i < aPointsCount; i++)
        *(aPoints + i) = *(Another.aPoints + i);
}

Function Function::operator = (const Function &Another)
{
    aMin = Another.aMin;
    aMax = Another.aMax;
    aSmooth = Another.aSmooth;
    aPointsCount = Another.aPointsCount;
    delete [] aPoints;
    aPoints = new real[aPointsCount];
    for (numerator i = 0; i < aPointsCount; i++)
        *(aPoints + i) = *(Another.aPoints + i);
    return *this;
}

void Function::PointsCount(numerator CountArg)
{
    if (!CountArg) THROW_LE(fnceBadCount); // zero is invalid
    if (CountArg == aPointsCount) return; // we have nothing to do
    PointsStore Temp = new real[CountArg]; // make store for new collection
    // calculating new section length:
    real aPeriod = aMax - aMin;
    real aSection = __SectionLength(aPeriod, CountArg, Smooth());
    // now just recalculate all the points, we're trying to save the shape
    for (register numerator i = 0; i < CountArg; i++)
        *(Temp + i) = operator()(aMin + i * aSection);
    if (aSmooth) *(Temp + CountArg - 1) = *(aPoints + aPointsCount - 1);
    delete [] aPoints;
    aPoints = Temp;
    aPointsCount = CountArg;
}

real Function::SectionLength() const
{
    return __SectionLength((aMax - aMin), PointsCount(), Smooth());
}

void Function::PointValue(real ValueArg, numerator PositionArg)
{
    if (PositionArg < aPointsCount) *(aPoints + PositionArg) = ValueArg;
    else THROW_LE(fnceAV);
}

real Function::PointValue(numerator PositionArg) const
{
    if (PositionArg < aPointsCount) return *(aPoints + PositionArg);
    else THROW_LE(fnceAV);
}

real Function::operator()(real Arg) const
{
    real aPeriod = aMax - aMin; // ok, we have to calculate it in advance
    if (aPeriod < 0) THROW_LE(fnceBadBounds); // bad..
    if ((PointsCount() == 1) || !aPeriod) return *aPoints; // special
    // here we often use macros because this code must be as fast as devil
    Arg = __WithinPeriod(Arg, aPeriod, aMin); // make Arg within period
    real aSection = __SectionLength(aPeriod, aPointsCount, aSmooth);
    // left value (it is begin for smooth fnc and all for step fnc)
    numerator   LeftIndex = (Arg - aMin) / aSection;
    real        LeftValue = *(aPoints + LeftIndex);
    if (!aSmooth) return LeftValue;
    // ..and this code only for smooth function:
    numerator   RightIndex = LeftIndex + 1;
    real        RightValue = *(aPoints + RightIndex);
    real aResult = (RightValue - LeftValue) / aSection;
    aResult *= Arg - /* left arg */ (aMin + LeftIndex * aSection);
    aResult += LeftValue;
    return aResult;
}

ENDIONSTD
