// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "FastNoise.h"
#include "ChunkInfo.h"
#include "ChunkInfoWorker.h"

#include "ProceduralMeshComponent.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine.h"
#include "ProceduralTerrain.generated.h"

UCLASS()
class TERRAINGEN_API AProceduralTerrain : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProceduralTerrain();

	UPROPERTY(EditAnywhere)
	float ChunkSize = 256.0f; ///< The size of a chunk in world units
	UPROPERTY(EditAnywhere)
	float HeightScale = 256.0f; ///< The height scaling constant used to scale heightmap form -1 to 1 to -HeightScale to HeightScale
	UPROPERTY(EditAnywhere)
	float WidthScale = 1.0f; ///< A multiplier for chunk width
	UPROPERTY(EditAnywhere)
	int ChunkResolution = 4; ///< The number of heightmap entries to go up by for each vertex
	UPROPERTY(EditAnywhere)
	float RenderRadius = 10000.0f; ///< The radius around which to render chunks in world units
	UPROPERTY(EditAnywhere)
	UMaterialInterface* TerrainMaterial; ///< The material to be used for the terrain
	UPROPERTY(EditAnywhere)
	UCurveFloat* TerrainCurve; ///< A function that will transform heightmap values

	FCriticalSection ChunkDeletion;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	ChunkGenParams GetParams();
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
private:
	TMap<TPair<int, int>, ChunkInfo> infoMap;
	TMap<TPair<int, int>, UProceduralMeshComponent*> chunkMap;
	FastNoise noiseGen;
	int seed;
	int heightmapLen = 0;
	void spawnChunk(int x, int y);
	void cullAndSpawnChunks(FVector2D playerLocation);
	ChunkInfoWorker infoWorker;
	FRunnableThread* infoWorkerThread;
};
