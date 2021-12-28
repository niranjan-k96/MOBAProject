// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Turret.generated.h"
class AProjectile;
UCLASS()
class LEAGUEOFLEGENDS_API ATurret : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATurret();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class UWidgetComponent* HealthBarWidget;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	float attackrate = 2;

	float firstdelay = 1.5;
	
	UPROPERTY()
	AActor* TurretTarget;
	//FORCEINLINE class AActor* GetTurretTarget() { return TurretTarget; }

	UFUNCTION()
	AActor* GetTurretTarget();

	UFUNCTION()
	void SetTurretTarget();


	UPROPERTY()
	bool ShouldCheckOverlap = true;

	UPROPERTY()
	TArray<AActor*> OverlappingActorsRes;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Teams")
	int TeamId = 1;

	FORCEINLINE int GetTeamID() { return TeamId; }

	UFUNCTION()
	float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	TArray<AActor*> EnemiesinRange;

	UFUNCTION(BlueprintCallable)
	float GetHelathPercent();

private:
	UPROPERTY(EditAnywhere)
	class USceneComponent* RootScene;

	UPROPERTY(EditAnywhere)
	class UCapsuleComponent* MeshCapsule;

	UPROPERTY(EditAnywhere)
	class USceneComponent* ProjectileSpawnPoint;

	UPROPERTY(EditAnywhere)
	class UCapsuleComponent* Attackrange;

	UPROPERTY(EditAnywhere)
	class USkeletalMeshComponent* TurretMesh;

	FTimerHandle TurretTimer;

	ACharacter* PlayerCharacter;

	UFUNCTION()
	void SpawnProjectile();

	UFUNCTION()
	void OnOverLapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverLapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectile", meta = (AllowPrivateAccess = true))
	TSubclassOf<AProjectile> ProjectileClass;

	float MaxHealth = 5000;

	float Health = 100;

	
};
