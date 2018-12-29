// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/SceneComponent.h"
#include "Engine/Classes/Components/BoxComponent.h"
#include "Paper2D/Classes/PaperTileMapComponent.h"
#include "TilemapComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BALLS_DEEP_SS_API UTilemapComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTilemapComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	// Allows for additional initialization when registering a Component, if necessary
	virtual void OnRegister() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
	UPROPERTY(BlueprintReadWrite)
	UBoxComponent* ClickBox;

	UPROPERTY(BlueprintReadWrite)
	UPaperTileMapComponent* Tilemap;
};
