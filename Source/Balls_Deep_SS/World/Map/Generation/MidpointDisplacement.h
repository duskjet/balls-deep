// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "World/Map/WorldMap.h"

/**
 * 
 */
class BALLS_DEEP_SS_API MidpointDisplacement
{
public:
	MidpointDisplacement();
	~MidpointDisplacement();

	static TArray<FIntPoint> CreateHorizon(int32 Width, int32 Height, float Range, float Roughness, int32 Steps);

private:
	static void DisplacePoints(TArray<FIntPoint>* Line, float Range, float Roughness, int32 Steps);
	static FIntPoint GetMidpoint(FIntPoint StartPoint, FIntPoint EndPoint, int32 HeightOffset);
};
