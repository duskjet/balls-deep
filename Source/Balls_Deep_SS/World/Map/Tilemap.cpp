// Fill out your copyright notice in the Description page of Project Settings.

#include "Tilemap.h"
#include "Delegate.h"
#include "Engine.h"

// Sets default values
ATilemap::ATilemap()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	Box = CreateAbstractDefaultSubobject<UBoxComponent>(TEXT("HitBox"));
	Box->AttachToComponent(RootComponent, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));

	Map = CreateAbstractDefaultSubobject<UPaperTileMapComponent>(TEXT("Tilemap"));
	Map->AttachTo(RootComponent);

}

// Called when the game starts or when spawned
void ATilemap::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATilemap::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATilemap::Create(const int32 TileCount, const int32 TileSize, const float PixelsPerUnrealUnit)
{
	// Set values
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

	OnClicked.AddDynamic(this, &ATilemap::HandleClick);
}

void ATilemap::HandleClick(AActor * TouchedActor, FKey ButtonPressed)
{
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Cyan, FString::Printf(TEXT("Clicked %s on %s"), *ButtonPressed.ToString(), *TouchedActor->GetFName().ToString()));
}

