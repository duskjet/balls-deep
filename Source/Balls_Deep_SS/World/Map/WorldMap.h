// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Balls_Deep_SS.h"
#include "TileArea.h"
#include "WorldMap.generated.h"

class UTileArea;
/**
 * 
 */
UCLASS(BlueprintType)
class BALLS_DEEP_SS_API UTile : public UObject
{
	GENERATED_BODY()

public:
	UTile()
	{
		bSolid = false;
		Index = DEFAULT_INDEX;
	}

	~UTile() {}

	UPROPERTY(BlueprintReadWrite)
		bool bSolid;

	UPROPERTY(BlueprintReadWrite)
		int32 Index;

	UPROPERTY(BlueprintReadOnly)
		FIntPoint Pos;

	bool HasGroup()
	{
		return Index >= 0;
	}

	static const int32 DEFAULT_INDEX = -1;
};

UCLASS(BlueprintType)
class BALLS_DEEP_SS_API UWorldMap : public UObject
{
	GENERATED_BODY()

public:
	UWorldMap();
	~UWorldMap();

	UPROPERTY(BlueprintReadWrite, Category = Grid)
	TArray<UTile*> Grid;

	UPROPERTY(BlueprintReadWrite, Category = Grid)
	int32 Width;

	UPROPERTY(BlueprintReadWrite, Category = Grid)
	int32 Height;

public:
	void Init(int32 Width, int32 Height);

	TArray<UTileArea*> GetGroups();

	UWorldMap* Copy();

private:
	void ClearTileIndexes();

};