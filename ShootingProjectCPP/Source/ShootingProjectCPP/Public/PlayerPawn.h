// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "PlayerPawn.generated.h"

UCLASS()
class SHOOTINGPROJECTCPP_API APlayerPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	// 이 폰의 속성에 대한 기본 값을 설정
	APlayerPawn();

protected:
	// Called when the game starts or when spawned
	// 게임이 시작될 때 또는 스폰될 때 호출됨
	virtual void BeginPlay() override;

public:	
	// Called every frame
	// 매 프레임마다 호출됨
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	// 기능을 입력에 연결하기 위해 호출됨
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere)
		class UBoxComponent* boxComp; // 클래스 전방 선언 기법, 박스 콜라이더 컴포넌트 클래스 호출

	UPROPERTY(EditAnywhere)
		class UStaticMeshComponent* meshComp; // 스태틱 메시 컴포넌트 변수 선언

	UPROPERTY(EditAnywhere)
		float movespeed = 500;

	// 총구 위치
	UPROPERTY(EditAnywhere)
		class UArrowComponent* firePosition;

	// 총알 블루프린트
	UPROPERTY(EditAnywhere)
		TSubclassOf<class ABullet> bulletFactory;

	UPROPERTY(EditAnywhere)
		class USoundBase* fireSound;
	

private:
	// 사용자의 키 입력을 받을 변수
	float h;
	float v;

	// 사용자 입력 처리 함수
	void MoveHorizontal(float value);
	void MoveVertical(float value);

	// 총알 발사 입력 처리 함수
	void Fire();
};
