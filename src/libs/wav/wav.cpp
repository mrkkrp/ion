/***************************************************************************
 *  C++ source
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

#define DLLBUILDING
#include <wav.h>

BEGINIONSTD

/***************************************************************************
 *  Data Section
 **************************************************************************/

const String  // chunk element identifiers
ChunkRiffId   ("RIFF"),
ChunkTypeWave ("WAVE"),
ChunkFmtId    ("fmt "),
ChunkDataId   ("data"),
ChunkFakeId   ("Fake");

const String               // exceptions wording
waveIncorrectSize          ("declared size of chunk is incorrect"),
waveIncorrectFormat        ("only PCM format is supported"),
waveIncorrectChannels      ("invalid number of channels"),
waveIncorrectSampleRate    ("sample rate is incorrect"),
waveIncorrectBitsPerSample ("invalid numeber of bits per sample"),
waveBadIndexation          ("invalid index for subchunk"),
waveSaving                 ("saving has fault, cannot create stream"),
waveLoading                ("loading has fault, cannot create stream"),
waveInvalidWAVE            ("the file has invalid structure or data"),
waveBadTimes               ("invalid time range is requested");

/***************************************************************************
 *  Definitions of Inline Functions
 **************************************************************************/

inline void __SDWORDToBuffer
    (SDWORD SampleArg, numerator BitsCountArg, Buffer BufferArg)
{
    union { SDWORD aSample; BYTE aBytes[sizeof(SDWORD)]; };
    aSample = SampleArg;
    numerator aBytesCount = __MultiByteRound(BitsCountArg);
    numerator aPadding = sizeof(SDWORD) - aBytesCount;
    for (register numerator i = 0; i < aBytesCount; i++)
        *(BufferArg + i) = *(aBytes + aPadding + i);
}

inline void __BufferToSDWORD
    (const Buffer BufferArg, numerator BitsCountArg, SDWORD &SampleArg)
{
    union { SDWORD aSample; BYTE aBytes[sizeof(SDWORD)]; };
    numerator aBytesCount = __MultiByteRound(BitsCountArg);
    numerator aPadding = sizeof(SDWORD) - aBytesCount;
    for (register numerator i = 0; i < aBytesCount; i++)
        *(aBytes + aPadding + i) = *(BufferArg + i);
    if (aSample > 0) aSample |= (0xffffffffu >> BitsCountArg);
    else aSample &= (0xffffffffu << ((sizeof(SDWORD) << 3) - BitsCountArg));
    SampleArg = aSample;
}

/***************************************************************************
 *  Definitions of Functions
 **************************************************************************/

PLACEEXCEPTIONFNCHERE

String GetWAVDescription()
{
    return "WAVE soundfile format library ("__DATE__")\n"
           "Copyright (c) 2014 Mark Karpov";
}

String GetWAVVersion()
{
    return "4.2.0";
}

/***************************************************************************
 *  Definitions of non-inline Methods
 **************************************************************************/

// class leChunk

leChunk::leChunk(String IdArg, numerator SizeArg) : aSize(SizeArg)
{
    // first: assign identifier to aId
    std::strncpy(aId, IdArg.c_str(), ChunkIdSize);
    // second: get memory from heap for our data-buffer
    aData = new BYTE[aSize];
}

leChunk::leChunk(std::ifstream &StreamArg)
{
    // first: read identifier and save it as aId
    StreamArg.read(aId, ChunkIdSize);
    // second: read size of chunk and save it as aSize
    StreamArg.read((Buffer)&aSize, ChunkIdSize);
    // third: read chunk data and save it in aData buffer
    aData = new BYTE[aSize]; // give memory for our data-buffer
    // now we finally can load all the data:
    StreamArg.read(aData, aSize);
    if ((numerator)StreamArg.gcount() != aSize) THROW_RE(waveIncorrectSize);
    if (aSize % 2) StreamArg.get(); // pick up pad byte
}

leChunk::leChunk(const leChunk &Another)
{
    // first: copy id
    std::strncpy(aId, Another.aId, ChunkIdSize);
    // second:
    aSize = Another.aSize; // assign new size
    aData = new BYTE[aSize]; // give memory..
    // ..and initialize new data:
    for (register numerator i = 0; i < aSize; i++)
        *(aData + i) = *(Another.aData + i);
}

