// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Bullet.generated.h"

UCLASS()
class SHOOTINGPROJECTCPP_API ABullet : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABullet();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	UPROPERTY(EditAnywhere)
		class UBoxComponent* boxComp; // 클래스 전방 선언 기법, 박스 콜라이더 컴포넌트 클래스 호출

	UPROPERTY(EditAnywhere)
		class UStaticMeshComponent* meshComp; // 스태틱 메시 컴포넌트 변수 선언

	UPROPERTY(EditAnywhere)
		float movespeed = 800.0f;

	UPROPERTY(EditAnywhere)
		class UParticleSystem* explosionFX;

	UFUNCTION()
		void OnBulletOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);



};
