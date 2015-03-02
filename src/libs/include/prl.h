/***************************************************************************
 *  C/C++ header
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

#ifndef PARAMETERLISTS
#define PARAMETERLISTS

#ifndef __cplusplus
#error This library use C++ syntax, you cannot use it in C programs.
#endif // __cplusplus

#include <cmn.h> // common header

BEGINIONSTD

/***************************************************************************
 *  Constants
 **************************************************************************/

const numerator
FloatNIN         (6),
DoubleNIN       (10),
NumericRadix    (10),
IndexationRadix (16);

/***************************************************************************
 *  Forward Declarations
 **************************************************************************/

class BinaryChunk;
class Info;
class BasicField;
template <typename> class Field;
class Map;
class Paramlist;
class LabelledEssence;
class Function;

/***************************************************************************
 *  Type Definitions
 **************************************************************************/

typedef BasicField *                     FieldPointer;
typedef std::map<String, FieldPointer>   FieldsStore;
typedef std::pair<String, FieldPointer>  FieldPair;
typedef FieldsStore::iterator            FieldIter;
typedef FieldsStore::const_iterator      ConstFieldIter;
typedef real *                           PointsStore;

/***************************************************************************
 *  Declarations of Functions
 **************************************************************************/

DLLFNC String   GetPRLDescription();
DLLFNC String   GetPRLVersion();
DLLFNC String   PairAsText(const PairPointer);
DLLFNC void     ThrowPairTypeMiss();
DLLFNC void     ThrowBinaryChunkAV();
DLLFNC void     SaveString(const String &, std::ofstream &);
DLLFNC void     ParamlistAddFunction(Paramlist *, String, String, Function);
DLLFNC Function ParamlistDataFunction(const Paramlist *, String, String);

/***************************************************************************
 *  Definitions of Classes
 **************************************************************************/

class BinaryChunk
{
    private:
    numerator aSize;
    BYTE *aData;

    DLLFNC void CopyData(const BinaryChunk &);

    public:
    BinaryChunk() : aSize(0), aData(NIL) { }
    BinaryChunk(const BinaryChunk &Another) : aSize(0), aData(NIL)
    { CopyData(Another); }
    BinaryChunk operator = (const BinaryChunk &Another)
    { CopyData(Another); return *this; }
    ~BinaryChunk() { Clear(); }

    // interface:

    DLLFNC void Size(numerator);
    numerator Size() const { return aSize; }
    void Clear() { Size(0); }
    template <typename data> void SetData(data, numerator);
    template <typename data> data GetData(numerator) const;

    DLLFNC void Save(std::ofstream &) const;
    DLLFNC void Load(std::ifstream &);

}; // class BinaryChunk

class Info
{
    private:
    String aName, aAuthor, aComment, aCopyright;

    public:
    Info (String Arg = String()) : aName(Arg) { }
    Info (const Info &Another) :
        aName      (Another.aName),
        aAuthor    (Another.aAuthor),
        aComment   (Another.aComment),
        aCopyright (Another.aCopyright)
    { }
    Info operator = (const Info &Another)
    {
        aName      = Another.aName;
        aAuthor    = Another.aAuthor;
        aComment   = Another.aComment;
        aCopyright = Another.aCopyright;
        return *this;
    }

    // interface:

    void   Name(String Arg) { aName = Arg; }
    String Name() const { return aName; }
    void   ClearName() { aName.clear(); }
    bool   NameExists() const { return !aName.empty(); }
    void   Author(String Arg) { aAuthor = Arg; }
    String Author() const { return aAuthor; }
    void   ClearAuthor() { aAuthor.clear(); }
    bool   AuthorExists() const { return !aAuthor.empty(); }
    void   Comment(String Arg) { aComment = Arg; }
    String Comment() const { return aComment; }
    void   ClearComment() { aComment.clear(); }
    bool   CommentExists() const { return !aComment.empty(); }
    void   Copyright(String Arg) { aCopyright = Arg; }
    String Copyright() const { return aCopyright; }
    void   ClearCopyright() { aCopyright.clear(); }
    bool   CopyrightExists() const { return !aCopyright.empty(); }

}; // class Info

class BasicField
{
    public:
    BasicField() { }
    virtual ~BasicField() { }

    // interface:

    virtual BYTE SB() const = 0; // returns unique byte for every datatype
    virtual String ShowData() const = 0;
    virtual void Save(std::ofstream &) const = 0;

}; // class BasicField

