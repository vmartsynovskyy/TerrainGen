// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProceduralMeshComponent.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralTerrainChunk.generated.h"

UCLASS()
class TERRAINGEN_API AProceduralTerrainChunk : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProceduralTerrainChunk();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void PostActorCreated() override;
	
	UPROPERTY(VisibleAnywhere)
	UProceduralMeshComponent* procMesh;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
private:
	TArray<TArray<float>> generateHeightmap(int width, int height);
	TArray<FVector> generateVertices(TArray<TArray<float>> heightmap);
	TArray<int32> generateTriangles(int32 width, int32 height);
};
