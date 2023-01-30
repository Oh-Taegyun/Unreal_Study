// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerPawn.h"
#include "Components/BoxComponent.h" // 소스 코드에서는 전방 선언한 것을 세세하게 정의하겠다고 약속했다.
#include "Components/StaticMeshComponent.h"
#include "Components/ArrowComponent.h"
#include "Bullet.h"
#include "Kismet/GameplayStatics.h"

// Sets default values 
// 기본 값 설정
APlayerPawn::APlayerPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	// 모든 프레임에서 Tick()를 호출하도록 이 폰을 설정합니다. 필요 없는 경우 성능 향상을 위해 이 기능을 해제할 수 있습니다.
	PrimaryActorTick.bCanEverTick = true;

	// 박스 콜라이더 컴포넌트 생성
	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("My Box Component"));

	// 생성한 박스 콜라이더 컴포넌트를 최상단 컴포넌트로 설정한다.
	SetRootComponent(boxComp);

	// 스태틱 메시 컴포넌트 생성
	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("My Static Mesh"));

	meshComp->SetupAttachment(boxComp); // 루트 컴포넌트인 박스 콜라이더 컴포넌트의 자식으로 설정하기 

	// 박스 콜라이더의 크기를 50 x 50 x 50 으로 설정한다.
	FVector boxsize = FVector(50.0f, 50.0f, 50.0f);
	boxComp->SetBoxExtent(boxsize);

	// 총구 표시 컴포넌트를 생성하고 박스 컴포넌트의 자식 컴포넌트로 설정한다. 
	firePosition = CreateDefaultSubobject<UArrowComponent>(TEXT("Fire Position"));
	firePosition->SetupAttachment(boxComp);


	// 오버랩 이벤트를 켠다
	boxComp->SetGenerateOverlapEvents(true);

	// 충돌 응답을 Qurey And Physics로 설정한다.
	boxComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	
	// Object Type을 1번 채널(Player)로 설정한다.
	boxComp->SetCollisionObjectType(ECC_GameTraceChannel1);

	// 모든 채널을 충돌 응답 없음으로 설정한다.
	boxComp->SetCollisionResponseToAllChannels(ECR_Ignore);

	//에너미와는 충돌 이벤트를 체크(Quary) 한다.
	boxComp->SetCollisionResponseToChannel(ECC_GameTraceChannel2, ECR_Overlap);
	boxComp->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);


}

// Called when the game starts or when spawned
// 게임이 시작되거나 생성될 때 호출됨
void APlayerPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
// 모든 프레임 호출
void APlayerPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//사용자 입력 키를 이용해서
	// 1. 상하 입력 값과 좌우 입력 값을 이용해서 방향 벡터를 만든다
	FVector dir = FVector(0, h, v);

	// 2. 방향 벡터의 길이가 1이 되도록 벡터를 정규화한다.
	dir.Normalize();

	// 3. 이동할 위치 좌표를 구한다
	FVector newLocation = GetActorLocation() + dir * movespeed * DeltaTime;

	// 4. 현재 액터의 위치 좌표를 앞에서 구한 새 좌표로 갱신한다.
	SetActorLocation(newLocation, true);

}

// Called to bind functionality to input
// 기능을 입력에 바인딩하기 위해 호출됩니다
void APlayerPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Axis 바인딩 된 값을 함수에 연결한다
	PlayerInputComponent->BindAxis("Horizontal", this, &APlayerPawn::MoveHorizontal);
	PlayerInputComponent->BindAxis("Vertical", this, &APlayerPawn::MoveVertical);

	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &APlayerPawn::Fire);

}


void APlayerPawn::MoveHorizontal(float value)
{
	// 사용자의 입력 값(Axis)을 h 변수에 넣는다
	h = value;
}

void APlayerPawn::MoveVertical(float value)
{
	// 사용자의 입력 값(Axis)을 v 변수에 넣는다
	v = value;
}

void APlayerPawn::Fire()
{
	ABullet* bullet = GetWorld()->SpawnActor<ABullet>(bulletFactory, firePosition->GetComponentLocation(), firePosition->GetComponentRotation());

	//fireSound 변수에 할당된 음원 파일 실행
	UGameplayStatics::PlaySound2D(GetWorld(), fireSound);

}
