// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSPlayer.h"
#include <GameFramework/SpringArmComponent.h>
#include <Camera/CameraComponent.h>
#include "Bullet.h"
#include <Blueprint/UserWidget.h>
#include <Kismet/GameplayStatics.h>
#include "EnemyFSM.h"
#include <GameFramework/CharacterMovementComponent.h>
#include "RE_PlayerAnim.h"

// Sets default values
ATPSPlayer::ATPSPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 1. 스켈레탈메시 데이터를 불러오고 싶다
	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/AnimStarterPack/UE4_Mannequin/Mesh/SK_Mannequin.SK_Mannequin'"));
	if (TempMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(TempMesh.Object);
		// 2. Mesh 컴포넌트의 위치와 회전값을 설정하고 싶다. 
		GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -90), FRotator(0, -90, 0));
	}

	// 3. TPS 카메라를 붙이고 싶다.
	// 3-1. SpringArm 컴포넌트 붙이기
	springArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	springArmComp->SetupAttachment(RootComponent);
	springArmComp->SetRelativeLocation(FVector(0, 70, 90));
	springArmComp->TargetArmLength = 400;
	// 폰에 회전 입력이 들어오면 스프링암 컴포넌트를 회전시킬지의 여부
	springArmComp->bUsePawnControlRotation = true;

	// 3-2. Camera 컴포넌트 붙이기
	tpsCamComp = CreateDefaultSubobject<UCameraComponent>(TEXT("TpsCamComp"));
	tpsCamComp->SetupAttachment(springArmComp);

	tpsCamComp->bUsePawnControlRotation = false;

	bUseControllerRotationYaw = true;

	// 2단 점프
	JumpMaxCount = 2;

	// 4. 총 스켈레탈메시 컴포넌트 등록
	gunMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("GunMeshComp"));
	// 4-1. 부모 컴포넌트를 Mesh 컴포넌트로 설정
	gunMeshComp->SetupAttachment(GetMesh(), TEXT("hand_rSocket"));
	// 4-2. 스켈레탈메시 데이터 로드
	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempGunMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/FPWeapon/Mesh/SK_FPGun.SK_FPGun'"));
	// 4-3. 데이터로드가 성공했다면
	if (TempGunMesh.Succeeded())
	{
		// 4-4. 스켈레탈메시 데이터 할당
		gunMeshComp->SetSkeletalMesh(TempGunMesh.Object);
		// 4-5. 위치 조정하기
		gunMeshComp->SetRelativeLocation(FVector(-17, 10, -3));
		gunMeshComp->SetRelativeRotation(FRotator(0, 90, 0));
	}

	// 5. 스나이퍼건 컴포넌트 등록
	sniperGunComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SniperGunComp"));
	// 5-1. 부모 컴포넌트를 Mesh 컴포넌트로 설정
	sniperGunComp->SetupAttachment(GetMesh(), TEXT("hand_rSocket"));
	// 5-2. 스태틱메시 데이터 로드
	ConstructorHelpers::FObjectFinder<UStaticMesh> TempSniperMesh(TEXT("/Script/Engine.StaticMesh'/Game/SniperGun/SniperGun/sniper1.sniper1'"));
	// 5-3. 데이터로드가 성공했다면
	if (TempSniperMesh.Succeeded())
	{
		// 5-4. 스태틱메시 데이터 할당
		sniperGunComp->SetStaticMesh(TempSniperMesh.Object);
		// 5-5. 위치 조정하기
		sniperGunComp->SetRelativeLocation(FVector(-42, 7, 1));
		sniperGunComp->SetRelativeRotation(FRotator(0, 90, 0));
		// 5-6. 크기 조정하기
		sniperGunComp->SetRelativeScale3D(FVector(0.15f));
	}

	// 총알 사운드 가져오기
	ConstructorHelpers::FObjectFinder<USoundBase> tempSound(TEXT("/Script/Engine.SoundWave'/Game/SniperGun/SniperGun/Rifle.Rifle'"));
	if (tempSound.Succeeded())
	{
		bulletSound = tempSound.Object;
	}

}

