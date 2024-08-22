// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "SG_HUD.generated.h"

class USG_GameplayWidget;

UCLASS()
class SNAKEGAME_API ASG_HUD : public AHUD
{
	GENERATED_BODY()

public:
	void SetGameTime(double NewGameTime);
	void SetGameScore(uint32 Score);

protected:
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<USG_GameplayWidget> GameplayWidgetClass;

	virtual void BeginPlay() override;

private:
	UPROPERTY();
	TObjectPtr<USG_GameplayWidget> GameplayWidget;

	double GameTime;
};
