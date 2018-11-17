// Fill out your copyright notice in the Description page of Project Settings.

#include "ProceduralTerrainChunk.h"
#include "FastNoise.h"
#include <stdlib.h>


// Sets default values
AProceduralTerrainChunk::AProceduralTerrainChunk()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	procMesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("TerrainChunkMesh"));
	RootComponent = procMesh;
}

void AProceduralTerrainChunk::Destroyed() {
	procMesh->DestroyComponent();
}

TArray<TArray<float>> AProceduralTerrainChunk::generateHeightmap(int width, int height) {
	TArray<TArray<float>> xArray;

	FastNoise myNoise;
	myNoise.SetSeed(Seed);
	myNoise.SetNoiseType(FastNoise::SimplexFractal);
	myNoise.SetFrequency(0.000625);

	for (int x = xStart; x < xStart + (heightMapLength+2) * resolutionFactor; x += resolutionFactor) {
		TArray<float> yArray;
		for (int y = yStart; y < yStart + (heightMapLength+2) * resolutionFactor; y += resolutionFactor) {
			yArray.Add(myNoise.GetNoise(x, y) * heightScale);
		}
		xArray.Add(yArray);
	}
	return xArray;
}

/**
*   \brief Generates vertices given a heightmap.
*   
*   Given a rectangular heightmap, generates a flattened array
*   of vectors representation. Flattened top-down then left-right.
*
*   \param heightmap A rectangular array representing a heightmap 
*   \return A flat array of vertices generated from the heightmap
*
**/
TArray<FVector> AProceduralTerrainChunk::generateVertices(TArray<TArray<float>> heightmap) {
	// TArray<TArray<float>> heightmap must be a rectangle
	TArray<FVector> vertices;
	for (int x = 1; x < heightmap.Num() - 1; x++) {
		for (int y = 1; y < heightmap[0].Num() - 1; y++) {
			vertices.Add(FVector((x-1)*widthScale, (y-1)*widthScale, heightmap[x][y]));
		}
	}
	return vertices;
}

TArray<FVector> AProceduralTerrainChunk::generateNormals(TArray<FVector> verts, TArray<TArray<float>> hMap, int width, int height) {
	TArray<FVector> normals;
	for (int x = 0; x < width; x++)
	{
		for (int y = 0; y < height; y++) {
			// based on triangle ordering above
			int centerInd = x * height + y;
			int leftInd = centerInd - height;
			int rightInd = centerInd + height;
			int topRightInd = rightInd - 1;
			int bottomLeftInd = leftInd + 1;
			int topInd = centerInd - 1;
			int bottomInd = centerInd + 1;

			FVector topLeftCross;
			FVector topRightUpperCross;
			FVector topRightLowerCross;
			FVector bottomRightCross;
			FVector bottomLeftLowerCross;
			FVector bottomLeftUpperCross;
			
			// we must check if the index is within bounds, otherwise we must generate new vertices from the heightmap
			// in order to have seamless edges
			// if it's in bounds then we can just use the vertices we already generated
			FVector centerVert = verts[centerInd];
			FVector leftVert = (x - 1 > 0 && x - 1 < width) ? verts[leftInd] : FVector((x-1)*widthScale, (y)*widthScale, hMap[x][y+1]);
			FVector rightVert = (x + 1 > 0 && x + 1 < width) ? verts[rightInd] : FVector((x+1)*widthScale, (y)*widthScale, hMap[x+2][y+1]);
			FVector topRightVert = (x + 1 > 0 && x + 1 < width && y - 1 > 0 && y - 1 < height) ? verts[topRightInd] : FVector((x+1)*widthScale, (y-1)*widthScale, hMap[x+2][y]);
			FVector bottomLeftVert = (x - 1 > 0 && x - 1 < width && y + 1 > 0 && y + 1 < height) ? verts[bottomLeftInd] : FVector((x-1)*widthScale, (y+1)*widthScale, hMap[x][y+2]);
			FVector topVert = (y - 1 > 0 && y - 1 < height) ? verts[topInd] : FVector((x)*widthScale, (y-1)*widthScale, hMap[x+1][y]);
			FVector bottomVert = (y + 1 > 0 && y + 1 < height) ? verts[bottomInd] : FVector((x)*widthScale, (y+1)*widthScale, hMap[x+1][y+2]);
			

			// we calculate the normals of each side, sum them, and normalize to get normal vectors for a vertex
			topLeftCross = FVector::CrossProduct(topVert - centerVert, leftVert - centerVert);
			bottomLeftLowerCross = FVector::CrossProduct(centerVert - bottomVert, bottomVert - bottomLeftVert);
			bottomLeftUpperCross = FVector::CrossProduct(leftVert - bottomLeftVert, centerVert - leftVert);
			topRightUpperCross = FVector::CrossProduct(topVert - centerVert, topRightVert - centerVert);
			topRightLowerCross = FVector::CrossProduct(topRightVert - rightVert, rightVert - centerVert);
			bottomRightCross = FVector::CrossProduct(centerVert - bottomVert, rightVert - centerVert);

			FVector normal = topLeftCross + topRightUpperCross + topRightLowerCross + bottomRightCross + bottomLeftLowerCross + bottomLeftUpperCross;
			normal.Normalize();
			normals.Add(normal);
		}
	}
	 
	return normals;
}

