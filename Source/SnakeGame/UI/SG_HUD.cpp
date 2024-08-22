// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SG_HUD.h"
#include "UI/SG_GameplayWidget.h"


void ASG_HUD::BeginPlay()
{
	Super::BeginPlay();

	GameplayWidget = CreateWidget<USG_GameplayWidget>(GetWorld(), GameplayWidgetClass);
	check(GameplayWidget);

	GameplayWidget->AddToViewport();
}

void ASG_HUD::SetGameTime(double NewGameTime)
{
	GameplayWidget->SetGameTime(NewGameTime);
}

void ASG_HUD::SetGameScore(uint32 Score) 
{
	GameplayWidget->SetGameScore(Score);
}