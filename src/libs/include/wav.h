/***************************************************************************
 *  C/C++ header
 *
 *  WAVE soundfile format library
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

#ifndef WAVESTRUCTURE
#define WAVESTRUCTURE

#ifndef __cplusplus
#error This library use C++ syntax, you cannot use it in C programs.
#endif // __cplusplus

#include <cmn.h> // common header

BEGINIONSTD

/***************************************************************************
 *  Constants
 **************************************************************************/

const numerator
ChunkIdSize         (4),
ChunkSizeSize       (4),
RiffTypeSize        (4),
WaveFmtSize         (16),
WaveFmtESize        (18),
RiffCriticalSize    (36),
RiffCriticalESize   (38);

const WORD
WaveFormatPCM       (0x0001),
WaveFormatIEEEFloat (0x0003),
WaveFormatMULAW     (0x0101),
WaveFormatALAW      (0x0102),
WaveFormatADPCM     (0x0103);

const numerator
mono                (1),
stereo              (2);

/***************************************************************************
 *  Forward Declarations
 **************************************************************************/

class leChunk;
class WaveStruct;

/***************************************************************************
 *  Type Definitions
 **************************************************************************/

struct WAVEFORMAT
{
    WORD    wFormatTag;     // format type
    WORD    nChannels;      // number of channels (i.e. mono, stereo..)
    DWORD   nSamplesPerSec; // sample rate
    DWORD   nAvgBytesPerSec;// buffer estimation
    WORD    nBlockAlign;    // block size
    WORD    wBitsPerSample; // number of bits per mono sample
    WORD    cbSize;         // length in bytes of extra information
}; // struct WAVEFORMAT

typedef float32                       sample;
typedef real                          time;
typedef BYTE *                        Buffer;
typedef sample *                      MediaBuffer;
typedef const MediaBuffer             ConstMediaBuffer;
typedef std::vector<leChunk>          leChunksStore;
typedef leChunksStore::iterator       leChunkIter;
typedef leChunksStore::const_iterator leChunkConstIter;
typedef std::vector<real>             PointStore;

struct WAVEDATA
{
    MediaBuffer Floor;
    numerator Size;
}; // struct WAVEDATA

/***************************************************************************
 *  Definitions of inline Functions
 **************************************************************************/

inline numerator __MultiByteLack(numerator Arg)
{
    return ((Arg % 8) ? (8 - (Arg % 8)) : 0);
}

inline numerator __MultiByteRound(numerator Arg)
{
    return (Arg + __MultiByteLack(Arg)) >> 3;
}

inline sample __Abs(sample Arg)
{
    return ((Arg < 0) ? -Arg : +Arg);
}

inline real __DbToFactor(real Arg)
{
    return (Arg ? std::pow(10., Arg / 10.) : 1.);
}

/***************************************************************************
 *  Declarations of Functions
 **************************************************************************/

DLLFNC String GetWAVDescription();
DLLFNC String GetWAVVersion();

/***************************************************************************
 *  Definitions of Classes
 **************************************************************************/

class leChunk
{
    private:
    BYTE aId[ChunkIdSize];
    numerator aSize;
    Buffer aData;

    public:
    DLLFNC leChunk(String, numerator);
    DLLFNC leChunk(std::ifstream &);
    DLLFNC leChunk(const leChunk &);
    DLLFNC leChunk operator = (const leChunk &);
    DLLFNC ~leChunk();

    // interface:

    String Id() const { return String(aId, ChunkIdSize); }
    numerator Size() const { return aSize; }
    Buffer Indexate(numerator Arg) { return (aData + Arg); }
    DLLFNC numerator TotalSize() const;

    DLLFNC void WriteTo(std::ofstream &) const;

}; // class LittleEndianChunk

class WaveStruct
{
    private:
    WAVEFORMAT aFmt;
    WAVEDATA aData;
    leChunksStore aUnknownChunks; // support of unknown chunks

    public:
    DLLFNC WaveStruct(DWORD, WORD, WORD, WORD = WaveFormatPCM);
    DLLFNC WaveStruct(String, time = 0, time = 0);
    DLLFNC WaveStruct(const WaveStruct &);
    DLLFNC WaveStruct operator = (const WaveStruct &);
    DLLFNC ~WaveStruct();

    // interface:
    // general export/import functions:

    DLLFNC bool Save(String) const;
    DLLFNC bool Load(String, time = 0, time = 0);
    DLLFNC ConstMediaBuffer GetData() const;
    DLLFNC leChunkIter GetUnknownChunk(numerator);
    DLLFNC void PushBackChunk(leChunk);
    DLLFNC void DeleteUnknownChunk(numerator);
    DLLFNC void ClearUnknownChunks();
    numerator UnknownChunksCount() const { return aUnknownChunks.size(); }

    // wave transform-functions

    DLLFNC void Format(WORD);
    DLLFNC void Gain(real);
    DLLFNC void Normalize();
    DLLFNC void SampleRate(DWORD);
    DLLFNC void BitsPerSample(WORD);
    DLLFNC void Channels(WORD);
    DLLFNC void Dither();

    // information:

    WORD Format() const { return aFmt.wFormatTag; }
    WORD Channels() const { return aFmt.nChannels; }
    WORD BlockAlign() const { return aFmt.nBlockAlign; }
    WORD BitsPerSample() const { return aFmt.wBitsPerSample; }
    DWORD SampleRate() const { return aFmt.nSamplesPerSec; }
    DWORD AvgBytesPerSec()const { return aFmt.nAvgBytesPerSec; }
    DWORD Bitrate() const { return aFmt.nAvgBytesPerSec / 125; }
    numerator SamplesInData() const { return (aData.Size / aFmt.nChannels); }
    time Duration() const { return (time)SamplesInData() / aFmt.nSamplesPerSec; }
    DLLFNC numerator BytesInData() const;
    DLLFNC numerator FileSize() const;
    DLLFNC sample Peak() const;
    DLLFNC real NormalGain() const;

    // data modification:

    DLLFNC void SamplesInData(numerator, sample = 0 /* default sample value */);
    void ClearData() { SamplesInData(0); /* for compatibility with old ver */ }
    void Sample(sample SampleArg, numerator PositionArg)
    { *(aData.Floor + PositionArg) = SampleArg; }
    sample Sample(numerator PositionArg) const
    { return *(aData.Floor + PositionArg); }
    void Superposition(sample SampleArg, numerator PositionArg)
    { *(aData.Floor + PositionArg) += SampleArg; }

}; // class WaveStruct

ENDIONSTD

#endif // WAVESTRUCTURE
