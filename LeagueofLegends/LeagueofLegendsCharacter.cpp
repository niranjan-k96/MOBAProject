// Copyright Epic Games, Inc. All Rights Reserved.

#include "LeagueofLegendsCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/DecalComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "Materials/Material.h"
#include "Engine/World.h"
#include "LeagueofLegendsPlayerController.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Creatures.h"
#include "ActorInfoWidget.h"
#include "Components/WidgetComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "KwangSword.h"
#include "Particles/ParticleSystemComponent.h"


ALeagueofLegendsCharacter::ALeagueofLegendsCharacter()
{
	// Set size for player capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate character to camera direction
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Rotate character to moving direction
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	//Create Health Bar
	HealthBarWidget = CreateDefaultSubobject<UWidgetComponent>("HealthBar");
	HealthBarWidget->AttachToComponent(GetCapsuleComponent(), FAttachmentTransformRules::KeepRelativeTransform);
	

	// Create a decal in the world to show the cursor's location
	CursorToWorld = CreateDefaultSubobject<UDecalComponent>("CursorToWorld");
	CursorToWorld->SetupAttachment(RootComponent);
	static ConstructorHelpers::FObjectFinder<UMaterial> DecalMaterialAsset(TEXT("Material'/Game/TopDownCPP/Blueprints/M_Cursor_Decal.M_Cursor_Decal'"));
	if (DecalMaterialAsset.Succeeded())
	{
		CursorToWorld->SetDecalMaterial(DecalMaterialAsset.Object);
	}
	CursorToWorld->DecalSize = FVector(16.0f, 32.0f, 32.0f);
	CursorToWorld->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f).Quaternion());

	// Activate ticking in order to update the cursor every frame.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	//Healt Set
	Health = MaxHealth;

	//Create collission box
	SwordCollission = CreateDefaultSubobject<UBoxComponent>(TEXT("Weapon Collission"));
	SwordCollission->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("SwordSocket"));
}

void ALeagueofLegendsCharacter::BeginPlay()
{
	Super::BeginPlay();
	PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	
	if (UActorInfoWidget* WidgetParent = Cast<UActorInfoWidget>(HealthBarWidget->GetUserWidgetObject()))
	{
		UE_LOG(LogTemp, Warning, TEXT("Owner Set As ths"));
		WidgetParent->SetOwnerHero(this);
	}
	FTimerHandle CountdownTimer;
	GetWorldTimerManager().SetTimer(CountdownTimer, this, &ALeagueofLegendsCharacter::FillTimer, 0.5,true, -1);

	UAIPerceptionSystem::RegisterPerceptionStimuliSource(this, UAISense_Sight::StaticClass(), this);
	SwordCollission->IgnoreActorWhenMoving(this, true);
	SwordCollission->OnComponentBeginOverlap.AddDynamic(this, &ALeagueofLegendsCharacter::OnOverLapBegin);
}

void ALeagueofLegendsCharacter::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

	if (CursorToWorld != nullptr)
	{
		if (ALeagueofLegendsPlayerController* LOLContorller = Cast<ALeagueofLegendsPlayerController>(GetController()))
		{
			FHitResult TraceHitResult;
			LOLContorller->GetHitResultUnderCursor(ECC_Visibility, true, TraceHitResult);
			FVector CursorFV = TraceHitResult.ImpactNormal;
			FRotator CursorR = CursorFV.Rotation();
			CursorToWorld->SetWorldLocation(TraceHitResult.Location);
			CursorToWorld->SetWorldRotation(CursorR);
			
			if (bSholdMoveAndAttack== true && AttackTarget != nullptr)
			{
				
				FVector TargetLocation = AttackTarget->GetActorLocation();
				float Distance = FVector::Dist(TargetLocation, this->GetActorLocation());
				if (Distance <= AttackRange)
				{
					bCanAttack = true;
				}
				else
				{
					bCanAttack = false;
				}
				LOLContorller->SetNewMoveDestination(TargetLocation);
				
			}
			else
			{
				bCanAttack = false;
			}
		}
	}
}

float ALeagueofLegendsCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float DamagetoApply =Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	DamagetoApply=FMath::Min(Health, DamagetoApply);
	Health -= DamagetoApply;
	UE_LOG(LogTemp, Warning, TEXT("Health: %f"), Health);
	if (Health<=0)
	{
		//dead;
	}
	return DamagetoApply;
}

AActor* ALeagueofLegendsCharacter::GetAttackTarget()
{
	return AttackTarget;
}

void ALeagueofLegendsCharacter::SetAttackTarget(AActor* ActorToSetTarget)
{
	UE_LOG(LogTemp, Warning, TEXT("Attack Target Set"));
	AttackTarget = ActorToSetTarget;
}

