#pragma once

#include "ChunkInfo.h"

#include "CoreMinimal.h"
#include "Engine.h"

class ChunkInfoWorker : public FRunnable {
protected:
	virtual uint32 Run() override;
public:
	float GenerateRadius;
	float ChunkSize;
	int Resolution;
	FastNoise* PtrToNoise;
	UCurveFloat* Curve;
	float HeightScale;
	TMap<TPair<int, int>, ChunkInfo>* infoMapPtr;
	FVector playerPos;
private:
	int heightMapLen = 0;
	void createChunkInfo(int x, int y);
};