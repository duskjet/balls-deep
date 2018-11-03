// Fill out your copyright notice in the Description page of Project Settings.

#include "TilemapDirector.h"
#include "Engine.h"
//PRAGMA_DISABLE_OPTIMIZATION
// Sets default values
ATilemapDirector::ATilemapDirector()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

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
	map->Width = width;
	map->Height = height;
	map->Grid.Init(NewObject<UTile>(), width * height);

	automata->FillWithRandomNoise(map, CellRatio);
	//Beb(&map);
	Map = map;

	FillWorldMapMono();
}

void ATilemapDirector::Beb(UWorldMap* map) {
	
	for (int32 y = 0; y < map->Height; y++)
		for (int32 x = 0; x < map->Width; x++)
		{
			if (x == 0 || y == 0 || (x == map->Width -1) || y == (map->Height - 1))
				map->Grid[x + map->Width * y]->bSolid = true;
		}
}

void ATilemapDirector::FillWorldMapMono(int32 XOffset, int32 YOffset)
{
	check(Tileset != nullptr);

	int32 alive = 0;
	int32 dead = 0;

	for (int32 x = 0; x < Map->Width; x++)
		for (int32 y = 0; y < Map->Height; y++)
		{
			// Find tilemap
			int32 chunk_x = x / ChunkSize,
				  chunk_y = y / ChunkSize;
			auto pos = FIntPoint(chunk_x, chunk_y);
			auto tilemap = Chunks.Find(pos);

			check(Chunks.Num() > 0);
			checkf(tilemap != nullptr, TEXT("XY: (%d,%d), Chunk: (%d,%d)"), x, y, chunk_x, chunk_y);

			// Find tile
			int32 tile_x = x % ChunkSize,
			// Inverse Y tile pos, 
			// because Tilemap coords start from TOP LEFT corner 
			// (engine starts at BOTTOM LEFT)
				  tile_y = (ChunkSize - 1) - y % ChunkSize; 

			// Set tile
			FPaperTileInfo LocalTileInfo;
			LocalTileInfo.TileSet = Tileset;
			LocalTileInfo.PackedTileIndex = 0;

			if (Map->Grid[x + Map->Width * y]->bSolid) {
				alive++;
				(*tilemap)->SetTile(tile_x, tile_y, 0, LocalTileInfo);
			}
			else {
				FPaperTileInfo NoTileInfo;
				LocalTileInfo.TileSet = Tileset;
				LocalTileInfo.PackedTileIndex = 1;
				dead++;
				(*tilemap)->SetTile(tile_x, tile_y, 0, NoTileInfo);
			}

		}

	if (GEngine) 
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, 
												  FString::Printf(TEXT("alive: %d, dead: %d"), alive, dead));
}

void ATilemapDirector::SmoothMap()
{
	automata->SmoothOnce(Map);
	FillWorldMapMono();
}

TArray<UTileGroup*> ATilemapDirector::CreateGroups()
{
	TArray<UTileGroup*> groups;

	for(int32 y = 0; y < Map->Height; y++)
		for (int32 x = 0; x < Map->Width; x++)
		{
			UTile* tile = Map->Grid[Map->Width * y + x];

			int32 endPosX, endPosY;

			// Create new group
			if (!tile->bSolid && !tile->HasGroup())
			{
				UTileGroup* group = NewObject<UTileGroup>();
				group->Init(tile, Map);

				groups.Add(group);
			}
		}


	return groups;
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

