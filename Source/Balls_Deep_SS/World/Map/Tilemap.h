// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/Classes/Components/BoxComponent.h"
#include "Paper2D/Classes/PaperTileMapComponent.h"
#include "Tilemap.generated.h"

UCLASS()
class BALLS_DEEP_SS_API ATilemap : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATilemap();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	UBoxComponent* Box;

	UPROPERTY(EditAnywhere)
	UPaperTileMapComponent* Map;
	
public:
	UFUNCTION(BlueprintCallable)
	void Create(const int32 TileCount = 32, const int32 TileSize = 8, const float PixelsPerUnrealUnit = 0.25f);

	UFUNCTION()
	void HandleClick(AActor* TouchedActor, FKey ButtonPressed);
private:

public:
	int32 GetTileCount() { return TileCount; }
	int32 GetTileSize() { return TileSize; }
	float GetPixelsPerUnrealUnit() { return PixelsPerUnrealUnit; }

private:
	int32 TileCount;
	int32 TileSize;
	float PixelsPerUnrealUnit;
	float TileSizeActual;
};
