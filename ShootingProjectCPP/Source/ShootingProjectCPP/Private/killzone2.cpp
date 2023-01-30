// Fill out your copyright notice in the Description page of Project Settings.


#include "killzone2.h"
#include "Components/BoxComponent.h"

// Sets default values
Akillzone2::Akillzone2()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collider"));
	SetRootComponent(boxComp);

	// �ڽ� ������Ʈ�� �����Ƽ�� ���� ���·� �����Ѵ�.
	boxComp->SetMobility(EComponentMobility::Static);

	// �ڽ��� ũ�⸦ 50,2000,50���� �����Ѵ�
	boxComp->SetBoxExtent(FVector(50, 2000, 50));

	// �ڽ� ������Ʈ�� �ݸ��� �������� Killzone���� �����Ѵ�.
	boxComp->SetCollisionProfileName(TEXT("KillZone"));

}

// Called when the game starts or when spawned
void Akillzone2::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void Akillzone2::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

