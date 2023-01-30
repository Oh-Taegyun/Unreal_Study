// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyFactory.h"
#include "EnemyActor.h"

// Sets default values
AEnemyFactory::AEnemyFactory()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AEnemyFactory::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEnemyFactory::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (currentTime > delayTime) // 만일 경과된 시간이 생성할 시간을 초과했다면
	{
		currentTime = 0;// 경과된 시간 0으로 초기화

		AEnemyActor* spawnActor = GetWorld()->SpawnActor<AEnemyActor>(enemy, GetActorLocation(), GetActorRotation());
	}
	else
	{
		currentTime += DeltaTime; // 현재 프레임의 경과 시간을 누적시킨다. 
	}

}

