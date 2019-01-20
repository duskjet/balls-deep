// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Generation/CellularAutomata.h"
#include "Generation/MidpointDisplacement.h"

#include "PaperTileMapComponent.h"
#include "Tilemap.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TilemapDirector.generated.h"

class ATilemap;

UCLASS()
class BALLS_DEEP_SS_API ATilemapDirector : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATilemapDirector();

public:
	/** Broadcasts whenever the layer changes */
	DECLARE_EVENT_OneParam(ATilemapDirector, FDoneEvent, FVector2D)
	FDoneEvent& OnDone() { return DoneEvent; }

private:
	/** Broadcasts whenever the layer changes */
	FDoneEvent DoneEvent;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(TArray< class FLifetimeProperty > & OutLifetimeProps) const override;

public:
	UPROPERTY(EditAnywhere, Category = MapSize)
		int32 ChunkWidth;

	UPROPERTY(EditAnywhere, Category = MapSize)
		int32 ChunkHeight;

	UPROPERTY(EditAnywhere, Category = MapSize)
		int32 ChunkSize;

	UPROPERTY(EditAnywhere)
		float CellRatio;

	UPROPERTY(EditAnywhere, Category = CellularAutomata)
		TArray<int32> NeighbourCountToBorn;

	UPROPERTY(EditAnywhere, Category = CellularAutomata)
		TArray<int32> NeighbourCountToSurvive;

	UPROPERTY(EditAnywhere, Category = TilemapSettings)
		int32 TilePixels = 8;

	UPROPERTY(EditAnywhere, Category = TilemapSettings)
		float PixelsPerUnrealUnit = 0.25f;

	UPROPERTY(EditAnywhere, Category = TilemapSettings)
		int32 TileFinalSize = TilePixels / PixelsPerUnrealUnit;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void OnConstruction(const FTransform& Transform) override;

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_Generate();

	UFUNCTION(NetMulticast, Reliable)
	void Replicate_Map(const TArray<UTile*>& Map);

	UFUNCTION(BlueprintCallable)
	void CreateCaves(TArray<int32> Born, TArray<int32> Survive);
	
	UFUNCTION(BlueprintCallable)
	void Draw(int32 XOffset = 0, int32 YOffset = 0);
	void Draw(const UWorldMap* InMap, int32 XOffset = 0, int32 YOffset = 0);

	UFUNCTION(BlueprintCallable)
	void DrawTile(FIntPoint Position, FPaperTileInfo TileInfo);
	void DrawTile(int32 X, int32 Y, FPaperTileInfo TileInfo);

	UFUNCTION(BlueprintCallable)
	FVector GetTileWorldPosition(const FIntPoint Position);

	UFUNCTION(BlueprintCallable)
	UPaperTileMapComponent* FindTilemap(const FIntPoint Position, int32& RelativeX, int32& RelativeY);

	UFUNCTION(BlueprintCallable)
	void SmoothMap();

	UFUNCTION(BlueprintCallable)
	void FillEmptySpaces(int32 VolumeThreshold, float Ratio = 1.0f);

	UFUNCTION(BlueprintCallable)
	void CreateHorizon();

	// Production Mode
	UPROPERTY(EditAnywhere)
	bool ProductionMode;

	// Map Grid
	UPROPERTY(EditAnywhere, Replicated)
	UWorldMap* Map;

	// Horizon line data
	TArray<FIntPoint> Horizon;

	UPROPERTY(EditAnywhere)
	TArray<UTileArea*> TileGroups;

	UPROPERTY(EditAnywhere)
	TMap<FIntPoint, ATilemap*> Tilemaps;

	UPROPERTY(EditAnywhere, Category = World)
	UPaperTileSet* Tileset;

	UFUNCTION(BlueprintCallable)
	void CreateChunks();

	UFUNCTION(BlueprintCallable)
	void RebuildCollision();

private:
	void GenerateDebugFrame(UWorldMapComponent* map);
	UCellularAutomata* automata;

	FVector GetPossiblePlayerStartLocation();

public:
	FPaperTileInfo GetEmptyTile() 
	{
		FPaperTileInfo empty;
		empty.TileSet = Tileset;
		empty.PackedTileIndex = 1;

		return empty;
	}

	FPaperTileInfo GetSolidTile()
	{
		FPaperTileInfo solid;
		solid.TileSet = Tileset;
		solid.PackedTileIndex = 0;

		return solid;
	}
};
