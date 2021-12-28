// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "GenericTeamAgentInterface.h"
#include "Perception/AIPerceptionTypes.h"
#include "Turret.h"
#include "MinionAIController.generated.h"


UCLASS()
class LEAGUEOFLEGENDS_API AMinionAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	AMinionAIController();

	UPROPERTY()
	class UBlackboardComponent* MinionBlackboard;

	UPROPERTY(EditAnywhere)
	class UBehaviorTree* MinionBehaviour;

	virtual void OnPossess(APawn* OwningPawn) override;

	class ACreatures* OwningMinion;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	FVector OppositeTeamLocation;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SRPerception")
	class  UAISenseConfig_Sight* sightConfig;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	float AISightRadius = 500.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	float AILoseSightRadius = 550.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	float PeriferalAngle = 360.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	float MaxAge = 3.0f;

	//virtual FGenericTeamId GetGenericTeamId() const override { return TeamId; }

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	//int TeamIdint = 1;

	TArray<AActor*> EnemiesinRange;

	void DetermineFocusActor();

	void GetNearestTurret();
private:

	APawn* PlayerPawn;

	UFUNCTION()
	void OnTargetPerceptionUpdated(AActor* SensedActor, FAIStimulus Stimulus);
	
	int TeamId = 0;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ATurret> TurretClass;

	UPROPERTY();
	AActor* FocussedTurret;

protected:

	void BeginPlay();
};
