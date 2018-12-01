// Fill out your copyright notice in the Description page of Project Settings.

#include "TilemapDirector.h"
#include "TileArea.h"
#include "Engine.h"
PRAGMA_DISABLE_OPTIMIZATION
// Sets default values
ATilemapDirector::ATilemapDirector()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ProductionMode = true;

	ChunkWidth = 4;
	ChunkHeight = 4;
	ChunkSize = 32;
	CellRatio = 0.5f;
}

void ATilemapDirector::OnConstruction(const FTransform& Transform)
{
	/*if (Chunks.Num() > 0)
		Chunks.Empty();*/

	//CreateChunks(ChunkWidth, ChunkHeight, ChunkSize);
}

// Called when the game starts or when spawned
void ATilemapDirector::BeginPlay()
{
	Super::BeginPlay();
	
	CreateChunks();

	if (ProductionMode)
	{
		Generate(TArray<int32>({ 5, 6, 7, 8 }), TArray<int32>({ 7,8 }));
		SmoothMap();
		SmoothMap();
		CreateHorizon();
	}
}

// Called every frame
void ATilemapDirector::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATilemapDirector::Generate(TArray<int32> Born, TArray<int32> Survive)
{
	automata = NewObject<UCellularAutomata>();
	automata->Born = Born;
	automata->Survive = Survive;

	int32 width = ChunkWidth * ChunkSize;
	int32 height = ChunkHeight * ChunkSize;
	UWorldMap* map = NewObject<UWorldMap>();
	map->Init(width, height);

	automata->FillWithRandomNoise(map, CellRatio);
	//GenerateDebugFrame(&map);
	Map = map;

	Draw();
}

void ATilemapDirector::GenerateDebugFrame(UWorldMap* map) {
	
	for (int32 y = 0; y < map->Height; y++)
		for (int32 x = 0; x < map->Width; x++)
		{
			if (x == 0 || y == 0 || (x == map->Width -1) || y == (map->Height - 1))
				map->Grid[x + map->Width * y]->bSolid = true;
		}
}

void ATilemapDirector::Draw(int32 XOffset, int32 YOffset)
{
	check(Tileset != nullptr);

	int32 alive = 0;
	int32 dead = 0;

	for (int32 x = 0; x < Map->Width; x++)
		for (int32 y = 0; y < Map->Height; y++)
		{
			//check(Chunks.Num() > 0);
			//checkf(tilemap != nullptr, TEXT("XY: (%d,%d), Chunk: (%d,%d)"), x, y, chunk_x, chunk_y);

			// Set tile
			FPaperTileInfo LocalTileInfo = GetSolidTile();
			FPaperTileInfo NoTileInfo = GetEmptyTile();

			if (Map->Grid[x + Map->Width * y]->bSolid) {
				alive++;
				DrawTile(x, y, LocalTileInfo);
			}
			else {
				dead++;
				DrawTile(x, y, NoTileInfo);
			}

		}

	if (GEngine) 
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, 
												  FString::Printf(TEXT("alive: %d, dead: %d"), alive, dead));
}

void ATilemapDirector::DrawTile(FIntPoint Position, FPaperTileInfo TileInfo)
{
	DrawTile(Position.X, Position.Y, TileInfo);
}

void ATilemapDirector::DrawTile(int32 X, int32 Y, FPaperTileInfo TileInfo)
{
	int32	chunk_x = X / ChunkSize,
		chunk_y = Y / ChunkSize;

	// Find tile
	int32 tile_x = X % ChunkSize,
		// Inverse Y tile pos, 
		// because Tilemap coords start from TOP LEFT corner 
		// (engine starts at BOTTOM LEFT)
		tile_y = (ChunkSize - 1) - Y % ChunkSize;

	UPaperTileMapComponent* tilemap = *Chunks.Find(FIntPoint(chunk_x, chunk_y));

	tilemap->SetTile(tile_x, tile_y, 0, TileInfo);
}

void ATilemapDirector::SmoothMap()
{
	automata->SmoothOnce(Map);

	//FillEmptySpaces(3);

	Draw();

	/*for (auto tilemap : Chunks)
	{
		tilemap.Value->RebuildCollision();
	}*/
}

void ATilemapDirector::FillEmptySpaces(int32 VolumeThreshold, float Ratio)
{
	for (UTileArea* group : Map->GetGroups())
	{
		if (group->Volume <= VolumeThreshold && FMath::FRand() <= Ratio)
		{
			for (UTile* tile : group->Tiles)
				tile->bSolid = true;
		}
	}
}

void ATilemapDirector::CreateHorizon()
{
	TArray<FIntPoint> line = MidpointDisplacement::CreateHorizon(Map, Map->Width, Map->Height, 20, 0.5, 10);

	for (FIntPoint pos : line) 
	{
		DrawTile(pos, GetSolidTile());

		// Remove tiles on top of it
		for (UTile* tile : Map->Grid)
			if (tile->Pos.X == pos.X && tile->Pos.Y > pos.Y)
				DrawTile(tile->Pos, GetEmptyTile());
	}

	// Remove areas above horizon line
	/*for (UTileArea* area : Map->GetGroups())
	{
		auto point = line.FindByPredicate([&area](const FIntPoint& point) {
			return point.X == area->StartPoint.X;
		});
	}*/
}

void ATilemapDirector::CreateChunks()
{
	//USceneComponent* Root = CreateDefaultSubobject<USceneComponent>("RootComponent");
	//Root->SetupAttachment(RootComponent);

	int32 tileSize = 8;
	float pixelsPerUnrealUnit = 0.25f;
	int32 tileSizeWorld = 8 / pixelsPerUnrealUnit;

	for (int32 y = 0; y < ChunkHeight; y++)
		for (int32 x = 0; x < ChunkWidth; x++)
		{
			auto pos = FIntPoint(x, y);
			auto tilemap = NewObject<UPaperTileMapComponent>(this,FName(*FString::Printf(TEXT("Tilemap_%d_%d"), x, y)));
			//tilemap->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepWorldTransform);

			tilemap->RegisterComponent();
			tilemap->SetRelativeLocation(FVector(x * tileSizeWorld * ChunkSize,
										 0,
										 y * tileSizeWorld * ChunkSize));

			tilemap->CreateNewTileMap(ChunkSize, ChunkSize, tileSize, tileSize, pixelsPerUnrealUnit);
			tilemap->MakeTileMapEditable();
			tilemap->RebuildCollision();
			
			Chunks.Add(pos, tilemap);
		}
}

