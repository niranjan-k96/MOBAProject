// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "LeagueofLegendsCharacter.generated.h"

UCLASS(Blueprintable)
class ALeagueofLegendsCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ALeagueofLegendsCharacter();

	// Called every frame.
	virtual void Tick(float DeltaSeconds) override;


	/** Returns CursorToWorld subobject **/
	FORCEINLINE class UDecalComponent* GetCursorToWorld() { return CursorToWorld; }

	UFUNCTION()
	float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	bool bSholdMoveAndAttack = false;

	float AttackRange = 120.f;

	float AutoAttackDamage = 61;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Teams")
	int TeamId = 1;

	FORCEINLINE int GetTeamID() { return TeamId; }

	AActor* GetAttackTarget();

	void SetAttackTarget(AActor* ActorToSetTarget);

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bCanAttack = false;

	UFUNCTION(BlueprintCallable)
	float GetHelathPercent();

	UPROPERTY(EditAnywhere)
	float FlashDistance =300;

	void Flash();

	void Ignite(AActor* Enemy);

	UPROPERTY(EditAnywhere)
	UParticleSystem* IgniteParticle;

	UPROPERTY(EditAnywhere)
	USoundBase* IgniteSound;

	UPROPERTY(EditAnywhere)
	UParticleSystem* SwordSpawnParticle;

	UPROPERTY(EditAnywhere)
	USoundBase* AbilityQSwordSound;

	UPROPERTY(EditAnywhere)
	USoundBase* SwordWhoosh;

	UPROPERTY(EditAnywhere)
	UParticleSystem* TeleportParticle;

	UPROPERTY(EditAnywhere)
	USoundBase* TeleportSound;

	UPROPERTY(EditAnywhere)
	UParticleSystem* UltParticle;

	UPROPERTY(EditAnywhere)
	USoundBase* UltSound;

	UPROPERTY(EditAnywhere)
	UParticleSystem* EParticle;

	UPROPERTY(EditAnywhere)
	USoundBase* EFireSound;
	
	UFUNCTION()
	void FillTimer();

	float AbilityQCooldown = 6;
	float AbilityECooldown = 8;
	float AbilityRCooldown = 100;
	float FlashCooldown = 200;
	float IgniteCooldown = 190;

	float AbilityQtime = 6;
	float AbilityEtime = 8;
	float AbilityRtime = 100;
	float Flashtime = 200;
	float Ignitetime = 190;

	UFUNCTION(BlueprintCallable)
	float GetAbilityQPercent();

	UFUNCTION(BlueprintCallable)
	float GetAbilityEPercent();

	UFUNCTION(BlueprintCallable)
	float GetAbilityRPercent();

	UFUNCTION(BlueprintCallable)
	float GetFlashPercent();

	UFUNCTION(BlueprintCallable)
	float GetIgnitePercent();
	

	void AbilityQ();
	FVector GetSwordLocationforQ();
	void AbilityQPart2();
	void AbilityQPart2Deactivate();

	void SpawnSword();

	void AbilityW();
	void ABilityWEmitter();
	void AbilityWDeactivate();

	void AbilityE();
	void AbilityR(ALeagueofLegendsCharacter* EnemyHero);

	UPROPERTY(BlueprintReadWrite)
	bool AbilityQActivated = false;

	UPROPERTY(BlueprintReadWrite)
	bool AbilityQPart2Activated = false;

	UPROPERTY(BlueprintReadWrite)
	bool AbilityWActivated = false;

	UPROPERTY(BlueprintReadWrite)
	bool AbilityEActivated = false;

	UPROPERTY(BlueprintReadWrite)
	bool AbilityRActivated = false;

	UPROPERTY(EditAnywhere)
	float QRagne = 500.0;

	UPROPERTY(EditAnywhere)
	float RRagne = 500.0;

	UPROPERTY(EditAnywhere)
	float RDamage = 250.0;

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	FVector SwordLocation;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> SwordClass;
	

private:
	
	bool FirsttimeTarget = true;
	
	AActor* AttackTarget;

	/** A decal that projects to the cursor location. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UDecalComponent* CursorToWorld;
	
	UPROPERTY(EditAnywhere, Category = Health)
	float Health;

	UPROPERTY(EditAnywhere, Category = Health)
	float MaxHealth=620;

	UPROPERTY(VisibleAnywhere)
	class UBoxComponent* SwordCollission;

	UFUNCTION()
	void OnOverLapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintCallable)
	void ApplyDamageAfterAttack();

	int Level=1;

	void IgniteDamage();

	//Ignite variables and function
	int IgniteTick=10;
	float IgniteDamageAmount=7;
	AActor* IgniteActor;
	FTimerHandle IgniteTimer;

	APlayerController* PC;

	AActor* SpawnedSword;

	class UParticleSystemComponent* IgniteParticleComp;
	class UParticleSystemComponent* AblityEParticleSystem;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	class UWidgetComponent* HealthBarWidget;
};