leChunk leChunk::operator = (const leChunk &Another)
{
    // fisrt: assign id
    std::strncpy(aId, Another.aId, ChunkIdSize);
    // second: it's just similar copy constructor
    delete [] aData; // clear old data
    aSize = Another.aSize;
    aData = new BYTE[aSize];
    for (register numerator i = 0; i < aSize; i++)
        *(aData + i) = *(Another.aData + i);
    return *this;
}

leChunk::~leChunk()
{
    if (aSize) delete [] aData;
}

numerator leChunk::TotalSize() const
{
    numerator aResult = Size(); // basic size
    if (aResult % 2) aResult++; // pad byte
    aResult += ChunkIdSize + ChunkSizeSize; // id and size bytes..
    return aResult;
}

void leChunk::WriteTo(std::ofstream &StreamArg) const
{
    // first: insert to stream our id
    StreamArg.write(aId, ChunkIdSize);
    // second: insert to stream size of data
    StreamArg.write((Buffer)&aSize, ChunkSizeSize);
    // third: insert to stream all our data
    StreamArg.write(aData, aSize);
    // fourth: add pad byte if it is needed
    if (aSize % 2) StreamArg.put(0x00);
}

// class WaveStruct

WaveStruct::WaveStruct
    (DWORD nSPSArg, WORD wBPSArg, WORD nChannelsArg, WORD wFormatTagArg)
{
    // - format type -
    if ((wFormatTagArg != WaveFormatPCM)
        && (wFormatTagArg != WaveFormatIEEEFloat))
            THROW_RE(waveIncorrectFormat);
    aFmt.wFormatTag = wFormatTagArg;
    // - number of channels -
    if ((!nChannelsArg) || (nChannelsArg > stereo))
        THROW_LE(waveIncorrectChannels);
    aFmt.nChannels = nChannelsArg;
    // - sample rate -
    if (!nSPSArg) THROW_LE(waveIncorrectSampleRate);
    aFmt.nSamplesPerSec = nSPSArg;
    // - number of bits per sample -
    if (!wBPSArg || wBPSArg > (sizeof(sample) << 3))
        THROW_LE(waveIncorrectBitsPerSample);
    // - number of bits in case of floating point -
    if (aFmt.wFormatTag == WaveFormatIEEEFloat)
        aFmt.wBitsPerSample = sizeof(sample) << 3;
    else aFmt.wBitsPerSample = wBPSArg;
    // - block size of data -
    aFmt.nBlockAlign =
        aFmt.nChannels * __MultiByteRound(aFmt.wBitsPerSample);
    // - buffer estimation -
    aFmt.nAvgBytesPerSec = aFmt.nSamplesPerSec * aFmt.nBlockAlign;
    // - size of extra information -
    aFmt.cbSize = 0; // in this version of the library we use only PCM
    aData.Size = 0;
}

WaveStruct::WaveStruct(String FileNameArg, time StartTimeArg, time EndTimeArg)
{
    // - standard parameters -
    aFmt.cbSize = 0;
    aData.Size = 0;
    // - loading of initial data -
    Load(FileNameArg, StartTimeArg, EndTimeArg);
}

WaveStruct::WaveStruct(const WaveStruct &Another)
{
    // all right man, we just take all format parameters:
    aFmt = Another.aFmt;
    // ok, it's time to copy our data
    aData.Size = Another.aData.Size;
    aData.Floor = new sample[aData.Size];
    for (register numerator i = 0; i < aData.Size; i++)
        *(aData.Floor + i) = *(Another.aData.Floor + i);
}

WaveStruct WaveStruct::operator = (const WaveStruct &Another)
{
    aFmt = Another.aFmt;
    SamplesInData(Another.aData.Size);
    for (register numerator i = 0; i < aData.Size; i++)
        *(aData.Floor) = *(Another.aData.Floor + i);
    return *this;
}

WaveStruct::~WaveStruct()
{
    if (aData.Size) delete [] aData.Floor;
}

