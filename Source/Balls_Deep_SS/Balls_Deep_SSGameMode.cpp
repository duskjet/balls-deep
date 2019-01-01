// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "Balls_Deep_SSGameMode.h"
#include "Balls_Deep_SSCharacter.h"
#include "Balls_Deep_SSPlayerController.h"
#include "Engine.h"
#include "World/Map/TilemapDirector.h"

ABalls_Deep_SSGameMode::ABalls_Deep_SSGameMode()
{
	// Set default pawn class to our character
	DefaultPawnClass = ABalls_Deep_SSCharacter::StaticClass();	
	PlayerControllerClass = ABalls_Deep_SSPlayerController::StaticClass();
}

void ABalls_Deep_SSGameMode::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(Gamemode, Warning, TEXT("BeginPlay()"));
}

void ABalls_Deep_SSGameMode::StartPlay()
{
	Super::StartPlay();
	UE_LOG(Gamemode, Warning, TEXT("StartPlay()"));
}

void ABalls_Deep_SSGameMode::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	//TArray<AActor*> FoundActors;
	//UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATilemapDirector::StaticClass(), FoundActors);

	//if (FoundActors.Num() > 0)
	//{
	//	UE_LOG(Gamemode, Warning, TEXT("Found TilemapDirector."));

	//	auto director = Cast<ATilemapDirector>(FoundActors[0]);
	//	/*director->OnDone().AddLambda([this]()
	//	{
	//		DefaultStart = NewObject<APlayerStart>(this);
	//		DefaultStart->SetActorLocation(FVector(400, 0, 0));
	//	});*/
	//	//TBaseStaticDelegateInstance<void, FVector2D> beb() {};
	//	director->OnDone().AddUObject(this, &ABalls_Deep_SSGameMode::DelegateTest);
	//}
	//else
	//	UE_LOG(Gamemode, Warning, TEXT("Could not find any Tilemap Director Actors."));
}

void ABalls_Deep_SSGameMode::RestartPlayer(AController * NewPlayer)
{
	FVector spawnLocation = GetSpawnPosition(NewPlayer);
	AActor* actor = GetWorld()->SpawnActor(ABalls_Deep_SSCharacter::StaticClass(), &spawnLocation);
	//APawn* pawn = Cast<APawn>(actor);
	//pawn->SetActorLocation(spawnLocation);
	NewPlayer->Possess((APawn*)actor);

	UE_LOG(Gamemode, Display, TEXT("Restart Player '%s' at %s"), *NewPlayer->GetFName().ToString(), *spawnLocation.ToString());
}

void ABalls_Deep_SSGameMode::PostLogin(APlayerController * NewPlayer)
{
	if (DefaultStart) 
	{
		Super::PostLogin(NewPlayer);
	}
	else
	{
		PlayerControllerList.Add(NewPlayer);
	}
}

void ABalls_Deep_SSGameMode::Logout(AController * Exiting)
{
	
}

void ABalls_Deep_SSGameMode::SetDefaultPlayerStart(FVector Position)
{
	if (DefaultStart == nullptr)
	{
		DefaultStart = GetWorld()->SpawnActor<APlayerStart>();
		DefaultStart->GetCapsuleComponent()->Mobility = EComponentMobility::Movable;
	}

	DefaultStart->SetActorLocation(Position);

	UE_LOG(Gamemode, Display, TEXT("Set new PlayerStart to %s"), *Position.ToString());

	for (APlayerController* Controller : PlayerControllerList)
	{
		RestartPlayer(Controller);
	}
}

FVector ABalls_Deep_SSGameMode::GetSpawnPosition(AController * Player)
{
	if (DefaultStart)
	{
		return DefaultStart->GetActorLocation();
	}

	return FVector(0, 0, 0);
}

//void ABalls_Deep_SSGameMode::DelegateTest(FVector2D pos)
//{
//	DefaultStart = GetWorld()->SpawnActor<APlayerStart>();
//	DefaultStart->GetCapsuleComponent()->Mobility = EComponentMobility::Movable;
//
//	FVector newPos = FVector(pos.X, 0, pos.Y);
//	DefaultStart->SetActorLocation(newPos);
//
//	UE_LOG(Gamemode, Display, TEXT("Set new PlayerStart to %s"), *newPos.ToString());
//
//	for (APlayerController* Controller : PlayerControllerList)
//	{
//		RestartPlayer(Controller);
//	}
//}
