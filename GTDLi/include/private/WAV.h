#pragma once
#include "public/Core.h"
#include "public/Retcodes.h"
#include "private/CommonMacros.h"
#include <memory>
#include <cstdint>

// https://stackoverflow.com/questions/13660777/c-reading-the-data-part-of-a-wav-file

    typedef struct  WAV_HEADER
    {
        /* RIFF Chunk Descriptor */
        uint8_t         RIFF[4];        // RIFF Header Magic header
        uint32_t        ChunkSize;      // RIFF Chunk Size
        uint8_t         WAVE[4];        // WAVE Header
        /* "fmt" sub-chunk */
        uint8_t         fmt[4];         // FMT header
        uint32_t        Subchunk1Size;  // Size of the fmt chunk
        uint16_t        AudioFormat;    // Audio format 1=PCM,6=mulaw,7=alaw,     257=IBM Mu-Law, 258=IBM A-Law, 259=ADPCM
        uint16_t        NumOfChan;      // Number of channels 1=Mono 2=Sterio
        uint32_t        SamplesPerSec;  // Sampling Frequency in Hz
        uint32_t        bytesPerSec;    // bytes per second
        uint16_t        blockAlign;     // 2=16-bit mono, 4=16-bit stereo
        uint16_t        bitsPerSample;  // Number of bits per sample
        /* "data" sub-chunk */
        uint8_t         Subchunk2ID[4]; // "data"  string
        uint32_t        Subchunk2Size;  // Sampled data length
    } wav_header_tag;

namespace GTDLi
{
    static RETCODE get_wav_header_from_buffer(char* buffer, WAV_HEADER& out_header)
    {
        out_header = { 0 };

        if (!IS_NULL(buffer))
        {
            memcpy(&out_header, buffer, sizeof(WAV_HEADER));
            return RTN_OK;
        }

        return RTN_NULL_OBJ;
    }
}