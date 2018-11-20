#pragma once

#include "ChunkInfo.h"
#include "ChunkGenParams.h"

#include "CoreMinimal.h"
#include "Engine.h"

class ChunkInfoWorker : public FRunnable {
protected:
	virtual uint32 Run() override;
	virtual void Stop() override;
public:
	ChunkInfoWorker();
	ChunkInfoWorker(ChunkGenParams params) : Params(params) {}
	ChunkGenParams Params;
	float GenerateRadius;
	TMap<TPair<int, int>, ChunkInfo>* infoMapPtr;
	FVector playerPos;
	FCriticalSection* ChunkDeletion;
	bool KeepGenerating = true;
private:
	int heightMapLen = 0;
	void createChunkInfo(int x, int y);
	void destroyChunkInfo(int x, int y);
};