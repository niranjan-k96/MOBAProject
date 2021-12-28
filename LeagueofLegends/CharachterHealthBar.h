// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CharachterHealthBar.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class LEAGUEOFLEGENDS_API UCharachterHealthBar : public UUserWidget
{
	GENERATED_BODY()
	
public:
	int number = 10;

};
