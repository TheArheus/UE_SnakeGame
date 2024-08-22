// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Core/Intrinsics.h"
#include "SG_Food.generated.h"

namespace game
{
	struct Food;
}

class UNiagaraSystem;

UCLASS()
class SNAKEGAME_API ASG_Food : public AActor
{
	GENERATED_BODY()
	
public:	
	ASG_Food();

	virtual void Tick(float DeltaTime) override;

	void SetModel(const TSharedPtr<game::Food>& Grid, uint32 CellSize, const game::dim& Dims);
	void UpdateColor(const FLinearColor& NewColor);

	void Explode();
	FVector GetPosition();

protected:
	UPROPERTY(VisibleAnywhere)
	USceneComponent* Origin;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* Mesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
	TObjectPtr<UNiagaraSystem> ExplosionEffect;

private:
	TWeakPtr<game::Food> Food;
	FLinearColor Color;
	uint32 CellSize;
	game::dim Dims;
};
