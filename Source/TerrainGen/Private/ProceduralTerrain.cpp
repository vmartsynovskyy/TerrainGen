// Fill out your copyright notice in the Description page of Project Settings.

#include "ProceduralTerrain.h"


// Sets default values
AProceduralTerrain::AProceduralTerrain()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AProceduralTerrain::BeginPlay()
{
	Super::BeginPlay();

	int seed = rand();
	for (int x = 0; x < 5; x++) {
		for (int y = 0; y < 5; y++) {
			FTransform spawnTransform;
			spawnTransform.SetTranslation(FVector(x*ChunkSize, y*ChunkSize, 0));
			auto chunk = GetWorld()->SpawnActorDeferred<AProceduralTerrainChunk>(AProceduralTerrainChunk::StaticClass(), spawnTransform, this, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
			int hMapLen = chunk->SetSizeAndResolution(ChunkSize, ChunkResolution);
			chunk->SetXandYStart(x*(hMapLen-1), y*(hMapLen-1));
			chunk->Seed = seed;
			chunk->FinishSpawning(spawnTransform);
		}
	}
}

// Called every frame
void AProceduralTerrain::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

