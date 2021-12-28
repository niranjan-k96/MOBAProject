// Fill out your copyright notice in the Description page of Project Settings.


#include "KwangSword.h"
#include "Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"
#include "LeagueOfLegendsCharacter.h"
#include "Creatures.h"



// Sets default values
AKwangSword::AKwangSword()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("Root Scene"));
	RootComponent = RootScene;

	MeshCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Mesh Capsule"));
	MeshCapsule->SetupAttachment(RootScene);

	SwordMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Sword MEsh"));
	SwordMesh->SetupAttachment(MeshCapsule);
}

// Called when the game starts or when spawned
void AKwangSword::BeginPlay()
{
	Super::BeginPlay();
	
	MeshCapsule->OnComponentBeginOverlap.AddDynamic(this, &AKwangSword::OnOverLapBegin);

	FTimerHandle StartDelay;
	GetWorldTimerManager().SetTimer(StartDelay, this, &AKwangSword::BeginPlayDelay, 0.1, false, -1);

}

// Called every frame
void AKwangSword::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	
}

void AKwangSword::OnOverLapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
	if (Sweeping && OtherActor != GetOwner())
	{
		UE_LOG(LogTemp, Warning, TEXT("Sweeping began"));
		if (ALeagueofLegendsCharacter* EnemyHero = Cast<ALeagueofLegendsCharacter>(OtherActor))
		{
			if (EnemyHero->GetTeamID() != PlayerPawn->GetTeamID())
			{
				FDamageEvent InitialDamageEvent;
				EnemyHero->TakeDamage(SwordSweepDamage, InitialDamageEvent, PlayerPawn->GetController(), GetOwner());
			}
		}

		else if (ACreatures* Minion = Cast<ACreatures>(OtherActor))
		{
			if (Minion->GetTeamID() != PlayerPawn->GetTeamID())
			{
				FDamageEvent InitialDamageEvent;
				Minion->TakeDamage(SwordSweepDamage, InitialDamageEvent, PlayerPawn->GetController(), GetOwner());
			}
		}

	}
}

void AKwangSword::BeginPlayDelay()
{
	MeshCapsule->GetOverlappingActors(ActorsthatOverlap, ACharacter::StaticClass());
	PlayerPawn = Cast<ALeagueofLegendsCharacter>(GetOwner());
	if (PlayerPawn)
	{
		UE_LOG(LogTemp, Warning, TEXT("Kwang Sword owner is player"));
		for (auto& eachActor : ActorsthatOverlap)
		{
			if (ALeagueofLegendsCharacter* EnemyHero = Cast<ALeagueofLegendsCharacter>(eachActor))
			{
				if (EnemyHero->GetTeamID() != PlayerPawn->GetTeamID())
				{
					FDamageEvent InitialDamageEvent;
					EnemyHero->TakeDamage(InitialSwordDamage, InitialDamageEvent, PlayerPawn->GetController(), GetOwner());
				}
			}

			else if (ACreatures* Minion = Cast<ACreatures>(eachActor))
			{
				if (Minion->GetTeamID() != PlayerPawn->GetTeamID())
				{
					FDamageEvent InitialDamageEvent;
					Minion->TakeDamage(InitialSwordDamage, InitialDamageEvent, PlayerPawn->GetController(), GetOwner());
				}
			}

		}
	}
}
