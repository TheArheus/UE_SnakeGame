// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Core/Snake.h"
#include "SG_WorldTypes.h"
#include "SG_Snake.generated.h"

class ASG_SnakeLink;

UCLASS()
class SNAKEGAME_API ASG_Snake : public AActor
{
	GENERATED_BODY()
	
public:
	ASG_Snake();

	void SetModel(const TSharedPtr<game::Snake>& Snake, uint32 CellSize, const game::dim& Dims);
	void UpdateColors(const FSnakeColors& NewColor);

	void Explode();

	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<ASG_SnakeLink> SnakeHeadClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<ASG_SnakeLink> SnakeLinkClass;

private:
	UPROPERTY()
	TArray<ASG_SnakeLink*> SnakeLinks;

	TWeakPtr<game::Snake> Snake;
	uint32 CellSize;
	game::dim Dims;

	FLinearColor SnakeLinkColor;
};