template <typename data> class Field : public BasicField
{
    private:
    data aData;

    public:
    Field(data Arg) : aData(Arg) { }
    ~Field() { }

    // interface:

    void Data(data Arg) { aData = Arg; }
    data Data() const { return aData; }
    DLLFNC BYTE SB() const;
    DLLFNC String ShowData() const;
    void Save(std::ofstream &) const;

}; // template class Field

class Map
{
    private:
    FieldsStore aFields;

    DLLFNC ConstFieldIter Ask(String) const;
    DLLFNC FieldIter Select(String);
    DLLFNC void CopyFields(const FieldsStore &);

    protected:
    DLLFNC String ShowFields() const;

    public:
    Map() { }
    Map(const Map &Another) { CopyFields(Another.aFields); }
    Map operator = (const Map &Another)
    { Clear(); CopyFields(Another.aFields); return *this; }
    virtual ~Map() { Clear(); }

    // interface:

    template <typename data> void Add(String, data);
    template <typename data> data Data(String) const;
    DLLFNC void CopyOne(ConstFieldIter);
    DLLFNC void PairKey(String, String);
    DLLFNC void Fuse(const Map &);
    DLLFNC void Remove(String);
    DLLFNC void Clear();

    ConstFieldIter Get(String KeyArg) { return Ask(KeyArg); }
    numerator PairsCount() const { return aFields.size(); }
    bool Empty() const { return aFields.empty(); }
    DLLFNC bool Exists(String) const;

    virtual String AsText() const { return ShowFields(); }
    DLLFNC void Save(std::ofstream &) const;

}; // class Map

// additional type definitions that depended on class Map

typedef std::map<String, Map>     MapsStore;
typedef std::pair<String, Map>    MapPair;
typedef MapsStore::iterator       MapIter;
typedef MapsStore::const_iterator ConstMapIter;

class Paramlist : public Info
{
    private:
    MapsStore aMaps;

    DLLFNC ConstMapIter Ask(String) const;
    DLLFNC MapIter Select(String);

    public:
    Paramlist(String Arg = String()) : Info (Arg) { }
    Paramlist(const Paramlist &Another) :
        Info(Another), aMaps(Another.aMaps) { }
    Paramlist operator = (const Paramlist &Another)
    {
        Info::operator = (Another);
        aMaps = Another.aMaps;
        return *this;
    }
    virtual ~Paramlist() { Clear(); }

    // interface:

    template <typename data> void Add(String, String, data);
    template <typename data> data Data(String, String) const;
    template <typename data> void Add(String, String, numerator, data);
    template <typename data> data Data(String, String, numerator) const;

    DLLFNC void PairKey(String, String, String);
    DLLFNC void CopyPair(String, String, String);
    DLLFNC void SendPair(String, String, String);
    DLLFNC void Remove(String, String);
    DLLFNC void MapKey(String, String);
    DLLFNC void DuplicateMap(String, String);
    DLLFNC void Clear(String);
    DLLFNC void Clear();
    DLLFNC void Fuse(const Paramlist &);
    DLLFNC void AddAsCount(String, String, numerator);
    DLLFNC numerator DataAsCount(String, String) const;
    numerator MapsCount() const { return aMaps.size (); }
    DLLFNC numerator PairsCount(String) const;
    DLLFNC numerator PairsCount() const;
    bool Empty() const { return aMaps.empty (); }
    DLLFNC bool Empty(String) const;
    DLLFNC bool Exists(String) const;
    DLLFNC bool Exists(String, numerator) const;
    DLLFNC bool Exists(String, String) const;
    DLLFNC bool Exists(String, String, numerator) const;

    DLLFNC String GenerateMapKey(String) const;
    DLLFNC String AsText() const;

    DLLFNC bool Save(String) const;
    DLLFNC bool Load(String);

}; // class Paramlist

class LabelledEssence
{
    private:
    const String aDefaultLabel;
    String aLabel;

    protected:
    DLLFNC String UseFilter(String) const;
    DLLFNC void WriteLabelTo(Paramlist &, String) const;
    DLLFNC void ReadLabelFrom(Paramlist, String);
    DLLFNC String GenerateUniqueMapKey(Paramlist) const;

    public:
    LabelledEssence(String LabelArg) :
        aDefaultLabel(UseFilter(LabelArg)),
        aLabel(UseFilter(LabelArg))
    { }
    LabelledEssence(const LabelledEssence &Another) :
        aDefaultLabel(Another.aDefaultLabel),
        aLabel(Another.aLabel)
    { }
    LabelledEssence operator = (const LabelledEssence &Another)
    {
        aLabel = Another.aLabel;
        return *this;
    }

