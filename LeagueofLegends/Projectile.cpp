// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "LeagueofLegendsCharacter.h"
#include "Turret.h"
#include "Kismet/GameplayStatics.h"
#include "Math/Vector.h"


void AProjectile::OnOverLapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->GetClass()->IsChildOf(ACharacter::StaticClass()))
	{
		FPointDamageEvent DamageEvent;
		AController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		
		UGameplayStatics::GetPlayerController(GetWorld(),0);
		OtherActor->TakeDamage(TurretDamage, DamageEvent, PC, this);
		Destroy();
		UE_LOG(LogTemp, Warning, TEXT("Projectile Destoryed"));
	}
}



void AProjectile::GetTargetfromTurret()
{
	UE_LOG(LogTemp, Warning, TEXT("Get Turret target called"));
	OwnerTurret = Cast<ATurret>(GetOwner());
	if (OwnerTurret != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Cast Successful"));
		EnemytoAttack = OwnerTurret->GetTurretTarget();

		if (EnemytoAttack != nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("Target aCQUIRED"));
			Attack = true;
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Target NOt found"));
		}

	}
}

// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ProjectileCapsule= CreateDefaultSubobject<UCapsuleComponent>(TEXT("Projecile Capsule"));
	RootComponent = ProjectileCapsule;

	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Projectile MEsh"));
	ProjectileMesh->SetupAttachment(ProjectileCapsule);

}


// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	
	GetTargetfromTurret();

	FTimerHandle StartDelay;
	// adding a 0.1 Second delay once (NOT LOOPING) done because attack target is taking some time 
	GetWorldTimerManager().SetTimer(StartDelay, this, &AProjectile::GetTargetfromTurret, 0.1,false);
	
	ProjectileCapsule->OnComponentBeginOverlap.AddDynamic(this, &AProjectile::OnOverLapBegin);
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (Attack==true)
	{
		FVector StartLocation = this->GetActorLocation();
		FVector EndLocation = EnemytoAttack->GetActorLocation();
		FVector LocationtoSet =FMath::VInterpConstantTo(StartLocation, EndLocation, DeltaTime, ProjectileSpeed);
		SetActorLocation(LocationtoSet);
	}

	

	
	

}

