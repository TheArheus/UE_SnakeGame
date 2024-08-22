// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Core/Intrinsics.h"
#include "World/SG_WorldTypes.h"
#include "SG_Grid.generated.h"

namespace game
{
	class Grid;
};

UCLASS()
class SNAKEGAME_API ASG_Grid : public AActor
{
	GENERATED_BODY()

	void DrawGrid();
	
public:
	ASG_Grid();

	virtual void Tick(float DeltaTime) override;

	void SetModel(const TSharedPtr<game::Grid>& Grid, uint32 CellSize);
	void UpdateColors(const FSnakeColors& NewColor);

protected:
	UPROPERTY(VisibleAnywhere)
	USceneComponent* Origin;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* Mesh;

	virtual void BeginPlay() override;

private:
	UPROPERTY()
	UMaterialInstanceDynamic* Mat;

	game::dim GridDim;
	uint32 CellSize;
	uint32 WorldWidth;
	uint32 WorldHeight;
};
