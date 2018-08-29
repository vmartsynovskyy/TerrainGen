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

// Called when the actor is created
void AProceduralTerrainChunk::PostActorCreated()
{
	FVector topLeft = FVector(0.0, 0.0, 0.0);
	FVector bottomLeft = FVector(0.0, 100.0, 0.0);
	FVector bottomRight = FVector(100.0, 100.0, 0.0);
	FVector topRight = FVector(100.0, 0.0, 0.0);
	TArray<FVector> vertices = {topLeft, bottomLeft, bottomRight, topRight};
	TArray<int32> triangles = {0, 1, 2, 0, 2, 3};
	TArray<FVector> normals;
	TArray<FVector2D> uv0;
	TArray<FLinearColor> colors;
	TArray<FProcMeshTangent> tangents;
	procMesh->CreateMeshSection_LinearColor(0, vertices, triangles, normals, uv0, colors, tangents, false);
}

// Called every frame
void AProceduralTerrainChunk::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