/**
*   \brief Generates triangles given the dimensions of a rectangular heightmap.
*
*   Generates triangles given the dimensions of a rectangular heightmap,
*   assuming a flattened list of vectors generated from heightmap top-down then left-right.
*
*   \param width The width of the rectangular heightmap.
*   \param height The height of the rectangular heightmap.
*   \return A list of int32 representing the triangles generated from the rectangular heightmap.
*
**/
TArray<int32> AProceduralTerrainChunk::generateTriangles(int32 width, int32 height) {
	// TArray<TArray<float>> heightmap must be a rectangle
	TArray<int32> triangles;
	for (int x = 0; x < width - 1; x++) {
		for (int y = 0; y < height - 1; y++) {
			int topLeft = x * height + y;
			int bottomLeft = x * height + y + 1;
			int bottomRight = (x + 1) * height + y + 1;
			int topRight = (x + 1) * height + y;

			// Bottom-left triangle
			triangles.Add(topLeft);
			triangles.Add(bottomLeft);
			triangles.Add(bottomRight);

			// Top-right triangle
			triangles.Add(topLeft);
			triangles.Add(bottomRight);
			triangles.Add(topRight);
		}
	}
	return triangles;
}

TArray<FVector2D> AProceduralTerrainChunk::generateUV(int width, int height) {
	TArray<FVector2D> uv;
	for (int32 x = 0; x < width; x++)
	{
		for (int32 y = 0; y < height; y++) {
			FVector2D uvVert = FVector2D(x, y);
			uv.Add(uvVert);
		}
	}

	return uv;
}

// Called when the actor is created
void AProceduralTerrainChunk::PostActorCreated()
{
	
}

// Called when the game starts or when spawned
void AProceduralTerrainChunk::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AProceduralTerrainChunk::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AProceduralTerrainChunk::CreateRandomMeshComponent()
{
	TArray<TArray<float>> heightmap = generateHeightmap(heightMapLength + 2, heightMapLength + 2);
	TArray<FVector> vertices = generateVertices(heightmap);
	TArray<int32> triangles = generateTriangles(heightMapLength, heightMapLength);
	TArray<FVector> normals = generateNormals(vertices, heightmap, heightMapLength, heightMapLength);
	TArray<FVector2D> uv0 = generateUV(heightMapLength, heightMapLength);
	TArray<FLinearColor> colors;
	TArray<FProcMeshTangent> tangents;
	procMesh->CreateMeshSection_LinearColor(0, vertices, triangles, normals, uv0, colors, tangents, false);
}

/**
*   \brief Sets the size of the chunk based on the resolution.
*   \param width The width of the chunk in UE4 units
*   \param resolution Number of faces on edges per 64 UE4 units
*   \return Returns the heightMap length
*
**/
int AProceduralTerrainChunk::SetSizeAndResolution(float width, int resolution = 4) {
	widthScale = 64 / resolution;
	heightMapLength = width / widthScale + 1;
	resolutionFactor = 4096 / heightMapLength;
	return (heightMapLength-1) * resolutionFactor;
}

void AProceduralTerrainChunk::SetXandYStart(int xStart, int yStart) {
	this->xStart = xStart;
	this->yStart = yStart;
}

void AProceduralTerrainChunk::SetMaterialForProcMesh(UMaterialInterface* material) {
	procMesh->SetMaterial(0, material);
}