bool WaveStruct::Save(String FileNameArg) const
{
    std::ofstream aFileOut // first: open stream for filename
        (FileNameArg.c_str(), std::ios::binary | std::ios::out);
    if (!aFileOut) THROW_RE(waveSaving);
    // second: write type of file "RIFF"
    aFileOut.write(ChunkRiffId.c_str(), ChunkIdSize);
    // third: write size of RIFF chunk
    BYTE aBuffer[ChunkSizeSize];
    numerator aNumericBuffer;
    numerator aSize = FileSize() - (ChunkIdSize + ChunkSizeSize);
    aFileOut.write((Buffer)&aSize, ChunkSizeSize);
    // fourth: write type of content
    aFileOut.write(ChunkTypeWave.c_str(), RiffTypeSize);
    // fifth: write format chunk
    aFileOut.write(ChunkFmtId.c_str(), ChunkIdSize);
    aNumericBuffer = 16u;
    aFileOut.write((Buffer)&aNumericBuffer, ChunkSizeSize);
    aFileOut.write((Buffer)&aFmt, WaveFmtSize);
    // sixth: write data chunk
    aFileOut.write(ChunkDataId.c_str(), ChunkIdSize);
    aNumericBuffer = BytesInData();
    aFileOut.write((Buffer)&aNumericBuffer, ChunkSizeSize);
    numerator aBytesInSample = __MultiByteRound(aFmt.wBitsPerSample);
    for (numerator i = 0; i < aData.Size;)
    {
        sample aSample = *(aData.Floor + i++);
        if (aFmt.wFormatTag == WaveFormatPCM)
        {
            // saturation arithmetic:
            if (aSample > 1.) aSample = 1.;
            else if (aSample < -1.) aSample = -1.;
            SDWORD iSample = (real)aSample * PMAX;
            if (aFmt.wBitsPerSample <= 8) iSample -= PMAX;
            __SDWORDToBuffer(iSample, aFmt.wBitsPerSample, aBuffer);
            aFileOut.write(aBuffer, aBytesInSample);
        }
        else /* 32 bit float */ aFileOut.
            write((Buffer)&aSample, aBytesInSample);
    }
    if (BytesInData() % 2) aFileOut.put(0x00); // the pad byte
    // seventh: write unknown chunks
    leChunksStore::const_iterator i = aUnknownChunks.begin();
    while (i != aUnknownChunks.end()) i++->WriteTo(aFileOut);
    aFileOut.close();
    return !aFileOut.good();
}

