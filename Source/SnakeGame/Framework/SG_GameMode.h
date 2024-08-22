// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SnakeGame/Core/Game.h"
#include "SnakeGame/Core/Grid.h"
#include "SnakeGame/World/SG_WorldTypes.h"
#include "GameFramework/GameModeBase.h"
#include "InputActionValue.h"
#include "SG_GameMode.generated.h"

/**
 * 
 */

class ASG_Grid;
class ASG_Snake;
class ASG_Food;
class ASG_HUD;
class AExponentialHeightFog;
class UInputAction;
class UInputMappingContext;

UCLASS()
class SNAKEGAME_API ASG_GameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	ASG_GameMode();
	virtual void StartPlay() override;
	virtual void Tick(float dt) override;

protected:
	UPROPERTY(EditDefaultsOnly, meta = (ClampMin = "10", ClampMax = "100"))
	FUintPoint GridSize{10, 10};

	UPROPERTY(EditDefaultsOnly, meta = (ClampMin = "10", ClampMax = "100"))
	uint32 CellSize{10};

	UPROPERTY(EditDefaultsOnly, meta = (ClampMin = "2"))
	uint32 SnakeDefaultSize{2};

	UPROPERTY(EditDefaultsOnly, Category = "Design")
	TSubclassOf<ASG_Grid> GridVisualClass;

	UPROPERTY(EditDefaultsOnly, Category = "Design")
	TSubclassOf<ASG_Snake> SnakeVisualClass;

	UPROPERTY(EditDefaultsOnly, Category = "Design")
	TSubclassOf<ASG_Food> FoodVisualClass;

	UPROPERTY(EditDefaultsOnly, Category = "Design")
	TObjectPtr<UDataTable> ColorsTable;

	UPROPERTY(EditDefaultsOnly, Category = "SnakeInput")
	TObjectPtr<UInputAction> MoveForwardInputAction;

	UPROPERTY(EditDefaultsOnly, Category = "SnakeInput")
	TObjectPtr<UInputAction> MoveRightInputAction;

	UPROPERTY(EditDefaultsOnly, Category = "SnakeInput")
	TObjectPtr<UInputAction> ResetGameInputAction;

	UPROPERTY(EditDefaultsOnly, Category = "SnakeInput")
	TObjectPtr<UInputMappingContext> InputMapping;

private:

	UPROPERTY()
	TObjectPtr<ASG_Grid> GridVisual;

	UPROPERTY()
	TObjectPtr<ASG_Snake> SnakeVisual;

	UPROPERTY()
	TObjectPtr<ASG_Food> FoodVisual;

	UPROPERTY()
	TObjectPtr<ASG_HUD> HUD;

	UPROPERTY()
	TObjectPtr<AExponentialHeightFog> Fog;

	UFUNCTION(Exec, Category = "Console command")
	void NextColor();

	TUniquePtr<game::Game> Game;
	uint32 ColorTableIndex;
	FIntVector2 SnakeInput{1, 0};

	void FindFog();
	void UpdateColors();
	void SetupInput();

	void OnMoveForward(const FInputActionValue& Value);
	void OnMoveRight(const FInputActionValue& Value);
	void OnGameReset(const FInputActionValue& Value);

	void SubscribeOnGameEvents();
};
