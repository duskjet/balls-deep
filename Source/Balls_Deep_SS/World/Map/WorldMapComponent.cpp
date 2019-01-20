// Fill out your copyright notice in the Description page of Project Settings.

#include "WorldMapComponent.h"
#include "UnrealNetwork.h"

// Sets default values for this component's properties
UWorldMapComponent::UWorldMapComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	//bReplicates = true;
	// ...
}


// Called when the game starts
void UWorldMapComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

//bool UWorldMapComponent::IsSupportedForNetworking() const
//{
//	return true;
//}
//
//void UWorldMapComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
//{
//	//Super::GetLifetimeReplicatedProps(OutLifetimeProps);
//	DOREPLIFETIME(UWorldMapComponent, Grid);
//	DOREPLIFETIME(UWorldMapComponent, Width);
//	DOREPLIFETIME(UWorldMapComponent, Height);
//}

// Called every frame
void UWorldMapComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UWorldMapComponent::Init(int32 Width, int32 Height)
{
	this->Width = Width;
	this->Height = Height;

	Grid.SetNum(Width * Height);

	for (int32 y = 0; y < Height; y++)
		for (int32 x = 0; x < Width; x++)
		{
			UTile* Tile = NewObject<UTile>();
			Tile->Pos = FIntPoint(x, y);

			Grid[Width * y + x] = Tile;
		}
}

TArray<UTileArea*> UWorldMapComponent::GetGroups()
{
	TArray<UTileArea*> groups;

	for (int32 y = 0; y < Height; y++)
		for (int32 x = 0; x < Width; x++)
		{
			UTile* tile = Grid[Width * y + x];

			// Create new group
			if (!tile->bSolid && !tile->HasGroup())
			{
				UTileArea* group = NewObject<UTileArea>();
				group->Init(tile, this);

				groups.Add(group);
			}
		}

	ClearTileIndexes();

	return groups;
}

UWorldMapComponent* UWorldMapComponent::Copy()
{
	UWorldMapComponent* NewMap = NewObject<UWorldMapComponent>();

	NewMap->Width = Width;
	NewMap->Height = Height;

	NewMap->Grid.SetNum(Grid.Num());

	for (int32 y = 0; y < Height; y++)
		for (int32 x = 0; x < Width; x++)
		{
			const int32 index = Width * y + x;
			UTile* TileCopy = DuplicateObject<UTile>(Grid[index], Grid[index]->GetOuter());

			NewMap->Grid[index] = TileCopy;
		}

	return NewMap;
}

void UWorldMapComponent::ClearTileIndexes()
{
	for (UTile* tile : Grid)
	{
		tile->Index = UTile::DEFAULT_INDEX;
	}
}
