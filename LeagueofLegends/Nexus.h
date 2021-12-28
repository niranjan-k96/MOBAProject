// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Nexus.generated.h"
class ACreatures;
UCLASS()
class LEAGUEOFLEGENDS_API ANexus : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ANexus();

	UFUNCTION()
	float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class UWidgetComponent* HealthBarWidget;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	float GetHelathPercent();

private:

	float MaxHealth = 5500;

	float Health = 5500;

	UPROPERTY(EditAnywhere)
	class USceneComponent* RootScene;

	UPROPERTY(EditAnywhere)
	class UCapsuleComponent* MeshCapsule;

	UPROPERTY(EditAnywhere)
	class USceneComponent* MinionSpawnPoint;

	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* NexusMesh;

	UFUNCTION()
	void SpawnMinions();

	UFUNCTION()
	void SpawnSingleMinion();

	float SpawnStartRate = 30.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MinionsSpawn", meta = (AllowPrivateAccess = true))
	TSubclassOf<ACreatures> MinionClass;

	int numberofspawns = 0;

	FTimerHandle SpawnTimer;

};
