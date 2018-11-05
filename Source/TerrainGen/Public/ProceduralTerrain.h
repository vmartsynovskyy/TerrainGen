// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine.h"
#include "ProceduralTerrainChunk.h"
#include "ProceduralTerrain.generated.h"

UCLASS()
class TERRAINGEN_API AProceduralTerrain : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProceduralTerrain();

	float ChunkSize = 256.0f;
	int ChunkResolution = 4;
	int Seed;

	float RenderRadius = 10000.0f;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
private:
	TMap<TPair<int, int>, AProceduralTerrainChunk*> chunkMap;
	void spawnChunk(int x, int y);
	void destroyChunk(AProceduralTerrainChunk* chunk);
	void cullAndSpawnChunks(FVector2D playerLocation);
};
