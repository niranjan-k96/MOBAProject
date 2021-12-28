// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

UCLASS()
class LEAGUEOFLEGENDS_API AProjectile : public AActor
{
	GENERATED_BODY()
	
private:


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = true));
	class UProjectileMovementComponent* ProjectileMovement;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = true));
	class UCapsuleComponent* ProjectileCapsule;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = true));
	class UStaticMeshComponent* ProjectileMesh;

	UPROPERTY(EditDefaultsOnly, Category = "Damage");
	TSubclassOf<UDamageType> DamageType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MOve", meta = (AllowPrivateAccess = true));
	float MovementSpeed = 10000;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Damage", meta = (AllowPrivateAccess = true));
	float Damage = 120;

	ACharacter* PlayerChar;

	FVector PlayerLocation;

	UFUNCTION()
	void OnOverLapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(EditAnywhere)
	float TurretDamage = 20.f;

	FHitResult HitResult;


	void GetTargetfromTurret();

	class ATurret* OwnerTurret;

public:	

	// Sets default values for this actor's properties
	AProjectile();

	FORCEINLINE UProjectileMovementComponent* GetProjectileMovement() const { return ProjectileMovement; }

	AActor* EnemytoAttack;

	bool Attack;

	UPROPERTY(EditAnywhere, Category = "Projectile Speed");
	float ProjectileSpeed = 200;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	
};
