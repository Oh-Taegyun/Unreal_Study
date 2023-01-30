// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyActor.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "EngineUtils.h"
#include "PlayerPawn.h"
#include "ShootingGameModeBase.h"

// Sets default values
AEnemyActor::AEnemyActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// ������Ʈ ȣ��
	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collider"));
	mashComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static mesh"));

	// ������Ʈ �θ� �ڽ� ����
	SetRootComponent(boxComp);
	mashComp->SetupAttachment(boxComp);

	// boxComp ����
	boxComp->SetBoxExtent(FVector(50.0f, 50.0f, 50.0f));

	// Collision presets�� Enemy ���������� �����Ѵ�.
	boxComp->SetCollisionProfileName(TEXT("Enemy"));
}

// Called when the game starts or when spawned
void AEnemyActor::BeginPlay()
{
	Super::BeginPlay();

	// 1 ~ 100 ������ ������ ���� ���� ��÷�Ѵ�.
	int32 drawResult = FMath::RandRange(1, 100);

	if (drawResult <= traceRate) // ���࿡ ��÷�� ���� ������ ������ �۴ٸ�
	{
		for (TActorIterator<APlayerPawn> player(GetWorld()); player; ++player) // �÷��̾ ã�Ƴ���. 
		{
			if (player->GetName().Contains(TEXT("BP_PlayerPawn")))
			{
				dir = player->GetActorLocation() - GetActorLocation();
				dir.Normalize(); // �÷��̾��� ������ �˾Ƴ���.
			}
		}
	}
	else
	{
		dir = GetActorForwardVector();
	}
	
	boxComp->OnComponentBeginOverlap.AddDynamic(this, &AEnemyActor::OnEnemyOverlap);
}

// Called every frame
void AEnemyActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector newLocation = GetActorLocation() + dir * moveSpeed * DeltaTime;
	SetActorLocation(newLocation);

}

void AEnemyActor::OnEnemyOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	APlayerPawn* player = Cast<APlayerPawn>(OtherActor);

	if (player != nullptr)
	{
		OtherActor->Destroy();

		AShootingGameModeBase* currentGameMode = Cast<AShootingGameModeBase>(GetWorld()->GetAuthGameMode());

		if (currentGameMode != nullptr)
		{
			// �޴� UI ���� �Լ��� ȣ���Ѵ�.
			currentGameMode->ShowMenu();
		}
	}

	Destroy();
}
