#pragma once

#include "ProceduralMeshComponent.h"
#include "FastNoise.h"
#include "CoreMinimal.h"
#include "Engine.h"

class ChunkInfo {
public:
	void GenerateChunk(int xStart, int yStart, int resolution, float chunkSize, FastNoise* ptrToNoise, UCurveFloat* TerrainCurve, float heightScale);
	TArray<FVector> GetVertices();
	TArray<int32> GetTriangles();
	TArray<FVector> GetNormals();
	TArray<FVector2D> GetUVMap();
	TArray<FLinearColor> GetColors();
	TArray<FProcMeshTangent> GetTangents();

	int GetDistanceTraversed();
private:
	FastNoise* noiseGen;
	int heightmapDistanceTraversed;
	TArray<FVector> vertices;
	TArray<int32> triangles;
	TArray<FVector> normals;
	TArray<FVector2D> uvMap;
	TArray<FLinearColor> colors;
	TArray<FProcMeshTangent> tangents;
	TArray<TArray<float>> generateHeightmap(int xStart, int yStart, int resolutionFactor, int heightMapLength, UCurveFloat* curve, float heightScale);
	TArray<FVector> generateVertices(TArray<TArray<float>> heightmap, float vertexDistance);
	TArray<int32> generateTriangles(int width, int height);
	TArray<FVector> generateNormals(TArray<FVector> verts, TArray<TArray<float>> hMap, int width, int height, float vertexDistance);
	TArray<FVector2D> generateUV(int width, int height);
};