#include "SoundNodeOggPlayer.h"
#include "Audio.h"
#include "ActiveSound.h"
#include "Sound/SoundWave.h"
#include "UObject/FrameworkObjectVersion.h"
#include "Math/UnrealMathUtility.h"
#include "Misc/FileHelper.h"

USoundNodeOggPlayer::USoundNodeOggPlayer(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	SoundWaveProcedural = nullptr;
	BufferSize = 0;
	CurrentPosition = 0;
}

void USoundNodeOggPlayer::Initialize()
{
	QualityInfo = { 0 };

	// Open ogg file
	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
	FString filePath = FPaths::ProjectContentDir() + OggFilePath;
	FileHandle.Reset(PlatformFile.OpenRead(*filePath));
	if (FileHandle.IsValid())
	{
		// Initialize vorbis audio info.
		int64 FileSize = FileHandle->Size();
		CompressedFile.Reset();
		CompressedFile.AddUninitialized(FileSize);
		FileHandle->Read(CompressedFile.GetData(), FileSize);
		VorbisAudioInfo.Reset(new FVorbisAudioInfo());
		VorbisAudioInfo->ReadCompressedInfo(CompressedFile.GetData(), FileSize, &QualityInfo);

		// Initialize internal params
		DecodedBuffer.Reset(0);
		BufferSize = 0;
		CurrentPosition = 0;

		// Initialize USoundWaveProcedural
		SoundWaveProcedural = NewObject<USoundWaveProcedural>();
		SoundWaveProcedural->SetSampleRate(QualityInfo.SampleRate);
		SoundWaveProcedural->NumChannels = QualityInfo.NumChannels;
		SoundWaveProcedural->SampleByteSize = sizeof(int16);
		SoundWaveProcedural->ResetAudio();
		SoundWaveProcedural->OnSoundWaveProceduralUnderflow =
			FOnSoundWaveProceduralUnderflow::CreateUObject(this, &USoundNodeOggPlayer::GenerateData);
	}
	else
	{
		QualityInfo.NumChannels = 0;
		UE_LOG(LogTemp, Error, TEXT("Invalid ogg file: %s"), *OggFilePath);
	}
}

void USoundNodeOggPlayer::GenerateData(USoundWaveProcedural* InProceduralWave, int32 SamplesRequested)
{
	int32 SampleByteSize = InProceduralWave->SampleByteSize;
	int32 NumChannels = SoundWaveProcedural->NumChannels;
	int32 BufferSizeRequested = SamplesRequested * SampleByteSize;

	// Acquire buffer memory
	if (BufferSize < BufferSizeRequested)
	{
		BufferSize = BufferSizeRequested;
		DecodedBuffer.Reset(BufferSize);
	}

	// Loop check
	int32 WriteSamples = SamplesRequested / NumChannels;
	int32 BufferOffset = 0;
	if (LoopStart >= 0 && LoopLength > 0
		&& CurrentPosition + WriteSamples > LoopStart + LoopLength)
	{
		// Decode ogg before loop end point
		WriteSamples = LoopStart + LoopLength - CurrentPosition;
		int32 WriteBufferSize = WriteSamples * NumChannels * SampleByteSize;
		VorbisAudioInfo->ReadCompressedData(DecodedBuffer.GetData(), false, WriteBufferSize);

		// Set position to loop start point
		CurrentPosition = LoopStart;
		VorbisAudioInfo->SeekToTime((float)CurrentPosition / QualityInfo.SampleRate);

		// Calc rest size
		WriteSamples = SamplesRequested / NumChannels - WriteSamples;
		BufferOffset = WriteBufferSize;
	}

	// Decode ogg
	int32 WriteBufferSize = WriteSamples * NumChannels * SampleByteSize;
	VorbisAudioInfo->ReadCompressedData(DecodedBuffer.GetData() + BufferOffset, false, WriteBufferSize);
	InProceduralWave->QueueAudio(DecodedBuffer.GetData(), BufferSizeRequested);

	// Move position
	CurrentPosition += WriteSamples;
}

void USoundNodeOggPlayer::ParseNodes(FAudioDevice* AudioDevice, const UPTRINT NodeWaveInstanceHash, FActiveSound& ActiveSound, const FSoundParseParameters& ParseParams, TArray<FWaveInstance*>& WaveInstances)
{
	RETRIEVE_SOUNDNODE_PAYLOAD(sizeof(int32));
	DECLARE_SOUNDNODE_ELEMENT(int32, tempInt);

	if (*RequiresInitialization)
	{
		Initialize();
		*RequiresInitialization = false;
	}

	if (!VorbisAudioInfo.IsValid()) {
		return;
	}

	if (!SoundWaveProcedural) {
		return;
	}

	FSoundParseParameters UpdatedParams = ParseParams;
	SoundWaveProcedural->Parse(AudioDevice, NodeWaveInstanceHash, ActiveSound, UpdatedParams, WaveInstances);
}

float USoundNodeOggPlayer::GetDuration()
{
	return INDEFINITELY_LOOPING_DURATION;
}

bool USoundNodeOggPlayer::IsPlayWhenSilent() const
{
	return false;
}

int32 USoundNodeOggPlayer::GetMaxChildNodes() const
{
	return 0;
}
