// Fill out your copyright notice in the Description page of Project Settings.


#include "Nexus.h"
#include "Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Creatures.h"
#include "Components/WidgetComponent.h"
#include "ActorInfoWidget.h"

// Sets default values
ANexus::ANexus()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("Root Scene"));
	RootComponent = RootScene;

	MeshCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Mesh Capsule"));
	MeshCapsule->SetupAttachment(RootScene);

	NexusMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Turret MEsh"));
	NexusMesh->SetupAttachment(MeshCapsule);

	HealthBarWidget = CreateDefaultSubobject<UWidgetComponent>("HealthBar");
	HealthBarWidget->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	MinionSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Projectile Spawner"));
	MinionSpawnPoint->SetupAttachment(NexusMesh);
}

// Called when the game starts or when spawned
void ANexus::BeginPlay()
{
	Super::BeginPlay();
	
	Health = MaxHealth;
	FTimerHandle BeginTimer;
	GetWorldTimerManager().SetTimer(BeginTimer, this, &ANexus::SpawnMinions, SpawnStartRate, true, 1.f);
	
	if (UActorInfoWidget* WidgetParent = Cast<UActorInfoWidget>(HealthBarWidget->GetUserWidgetObject()))
	{
		UE_LOG(LogTemp, Warning, TEXT("Owner Set As ths"));
		WidgetParent->SetOwnerNexus(this);
	}
}

// Called every frame
void ANexus::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

float ANexus::GetHelathPercent()
{
	return Health/MaxHealth;
}

void ANexus::SpawnMinions()
{
	
	float SpawnRate = 1;
	GetWorldTimerManager().SetTimer(SpawnTimer, this, &ANexus::SpawnSingleMinion, SpawnRate, true, 0.1);

}

void ANexus::SpawnSingleMinion()
{
	numberofspawns += 1;

	if (numberofspawns<=5)
	{
		FVector SpawnLocation = MinionSpawnPoint->GetComponentLocation();
		FRotator SpawnRotation = MinionSpawnPoint->GetComponentRotation();

		FTransform SpawnTransform = MinionSpawnPoint->GetComponentTransform();
		ACreatures* SpawnedMinion = GetWorld()->SpawnActor<ACreatures>(MinionClass,SpawnLocation,SpawnRotation);
	}
	else
	{
		numberofspawns = 0;
		GetWorldTimerManager().ClearTimer(SpawnTimer);
	}

}

float ANexus::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	float DamagetoApply = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	DamagetoApply = FMath::Min(Health, DamagetoApply);
	Health -= DamagetoApply;
	UE_LOG(LogTemp, Warning, TEXT("Turret Health: %f"), Health);
	return 0.0f;
}


