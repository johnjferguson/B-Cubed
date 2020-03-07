#pragma once
#include <iostream>
#include <unordered_map>
#include "openal/include/AL/al.h"
#include "openal/include/AL/alc.h"
#include "physx/include/PxPhysicsAPI.h"

class Sound
{
private:
	struct  WavHeader
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
	};
private:
	class Source
	{
	public:
		Source(ALuint source, ALuint buffer);
		// need this move constructor to stop calling the destructor on source and buffer before setting them to nullptr
		Source(Source&& rhs);
		Source(Source&) = delete;
		Source& operator=(Source&) = delete;
		~Source();
		void Play();
	private:
		ALuint source;
		ALuint buffer;
	};
public:
	Sound();
	~Sound();
	Sound(Sound&) = delete;
	Sound& operator=(Sound&) = delete;
	static void Play(const std::string& file_path, float gain, physx::PxVec3 listenerPos, physx::PxVec3 sourcePos, bool loop);
private:
	static bool Load(const std::string& file_path, float gain, physx::PxVec3 listenerPos, physx::PxVec3 sourcePos, bool loop);
private:
	ALCdevice* device;
	ALCcontext* context;
private:
	static std::unordered_map<std::string, Source> soundMap;
};
