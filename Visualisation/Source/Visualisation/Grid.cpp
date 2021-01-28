// Fill out your copyright notice in the Description page of Project Settings.


#include "Grid.h"
#include "SearchAlgorithm/Src/mission.h"

// Sets default values
AGrid::AGrid()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootScene"));

    Walls = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Walls"));
    Walls->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
    Floor = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Floor"));
    Floor->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
}

// Called when the game starts or when spawned
void AGrid::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AGrid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGrid::PrepareMap()
{
    Mission mission(TCHAR_TO_ANSI(*PathToTask));

    if (!mission.getMap() || !mission.getConfig() || !mission.createLog()) {
        UE_LOG(LogTemp, Warning, TEXT("Fail!"));
        return;
    }
    
    Walls->ClearInstances();
    Floor->ClearInstances();

    Map CurrentMap = mission.getMapObject();
    
    Height = CurrentMap.getMapHeight();
    Width = CurrentMap.getMapWidth();

    for (int GridX = 0; GridX < Height; ++GridX)
        for (int GridY = 0; GridY < Width; ++GridY)
        {
            FTransform NewTransform; // Local 0 in terms of transform
            NewTransform.AddToTranslation({ GridX * MeshSize, GridY * MeshSize, 0 });

            if (CurrentMap.CellIsTraversable(GridX, GridY))
                Floor->AddInstance(NewTransform);
            else
                Walls->AddInstance(NewTransform);
        }

    mission.createEnvironmentOptions();
    mission.createSearch();

    UE_LOG(LogTemp, Warning, TEXT("Search created, size %d x %d"), Height, Width);

    /*
    mission.startSearch();

    mission.printSearchResultsToConsole();
    mission.saveSearchResultsToLog();
    */
}