void ALeagueofLegendsCharacter::FillTimer()
{
	//UE_LOG(LogTemp, Warning, TEXT(" Q filling %f"), AbilityQtime);
	if (AbilityQtime < AbilityQCooldown)
	{
		
		AbilityQtime += 0.5;
	}
	if (AbilityEtime < AbilityECooldown)
	{
		AbilityEtime += 0.5;
	}
	if (AbilityRtime < AbilityRCooldown)
	{
		AbilityRtime += 0.5;
	}
	if (Flashtime < FlashCooldown)
	{
		Flashtime += 0.5;
	}
	if (Ignitetime < IgniteCooldown)
	{
		Ignitetime += 0.5;
	}
}

float ALeagueofLegendsCharacter::GetHelathPercent()
{
	return Health/MaxHealth;
}

void ALeagueofLegendsCharacter::Flash()
{
	if (Flashtime == FlashCooldown)
	{
		FVector ActorLoc = GetActorLocation();
		FVector UnitDirection = UKismetMathLibrary::GetDirectionUnitVector(ActorLoc, CursorToWorld->GetComponentLocation());
		FVector ResultLocaiton{ (ActorLoc.X + (UnitDirection.X * FlashDistance)), (ActorLoc.Y + (UnitDirection.Y * FlashDistance)), ActorLoc.Z };
		SetActorLocation(ResultLocaiton);
		Flashtime = 0;
	}
}

void ALeagueofLegendsCharacter::Ignite(AActor* Enemy)
{
	if (Ignitetime == IgniteCooldown)
	{
		if (Enemy == nullptr)
		{
			return;
		}
		IgniteActor = Enemy;
		//Enemy = Cast<ALeagueofLegendsCharacter>(Enemy);
		IgniteParticleComp = UGameplayStatics::SpawnEmitterAttached(IgniteParticle, Enemy->GetRootComponent());
		UGameplayStatics::SpawnSoundAttached(IgniteSound, Enemy->GetRootComponent());
		GetWorldTimerManager().SetTimer(IgniteTimer, this, &ALeagueofLegendsCharacter::IgniteDamage, 0.5, true, 0);
		Ignitetime = 0;
	}
}

void ALeagueofLegendsCharacter::IgniteDamage()
{
	while (IgniteTick-- > 0)
	{
		FDamageEvent IgniteDamageEvent;
		IgniteActor->TakeDamage(IgniteDamageAmount, IgniteDamageEvent, PC, this);
		return;
	}
	IgniteTick = 10;
	GetWorldTimerManager().ClearTimer(IgniteTimer);
	IgniteParticleComp->DestroyComponent();
}


float ALeagueofLegendsCharacter::GetAbilityQPercent()
{
	//UE_LOG(LogTemp, Warning, TEXT("Q percenti si  %f"), (AbilityQtime / AbilityQCooldown));
	return AbilityQtime / AbilityQCooldown;
}

float ALeagueofLegendsCharacter::GetAbilityEPercent()
{
	return AbilityEtime / AbilityECooldown;
}

float ALeagueofLegendsCharacter::GetAbilityRPercent()
{
	return AbilityRtime / AbilityRCooldown;
}

float ALeagueofLegendsCharacter::GetFlashPercent()
{
	return Flashtime / FlashCooldown;
}

float ALeagueofLegendsCharacter::GetIgnitePercent()
{
	return Ignitetime / IgniteCooldown;
}

// ABility Q Functions 
void ALeagueofLegendsCharacter::AbilityQ()
{
	if (AbilityQtime == AbilityQCooldown)
	{

		if (AbilityQActivated == false)
		{
			UE_LOG(LogTemp, Warning, TEXT("ABility Q activated"));
			AbilityQActivated = true;
			//check if needed
			SwordLocation = GetSwordLocationforQ();

			//SetActor Location to face the sword throw
			FRotator LookAtRotation{ (UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), SwordLocation)) };
			SetActorRotation(LookAtRotation);
			FTimerHandle SpawnTimer;
			GetWorldTimerManager().SetTimer(SpawnTimer, this, &ALeagueofLegendsCharacter::SpawnSword, 0.5, false, -1);

			FTimerHandle AbilityQHandle;
			GetWorldTimerManager().SetTimer(AbilityQHandle, this, &ALeagueofLegendsCharacter::AbilityQPart2, 5, false, -1);
		}
		else if (AbilityQActivated == true)
		{
			AbilityQPart2();
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Unable to activate Q"));
		}
	}
}

FVector ALeagueofLegendsCharacter::GetSwordLocationforQ()
{
	FVector CursorLoc = CursorToWorld->GetComponentLocation();
	float CursorDist = FVector::Distance(GetActorLocation(), CursorLoc);
	if (CursorDist < QRagne)
	{
		return CursorLoc;
	}
	else
	{
		FVector ActorLoc = GetActorLocation();
		FVector UnitDirection = UKismetMathLibrary::GetDirectionUnitVector(ActorLoc, CursorToWorld->GetComponentLocation());
		FVector ResultLocaiton{ (ActorLoc.X + (UnitDirection.X * QRagne)), (ActorLoc.Y + (UnitDirection.Y * QRagne)), ActorLoc.Z };
		return ResultLocaiton;
	}
	
}
void ALeagueofLegendsCharacter::SpawnSword()
{
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), SwordSpawnParticle,SwordLocation);
	UGameplayStatics::SpawnSoundAtLocation(GetWorld(), AbilityQSwordSound, SwordLocation);

	FRotator SwordRotaion{ 0,0,0 };
	SpawnedSword = GetWorld()->SpawnActor<AActor>(SwordClass, SwordLocation, SwordRotaion);
	SpawnedSword->SetOwner(this);
}

