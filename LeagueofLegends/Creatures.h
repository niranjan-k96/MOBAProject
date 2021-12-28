// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Creatures.generated.h"

UCLASS()
class LEAGUEOFLEGENDS_API ACreatures : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACreatures();

	UPROPERTY(EditAnywhere)
	float MaxHealth=100;

	UPROPERTY(EditAnywhere)
	float Health;

	UFUNCTION()
	float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	UPROPERTY(EditDefaultsOnly)
	float MinionDamage = 12;

	UPROPERTY(BlueprintReadWrite)
	bool CanAttack = false;

	void SetAttackTarget(AActor* TargetToSet);

	AActor* GetAttackTarget();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Teams")
	int TeamId=1;

	FORCEINLINE int GetTeamID() { return TeamId; }

	void ChangeFocusSetup(AActor* DeadActor);

private:

	AActor* AttackTarget;

	class AMinionAIController* MinionController;

	class UAIPerceptionSystem* AIPerceptionSystemsight;


	float AttackRange = 150;

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere)
	class UBoxComponent* WeaponCollission;

	UFUNCTION(BlueprintCallable)
	void ApplyDamagetoOtherActor();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class UWidgetComponent* HealthBarWidget;

	UFUNCTION(BlueprintCallable)
	float GetHealthPercent();

	UFUNCTION(BlueprintCallable)
	bool IsAtLocation();

	float GetAttackRange() {return AttackRange;}
};
