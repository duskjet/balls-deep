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
		Generate(NeighbourCountToBorn, NeighbourCountToSurvive);
		SmoothMap();
		SmoothMap();
		CreateHorizon();
		RebuildCollision();

		FVector2D SuggestedPlayerStart = GetPossiblePlayerStartLocation();
		
		UE_LOG(WorldGen, Log, TEXT("World generated. Broadcasting suggested PlayerStart Vector: %s."), *SuggestedPlayerStart.ToString());
		DoneEvent.Broadcast(SuggestedPlayerStart);
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

FVector2D ATilemapDirector::GetPossiblePlayerStartLocation()
{
	int32 pointsTotal = Horizon.Num();

	float lowerBound = 0.33f;
	float upperBound = 0.66f;
	float addedHeight = 200;
	float randomPos = FMath::FRandRange(lowerBound, upperBound);

	FIntPoint point = Horizon[FMath::RoundToInt(pointsTotal * randomPos)];

	FVector tileLocation = GetTileWorldPosition(point);
	FVector2D finalLocation = FVector2D(tileLocation.X, tileLocation.Z + addedHeight);
	//DrawDebugSphere(GetWorld(), )
	return finalLocation;
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
	int32 tile_x, tile_y;
	UPaperTileMapComponent* tilemap = FindTilemap(FIntPoint(X, Y), tile_x, tile_y);
	
	tilemap->SetTile(tile_x, tile_y, 0, TileInfo);
}

FVector ATilemapDirector::GetTileWorldPosition(const FIntPoint Position)
{
	int32 x, y;
	UPaperTileMapComponent* tilemap = FindTilemap(Position, x, y);

	FVector loc = tilemap->GetComponentTransform().GetLocation();
	loc.X += x * TileFinalSize;
	loc.Z -= y * TileFinalSize;

	return loc;
}

UPaperTileMapComponent * ATilemapDirector::FindTilemap(const FIntPoint Position, int32 & RelativeX, int32 & RelativeY)
{
	int32	x = Position.X / ChunkSize,
			y = Position.Y / ChunkSize;

	// Find tile
	RelativeX = Position.X % ChunkSize;
	// Inverse Y tile pos, 
	// because Tilemap coords start from TOP LEFT corner 
	// (engine starts at BOTTOM LEFT)
	RelativeY = (ChunkSize - 1) - Position.Y % ChunkSize;
	UPaperTileMapComponent* tilemap = *Chunks.Find(FIntPoint(x, y));

	return tilemap;
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

	Horizon = line;
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

	//RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	for (int32 y = 0; y < ChunkHeight; y++)
		for (int32 x = 0; x < ChunkWidth; x++)
		{
			auto pos = FIntPoint(x, y);
			FVector location = FVector(x * TileFinalSize * ChunkSize, 0, y * TileFinalSize * ChunkSize);
			auto tilemap = NewObject<UPaperTileMapComponent>(this,FName(*FString::Printf(TEXT("Tilemap_%d_%d"), x, y)));
			//tilemap->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepWorldTransform);
			tilemap->RegisterComponent();
			tilemap->SetIsReplicated(true);
			tilemap->SetRelativeLocation(location);

			tilemap->CreateNewTileMap(ChunkSize, ChunkSize, TilePixels, TilePixels, PixelsPerUnrealUnit);
			tilemap->MakeTileMapEditable();
			tilemap->RebuildCollision();

			Chunks.Add(pos, tilemap);

			FActorSpawnParameters spawnParams;
			spawnParams.Name = FName(*FString::Printf(TEXT("Tilemap_%d_%d)"), x, y));
			auto beb = GetWorld()->SpawnActor<ATilemap>(location, FRotator(), spawnParams);//<ATilemap>(this, FName(*FString::Printf(TEXT("Tilemap(%d,%d)"), x, y)));

			beb->Create(ChunkSize, TilePixels, PixelsPerUnrealUnit);
			//beb->Map->CreateNewTileMap(ChunkSize, ChunkSize, TilePixels, TilePixels, PixelsPerUnrealUnit);
			//beb->Map->MakeTileMapEditable();
			beb->Tags.Add("Tilemap");

			Tilemaps.Add(pos, beb);
		}
}

void ATilemapDirector::RebuildCollision()
{
	for (auto tilemap : Chunks)
	{
		tilemap.Value->RebuildCollision();
	}
}

