// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WorldMap.h"
#include "TileArea.h"
#include "WorldMapComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BALLS_DEEP_SS_API UWorldMapComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UWorldMapComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	//virtual bool IsSupportedForNetworking() const override;

	//virtual void GetLifetimeReplicatedProps(TArray< class FLifetimeProperty > & OutLifetimeProps) const override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(BlueprintReadWrite, Category = Grid)
		TArray<class UTile*> Grid;

	UPROPERTY(BlueprintReadWrite, Category = Grid)
		int32 Width;

	UPROPERTY(BlueprintReadWrite, Category = Grid)
		int32 Height;

public:
	void Init(int32 Width, int32 Height);

	TArray<class UTileArea*> GetGroups();

	UWorldMapComponent* Copy();

private:
	void ClearTileIndexes();
};
