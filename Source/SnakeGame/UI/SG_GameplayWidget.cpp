// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SG_GameplayWidget.h"
#include "Components/TextBlock.h"

#include "World/SG_WorldUtils.h"

void USG_GameplayWidget::SetGameTime(double NewTime)
{
	if (TimeText) 
	{
		TimeText->SetText(game::WorldUtils::FormatSeconds(NewTime));
	}
}

void USG_GameplayWidget::SetGameScore(uint32 NewScore) 
{
	if (ScoreText) 
	{
		ScoreText->SetText(game::WorldUtils::FormatScore(NewScore));
	}
}
