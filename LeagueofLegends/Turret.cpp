// Fill out your copyright notice in the Description page of Project Settings.


#include "Turret.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "LeagueofLegendsCharacter.h"
#include "Projectile.h"
#include "Creatures.h"
#include "Kismet/GameplayStatics.h"
#include "Components/WidgetComponent.h"
#include "ActorInfoWidget.h"


// Sets default values
ATurret::ATurret()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("Root Scene"));
	RootComponent = RootScene;

	MeshCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Mesh Capsule"));
	MeshCapsule->SetupAttachment(RootScene);

	TurretMesh =CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Turret MEsh"));
	TurretMesh->SetupAttachment(MeshCapsule);

	ProjectileSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Projectile Spawner"));
	ProjectileSpawnPoint->SetupAttachment(TurretMesh);

	HealthBarWidget = CreateDefaultSubobject<UWidgetComponent>("HealthBar");
	HealthBarWidget->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	Attackrange = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Attack Range"));
	Attackrange->SetupAttachment(RootScene);
	
}

// Called when the game starts or when spawned
void ATurret::BeginPlay()
{
	Super::BeginPlay();
	Health = MaxHealth;

	if (UActorInfoWidget* WidgetParent = Cast<UActorInfoWidget>(HealthBarWidget->GetUserWidgetObject()))
	{
		UE_LOG(LogTemp, Warning, TEXT("Owner Set As ths"));
		WidgetParent->SetOwnerTurret(this);
	}

	Attackrange->OnComponentBeginOverlap.AddDynamic(this,&ATurret::OnOverLapBegin);
	Attackrange->OnComponentEndOverlap.AddDynamic(this, &ATurret::OnOverLapEnd);
	PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
}

// Called every frame
void ATurret::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATurret::SetTurretTarget()
{
	if (TurretTarget != nullptr)
	{
		return;
	}

	else
	//There is no turret target set
	{
		if (EnemiesinRange.Num() == 0)
		{
			TurretTarget = nullptr;
		}

		else if (EnemiesinRange.Num() == 1)
		{
			TurretTarget = EnemiesinRange[0];
		}

		else
		{
			bool minioninRange = false;
			float shortestDistance = 99999;
			int ivalueminion = 99;
			int ivaluehero=99;

			for (int i = 0; i < EnemiesinRange.Num(); i++)
			{
				if (ACreatures* Minion = Cast<ACreatures>(EnemiesinRange[i]))
				{
					if (Minion->GetTeamID() != TeamId)
					{
						minioninRange = true;
						float Distance = this->GetDistanceTo(Minion);

						if (Distance < shortestDistance)
						{
							shortestDistance = Distance;
							ivalueminion = i;
						}
					}
				}

				if (minioninRange == false)
				{
					if (Cast<ALeagueofLegendsCharacter>(EnemiesinRange[i]))
					{
						ivaluehero = i;
					}
				}
			}

			if (minioninRange == true && ivalueminion < 30)
			{
				TurretTarget = EnemiesinRange[ivalueminion];
			}
		
			else if(ivaluehero < 30)
			{
				TurretTarget = OverlappingActorsRes[ivaluehero];	
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Can't Set the enemy as Minion or HEro so Retargetting"));
				SetTurretTarget();
			}
		}
	}
}

float ATurret::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	float DamagetoApply = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	DamagetoApply = FMath::Min(Health, DamagetoApply);
	Health -= DamagetoApply;
	UE_LOG(LogTemp, Warning, TEXT("Turret Health: %f"), Health);
	return 0.0f;
}

float ATurret::GetHelathPercent()
{
	return Health/MaxHealth;
}

AActor* ATurret::GetTurretTarget()
{
	return TurretTarget;
}

void ATurret::SpawnProjectile()
{
	SetTurretTarget();
	if (TurretTarget == nullptr)
	{
		ShouldCheckOverlap = true;
		GetWorldTimerManager().ClearTimer(TurretTimer);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Projectile Spawned"));
		FTransform SpawnTransform = ProjectileSpawnPoint->GetComponentTransform();
		AProjectile* TempProjectile = GetWorld()->SpawnActor<AProjectile>(ProjectileClass, SpawnTransform);
		TempProjectile->SetOwner(this);
	}
}

void ATurret::OnOverLapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("BEgin Overlap Happended"));
	if (ACreatures* Minion = Cast<ACreatures>(OtherActor))
	{
		if (Minion->GetTeamID() != TeamId)
		{
			EnemiesinRange.Add(Minion);
		}
	}
	else if (ALeagueofLegendsCharacter* Hero = Cast<ALeagueofLegendsCharacter>(OtherActor))
	{
		if (Hero->GetTeamID() != TeamId)
		{
			EnemiesinRange.Add(Hero);
		}
	}

	if (ShouldCheckOverlap)
	{
		//UE_LOG(LogTemp, Warning, TEXT("BEgin overlap Happend"));
		GetWorldTimerManager().SetTimer(TurretTimer, this, &ATurret::SpawnProjectile, attackrate, true, 0.5f);
		ShouldCheckOverlap = false;
	}
	
}

void ATurret::OnOverLapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	EnemiesinRange.Remove(OtherActor);
}

