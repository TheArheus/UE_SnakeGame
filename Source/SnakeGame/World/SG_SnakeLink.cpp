// Fill out your copyright notice in the Description page of Project Settings.


#include "World/SG_SnakeLink.h"
#include "Components/LineBatchComponent.h"
#include "Components/StaticMeshComponent.h"

#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"


ASG_SnakeLink::ASG_SnakeLink()
{
	PrimaryActorTick.bCanEverTick = false;

	Origin = CreateDefaultSubobject<USceneComponent>("Origin");
	check(Origin);
	SetRootComponent(Origin);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	check(Mesh);
	Mesh->SetupAttachment(Origin);
}

void ASG_SnakeLink::UpdateColor(const FLinearColor& NewColor) 
{
	Color = NewColor;
	if (auto* Mat = Mesh->CreateAndSetMaterialInstanceDynamic(0))
	{
		Mat->SetVectorParameterValue("Color", Color);
	}
}

void ASG_SnakeLink::UpdateScale(uint32 CellSize) 
{
	check(Mesh->GetStaticMesh());
	const FBox Box = Mesh->GetStaticMesh()->GetBoundingBox();
	const auto Size = Box.GetSize();

	check(Size.X);
	check(Size.Y);
	Mesh->SetRelativeScale3D(FVector(CellSize / Size.X, CellSize / Size.Y, CellSize / Size.Z));
}

void ASG_SnakeLink::Explode()
{
	if (UNiagaraComponent* FX = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ExplosionEffect, GetActorLocation()))
	{
		FX->SetNiagaraVariableLinearColor("SnakeColor", Color);
	}
	SetActorHiddenInGame(true);
}
