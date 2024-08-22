// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Camera/CameraComponent.h"
#include "Core/Intrinsics.h"
#include "SG_Camera.generated.h"

class UCameraComponent;

UCLASS()
class SNAKEGAME_API ASG_Camera : public APawn
{
	GENERATED_BODY()

public:
	ASG_Camera();

	void UpdateLocation(const game::dim& Dim, uint32 CellSize, const FTransform& GridOrigin);

protected:
	UPROPERTY(VisibleAnywhere)
	USceneComponent* Origin;

    UPROPERTY(VisibleAnywhere)
    UCameraComponent* Camera;

private:
    game::dim Dim;
    uint32 CellSize;
    FTransform GridOrigin;

    FDelegateHandle ResizeHandle;

    void OnViewportResized(FViewport* Viewport, uint32 Val);
};
