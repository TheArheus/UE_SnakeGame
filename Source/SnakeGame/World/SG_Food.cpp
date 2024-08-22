// Fill out your copyright notice in the Description page of Project Settings.


#include "World/SG_Food.h"
#include "World/SG_WorldUtils.h"
#include "Core/Food.h"

#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"

ASG_Food::ASG_Food()
{
	PrimaryActorTick.bCanEverTick = true;

	Origin = CreateDefaultSubobject<USceneComponent>("Origin");
	check(Origin);
	SetRootComponent(Origin);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	check(Mesh);
	Mesh->SetupAttachment(Origin);
}

void ASG_Food::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Food.IsValid())
	{
		SetActorLocation(GetPosition());
	}
}

void ASG_Food::SetModel(const TSharedPtr<game::Food>& InFood, uint32 InCellSize, const game::dim& InDims)
{
	Food = InFood;
	CellSize = InCellSize;
	Dims = InDims;

	check(Mesh->GetStaticMesh());
	const FBox Box = Mesh->GetStaticMesh()->GetBoundingBox();
	const auto Size = Box.GetSize();

	check(Size.X);
	check(Size.Y);
	Mesh->SetRelativeScale3D(FVector(CellSize / Size.X, CellSize / Size.Y, CellSize / Size.Z));
}

void ASG_Food::UpdateColor(const FLinearColor& NewColor)
{
	Color = NewColor;
	if (auto* Mat = Mesh->CreateAndSetMaterialInstanceDynamic(0))
	{
		Mat->SetVectorParameterValue("Color", Color);
	}
}

void ASG_Food::Explode()
{
	if (UNiagaraComponent* FX = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ExplosionEffect, GetPosition())) 
	{
		FX->SetNiagaraVariableLinearColor("SnakeColor", Color);
	}
}

FVector ASG_Food::GetPosition() 
{
	if (!Food.IsValid()) return {};
	return game::WorldUtils::LinkPositionToVector(Food.Pin()->Pos, CellSize, Dims);
}