void ALeagueofLegendsCharacter::AbilityQPart2()
{
	if (AbilityQActivated)
	{
		UE_LOG(LogTemp, Warning, TEXT("Ability Q part 2 activated"));
		AbilityQActivated = false;

		//change the rotaion to face the sword
		FRotator LookAtRotation{(UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), SwordLocation))};
		SetActorRotation(LookAtRotation);
		AbilityQPart2Activated = true;

		UGameplayStatics::SpawnSoundAtLocation(GetWorld(), SwordWhoosh, SwordLocation);

		FTimerHandle DeactivateAbilityQ;
		GetWorldTimerManager().SetTimer(DeactivateAbilityQ, this, &ALeagueofLegendsCharacter::AbilityQPart2Deactivate, 1.2, false, -1);

		FHitResult SweepResult;
		(Cast<AKwangSword>(SpawnedSword)->Sweeping = true);
		SpawnedSword->SetActorHiddenInGame(true);
		SpawnedSword->SetActorLocation(GetActorLocation(), true, &SweepResult, ETeleportType::None);

		AbilityQtime = 0;

	}

}

void ALeagueofLegendsCharacter::AbilityQPart2Deactivate()
{
	if (AbilityQPart2Activated)
	{
		(Cast<AKwangSword>(SpawnedSword)->Sweeping = false);
		AbilityQActivated = false;
		SpawnedSword->Destroy();
		AbilityQPart2Activated = false;
	}
}



// ABility W Function
void ALeagueofLegendsCharacter::AbilityW()
{
	
	if (AbilityQActivated)
	{
		AbilityWActivated = true;
		FTimerHandle AbilityWEmitters;
		FTimerHandle AbilitYWDeactivateHandle;
		GetWorldTimerManager().SetTimer(AbilityWEmitters, this, &ALeagueofLegendsCharacter::ABilityWEmitter, 0.42, false, -1);
		GetWorldTimerManager().SetTimer(AbilitYWDeactivateHandle, this, &ALeagueofLegendsCharacter::AbilityWDeactivate, 0.42, false, -1);

	}
}
void ALeagueofLegendsCharacter::ABilityWEmitter()
{
	AbilityQActivated = false;
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), TeleportParticle, SwordLocation);
	UGameplayStatics::SpawnSoundAtLocation(GetWorld(), TeleportSound, SwordLocation);
	SpawnedSword->Destroy();
	SetActorLocation(SwordLocation, false);
	SwordLocation = GetActorLocation();
}
void ALeagueofLegendsCharacter::AbilityWDeactivate()
{
	AbilityWActivated = false;
}
void ALeagueofLegendsCharacter::AbilityE()
{
	if(AbilityEtime == AbilityECooldown)
	{
		AbilityEActivated = true;
		AblityEParticleSystem = UGameplayStatics::SpawnEmitterAttached(EParticle, GetMesh(), TEXT("FX_weapon_tip"));
		AbilityEtime = 0;
	}
}
void ALeagueofLegendsCharacter::AbilityR(ALeagueofLegendsCharacter* EnemyHero)
{	
	if (AbilityRtime == AbilityRCooldown)
	{
		float EnemyDist = GetDistanceTo(EnemyHero);
		if (EnemyDist <= RRagne)
		{
			UE_LOG(LogTemp, Warning, TEXT("Ability R Activated"));
			AbilityRActivated = true;
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), UltParticle, EnemyHero->GetActorLocation());
			UGameplayStatics::SpawnSoundAtLocation(GetWorld(), UltSound, EnemyHero->GetActorLocation());
			FDamageEvent UltDamgeEvent;
			EnemyHero->TakeDamage(RDamage, UltDamgeEvent, PC, this);
			AbilityRtime = 0;
		}

	}
}

void ALeagueofLegendsCharacter::OnOverLapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
}

void ALeagueofLegendsCharacter::ApplyDamageAfterAttack()
{
	if (AttackTarget)
	{
		if (AbilityEActivated)
		{
			AutoAttackDamage += (AutoAttackDamage / 2);
			FPointDamageEvent DamageEvent;
			AttackTarget->TakeDamage(AutoAttackDamage, DamageEvent, PC, this);
			if (AblityEParticleSystem)
			{
				AblityEParticleSystem->DestroyComponent();
			}
		}
		else
		{
			FPointDamageEvent DamageEvent;
			AttackTarget->TakeDamage(AutoAttackDamage, DamageEvent, PC, this);
		}
	}
}




