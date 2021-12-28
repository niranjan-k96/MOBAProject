// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorInfoWidget.h"

ACreatures* UActorInfoWidget::GetOwnerMinion()
{
	return OwnerMinion;
}

void UActorInfoWidget::SetOwnerActor(ACreatures* RefCreature)
{
	//UE_LOG(LogTemp, Warning, TEXT("Owner Minion set from Widget"));
	OwnerMinion = RefCreature;
}

ALeagueofLegendsCharacter* UActorInfoWidget::GetOwnerHero()
{
	return OwnerHero;
}

void UActorInfoWidget::SetOwnerHero(ALeagueofLegendsCharacter* RefHero)
{
	//UE_LOG(LogTemp, Warning, TEXT("Owner hero set from Widget"));
	OwnerHero = RefHero;
}

ANexus* UActorInfoWidget::GetOwnerNexus()
{
	return OwnerNexus;
}

void UActorInfoWidget::SetOwnerNexus(ANexus* RefNexus)
{
	OwnerNexus = RefNexus;
}

ATurret* UActorInfoWidget::GetOwnerTurret()
{
	return OwnerTurret;
}

void UActorInfoWidget::SetOwnerTurret(ATurret* RefTurret)
{
	OwnerTurret = RefTurret;
}
