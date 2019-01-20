// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "World/Map/WorldMap.h"
#include "CellularAutomata.generated.h"

//USTRUCT()
//struct BALLS_DEEP_SS_API FCellularAutomataOptions
//{
//	TArray<int32> Born;
//	TArray<int32> Survive;
//};

/**
 * 
 */
UCLASS()
class BALLS_DEEP_SS_API UCellularAutomata : public UObject
{
	GENERATED_BODY()

public:
	UCellularAutomata();
	UCellularAutomata(TArray<int32> InBorn, TArray<int32> InSurvive);
	~UCellularAutomata();

	void FillWithRandomNoise(UWorldMap* Map, float balance = 0.5f);
	//void Smooth(UWorldMap*& Map, int32 Iterations);
	//void SmoothOnce(UWorldMap*& Map);

	static void FillWithNoise(TArray<UTile*>& InMap, int32 Width, int32 Height, float balance);
	static void Smooth(TArray<UTile*>& InMap, int32 Width, int32 Height, TArray<int32> InBorn, TArray<int32> InSurvive);

	UPROPERTY()
	TArray<int32> Born;

	UPROPERTY()
	TArray<int32> Survive;

private:
	static TArray<UTile*> CopyTileArray(TArray<UTile*>& InMap);
	static int32 GetAliveNeighborsCount(TArray<UTile*>& Grid, int32 Width, int32 X, int32 Y);
};
