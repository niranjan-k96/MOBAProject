// Fill out your copyright notice in the Description page of Project Settings.


#include "MinionAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Turret.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AIPerceptionTypes.h"
#include "Creatures.h"
#include "LeagueofLegendsCharacter.h"
#include "Kismet/GameplayStatics.h"

AMinionAIController::AMinionAIController()
{
	//TeamId = FGenericTeamId(TeamIdint);

	sightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
	SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerception Component")));
	
	sightConfig->SightRadius = AISightRadius;
	sightConfig->LoseSightRadius = AILoseSightRadius;
	sightConfig->PeripheralVisionAngleDegrees = PeriferalAngle;
	sightConfig->SetMaxAge(MaxAge);

	sightConfig->DetectionByAffiliation.bDetectEnemies = true;
	sightConfig->DetectionByAffiliation.bDetectNeutrals = true;
	sightConfig->DetectionByAffiliation.bDetectFriendlies = true;

	GetPerceptionComponent()->SetDominantSense(*sightConfig->GetSenseImplementation());
	GetPerceptionComponent()->ConfigureSense(*sightConfig);
	GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &AMinionAIController::OnTargetPerceptionUpdated);
	
}

void AMinionAIController::OnPossess(APawn* OwningPawn)
{
	Super::OnPossess(OwningPawn);
	UE_LOG(LogTemp, Warning, TEXT("Possessed"));
	OwningMinion = Cast<ACreatures>(OwningPawn);

	GetNearestTurret();
	if (MinionBehaviour != nullptr)
	{
		RunBehaviorTree(MinionBehaviour);
	}

	if (FocussedTurret == nullptr)
	{
		//UE_LOG(LogTemp, Warning, TEXT("HELOOOOOOO"));
		
	}
	if (FocussedTurret != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("FocussedTurretis set"));
		//UE_LOG(LogTemp, Warning, TEXT("HELOOOOOOO"));
		FVector NearestTurretLOc = FocussedTurret->GetRootComponent()->GetComponentLocation();
		GetBlackboardComponent()->SetValueAsVector("NearestTurretLocation", NearestTurretLOc);
	}
}

void AMinionAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


void AMinionAIController::OnTargetPerceptionUpdated(AActor* SensedActor, FAIStimulus Stimulus)
{
	if (Stimulus.WasSuccessfullySensed())
	{
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "Updated perception");

		if (ACreatures* DetectedCreature = Cast<ACreatures>(SensedActor))
		{
			if (DetectedCreature->GetTeamID() != OwningMinion->GetTeamID())
			{
				//UE_LOG(LogTemp, Warning, TEXT("Attack Target NOt set"));
				EnemiesinRange.Add(DetectedCreature);
			}
		}
		else if (ALeagueofLegendsCharacter* Hero = Cast<ALeagueofLegendsCharacter>(SensedActor))
		{
			if (Hero->GetTeamID() != OwningMinion->GetTeamID())
			{
				UE_LOG(LogTemp, Warning, TEXT("Detected hero"));
				EnemiesinRange.Add(Hero);
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Sensed actor is none of theses"));
			return;
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Hero Went out"));
		EnemiesinRange.Remove(SensedActor);
		if (OwningMinion->GetAttackTarget() == SensedActor)
		{
			OwningMinion->CanAttack = false;
			OwningMinion->SetAttackTarget(nullptr);
			GetBlackboardComponent()->SetValueAsObject("FocusActor", nullptr);
		}
	}
	DetermineFocusActor();
}

void AMinionAIController::DetermineFocusActor()
{
	
	if (EnemiesinRange.Num() == 0)
	{	
		
		OwningMinion->SetAttackTarget(nullptr);
		GetBlackboardComponent()->SetValueAsObject("FocusActor", nullptr);
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "No actors in range");
	}
	else if (EnemiesinRange.Num() == 1)
	{
		if (GetBlackboardComponent() == nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("BB Is nullptr"));
		}
		OwningMinion->SetAttackTarget(EnemiesinRange[0]);
		//GetBlackboardComponent()->SetValueAsObject("FocusActor", EnemiesinRange[0]);
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "2 actor in range");
		return;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Enemies >1"));
		UObject* FocusActor = GetBlackboardComponent()->GetValueAsObject("FocusActor");
		if (FocusActor == nullptr)
		{
			int ivalueminion = 99;
			int ivaluehero = 99;
			bool Minioninrange = false;
			float shortestDistance = 99999;
			
			for (int32 i = 0; i < EnemiesinRange.Num(); i++)
			{
				if (ACreatures* Minion = Cast<ACreatures>(EnemiesinRange[i]))
				{
					Minioninrange = true;
					float Distance = OwningMinion->GetDistanceTo(Minion);

					if (Distance < shortestDistance)
					{
						shortestDistance = Distance;
						ivalueminion = i;
					}
				}

				if (Minioninrange == false)
				{
					if (Cast<ALeagueofLegendsCharacter>(EnemiesinRange[i]))
					{
						ivaluehero = i;
					}
				}
			}
			if (Minioninrange==true)
			{
				OwningMinion->SetAttackTarget(EnemiesinRange[ivalueminion]);
				GetBlackboardComponent()->SetValueAsObject("FocusActor", EnemiesinRange[ivalueminion]);
			}
			
			else
			{
				OwningMinion->SetAttackTarget(EnemiesinRange[ivaluehero]);
				GetBlackboardComponent()->SetValueAsObject("FocusActor", EnemiesinRange[ivaluehero]);
			}
			
		}
	}
}

void AMinionAIController::GetNearestTurret()
{
	TArray<AActor*> AllTurrets;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), TurretClass, AllTurrets);
	float NearestDist = 9999;
	AActor* NearestTurret = nullptr;
	//UE_LOG(LogTemp, Warning, TEXT("Get nearest turret called"));
	
	for (int i = 0; i < AllTurrets.Num(); i++) 
	{
		//UE_LOG(LogTemp, Warning, TEXT("Get nearest turret called"));
		float Dist= OwningMinion->GetDistanceTo(AllTurrets[i]);
		if (Dist < NearestDist)
		{
			Dist = NearestDist;
			NearestTurret=AllTurrets[i];
			UE_LOG(LogTemp, Warning, TEXT("Distance is less"))
		}
		else
		{ 
			UE_LOG(LogTemp, Warning, TEXT("Distance is actually more"));
		}
	}
	if (NearestTurret==nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Nearest Turret is null ptr"));
	}
	FocussedTurret = NearestTurret;
}

void AMinionAIController::BeginPlay()
{
	Super::BeginPlay();

	
	if (GetPerceptionComponent()!= nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("ALL SEt"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Perception component is nullptr"));
	}
	
}