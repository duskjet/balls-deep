// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Generation/CellularAutomata.h"

#include "PaperTileMapComponent.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TilemapDirector.generated.h"

UCLASS()
class BALLS_DEEP_SS_API ATilemapDirector : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATilemapDirector();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void OnConstruction(const FTransform& Transform) override;

	UFUNCTION(BlueprintCallable)
	void Generate(TArray<int32> Born, TArray<int32> Survive);
	
	UFUNCTION(BlueprintCallable)
	void FillWorldMapMono(int32 XOffset = 0, int32 YOffset = 0);

	UFUNCTION(BlueprintCallable)
	void SmoothMap();

	UFUNCTION(BlueprintCallable)
	TArray<UTileGroup*> CreateGroups();

	// Map Grid
	UPROPERTY(EditAnywhere)
	UWorldMap* Map;

	// Chunk Info
	UPROPERTY(EditAnywhere)
	TMap<FIntPoint, UPaperTileMapComponent*> Chunks;

	UPROPERTY(EditAnywhere)
	int32 ChunkWidth;

	UPROPERTY(EditAnywhere)
	int32 ChunkHeight;

	UPROPERTY(EditAnywhere)
	int32 ChunkSize;

	UPROPERTY(EditAnywhere)
	float CellRatio;

	UPROPERTY(EditAnywhere, Category = World)
	UPaperTileSet* Tileset;

	UFUNCTION(BlueprintCallable)
	void CreateChunks();

private:
	void Beb(UWorldMap* map);
	UCellularAutomata* automata;
};
