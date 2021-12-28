// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Creatures.h"
#include "LeagueofLegendsCharacter.h"
#include "Nexus.h"
#include "Turret.h"
#include "ActorInfoWidget.generated.h"


UCLASS()
class LEAGUEOFLEGENDS_API UActorInfoWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	
	UPROPERTY(BlueprintReadWrite)
	ACreatures* OwnerMinion;

	UPROPERTY(BlueprintReadWrite)
	ALeagueofLegendsCharacter* OwnerHero;

	UPROPERTY(BlueprintReadWrite)
	ATurret* OwnerTurret;

	UPROPERTY(BlueprintReadWrite)
	ANexus* OwnerNexus;

public:

	UFUNCTION(BlueprintCallable)
	ACreatures* GetOwnerMinion();

	void SetOwnerActor(ACreatures* RefCreature);

	UFUNCTION(BlueprintCallable)
	ALeagueofLegendsCharacter* GetOwnerHero();

	void SetOwnerHero(ALeagueofLegendsCharacter* RefHero);

	UFUNCTION(BlueprintCallable)
	ANexus* GetOwnerNexus();

	void SetOwnerNexus(ANexus* RefNexus);

	UFUNCTION(BlueprintCallable)
	ATurret* GetOwnerTurret();

	void SetOwnerTurret(ATurret* RefTurret);
};