bool WaveStruct::Load(String FileNameArg, time StartTimeArg, time EndTimeArg)
{
    std::ifstream aFileIn // first: open stream for filename
        (FileNameArg.c_str(), std::ios::binary | std::ios::in);
    if (!aFileIn) THROW_RE(waveLoading);
    // second: load "RIFF" identifier and check it
    BYTE aBuffer[ChunkIdSize];
    aFileIn.read(aBuffer, ChunkIdSize);
    if (std::strncmp(aBuffer, ChunkRiffId.c_str(), ChunkIdSize))
        THROW_RE(waveInvalidWAVE);
    // third: read size of common chunk and save it
    numerator aCommonSize;
    aFileIn.read((Buffer)&aCommonSize, ChunkSizeSize);
    if (aCommonSize < RiffCriticalSize)
        THROW_RE(waveInvalidWAVE);
    // fourth: read type of content
    aFileIn.read(aBuffer, RiffTypeSize);
    if (std::strncmp(aBuffer, ChunkTypeWave.c_str(), RiffTypeSize))
        THROW_RE(waveInvalidWAVE);
    // fifth: read WAVE format chunk
    aFileIn.read(aBuffer, ChunkIdSize);
    if (std::strncmp(aBuffer, ChunkFmtId.c_str(), ChunkIdSize))
        THROW_RE(waveInvalidWAVE);
    numerator aFormatChunkSize;
    bool aExtendedFmt = false;
    aFileIn.read((Buffer)&aFormatChunkSize, ChunkSizeSize);
    if ((aFormatChunkSize != WaveFmtSize)
        && (aFormatChunkSize != WaveFmtESize))
            THROW_RE(waveInvalidWAVE);
    aFileIn.read((Buffer)&aFmt, WaveFmtSize);
    if (aFormatChunkSize == WaveFmtESize)
    {
        aFileIn.read(aBuffer, WaveFmtESize - WaveFmtSize);
        aExtendedFmt = true;
    }
    // check if we have supported WAVE format
    if ((aFmt.wFormatTag != WaveFormatPCM)
        && (aFmt.wFormatTag != WaveFormatIEEEFloat))
            THROW_RE(waveIncorrectFormat);
    // we work only with 32 bit floating point samples
    if ((aFmt.wFormatTag == WaveFormatIEEEFloat)
        && (aFmt.wBitsPerSample != (sizeof(sample) << 3)))
            THROW_RE(waveIncorrectBitsPerSample);
    // sixth: read WAVE data
    aFileIn.read(aBuffer, ChunkIdSize);
    if (!std::strncmp(aBuffer, ChunkFakeId.c_str(), ChunkIdSize))
    {
        // how to properly ignore fake chunk:
        numerator aFakeChunkSize; // get size of the chunk
        aFileIn.read((Buffer)&aFakeChunkSize, ChunkSizeSize);
        aFileIn.ignore(aFakeChunkSize); // ignore content of the chunk
        if (aFakeChunkSize % 2) { aFileIn.get(); aCommonSize--; }
        aFileIn.read(aBuffer, ChunkIdSize); // try again to get data id
        aCommonSize -= ChunkIdSize + ChunkSizeSize + aFakeChunkSize;
    }
    if (std::strncmp(aBuffer, ChunkDataId.c_str(), ChunkIdSize))
        THROW_RE(waveInvalidWAVE);
    numerator aDataChunkSize;
    aFileIn.read((Buffer)&aDataChunkSize, ChunkSizeSize);
    numerator aBytesInSample = __MultiByteRound(aFmt.wBitsPerSample);
    numerator aSamplesInData = aDataChunkSize / aBytesInSample;
    numerator
    aStartIndex = StartTimeArg * aFmt.nSamplesPerSec,
    aEndIndex = EndTimeArg ?
        EndTimeArg * aFmt.nSamplesPerSec : aSamplesInData;
    aStartIndex *= aFmt.nChannels;
    if (EndTimeArg) aEndIndex *= aFmt.nChannels;
    if (aStartIndex > aEndIndex) THROW_RE(waveBadTimes);
    SamplesInData(aEndIndex - aStartIndex);
    for (numerator i = 0; i < aSamplesInData; i++)
    {
        sample aSample;
        if (aFmt.wFormatTag == WaveFormatPCM)
        {
            SDWORD iSample;
            aFileIn.read(aBuffer, aBytesInSample);
            __BufferToSDWORD(aBuffer, aFmt.wBitsPerSample, iSample);
            aSample = (real)iSample / PMAX;
        }
        else /* 32 bit float */ aFileIn.
            read((Buffer)&aSample, aBytesInSample);
        if ((i >= aStartIndex) && (i < aEndIndex))
            *(aData.Floor + (i - aStartIndex)) = aSample;
    }
    if (aDataChunkSize % 2) aFileIn.get(); // pick up the pad byte
    // seventh: read unknown chunks
    if (aCommonSize % 2) aCommonSize++;
    numerator aUnknownZone = aCommonSize - aDataChunkSize;
    if (aDataChunkSize % 2) aUnknownZone--; // pad byte of data chunk
    if (aExtendedFmt) aUnknownZone -= RiffCriticalESize;
    else aUnknownZone -= RiffCriticalSize;
    for (numerator i = 0; i < aUnknownZone;)
    {
        aUnknownChunks.push_back(leChunk(aFileIn));
        i += (aUnknownChunks.end() - 1)->TotalSize();
    }
    aFileIn.close();
    return !aFileIn.good();
}

ConstMediaBuffer WaveStruct::GetData() const
{
    return aData.Floor; // return pointer to 32 bit floating point samples
}

leChunkIter WaveStruct::GetUnknownChunk(numerator IndexArg)
{
    if (IndexArg >= aUnknownChunks.size()) THROW_LE(waveBadIndexation);
    return (aUnknownChunks.begin() + IndexArg);
}

void WaveStruct::PushBackChunk(leChunk ChunkArg)
{
    aUnknownChunks.push_back(ChunkArg);
}