// Called when the game starts or when spawned
void ATPSPlayer::BeginPlay()
{
	Super::BeginPlay();

	// 기본으로 스나이퍼건을 사용하도록 설정
	ChangeToSniperGun();

	// 1. 스나이퍼 UI 위젯 인스턴스 생성
	_sniperUI = CreateWidget(GetWorld(), sniperUIFactory);

	// 2. 일반 조준 UI 크로스헤어 인스턴스 생성
	_crosshairUI = CreateWidget(GetWorld(), crosshairUIFactory);

	// 3. 일반 조준 UI 등록
	_crosshairUI->AddToViewport();

	// 초기 속도를 걷기로 설정
	GetCharacterMovement()->MaxWalkSpeed = walkSpeed;
	
}

// Called every frame
void ATPSPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);	

	Move();
}

// Called to bind functionality to input
void ATPSPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &ATPSPlayer::Turn);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &ATPSPlayer::LookUp);
	PlayerInputComponent->BindAxis(TEXT("Horizontal"), this, &ATPSPlayer::InputHorizontal);
	PlayerInputComponent->BindAxis(TEXT("Vertical"), this, &ATPSPlayer::InputVertical);
	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &ATPSPlayer::InputJump);

	PlayerInputComponent->BindAction(TEXT("Fire"), IE_Pressed, this, &ATPSPlayer::InputFire);

	// 총 교체 이벤트 처리 함수 바인딩
	PlayerInputComponent->BindAction(TEXT("GrenadeGun"), IE_Pressed, this, &ATPSPlayer::ChangeToGrenadeGun);
	PlayerInputComponent->BindAction(TEXT("SniperGun"), IE_Pressed, this, &ATPSPlayer::ChangeToSniperGun);

	// 스나이퍼 조준 모드 이벤트 처리 함수 바인딩
	PlayerInputComponent->BindAction(TEXT("Sniper"), IE_Pressed, this, &ATPSPlayer::SniperAim);
	PlayerInputComponent->BindAction(TEXT("Sniper"), IE_Released, this, &ATPSPlayer::SniperAim);

	// 달리기 입력 이벤트 처리 함수 바인딩
	PlayerInputComponent->BindAction(TEXT("Run"), IE_Pressed, this, &ATPSPlayer::InputRun);
	PlayerInputComponent->BindAction(TEXT("Run"), IE_Released, this, &ATPSPlayer::InputRun);
}

void ATPSPlayer::Turn(float value)
{
	AddControllerYawInput(value);
}


void ATPSPlayer::LookUp(float value)
{
	AddControllerYawInput(value);
}



void ATPSPlayer::InputHorizontal(float value)
{
	direction.Y = value;
}


void ATPSPlayer::InputVertical(float value)
{
	direction.X = value;
}

void ATPSPlayer::InputJump()
{
	Jump();
}

void ATPSPlayer::Move()
{
	/*
	플레이어 이동 처리
	등속 운동 : P(결과 위치) = P0(현재 위치) + V(속도) * t(시간)
	*/
	direction = FTransform(GetControlRotation()).TransformVector(direction);
	/*FVector P0 = GetActorLocation();
	FVector vt = direction * walkSpeed * DeltaTime;
	FVector P = P0 + vt;
	SetActorLocation(P);*/
	AddMovementInput(direction);
	direction = FVector::ZeroVector;
}

