// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerPawn.h"
#include "Components/BoxComponent.h" // �ҽ� �ڵ忡���� ���� ������ ���� �����ϰ� �����ϰڴٰ� ����ߴ�.
#include "Components/StaticMeshComponent.h"
#include "Components/ArrowComponent.h"
#include "Bullet.h"
#include "Kismet/GameplayStatics.h"

// Sets default values 
// �⺻ �� ����
APlayerPawn::APlayerPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	// ��� �����ӿ��� Tick()�� ȣ���ϵ��� �� ���� �����մϴ�. �ʿ� ���� ��� ���� ����� ���� �� ����� ������ �� �ֽ��ϴ�.
	PrimaryActorTick.bCanEverTick = true;

	// �ڽ� �ݶ��̴� ������Ʈ ����
	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("My Box Component"));

	// ������ �ڽ� �ݶ��̴� ������Ʈ�� �ֻ�� ������Ʈ�� �����Ѵ�.
	SetRootComponent(boxComp);

	// ����ƽ �޽� ������Ʈ ����
	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("My Static Mesh"));

	meshComp->SetupAttachment(boxComp); // ��Ʈ ������Ʈ�� �ڽ� �ݶ��̴� ������Ʈ�� �ڽ����� �����ϱ� 

	// �ڽ� �ݶ��̴��� ũ�⸦ 50 x 50 x 50 ���� �����Ѵ�.
	FVector boxsize = FVector(50.0f, 50.0f, 50.0f);
	boxComp->SetBoxExtent(boxsize);

	// �ѱ� ǥ�� ������Ʈ�� �����ϰ� �ڽ� ������Ʈ�� �ڽ� ������Ʈ�� �����Ѵ�. 
	firePosition = CreateDefaultSubobject<UArrowComponent>(TEXT("Fire Position"));
	firePosition->SetupAttachment(boxComp);


	// ������ �̺�Ʈ�� �Ҵ�
	boxComp->SetGenerateOverlapEvents(true);

	// �浹 ������ Qurey And Physics�� �����Ѵ�.
	boxComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	
	// Object Type�� 1�� ä��(Player)�� �����Ѵ�.
	boxComp->SetCollisionObjectType(ECC_GameTraceChannel1);

	// ��� ä���� �浹 ���� �������� �����Ѵ�.
	boxComp->SetCollisionResponseToAllChannels(ECR_Ignore);

	//���ʹ̿ʹ� �浹 �̺�Ʈ�� üũ(Quary) �Ѵ�.
	boxComp->SetCollisionResponseToChannel(ECC_GameTraceChannel2, ECR_Overlap);
	boxComp->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);


}

// Called when the game starts or when spawned
// ������ ���۵ǰų� ������ �� ȣ���
void APlayerPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
// ��� ������ ȣ��
void APlayerPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//����� �Է� Ű�� �̿��ؼ�
	// 1. ���� �Է� ���� �¿� �Է� ���� �̿��ؼ� ���� ���͸� �����
	FVector dir = FVector(0, h, v);

	// 2. ���� ������ ���̰� 1�� �ǵ��� ���͸� ����ȭ�Ѵ�.
	dir.Normalize();

	// 3. �̵��� ��ġ ��ǥ�� ���Ѵ�
	FVector newLocation = GetActorLocation() + dir * movespeed * DeltaTime;

	// 4. ���� ������ ��ġ ��ǥ�� �տ��� ���� �� ��ǥ�� �����Ѵ�.
	SetActorLocation(newLocation, true);

}

// Called to bind functionality to input
// ����� �Է¿� ���ε��ϱ� ���� ȣ��˴ϴ�
void APlayerPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Axis ���ε� �� ���� �Լ��� �����Ѵ�
	PlayerInputComponent->BindAxis("Horizontal", this, &APlayerPawn::MoveHorizontal);
	PlayerInputComponent->BindAxis("Vertical", this, &APlayerPawn::MoveVertical);

	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &APlayerPawn::Fire);

}


void APlayerPawn::MoveHorizontal(float value)
{
	// ������� �Է� ��(Axis)�� h ������ �ִ´�
	h = value;
}

void APlayerPawn::MoveVertical(float value)
{
	// ������� �Է� ��(Axis)�� v ������ �ִ´�
	v = value;
}

void APlayerPawn::Fire()
{
	ABullet* bullet = GetWorld()->SpawnActor<ABullet>(bulletFactory, firePosition->GetComponentLocation(), firePosition->GetComponentRotation());

	//fireSound ������ �Ҵ�� ���� ���� ����
	UGameplayStatics::PlaySound2D(GetWorld(), fireSound);

}
