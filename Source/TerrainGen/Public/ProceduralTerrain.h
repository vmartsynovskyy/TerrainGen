// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "FastNoise.h"
#include "ChunkInfo.h"

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
	float ChunkSize = 256.0f;
	UPROPERTY(EditAnywhere)
	float HeightScale = 256.0f;
	UPROPERTY(EditAnywhere)
	float WidthScale = 1.0f;
	UPROPERTY(EditAnywhere)
	int ChunkResolution = 4;
	UPROPERTY(EditAnywhere)
	float RenderRadius = 10000.0f;
	UPROPERTY(EditAnywhere)
	UMaterialInterface* TerrainMaterial;
	UPROPERTY(EditAnywhere)
	UCurveFloat* TerrainCurve;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
private:
	TMap<TPair<int, int>, UProceduralMeshComponent*> chunkMap;
	FastNoise noiseGen;
	int seed;
	int heightmapLen = 0;
	void spawnChunk(int x, int y);
	void cullAndSpawnChunks(FVector2D playerLocation);
};
