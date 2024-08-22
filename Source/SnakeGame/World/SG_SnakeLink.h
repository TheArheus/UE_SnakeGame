// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SG_SnakeLink.generated.h"

class UNiagaraSystem;

UCLASS()
class SNAKEGAME_API ASG_SnakeLink : public AActor
{
	GENERATED_BODY()

public:
	ASG_SnakeLink();

	void UpdateColor(const FLinearColor& NewColor);
	void UpdateScale(uint32 CellSize);

	void Explode();

protected:
	UPROPERTY(VisibleAnywhere)
	USceneComponent* Origin;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* Mesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
	TObjectPtr<UNiagaraSystem> ExplosionEffect;

private:
	FLinearColor Color;
};