void WaveStruct::DeleteUnknownChunk(numerator IndexArg)
{
    if (IndexArg >= aUnknownChunks.size()) THROW_LE(waveBadIndexation);
    aUnknownChunks.erase(aUnknownChunks.begin() + IndexArg);
}

void WaveStruct::ClearUnknownChunks()
{
    aUnknownChunks.clear();
}

void WaveStruct::Format(WORD wFormatTagArg)
{
    if ((aFmt.wFormatTag != WaveFormatPCM)
        && (aFmt.wFormatTag != WaveFormatIEEEFloat))
            THROW_RE(waveIncorrectFormat);
    aFmt.wFormatTag = wFormatTagArg;
    // only 32 bit for floating point:
    if (aFmt.wFormatTag == WaveFormatIEEEFloat)
        BitsPerSample(sizeof(sample) << 3);
}

void WaveStruct::Gain(real GainArg)
{
    // first: calculate factor
    real aFactor = __DbToFactor(GainArg);
    // second: just use it for all samples
    numerator i = 0;
    while (i < aData.Size) *(aData.Floor + i++) *= aFactor;
}

void WaveStruct::Normalize()
{
    Gain(NormalGain());
}

void WaveStruct::SampleRate(DWORD nSPSArg)
{
    if (!nSPSArg) THROW_LE(waveIncorrectSampleRate);
    if (nSPSArg == aFmt.nSamplesPerSec) return; // we have nothing to do
    // first: devide the data to two buffers for stereo:
    MediaBuffer aLeftB, aRightB; // ..and copy to left buffer for mono
    if (aFmt.nChannels == mono)
    {
        aLeftB = new sample[aData.Size];
        for (numerator i = 0; i < aData.Size; i++)
            *(aLeftB + i) = *(aData.Floor + i);
    }
    else // stereo variant
    {
        aLeftB = new sample[aData.Size >> 1];
        aRightB = new sample[aData.Size >> 1];
        for (numerator i = 0; i < aData.Size;)
        {
            *(aLeftB + (i >> 1)) = *(aData.Floor + i), i++;
            *(aRightB + (i >> 1)) = *(aData.Floor + i), i++;
        }
    }
    // second: resize data
    numerator aMonoSamplesCount = (Duration() * nSPSArg);
    aMonoSamplesCount *= Channels();
    SamplesInData(aMonoSamplesCount);
    // third: use linear approximation and take new values:
    time aSection = 1. / (real)aFmt.nSamplesPerSec; // time between two points
    time bSection = 1. / (real)nSPSArg; // time between two samples (new rate)
    for (numerator i = 0; i < aMonoSamplesCount;)
    {
        time aTime = ((aFmt.nChannels == mono) ?
            i : (i >> 1)) * bSection; // calculate actual time
        numerator LeftIndex = aTime / aSection;
        // find index of left value
        real // take left and right samples
        LeftSample = *(aLeftB + LeftIndex),
        RightSample = *(aLeftB + LeftIndex + 1);
        // linear approximation and writing sample to data:
        real aResult = (RightSample - LeftSample) / aSection;
        aResult *= aTime - LeftIndex * aSection;
        aResult += LeftSample;
        *(aData.Floor + i++) = (sample)aResult;
        if (aFmt.nChannels == mono) continue;
        // variant for right buffer:
        LeftSample = *(aRightB + LeftIndex);
        RightSample = *(aRightB + LeftIndex + 1);
        // linear approximation and writing sample to data:
        aResult = (RightSample - LeftSample) / aSection;
        aResult *= aTime - LeftIndex * aSection;
        aResult += LeftSample;
        *(aData.Floor + i++) = (sample)aResult;
    }
    // release our buffers:
    delete [] aLeftB;
    if (aFmt.nChannels == stereo) delete [] aRightB;
    // change sample rate:
    aFmt.nSamplesPerSec = nSPSArg;
    // dependent values:
    aFmt.nAvgBytesPerSec = aFmt.nSamplesPerSec * aFmt.nBlockAlign;
}

