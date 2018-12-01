// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "World/Map/WorldMap.h"
#include "CellularAutomata.generated.h"

/**
 * 
 */
UCLASS()
class BALLS_DEEP_SS_API UCellularAutomata : public UObject
{
	GENERATED_BODY()

public:
	UCellularAutomata();
	UCellularAutomata(TArray<int32> born, TArray<int32> survive);
	~UCellularAutomata();

	void FillWithRandomNoise(UWorldMap* Map, float balance = 0.5f);
	void Smooth(UWorldMap*& Map, int32 Iterations);
	void SmoothOnce(UWorldMap*& Map);

	UPROPERTY()
	TArray<int32> Born;

	UPROPERTY()
	TArray<int32> Survive;

private:
	int32 GetAliveNeighbors(TArray<UTile*>* Grid, int32 Width, int32 X, int32 Y);
};
