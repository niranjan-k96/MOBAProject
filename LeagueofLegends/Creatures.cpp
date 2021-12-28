// Fill out your copyright notice in the Description page of Project Settings.


#include "Creatures.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Turret.h"
#include "Kismet/GameplayStatics.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AIPerceptionSystem.h"
#include "LeagueofLegendsCharacter.h"
#include "MinionAIController.h"
#include "Components/WidgetComponent.h"
#include "ActorInfoWidget.h"
#include "BehaviorTree/BlackboardComponent.h"


// Sets default values
ACreatures::ACreatures()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	WeaponCollission=CreateDefaultSubobject<UBoxComponent>(TEXT("Weapon Collission"));
	WeaponCollission->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("WeaponRightSocket"));
	
	//Create Health Bar
	HealthBarWidget = CreateDefaultSubobject<UWidgetComponent>("HealthBar");
	HealthBarWidget->AttachToComponent(GetCapsuleComponent(), FAttachmentTransformRules::SnapToTargetNotIncludingScale);

}

void ACreatures::ChangeFocusSetup(AActor* DeadActor)
{
	if (DeadActor != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Removing enemies from list "));
		MinionController->EnemiesinRange.Remove(DeadActor);
		CanAttack = false;
		AttackTarget = nullptr;
		MinionController->GetBlackboardComponent()->SetValueAsObject("FocusActor", nullptr);
		MinionController->DetermineFocusActor();
	}
}

// Called when the game starts or when spawned
void ACreatures::BeginPlay()
{
	Super::BeginPlay();
	Health = MaxHealth;

	if (UActorInfoWidget* MinionWidget = Cast<UActorInfoWidget>(HealthBarWidget->GetUserWidgetObject()))
	{
		MinionWidget->SetOwnerActor(this);
		//UE_LOG(LogTemp, Warning, TEXT("Owner Minion set from minion"));
	}
	else
	{
		//UE_LOG(LogTemp, Warning, TEXT("Cast Failed"));
	}

	AIPerceptionSystemsight = UAIPerceptionSystem::GetCurrent(GetWorld());
	if (AIPerceptionSystemsight->RegisterPerceptionStimuliSource(this, UAISense_Sight::StaticClass(), this))
	{
		UE_LOG(LogTemp, Warning, TEXT("Perception Stimuli Source Set"));
		
	}
	MinionController = Cast<AMinionAIController>(GetController());
	
}

// Called every frame
void ACreatures::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//testing only
	

}

// Called to bind functionality to input
void ACreatures::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ACreatures::ApplyDamagetoOtherActor()
{
	FPointDamageEvent MinionDamageEvent;

	if (AttackTarget!=nullptr)
	{
		AttackTarget->TakeDamage(MinionDamage, MinionDamageEvent,MinionController,this);
	}
	else
	{
		MinionController->GetBlackboardComponent()->SetValueAsObject("FocusActor", nullptr);
		UE_LOG(LogTemp, Warning, TEXT("Attack Target NOt set"));
	}

}

float ACreatures::GetHealthPercent()
{
	return Health/MaxHealth;
}

float ACreatures::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	//Super::TakeDamage(DamageAmount,DamageEvent,EventInstigator,DamageCauser);
	float DamagetoApply = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	DamagetoApply = FMath::Min(Health, DamagetoApply);
	Health -= DamagetoApply;
	//UE_LOG(LogTemp, Warning, TEXT("Health: %f"), Health);
	if (Health <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Health is 0"));
		//if befor it is dead all the mininos attacking it should change the focus
		TArray<AActor*> Enemies = MinionController->EnemiesinRange;
		for (int32 i = 0; i < Enemies.Num(); i++)
		{
			if (ACreatures* Creature = Cast<ACreatures>(Enemies[i]))
			{
				if (Creature->GetAttackTarget() == this)
				{
					UE_LOG(LogTemp, Warning, TEXT("Attack target of enemy is this"));
					Creature->ChangeFocusSetup(this);
				}
			}
			else if (ATurret* EnemyTurret = Cast<ATurret>(Enemies[i]))
			{
				EnemyTurret->EnemiesinRange.Remove(this);
				EnemyTurret->TurretTarget = nullptr;
				EnemyTurret->SetTurretTarget();
			}
			

		};
		if (ALeagueofLegendsCharacter* LOLChar = Cast<ALeagueofLegendsCharacter>(DamageCauser))
		{
			LOLChar->SetAttackTarget(nullptr);
		}
		
		Destroy();
	}
	return DamagetoApply;
}

void ACreatures::SetAttackTarget(AActor* TargetToSet)
{
	AttackTarget = TargetToSet;
}

AActor* ACreatures::GetAttackTarget()
{
	return AttackTarget;
}

bool ACreatures::IsAtLocation()
{
	if (AttackTarget != nullptr)
	{
		if(GetDistanceTo(AttackTarget) <= AttackRange)
		{
			return true;
		}
	}
	return nullptr;
	
}
