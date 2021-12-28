// Copyright Epic Games, Inc. All Rights Reserved.

#include "LeagueofLegendsPlayerController.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Runtime/Engine/Classes/Components/DecalComponent.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "LeagueofLegendsCharacter.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"
#include "NavigationSystem.h"
#include "Kismet/KismetMathLibrary.h"


ALeagueofLegendsPlayerController::ALeagueofLegendsPlayerController()
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Crosshairs;
}

void ALeagueofLegendsPlayerController::BeginPlay()
{
	Super::BeginPlay();
	MyPawn = Cast<ALeagueofLegendsCharacter>(GetPawn());
	UUserWidget* HUD = CreateWidget(this,HUDClass);
	if (HUD != nullptr)
	{
		HUD->AddToViewport();
	}
	
}
void ALeagueofLegendsPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
}

void ALeagueofLegendsPlayerController::SetupInputComponent()
{
	// set up gameplay key bindings
	Super::SetupInputComponent();

	InputComponent->BindAction("DetermineMovementAction", IE_Pressed, this, &ALeagueofLegendsPlayerController::DeterminInputAction);
	InputComponent->BindAction("Flash", IE_Pressed, this, &ALeagueofLegendsPlayerController::Flash);
	InputComponent->BindAction("Ignite", IE_Pressed, this, &ALeagueofLegendsPlayerController::Ignite);
	InputComponent->BindAction("AbilityQ", IE_Pressed, this, &ALeagueofLegendsPlayerController::AbilityQ);
	InputComponent->BindAction("AbilityW", IE_Pressed, this, &ALeagueofLegendsPlayerController::AbilityW);
	InputComponent->BindAction("AbilityE", IE_Pressed, this, &ALeagueofLegendsPlayerController::AbilityE);
	InputComponent->BindAction("AbilityR", IE_Pressed, this, &ALeagueofLegendsPlayerController::AbilityR);
}

void ALeagueofLegendsPlayerController::Flash()
{
	if (MyPawn)
	{
		MyPawn->Flash();
	}
}

void ALeagueofLegendsPlayerController::Ignite()
{
	if (MyPawn)
	{
		FHitResult Hit;
		GetHitResultUnderCursor(ECC_Visibility, false, Hit);
		ActorthatHit = Hit.GetActor();
		
		if (ActorthatHit == nullptr)
		{return;}

		//ActorthatHit->IsA<ALeagueofLegendsCharacter>()
		if (Cast<ALeagueofLegendsCharacter>(ActorthatHit))
		{
			UE_LOG(LogTemp, Warning, TEXT("Enemy Ignited"));
			MyPawn->Ignite(ActorthatHit);
		}
	}
}

void ALeagueofLegendsPlayerController::AbilityQ()
{
	MyPawn->AbilityQ();
}
void ALeagueofLegendsPlayerController::AbilityW()
{
	MyPawn->AbilityW();
}
void ALeagueofLegendsPlayerController::AbilityE()
{
	MyPawn->AbilityE();
}
void ALeagueofLegendsPlayerController::AbilityR()
{
	FHitResult Hit;
	GetHitResultUnderCursor(ECC_Visibility, false, Hit);
	ActorthatHit = Hit.GetActor();
	UE_LOG(LogTemp, Warning, TEXT("Ability R Activated"));
	if (ActorthatHit == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("actor that hit is a nullptr"));
		return;
	}

	//ActorthatHit->IsA<ALeagueofLegendsCharacter>()
	if (ALeagueofLegendsCharacter* EnemyHero =Cast<ALeagueofLegendsCharacter>(ActorthatHit))
	{
		if (EnemyHero->GetTeamID() != MyPawn->GetTeamID())
		{
			UE_LOG(LogTemp, Warning, TEXT("Ability R Activated"));
			MyPawn->AbilityR(EnemyHero);
		}
	}
}

void ALeagueofLegendsPlayerController::DeterminInputAction()
{
	// Trace to see what is under the mouse cursor
	FHitResult Hit;
	GetHitResultUnderCursor(ECC_Visibility, false, Hit);
	ALeagueofLegendsCharacter* LOLCharacter = Cast<ALeagueofLegendsCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	ActorthatHit = Hit.GetActor();

	if(ActorthatHit== nullptr)
	{
		return;
	}
	if (ActorthatHit->IsA<ACharacter>() && LOLCharacter != nullptr)
	{
		if (ActorthatHit != MyPawn)
		{
			LOLCharacter->SetAttackTarget(ActorthatHit);
			UE_LOG(LogTemp, Warning, TEXT("Is an actor ! GOing FOR ATTTACCCKKK"));
			LOLCharacter->bSholdMoveAndAttack = true;
		}
	}
		
	else
	{
		FString actName = Hit.GetActor()->GetName();

		//UE_LOG(LogTemp, Warning, TEXT("actor name %s"),*actName);
		//Just move to location
		LOLCharacter->SetAttackTarget(nullptr);
		LOLCharacter->bCanAttack = false;

		if (MyPawn)
		{
			if (MyPawn->GetCursorToWorld())
			{
				UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, MyPawn->GetCursorToWorld()->GetComponentLocation());
			}
		}

		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Character not found under cursor"));
			SetNewMoveDestination(Hit.ImpactPoint);
		}
	}
}


void ALeagueofLegendsPlayerController::SetNewMoveDestination(const FVector DestLocation)
{
	if (MyPawn)
	{
		float const Distance = FVector::Dist(DestLocation, MyPawn->GetActorLocation());
		// We need to issue move command only if far enough in order for walk animation to play correctly
		if ((Distance > 120.0f))
		{
			UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, DestLocation);
		}
	}
}



