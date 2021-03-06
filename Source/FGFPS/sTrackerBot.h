// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "sTrackerBot.generated.h"

UCLASS()
class FGFPS_API AsTrackerBot : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AsTrackerBot();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleDefaultsOnly, Category = Components)
		UStaticMeshComponent* MeshComp;

	FVector GetNextPathPoint();

	FVector NextPathPoint;

	UPROPERTY(EditDefaultsOnly, Category = TrackerBot)
	float MovementForce;

	UPROPERTY(EditDefaultsOnly, Category = TrackerBot)
	bool bUseVelocityChange;

	UPROPERTY(EditDefaultsOnly, Category = TrackerBot)
	float RequiredDistanceToTarget;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
