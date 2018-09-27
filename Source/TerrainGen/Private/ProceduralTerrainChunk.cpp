// Fill out your copyright notice in the Description page of Project Settings.

#include "ProceduralTerrainChunk.h"


// Sets default values
AProceduralTerrainChunk::AProceduralTerrainChunk()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	procMesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("TerrainChunkMesh"));
	RootComponent = procMesh;
}

// Called when the game starts or when spawned
void AProceduralTerrainChunk::BeginPlay()
{
	Super::BeginPlay();
	
}

TArray<TArray<float>> AProceduralTerrainChunk::generateHeightmap(int width, int height) {
	TArray<TArray<float>> xArray;
	for (int x = 0; x < height; x++) {
		TArray<float> yArray;
		for (int y = 0; y < height; y++) {
			// TODO: use Simplex rather than all 0's
			yArray.Add(0.0);
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
	// TArray<TArray<float>> heightmap must be a rectang
	TArray<FVector> vertices;
	const int scaleFactor = 10;
	for (int x = 0; x < heightmap.Num(); x++) {
		for (int y = 0; y < heightmap[0].Num(); y++) {
			vertices.Add(FVector(x*scaleFactor, y*scaleFactor, heightmap[x][y]));
		}
	}
	return vertices;
}

TArray<FVector> generateNormals(TArray<TArray<float>> heightmap) {
	TArray<FVector> normals;
	normals.SetNum(heightmap.Num() * heightmap[0].Num());
	// TODO: Implement
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

// Called when the actor is created
void AProceduralTerrainChunk::PostActorCreated()
{
	TArray<TArray<float>> heightmap = generateHeightmap(100, 100);
	TArray<FVector> vertices = generateVertices(heightmap);
	TArray<int32> triangles = generateTriangles(100, 100);
	TArray<FVector> normals = generateNormals(heightmap);
	TArray<FVector2D> uv0;
	TArray<FLinearColor> colors;
	TArray<FProcMeshTangent> tangents;
	procMesh->CreateMeshSection_LinearColor(0, vertices, triangles, normals, uv0, colors, tangents, true);
}

// Called every frame
void AProceduralTerrainChunk::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

