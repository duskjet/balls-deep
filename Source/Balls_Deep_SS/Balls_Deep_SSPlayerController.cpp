// Fill out your copyright notice in the Description page of Project Settings.

#include "Balls_Deep_SSPlayerController.h"
#include "Engine.h"
#include "Engine/EngineTypes.h"

ABalls_Deep_SSPlayerController::ABalls_Deep_SSPlayerController()
{
	bShowMouseCursor = true;
	bEnableClickEvents = true;
	bEnableMouseOverEvents = true;
	//bool GetHitResultUnderCursorForObjects(const TArray<TEnumAsByte<EObjectTypeQuery> > & ObjectTypes, bool bTraceComplex, FHitResult& HitResult) const;
	//bool WasInputKeyJustPressed(FKey Key) const;
}

ABalls_Deep_SSPlayerController::~ABalls_Deep_SSPlayerController()
{
}

bool ABalls_Deep_SSPlayerController::InputKey(FKey Key, EInputEvent EventType, float AmountDepressed, bool bGamepad)
{
	Super::InputKey(Key, EventType, AmountDepressed, bGamepad);

	if (EventType == EInputEvent::IE_Released) 
	{
		if (Key.GetFName() == FName("LeftMouseButton")) 
		{
			TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
			ObjectTypes.Add(EObjectTypeQuery::ObjectTypeQuery1);
			ObjectTypes.Add(EObjectTypeQuery::ObjectTypeQuery2);

			FHitResult HitResult;

			GetHitResultUnderCursorForObjects(ObjectTypes, true, HitResult);

			ATilemap* tilemap = Cast<ATilemap>(HitResult.Actor);
			if (tilemap)
			{
				Server_SetTile(HitResult.Location, tilemap);
			}
		}
	}

	return false;
}

void ABalls_Deep_SSPlayerController::ClientLoadWorldMap_Implementation(UWorldMap * Map)
{
	if (Map) {
		UE_LOG(LogTemp, Log, TEXT("[CLIENT] Server sent a map %dx%d"), Map->Width, Map->Height);
	}
	else {
		UE_LOG(LogTemp, Log, TEXT("[CLIENT] Server sent no map!"));
	}

	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATilemapDirector::StaticClass(), FoundActors);

	if (FoundActors.Num() > 0)
	{
		UE_LOG(LogTemp, Log, TEXT("[CLIENT] Found TilemapDirector."));

		auto director = Cast<ATilemapDirector>(FoundActors[0]);
		director->Map = Map;
	}
	else
		UE_LOG(LogTemp, Log, TEXT("[CLIENT] Could not find any Tilemap Director Actors."));
}

void ABalls_Deep_SSPlayerController::Server_SetTile_Implementation(const FVector ClickWorldPos, ATilemap* Tilemap)
{
	Tilemap->Multicast_SetTile(ClickWorldPos);
}

bool ABalls_Deep_SSPlayerController::Server_SetTile_Validate(const FVector ClickWorldPos, ATilemap* Tilemap)
{
	return true;
}

