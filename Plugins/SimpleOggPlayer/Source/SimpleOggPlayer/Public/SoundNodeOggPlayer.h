#pragma once

#include "CoreMinimal.h"
#include "DSP/Dsp.h"
#include "SampleBuffer.h"
#include "AudioDecompress.h"
#include "GenericPlatform/GenericPlatformFile.h"
#include "Interfaces/IAudioFormat.h"
#include "VorbisAudioInfo.h"
#include "UObject/ObjectMacros.h"
#include "Sound/SoundNode.h"
#include "Sound/SoundWaveProcedural.h"

#include "SoundNodeOggPlayer.generated.h"

/**
 * USoundNodeOggPlayer
 */
UCLASS(hidecategories = Object, editinlinenew, meta = (DisplayName = "OggPlayer"))
class USoundNodeOggPlayer : public USoundNode
{
	GENERATED_UCLASS_BODY()

	// Ogg relative file path from Content Dir.
	UPROPERTY(EditAnywhere, Category = OggPlayer)
		FString OggFilePath;

	UPROPERTY(EditAnywhere, Category = OggPlayer)
		int64 LoopStart;

	UPROPERTY(EditAnywhere, Category = OggPlayer)
		int64 LoopLength;

private:
	TUniquePtr<IFileHandle> FileHandle;
	TUniquePtr<FVorbisAudioInfo> VorbisAudioInfo;
	FSoundQualityInfo QualityInfo;
	TArray<uint8> CompressedFile;
	TArray<uint8> DecodedBuffer;
	USoundWaveProcedural* SoundWaveProcedural;

	int32 BufferSize;
	int64 CurrentPosition;

private:
	// Internal Functions
	void Initialize();
	void GenerateData(USoundWaveProcedural* InProceduralWave, int32 SamplesRequested);

public:
	//~ Begin USoundNode Interface
	virtual int32 GetMaxChildNodes() const override;
	virtual float GetDuration() override;
	virtual bool IsPlayWhenSilent() const override;
	virtual int32 GetNumSounds(const UPTRINT NodeWaveInstanceHash, FActiveSound& ActiveSound) const { return 1; }
	virtual void ParseNodes(FAudioDevice* AudioDevice, const UPTRINT NodeWaveInstanceHash, FActiveSound& ActiveSound, const FSoundParseParameters& ParseParams, TArray<FWaveInstance*>& WaveInstances) override;
	//~ End USoundNode Interface.
};