    // interface:

    void Label(String Arg) { aLabel = UseFilter(Arg); }
    String Label() const { return aLabel; }

}; // class LabelledEssence

class Function
{
    private:
    real aMin, aMax;
    bool aSmooth;
    PointsStore aPoints;
    numerator aPointsCount;

    public:
    DLLFNC Function(real, real, real, numerator = 1);
    DLLFNC Function(const Function &);
    DLLFNC Function operator = (const Function &);
    virtual ~Function() { delete [] aPoints; }

    // interface:

    void Min(real Arg) { aMin = Arg; }
    real Min() const { return aMin; }
    void Max(real Arg) { aMax = Arg; }
    real Max() const { return aMax; }
    void Smooth(bool Arg) { aSmooth = Arg; }
    bool Smooth() const { return aSmooth; }
    DLLFNC void PointsCount(numerator);
    numerator PointsCount() const { return aPointsCount; }
    DLLFNC real SectionLength() const;
    DLLFNC void PointValue(real, numerator);
    DLLFNC real PointValue(numerator) const;
    DLLFNC real operator() (real) const;

}; // class Function

/***************************************************************************
 *  Templates
 **************************************************************************/

// class BinaryChunk

template <typename data> void BinaryChunk::SetData
    (data DataArg, numerator PositionArg)
{
    if (PositionArg >= aSize / sizeof(data)) ThrowBinaryChunkAV();
    *((data *)aData + PositionArg) = DataArg;
}

template <typename data> data BinaryChunk::GetData
    (numerator PositionArg) const
{
    if (PositionArg >= aSize / sizeof(data)) ThrowBinaryChunkAV();
    return *((data *)aData + PositionArg);
}

// class Field

template <typename data> void Field<data>::Save(std::ofstream &S) const
{
    S.put(SB());
    data E = Data();
    S.write((BYTE *)&E, sizeof (data));
}

template <> inline void Field<String>::Save(std::ofstream &S) const
{
    SaveString (Data(), S);
}

template <> inline void Field<BinaryChunk>::Save(std::ofstream &S) const
{
    Data().Save(S);
}

// class Map

template <typename data> void Map::Add(String KeyArg, data DataArg)
{
    FieldPointer aPretender = new Field<data>(DataArg);
    if (Exists(KeyArg))
    {
        FieldIter aField = Select(KeyArg);
        delete aField->second;
        aField->second = aPretender;
    }
    else aFields.insert(FieldPair(KeyArg, aPretender));
}

template <typename data> data Map::Data(String KeyArg) const
{
    Field<data> *aPretender =
        dynamic_cast<Field<data> *>(Ask(KeyArg)->second);
    if (!aPretender) ThrowPairTypeMiss();
    return aPretender->Data();
}

// class Paramlist

template <typename data> void Paramlist::Add
    (String MapKeyArg, String PairKeyArg, data DataArg)
{
    if (!Exists(MapKeyArg)) aMaps.insert(MapPair(MapKeyArg, Map()));
    Select(MapKeyArg)->second.Add(PairKeyArg, DataArg);
}

template <typename data> inline data Paramlist::Data
    (String MapKeyArg, String PairKeyArg) const
{
    return Ask(MapKeyArg)->second.Data<data>(PairKeyArg);
}

template <typename data> inline void Paramlist::Add
    (String MapKeyArg, String PairKeyArg, numerator IndexArg, data DataArg)
{
    CHAR aIndexImage[SMALLBUFFERSIZE];
    std::itoa(IndexArg, aIndexImage, IndexationRadix);
    Add<data>(MapKeyArg, PairKeyArg + aIndexImage, DataArg);
}

template <typename data> inline data Paramlist::Data
    (String MapKeyArg, String PairKeyArg, numerator IndexArg) const
{
    CHAR aIndexImage[SMALLBUFFERSIZE];
    std::itoa(IndexArg, aIndexImage, IndexationRadix);
    return Data<data>(MapKeyArg, PairKeyArg + aIndexImage);
}

template <> inline void Paramlist::Add<Function>
    (String MapKeyArg, String PairKeyArg, Function DataArg)
{
    ParamlistAddFunction(this, MapKeyArg, PairKeyArg, DataArg);
}

template <> inline Function Paramlist::Data<Function>
    (String MapKeyArg, String PairKeyArg) const
{
    return ParamlistDataFunction(this, MapKeyArg, PairKeyArg);
}

ENDIONSTD

#endif // PARAMETERLISTS
