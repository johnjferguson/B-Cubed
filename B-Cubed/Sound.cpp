#include "Sound.h"
#include <fstream>
#include <cassert>

std::unordered_map<std::string, Sound::Source>  Sound::soundMap;

Sound::Sound()
{
	device = alcOpenDevice(nullptr);
	if (!device)
		std::cerr << "error sound constructor device" << std::endl;

	context = alcCreateContext(device, nullptr);
	alcMakeContextCurrent(context);

	if (!device)
		std::cerr << "error sound constructor constext" << std::endl;
}

void Sound::Play(const std::string & file_path)
{
	bool good = true;
	std::unordered_map<std::string, Source>::iterator iter = soundMap.find(file_path);

	if (iter == soundMap.end())
	{
		good = Load(file_path);
	}

	if (good)
	{
		iter->second.Play();
	}
}

bool Sound::Load(const std::string & path)
{
	std::ifstream file(path, std::ios::binary);

	if (file.bad())
	{
		std::cerr << "bad file path: " << path.c_str() << std::endl;
		return false;
	}

	WavHeader header;
	ALenum format;

	file.read((char*)&header, sizeof(header));

	// read wav file header data
	char* buf = new char[header.Subchunk2Size];
	file.read(&buf[0], header.Subchunk2Size);

	if (header.bitsPerSample == 8)
	{
		if (header.NumOfChan == 1)
			format = AL_FORMAT_MONO8;
		else if (header.NumOfChan == 2)
			format = AL_FORMAT_STEREO8;
	}
	else if (header.bitsPerSample == 16)
	{
		if (header.NumOfChan == 1)
			format = AL_FORMAT_MONO16;
		else if (header.NumOfChan == 2)
			format = AL_FORMAT_STEREO16;
	}

	ALuint frequency = header.SamplesPerSec;

	ALuint source;
	ALuint buffer;
	alGenBuffers(1, &buffer);
	alGenSources(1, &source);

	alBufferData(buffer, format, buf, header.Subchunk2Size, frequency);

	ALfloat SourcePos[] = { 0.0f,0.0f,0.0f };
	ALfloat SourceVel[] = { 0.0f,0.0f,0.0f };
	ALfloat ListenerPos[] = { 0.0f,0.0f,0.0f };
	ALfloat ListenerVel[] = { 0.0f,0.0f,0.0f };
	ALfloat ListenerOri[] = { 0.0f,0.0f,-1.0f, 0.0f, 1.0, 0.0};

	alListenerfv(AL_POSITION, ListenerPos);
	alListenerfv(AL_VELOCITY, ListenerVel);
	alListenerfv(AL_ORIENTATION, ListenerOri);

	alSourcei(source, AL_BUFFER, buffer);
	alSourcef(source,AL_PITCH, 1.0f);
	alSourcef(source, AL_GAIN, 1.0f);
	alSourcefv(source, AL_POSITION, SourcePos);
	alSourcefv(source, AL_VELOCITY, SourceVel);
	alSourcei(source, AL_LOOPING, AL_FALSE);

	soundMap.insert({ path, Source(source, buffer) });

	return true;
}

Sound::Source::Source(ALuint source, ALuint buffer)
	:
	source(source),
	buffer(buffer)
{
}

void Sound::Source::Play()
{
	alSourcePlay(source);
}
