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

void AGrid::PrepareMap(char *Path)
{
    Mission mission(Path);
    //mission.call(true);

    /*
    std::cout << Path << std::endl;
    std::cout << "Parsing the map from XML:" << std::endl;

    if (!mission.getMap()) {
        std::cout << "Incorrect map! Program halted!" << std::endl;
    }
    else {
        std::cout << "Map OK!" << std::endl << "Parsing configurations (algorithm, log) from XML:" << std::endl;
        if (!mission.getConfig())
            std::cout << "Incorrect configurations! Program halted!" << std::endl;
        else {
            std::cout << "Configurations OK!" << std::endl << "Creating log channel:" << std::endl;

            if (!mission.createLog())
                std::cout << "Log chanel has not been created! Program halted!" << std::endl;
            else {
                std::cout << "Log OK!" << std::endl << "Start searching the path:" << std::endl;

                mission.createEnvironmentOptions();
                mission.createSearch();

                /*
                mission.startSearch();

                std::cout << "Search is finished!" << std::endl;

                mission.printSearchResultsToConsole();
                mission.saveSearchResultsToLog();

                std::cout << "Results are saved (if chosen) via created log channel." << std::endl;
            }
        }
    }*/
}
