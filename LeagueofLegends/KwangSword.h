// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "KwangSword.generated.h"

UCLASS()
class LEAGUEOFLEGENDS_API AKwangSword : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AKwangSword();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly)
	class USceneComponent* RootScene;
	UPROPERTY(EditDefaultsOnly)
	class UCapsuleComponent* MeshCapsule;
	UPROPERTY(EditDefaultsOnly)
	class UStaticMeshComponent* SwordMesh;
	
	UPROPERTY(EditAnywhere)
	float InitialSwordDamage = 100;

	UPROPERTY(EditAnywhere)
	float SwordSweepDamage = 60;

	UFUNCTION()
	void OnOverLapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	bool Sweeping = false;

	void BeginPlayDelay();

private:
	TSet<AActor*> ActorsthatOverlap;
	
	class ALeagueofLegendsCharacter* PlayerPawn;
};


