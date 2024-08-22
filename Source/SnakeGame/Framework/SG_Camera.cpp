// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/SG_Camera.h"

static double CalculateFovTan(double Degrees) 
{
	return FMath::Tan(FMath::DegreesToRadians(Degrees * 0.5));
}

static double VerticalFOV(double HorFOVDegrees, double ViewportAspectHW)
{
	return FMath::RadiansToDegrees(2.0 * FMath::Atan(FMath::Tan(FMath::DegreesToRadians(HorFOVDegrees) * 0.5) * ViewportAspectHW));
}

ASG_Camera::ASG_Camera()
{
	PrimaryActorTick.bCanEverTick = false;

	Origin = CreateDefaultSubobject<USceneComponent>("Origin");
	check(Origin);
	SetRootComponent(Origin);

	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	check(Camera);
	Camera->SetupAttachment(Origin);
}

void ASG_Camera::UpdateLocation(const game::dim& InDim, uint32 InCellSize, const FTransform& InGridOrigin)
{
    Dim = InDim;
    CellSize = InCellSize;
    GridOrigin = InGridOrigin;

    check(GEngine);
    check(GEngine->GameViewport);
    check(GEngine->GameViewport->Viewport);

    auto* Viewport = GEngine->GameViewport->Viewport;
    Viewport->ViewportResizedEvent.Remove(ResizeHandle);
    ResizeHandle = Viewport->ViewportResizedEvent.AddUObject(this, &ASG_Camera::OnViewportResized);

#if WITH_EDITOR
    OnViewportResized(Viewport, 0);
#endif
}

void ASG_Camera::OnViewportResized(FViewport* Viewport, uint32 Val)
{
    if (!Viewport || Viewport->GetSizeXY().Y == 0 || Dim.Height == 0) return;

    const double WorldWidth  = Dim.Width  * CellSize;
    const double WorldHeight = Dim.Height * CellSize;

    const double Margin = 5.0;
    const double MarginWidth  = (Dim.Width  + Margin) * CellSize;
    const double MarginHeight = (Dim.Height + Margin) * CellSize;

    double LocationZ = 0.0f;
    const double ViewportAspect = static_cast<double>(Viewport->GetSizeXY().X) / Viewport->GetSizeXY().Y;
    const double GridAspect = static_cast<double>(Dim.Width) / Dim.Height;

    if (ViewportAspect <= GridAspect)
    {
        LocationZ = MarginWidth / CalculateFovTan(Camera->FieldOfView);
    }
    else
    {
        check(ViewportAspect);
        const double VFOV = VerticalFOV(Camera->FieldOfView, 1.0 / ViewportAspect);
        LocationZ = MarginHeight / CalculateFovTan(VFOV);
    }

	const FVector NewLocation = GridOrigin.GetLocation() + 0.5 * FVector(WorldHeight, WorldWidth, LocationZ);
	SetActorLocation(NewLocation);
}
