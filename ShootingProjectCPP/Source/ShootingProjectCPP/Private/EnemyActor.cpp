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

	// 컴포넌트 호출
	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collider"));
	mashComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static mesh"));

	// 컴포넌트 부모 자식 설정
	SetRootComponent(boxComp);
	mashComp->SetupAttachment(boxComp);

	// boxComp 설정
	boxComp->SetBoxExtent(FVector(50.0f, 50.0f, 50.0f));

	// Collision presets을 Enemy 프리셋으로 변경한다.
	boxComp->SetCollisionProfileName(TEXT("Enemy"));
}

// Called when the game starts or when spawned
void AEnemyActor::BeginPlay()
{
	Super::BeginPlay();

	// 1 ~ 100 사이의 임의의 정수 값을 추첨한다.
	int32 drawResult = FMath::RandRange(1, 100);

	if (drawResult <= traceRate) // 만약에 추첨한 값이 설정한 값보다 작다면
	{
		for (TActorIterator<APlayerPawn> player(GetWorld()); player; ++player) // 플레이어를 찾아낸다. 
		{
			if (player->GetName().Contains(TEXT("BP_PlayerPawn")))
			{
				dir = player->GetActorLocation() - GetActorLocation();
				dir.Normalize(); // 플레이어의 방향을 알아낸다.
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
			// 메뉴 UI 생성 함수를 호출한다.
			currentGameMode->ShowMenu();
		}
	}

	Destroy();
}
