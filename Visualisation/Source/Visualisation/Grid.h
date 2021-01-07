// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "Grid.generated.h"

UCLASS()
class VISUALISATION_API AGrid : public AActor
{
	GENERATED_BODY()
	
protected:
    UPROPERTY()
    int Height;
    
    UPROPERTY()
    int Width;

public:
    UPROPERTY(EditAnywhere)
    FString PathToTask = "";

    UPROPERTY(EditDefaultsOnly)
    UInstancedStaticMeshComponent* Walls;

    UPROPERTY(EditDefaultsOnly)
    UInstancedStaticMeshComponent* Floor;

    UPROPERTY(EditDefaultsOnly)
    float MeshSize;

	// Sets default values for this actor's properties
	AGrid();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

    UFUNCTION(BlueprintCallable)
    void PrepareMap();
};
