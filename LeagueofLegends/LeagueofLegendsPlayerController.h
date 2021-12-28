// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "LeagueofLegendsPlayerController.generated.h"

UCLASS()
class ALeagueofLegendsPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ALeagueofLegendsPlayerController();

	/** Navigate player to the given world location. */
	void SetNewMoveDestination(const FVector DestLocation);

	AActor* ActorthatHit;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UUserWidget> HUDClass;

protected:
	/** True if the controlled character should navigate to the mouse cursor. */
	uint32 bMoveToMouseCursor : 1;

	// Begin PlayerController interface
	virtual void PlayerTick(float DeltaTime) override;
	virtual void SetupInputComponent() override;
	// End PlayerController interface

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	/** Determine wheter player should just move or move nad attack. */
	void DeterminInputAction();

	void Flash();

	void Ignite();

	void AbilityQ();
	void AbilityW();
	void AbilityE();
	void AbilityR();
	class ALeagueofLegendsCharacter* MyPawn;

};


