// Fill out your copyright notice in the Description page of Project Settings.

#include "CellularAutomata.h"
#include "Engine.h"
//PRAGMA_DISABLE_OPTIMIZATION
UCellularAutomata::UCellularAutomata()
{
}

UCellularAutomata::UCellularAutomata(TArray<int32> born, TArray<int32> survive)
{
	Born = born;
	Survive = survive;
}

UCellularAutomata::~UCellularAutomata()
{
}

void UCellularAutomata::FillWithRandomNoise(UWorldMap * Map, float balance)
{
	int32 width = Map->Width;
	int32 height = Map->Height;

	for (int32 y = 0; y < height; y++)
		for (int32 x = 0; x < width; x++)
		{
			float random = FMath::RandRange(0.0f, 1.0f);
			bool cell = random > balance;

			Map->Grid[width * y + x]->bSolid = cell;//FMath::RandBool();
		}
}

void UCellularAutomata::Smooth(UWorldMap * Map, int32 Iterations)
{
	for (int32 iteration = 0; iteration < Iterations; iteration++) {
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Current iteration: %d"), iteration));
		SmoothOnce(Map);
	}
}

void UCellularAutomata::SmoothOnce(UWorldMap * Map)
{
	int32 width = Map->Width;
	int32 height = Map->Height;

	TArray<UTile*> gridCopy;
	gridCopy.Init(NewObject<UTile>(), Map->Grid.Num());

	for (int32 y = 1; y < height - 1; y++)
		for (int32 x = 1; x < width - 1; x++)
		{
			int32 alive = GetAliveNeighbors(&(Map->Grid), width, x, y);

			/*if (Map->Grid[width * y + x]) {
				gridCopy[width * y + x] = Survive.Contains(alive);
			}
			else {
				gridCopy[width * y + x] = Born.Contains(alive);
			}*/

			gridCopy[width * y + x]->bSolid = // sometimes crashes
				(!Map->Grid[width * y + x]->bSolid && Survive.Contains(alive))
				|| (Map->Grid[width * y + x]->bSolid && Born.Contains(alive));
		}
	
	Map->Grid = gridCopy;
}

int32 UCellularAutomata::GetAliveNeighbors(TArray<UTile*>* Grid, int32 Width, int32 X, int32 Y)
{
	bool cells[8] =
	{
		(*Grid)[Width * (Y - 1) + (X)]->bSolid,		// top
		(*Grid)[Width * (Y - 1) + (X + 1)]->bSolid,	// top right
		(*Grid)[Width * (Y)+(X + 1)]->bSolid,		// right
		(*Grid)[Width * (Y + 1) + (X + 1)]->bSolid,	// bot right
		(*Grid)[Width * (Y + 1) + (X)]->bSolid,		// bot
		(*Grid)[Width * (Y + 1) + (X - 1)]->bSolid,	// bot left
		(*Grid)[Width * (Y)+(X - 1)]->bSolid,		// left
		(*Grid)[Width * (Y - 1) + (X - 1)]->bSolid,	// top left
	};

	int32 alive = 0;
	for (bool cell : cells)
		if (cell) alive++;

	return alive;
}
