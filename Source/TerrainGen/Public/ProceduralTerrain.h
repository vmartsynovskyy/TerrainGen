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

	/// The size of a chunk in world units
	UPROPERTY(EditAnywhere)
	float ChunkSize = 4096.0f;
	/// The scale ratio of height to width
	UPROPERTY(EditAnywhere)
	float HeightToWidthRatio = 1.25f;
	/// The scale factor of the terrain
	UPROPERTY(EditAnywhere)
	float Scale = 256.0f;
	/// The number of samples of the heightmap per 256 world units
	UPROPERTY(EditAnywhere)
	int ChunkResolution = 8;
	/// The radius around which to render chunks in world units
	UPROPERTY(EditAnywhere)
	float RenderRadius = 30000.0f;
	/// The material to be used for the terrain
	UPROPERTY(EditAnywhere)
	UMaterialInterface* TerrainMaterial;
	/// A function that will transform heightmap values
	UPROPERTY(EditAnywhere)
	UCurveFloat* TerrainCurve;

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
	/// The height scaling constant used to scale heightmap form -1 to 1 to -HeightScale to HeightScale
	float heightScale;
	/// A multiplier for chunk width
	float frequency;
	ChunkInfoWorker infoWorker;
	FRunnableThread* infoWorkerThread;
};
