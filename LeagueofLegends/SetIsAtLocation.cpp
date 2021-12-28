// Fill out your copyright notice in the Description page of Project Settings.


#include "SetIsAtLocation.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Creatures.h"
#include "AIController.h"

void USetIsAtLocation::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	{
		if (ACreatures* OwnerMinion=Cast<ACreatures>(OwnerComp.GetAIOwner()->GetPawn()))
		{

			float enemyDistance =OwnerMinion->GetDistanceTo(OwnerMinion->GetAttackTarget());
			UE_LOG(LogTemp, Warning, TEXT("enemy Distance %f"),enemyDistance);
			if (enemyDistance <= OwnerMinion->GetAttackRange())
			{
				OwnerComp.GetBlackboardComponent()->SetValueAsBool(GetSelectedBlackboardKey(), true);
				UE_LOG(LogTemp, Warning, TEXT("Is at lOcation "));
			}
			else
			{
				OwnerComp.GetBlackboardComponent()->SetValueAsBool(GetSelectedBlackboardKey(), false);
				UE_LOG(LogTemp, Warning, TEXT("Is NOt at lOcation "));
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("helloo"));
		}
		
	}
}