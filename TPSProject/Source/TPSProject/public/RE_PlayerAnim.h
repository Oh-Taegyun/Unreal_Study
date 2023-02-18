// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "RE_PlayerAnim.generated.h"

/**
 * 
 */
UCLASS()
class TPSPROJECT_API URE_PlayerAnim : public UAnimInstance
{
	GENERATED_BODY()

public:
	// �÷��̾� �̵� �ӵ�
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = RE_PlayerAnim)
		float speed = 0;

	// �÷��̾� �� �� �̵� �ӵ�
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = RE_PlayerAnim)
		float direction = 0;

	// �� ������ ���ŵǴ� �Լ�
	virtual void NativeUpdateAnimation(float DeltSeconds) override;

	// �÷��̾ ���߿� �ִ��� ����
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = RE_PlayerAnim)
		bool isInAir = false;

	// ����� ���� �ִϸ��̼� ��Ÿ��
	UPROPERTY(EditDefaultsOnly, Category = PlayerAnim)
		class UAnimMontage* attackAnimMontage;

	// ���� �ִϸ��̼� ��� �Լ�
	void PlayAttackAnim();
};