void ATPSPlayer::InputFire()
{
	UGameplayStatics::PlaySound2D(GetWorld(), bulletSound);

	// 유탄총 사용시
	if (bUsingGrenadeGun)
	{
		// 총알 발사 처리
		FTransform firePosition = gunMeshComp->GetSocketTransform(TEXT("FirePosition"));
		GetWorld()->SpawnActor<ABullet>(bulletFactory, firePosition);
	}
	// 스나이퍼 건 사용시
	else
	{
		// LineTrace의 시작 위치
		FVector startPos = tpsCamComp->GetComponentLocation(); //GetComponentLocation()로 카메라 컴포넌트의 위치를 가져온다. 이는 부모의 상대좌표가 아닌 월드좌표값이다.
		// LineTrace의 종료 위치
		FVector endPos = tpsCamComp->GetComponentLocation() + tpsCamComp->GetForwardVector() * 5000; // 카메라가 바라보는 방향으로부터 5,000cm 떨어진 지점을 종료 위치로 설정
		// LineTrace의 충돌 정보를 담을 변수
		FHitResult hitInfo;
		// 충돌 옵션 설정 변수
		FCollisionQueryParams params;
		// 자기 자신(플레이어)는 충돌에서 제외
		params.AddIgnoredActor(this);
		// Channel 필터를 이용한 LineTrace 충돌 검출(충돌 정보, 시작 위치, 종료 위치, 검출 채널, 충돌 옵션)
		bool bHit = GetWorld()->LineTraceSingleByChannel(hitInfo, startPos, endPos, ECC_Visibility, params);
		// LineTrace가 부딪혔을 때
		if (bHit)
		{
			// 충돌 처리 -> 총알 파편 효과 재생
			// 총알 파편 효과 트랜스폼
			FTransform bulletTrans;
			// 부딪힌 위치 할당
			bulletTrans.SetLocation(hitInfo.ImpactPoint);
			// 총알 파편 효과 인스턴스 생성
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), bulletEffectFactory, bulletTrans);

			auto hitComp = hitInfo.GetComponent();
			// 1. 만약 컴포넌트에 물리가 적용되어 있다면
			// hitComp가 nullptr이 아닌지 검사하여 변수에 정상적인 값이 들어 있다면 컴포넌트의 물리작용 여부는 IsSimulatingPhysics( )를 이용하여 얻어올 수 있다.
			if (hitComp && hitComp->IsSimulatingPhysics())
			{
				// 2. 날려버릴 힘과 방향이 필요
				FVector force = -hitInfo.ImpactNormal * hitComp->GetMass();

				// 3. 그 방향으로 날려버리고 싶다.
				hitComp->AddForce(force);
			}

			// 부딪힌 대상이 적인지 판단하기
			auto enemy = hitInfo.GetActor()->GetDefaultSubobjectByName(TEXT("FSM"));
			if (enemy)
			{
				auto enemyFSM = Cast<UEnemyFSM>(enemy);
				enemyFSM->OnDamageProcess();
			}
		}
	}
	// 카메라 셰이크 재생
	auto controller = GetWorld()->GetFirstPlayerController();
	controller->PlayerCameraManager->StartCameraShake(cameraShake);

	// 공격 애니메이션 재생
	auto anim = Cast<URE_PlayerAnim>(GetMesh()->GetAnimInstance());
	if (anim)
	{
		anim->PlayAttackAnim();
	}

}

void ATPSPlayer::ChangeToGrenadeGun()
{
	bUsingGrenadeGun = true;
	gunMeshComp->SetVisibility(bUsingGrenadeGun);
	sniperGunComp->SetVisibility(!bUsingGrenadeGun);
}

void ATPSPlayer::ChangeToSniperGun()
{
	bUsingGrenadeGun = false;
	gunMeshComp->SetVisibility(bUsingGrenadeGun);
	sniperGunComp->SetVisibility(!bUsingGrenadeGun);
}

void ATPSPlayer::SniperAim()
{
	// 스나이퍼건 모드가 아니라면 처리하지 않는다.
	if (bUsingGrenadeGun)
	{
		return;
	}

	// Pressed 입력 처리
	if (bSniperAim == false)
	{
		// 1. 스나이퍼 조준 모드 활성화
		bSniperAim = true;
		// 2. 스나이퍼조준 UI 등록
		_sniperUI->AddToViewport();
		// 3. 카메라의 시야각 Field Of View 설정
		tpsCamComp->SetFieldOfView(45.0f);
		// 4. 일반 조준 UI 제거
		_crosshairUI->RemoveFromParent();
	}
	// Released 입력 처리
	else
	{
		// 1. 스나이퍼 조준 모드 비활성화
		bSniperAim = false;
		// 2. 스나이퍼 조준 UI 화면에서 제거
		_sniperUI->RemoveFromParent();
		// 3. 카메라 시야각 원래대로 복원
		tpsCamComp->SetFieldOfView(90.0f);
		// 4. 일반 조준 UI 등록
		_crosshairUI->AddToViewport();
	}
}

void ATPSPlayer::InputRun()
{
	auto movement = GetCharacterMovement();
	if (movement->MaxWalkSpeed > walkSpeed)
	{
		// 걷기 속도로 전환
		movement->MaxWalkSpeed = walkSpeed;
	}
	else
	{
		movement->MaxWalkSpeed = runSpeed;
	}
}