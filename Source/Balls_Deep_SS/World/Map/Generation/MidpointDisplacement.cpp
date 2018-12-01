// Fill out your copyright notice in the Description page of Project Settings.

#include "MidpointDisplacement.h"
#include "Engine.h"

MidpointDisplacement::MidpointDisplacement()
{
}

MidpointDisplacement::~MidpointDisplacement()
{
}

TArray<FIntPoint> MidpointDisplacement::CreateHorizon(UWorldMap * Map, int32 Width, int32 Height, float Range, float Roughness, int32 Steps)
{
	TArray<FIntPoint> line;

	// Add start
	line.Add(FIntPoint(0, Height / 2));
	// Add end
	line.Add(FIntPoint(Width - 1, Height / 2));

	DisplacePoints(&line, Range, Roughness, Steps);

	UE_LOG(WorldGen, Log, TEXT("Returning Line with (%d) elements"),
		   line.Num());

	return line;
}

void MidpointDisplacement::DisplacePoints(TArray<FIntPoint>* Line, float Range, float Roughness, int32 Steps)
{
	UE_LOG(WorldGen, Log, TEXT("Displacing points in Line with (%d) elements"),
		   Line->Num());

	for (int i = 0; i < Line->Num() - 1; i += 2)
	{
		float heightOffset = FMath::FRandRange(-Range, Range);

		FIntPoint midpoint = GetMidpoint((*Line)[i], (*Line)[i + 1], (int32)heightOffset);

		Line->Insert(midpoint, i + 1);
	}

	// Reduce height offset for the next step;
	Range *= Roughness;

	// Continue recursion
	Steps--;
	if (Steps > 0) DisplacePoints(Line, Range, Roughness, Steps);
}

FIntPoint MidpointDisplacement::GetMidpoint(FIntPoint StartPoint, FIntPoint EndPoint, int32 HeightOffset)
{
	int32 x = (StartPoint.X + EndPoint.X) / 2;
	int32 y = (StartPoint.Y + EndPoint.Y) / 2 + HeightOffset;

	return FIntPoint(x, y);
}
