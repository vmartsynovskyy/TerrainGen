#pragma once

#include "FastNoise.h"
#include "CoreMinimal.h"
#include "Engine.h"

class ChunkGenParams {
public:
	ChunkGenParams();
	ChunkGenParams(	int resolution, 
					float chunkSize, 
					FastNoise* ptrToNoise,
					UCurveFloat* terrainCurve,
					float heightScale) : Resolution(resolution), Size(chunkSize), PtrToNoise(ptrToNoise), TerrainCurve(terrainCurve), HeightScale(heightScale) {}
	int Resolution;
	float Size;
	FastNoise* PtrToNoise;
	UCurveFloat* TerrainCurve;
	float HeightScale;
};