// Fill out your copyright notice in the Description page of Project Settings.


#include "Grid.h"
#include "SearchAlgorithm/Src/mission.h"

// Sets default values
AGrid::AGrid()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

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
        return;
    }

    mission.createEnvironmentOptions();
    mission.createSearch();

    UE_LOG(LogTemp, Warning, TEXT("Search created"));

    /*
    mission.startSearch();

    mission.printSearchResultsToConsole();
    mission.saveSearchResultsToLog();
    */
}
