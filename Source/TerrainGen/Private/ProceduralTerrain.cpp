// Fill out your copyright notice in the Description page of Project Settings.

#include "ProceduralTerrain.h"


// Sets default values
AProceduralTerrain::AProceduralTerrain()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	seed = rand();
}

// Called when the game starts or when spawned
void AProceduralTerrain::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AProceduralTerrain::Tick(float DeltaTime)
{
	FVector playerPos = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();
	cullAndSpawnChunks(FVector2D(playerPos));
	Super::Tick(DeltaTime);
}

void AProceduralTerrain::spawnChunk(int x, int y) {
	// first, check if this chunk has already been spawned
	TPair<int, int> chunkPos(x, y);
	if (!chunkMap.Contains(chunkPos)) {
		FTransform spawnTransform;
		spawnTransform.SetTranslation(FVector(x*ChunkSize, y*ChunkSize, 0));
		auto chunk = GetWorld()->SpawnActorDeferred<AProceduralTerrainChunk>(AProceduralTerrainChunk::StaticClass(), spawnTransform, this, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
		int hMapLen = chunk->SetSizeAndResolution(ChunkSize, ChunkResolution);
		chunk->SetXandYStart(x*(hMapLen - 1), y*(hMapLen - 1));
		chunk->Seed = seed;
		chunk->SetMaterialForProcMesh(TerrainMaterial);
		chunk->FinishSpawning(spawnTransform);
		chunkMap.Add(TPair<int, int>(x, y), chunk);
	}
}

void AProceduralTerrain::cullAndSpawnChunks(FVector2D playerLocation) {
	float renderRadiusSquare = powf(RenderRadius, 2);

	// spawn chunks
	int xStart = (playerLocation.X - RenderRadius) / ChunkSize;
	int xEnd = (playerLocation.X + RenderRadius) / ChunkSize;
	int yStart = (playerLocation.Y - RenderRadius) / ChunkSize;
	int yEnd = (playerLocation.Y + RenderRadius) / ChunkSize;
	// iterate through all chunks in (playerLocation.X - RenderRadius, playerLocation.X + RenderRadius)
	for (int x = xStart; x < xEnd; x++) {
		// iterate through all chunks in (playerLocation.Y - RenderRadius, playerLocation.Y + RenderRadius)
		for (int y = yStart; y < yEnd; y++) {
			// check that the chunk is within radius
			float distanceFromPlayerSquare = powf((x*ChunkSize - playerLocation.X), 2) + powf((y*ChunkSize - playerLocation.Y), 2);
			if (distanceFromPlayerSquare < renderRadiusSquare) {
				spawnChunk(x, y);
			}
		}
	}

	// cull chunks
	for (auto& Elem : chunkMap) {
		// pythagoras
		float distanceFromPlayerSquare = powf((Elem.Key.Key*ChunkSize - playerLocation.X), 2) + powf((Elem.Key.Value*ChunkSize - playerLocation.Y), 2);
		// destroy chunk if its further than the render radius
		if (distanceFromPlayerSquare > renderRadiusSquare) {
			Elem.Value->Destroy();
			chunkMap.Remove(TPair<int, int>(Elem.Key.Key, Elem.Key.Value));
		}
	}
}
