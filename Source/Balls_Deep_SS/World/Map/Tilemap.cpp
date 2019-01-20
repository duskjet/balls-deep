// Fill out your copyright notice in the Description page of Project Settings.

#include "Tilemap.h"
#include "Delegate.h"
#include "Engine.h"

// Sets default values
ATilemap::ATilemap()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	bNetLoadOnClient = true;
	
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("HitBox"));
	Box->SetIsReplicated(true);
	Box->AttachToComponent(RootComponent, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));

	Map = CreateDefaultSubobject<UPaperTileMapComponent>(TEXT("Tilemap"));
	Map->SetIsReplicated(true);
	Map->AttachToComponent(RootComponent, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));

}

// Called when the game starts or when spawned
void ATilemap::BeginPlay()
{
	Super::BeginPlay();
	
}

void ATilemap::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	//Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	//DOREPLIFETIME(UWorldMapComponent, Grid);
	//DOREPLIFETIME(UWorldMapComponent, Width);
	//DOREPLIFETIME(UWorldMapComponent, Height);
}

// Called every frame
void ATilemap::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATilemap::Create(ATilemapDirector* Director, const int32 TileCount, const int32 TileSize, const float PixelsPerUnrealUnit)
{
	// Set values
	this->Director = Director;
	this->TileCount = TileCount;
	this->TileSize = TileSize;
	this->PixelsPerUnrealUnit = PixelsPerUnrealUnit;
	this->TileSizeActual = TileSize / PixelsPerUnrealUnit;

	// Create TileMap
	Map->CreateNewTileMap(TileCount, TileCount, TileSize, TileSize, PixelsPerUnrealUnit);
	Map->MakeTileMapEditable();

	// Adjust BoxComponent size and location
	auto bounds = Map->Bounds;
	bounds.BoxExtent.Y = 10;
	Box->SetBoxExtent(bounds.BoxExtent);

	float tilemapCenterPoint = (TileCount * TileSizeActual / 2) - TileSizeActual / 2;
	FVector mapCenter = FVector(tilemapCenterPoint, 0, (-1) * tilemapCenterPoint);
	Box->SetRelativeLocation(mapCenter);

	// Set BoxComponent collision
	// Overlap all channels
	Box->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	// Except Visibility (Block)
	Box->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);

	//OnClicked.AddDynamic(this, &ATilemap::HandleClick);
}

void ATilemap::HandleClick(AActor * TouchedActor, FKey ButtonPressed)
{
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Cyan, FString::Printf(TEXT("Clicked %s on %s"), *ButtonPressed.ToString(), *TouchedActor->GetFName().ToString()));
}

FIntPoint ATilemap::GetTileFromWorldPos(FVector WorldPos)
{
	FVector localPos = GetTransform().InverseTransformPosition(WorldPos) - FVector(TileSizeActual / 2 * -1, 0, TileSizeActual / 2);
	
	int32 x = localPos.X / TileSizeActual;
	int32 y = localPos.Z / TileSizeActual * -1;

	return FIntPoint(x, y);
}

void ATilemap::Multicast_SetTile_Implementation(const FVector ClickWorldPos)
{
	FIntPoint tileCoords = GetTileFromWorldPos(ClickWorldPos);

	UE_LOG(LogTemp, Log, TEXT("Set tile in %s. Click Position: %s"), *this->GetFName().ToString(), *ClickWorldPos.ToString());

	FPaperTileInfo tileInfo = Director->GetSolidTile();

	Map->SetTile(tileCoords.X, tileCoords.Y, 0, tileInfo);
	Map->RebuildCollision();
}

