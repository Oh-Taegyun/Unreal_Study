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
	// �� ���� �Ӽ��� ���� �⺻ ���� ����
	APlayerPawn();

protected:
	// Called when the game starts or when spawned
	// ������ ���۵� �� �Ǵ� ������ �� ȣ���
	virtual void BeginPlay() override;

public:	
	// Called every frame
	// �� �����Ӹ��� ȣ���
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	// ����� �Է¿� �����ϱ� ���� ȣ���
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere)
		class UBoxComponent* boxComp; // Ŭ���� ���� ���� ���, �ڽ� �ݶ��̴� ������Ʈ Ŭ���� ȣ��

	UPROPERTY(EditAnywhere)
		class UStaticMeshComponent* meshComp; // ����ƽ �޽� ������Ʈ ���� ����

	UPROPERTY(EditAnywhere)
		float movespeed = 500;

	// �ѱ� ��ġ
	UPROPERTY(EditAnywhere)
		class UArrowComponent* firePosition;

	// �Ѿ� �������Ʈ
	UPROPERTY(EditAnywhere)
		TSubclassOf<class ABullet> bulletFactory;

	UPROPERTY(EditAnywhere)
		class USoundBase* fireSound;
	

private:
	// ������� Ű �Է��� ���� ����
	float h;
	float v;

	// ����� �Է� ó�� �Լ�
	void MoveHorizontal(float value);
	void MoveVertical(float value);

	// �Ѿ� �߻� �Է� ó�� �Լ�
	void Fire();
};