void WaveStruct::BitsPerSample(WORD wBPSArg)
{
    if (!wBPSArg || wBPSArg > (sizeof(sample) << 3))
        THROW_LE(waveIncorrectBitsPerSample);
    if ((aFmt.wFormatTag == WaveFormatIEEEFloat)
        && (wBPSArg != (sizeof(sample) << 3)))
            THROW_RE(waveIncorrectBitsPerSample);
    aFmt.wBitsPerSample = wBPSArg;
    // dependent values:
    aFmt.nBlockAlign =
        aFmt.nChannels * __MultiByteRound(aFmt.wBitsPerSample);
    aFmt.nAvgBytesPerSec = aFmt.nSamplesPerSec * aFmt.nBlockAlign;
}

void WaveStruct::Channels(WORD nChannelsArg)
{
    if ((!nChannelsArg) || (nChannelsArg > stereo))
        THROW_LE(waveIncorrectChannels);
    if (nChannelsArg == aFmt.nChannels) return; // we have nothing to do
    MediaBuffer aNewData; // here will be new samples
    numerator aNewSize; // it's our new size of data
    register sample aSample; // it's our buffer..
    if (nChannelsArg == mono) // from stereo to mono convert
    {
        aNewSize = aData.Size >> 1;
        aNewData = new sample[aNewSize];
        for (numerator i = 0; i < aData.Size;)
        {
            aSample = /* left */ *(aData.Floor + i++) / 2;
            aSample += /* right */ *(aData.Floor + i) / 2;
            *(aNewData + (i++ >> 1)) = aSample; // one more sample
        }
    }
    else // from mono to stereo convert
    {
        aNewSize = aData.Size << 1;
        aNewData = new sample[aNewSize];
        for (numerator i = 0; i < aNewSize;)
        {
            aSample = /* mono */ *(aData.Floor + (i >> 1));
            *(aNewData + i++) = aSample; // write it as left
            *(aNewData + i++) = aSample; // write it as right
        }
    }
    delete [] aData.Floor;
    aData.Floor = aNewData;
    aData.Size = aNewSize;
    aFmt.nChannels = nChannelsArg;
    // dependent values:
    aFmt.nBlockAlign =
        aFmt.nChannels * __MultiByteRound(aFmt.wBitsPerSample);
    aFmt.nAvgBytesPerSec = aFmt.nSamplesPerSec * aFmt.nBlockAlign;
}

void WaveStruct::Dither()
{
    real LSB = 1 << ((sizeof(sample) << 3) - aFmt.wBitsPerSample);
    for (numerator i = 0; i < aData.Size; i++)
    {
        real aNoise = (std::rand() % 3) + (std::rand() % 3) - 2;
        *(aData.Floor + i) += (aNoise * LSB) / PMAX;
    }
}

numerator WaveStruct::BytesInData() const
{
    numerator aResult = SamplesInData() * Channels();
    aResult *= __MultiByteRound(aFmt.wBitsPerSample);
    return aResult;
}

numerator WaveStruct::FileSize() const
{
    // RIFF chunk
    numerator aResult = ChunkIdSize + ChunkSizeSize + RiffTypeSize;
    // format chunk
    aResult += WaveFmtSize + ChunkIdSize + ChunkSizeSize;
    // data chunk
    aResult += ChunkIdSize + ChunkSizeSize; // basic for data
    aResult += BytesInData(); // the data
    if (BytesInData() % 2) aResult++; // pad byte
    // ..and unknown chunks after all
    leChunksStore::const_iterator i = aUnknownChunks.begin();
    while (i != aUnknownChunks.end()) aResult += i++->TotalSize();
    return aResult;
}

sample WaveStruct::Peak() const
{
    sample aPeak = 0;
    for (numerator i = 0; i < aData.Size;)
    {
        sample aPretender = __Abs(*(aData.Floor + i++));
        if (aPretender > aPeak) aPeak = aPretender;
    }
    return aPeak;
}

real WaveStruct::NormalGain() const
{
    sample aPeak = Peak();
    if (aPeak) return 10. * std::log10((real)1. / aPeak);
    else return 0.;
}

void WaveStruct::SamplesInData(numerator SizeArg, sample SampleArg)
{
    // first: clear old data
    if (aData.Size) delete [] aData.Floor;
    aData.Floor = new sample[SizeArg];
    aData.Size = SizeArg;
    numerator i = 0;
    while (i < aData.Size) *(aData.Floor + i++) = SampleArg;
}

ENDIONSTD
