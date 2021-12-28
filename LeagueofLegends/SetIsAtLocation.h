// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "SetIsAtLocation.generated.h"

/**
 * 
 */
UCLASS()
class LEAGUEOFLEGENDS_API USetIsAtLocation : public UBTService_BlackboardBase
{
	GENERATED_BODY()
	
protected:

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	bool NullValue;
};
