// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SG_GameplayWidget.generated.h"


class UTextBlock;

UCLASS()
class SNAKEGAME_API USG_GameplayWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void SetGameTime(double NewTime);
	void SetGameScore(uint32 NewScore);

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TimeText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ScoreText;

private:
};
