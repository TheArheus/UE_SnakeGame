// Fill out your copyright notice in the Description page of Project Settings.


#include "World/SG_Grid.h"
#include "Core/Grid.h"
#include "DrawDebugHelpers.h"
#include "Components/LineBatchComponent.h"
#include "Components/StaticMeshComponent.h"


ASG_Grid::ASG_Grid()
{
	PrimaryActorTick.bCanEverTick = false;

	Origin = CreateDefaultSubobject<USceneComponent>("Origin");
	check(Origin);
	SetRootComponent(Origin);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	check(Mesh);
	Mesh->SetupAttachment(Origin);
}

void ASG_Grid::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASG_Grid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASG_Grid::SetModel(const TSharedPtr<game::Grid>& Grid, uint32 InCellSize)
{
	if (!Grid.IsValid()) 
	{
		UE_LOG(LogTemp, Fatal, TEXT("Grid pointer is nullptr, game is terminated"));
	}

	GridDim = Grid.Get()->c_dim;
	CellSize = InCellSize;
	WorldWidth = GridDim.Width * CellSize;
	WorldHeight = GridDim.Height * CellSize;

	check(Mesh->GetStaticMesh());
	const FBox Box  = Mesh->GetStaticMesh()->GetBoundingBox();
	const auto Size = Box.GetSize();

	check(Size.X);
	check(Size.Y);
	Mesh->SetRelativeScale3D(FVector(WorldHeight / Size.X, WorldWidth / Size.Y, 1.0));
	Mesh->SetRelativeLocation(0.5 * FVector(WorldHeight, WorldWidth, -Size.Z));

	Mat = Mesh->CreateAndSetMaterialInstanceDynamic(0);
	if (Mat) 
	{
		Mat->SetVectorParameterValue("Division", FVector(GridDim.Height, GridDim.Width, 0.0));
	}
}

void ASG_Grid::UpdateColors(const FSnakeColors& NewColor)
{
	if (Mat) 
	{
		Mat->SetVectorParameterValue("BackgroundColor", NewColor.GridBackgroundColor);
		Mat->SetVectorParameterValue("LineColor", NewColor.GridLineColor);
		Mat->SetVectorParameterValue("WallColor", NewColor.GridWallColor);
	}
}

void ASG_Grid::DrawGrid()
{
	if (!GetWorld() || !GetWorld()->LineBatcher) return;

	for (uint32 y = 0; y <= GridDim.Height; y++)
	{
		const FVector Start = GetActorLocation() + GetActorForwardVector() * CellSize * y;
		//DrawDebugLine(GetWorld(), Start, Start + GetActorRightVector() * WorldWidth, FColor::Red, false, -1.0, 0, 1.0f);
		GetWorld()->LineBatcher->DrawLine(Start, Start + GetActorRightVector() * WorldWidth, FLinearColor::Red, 0, 1.0f);
	}

	for (uint32 x = 0; x <= GridDim.Width; x++)
	{
		const FVector Start = GetActorLocation() + GetActorRightVector() * CellSize * x;
		//DrawDebugLine(GetWorld(), Start, Start + GetActorForwardVector() * WorldHeight, FColor::Red, false, -1.0, 0, 1.0f);
		GetWorld()->LineBatcher->DrawLine(Start, Start + GetActorForwardVector() * WorldHeight, FLinearColor::Red, 0, 1.0f);
	}
}

