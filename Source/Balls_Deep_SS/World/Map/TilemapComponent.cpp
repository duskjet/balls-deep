// Fill out your copyright notice in the Description page of Project Settings.

#include "TilemapComponent.h"


// Sets default values for this component's properties
UTilemapComponent::UTilemapComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	ClickBox = CreateDefaultSubobject<UBoxComponent>(TEXT("ClickBox"));
	ClickBox->AttachTo(this);

	Tilemap = CreateDefaultSubobject<UPaperTileMapComponent>(TEXT("Tilemap"));
	Tilemap->AttachTo(this);
}


// Called when the game starts
void UTilemapComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UTilemapComponent::OnRegister()
{
}


// Called every frame
void UTilemapComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

