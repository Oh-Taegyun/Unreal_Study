// Fill out your copyright notice in the Description page of Project Settings.


#include "RE_PlayerAnim.h"
#include "TPSPlayer.h"
#include <GameFramework/CharacterMovementComponent.h>

void URE_PlayerAnim::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	// �÷��̾��� �̵� �ӵ��� ������ speed�� �Ҵ��ϰ� �ʹ�.
	// 1. ���� �� ��� ����
	auto ownerPawn = TryGetPawnOwner();
	// 2. �÷��̾�� ĳ����
	auto player = Cast<ATPSPlayer>(ownerPawn);
	// ĳ���� �����ߴٸ�
	if (player)
	{
		// 3. �̵� �ӵ��� �ʿ�
		FVector velocity = player->GetVelocity();
		// 4. �÷��̾��� ���� ���Ͱ� �ʿ�
		FVector forwardVector = player->GetActorForwardVector();
		// 5. speed�� ��(����) �Ҵ��ϱ�
		speed = FVector::DotProduct(forwardVector, velocity);

		// 6. �¿� �ӵ� �Ҵ��ϱ�
		FVector rightVector = player->GetActorRightVector();
		direction = FVector::DotProduct(rightVector, velocity);

		auto movement = player->GetCharacterMovement();
		isInAir = movement->IsFalling();
	}

	
}

void URE_PlayerAnim::PlayAttackAnim()
{
	Montage_Play(attackAnimMontage);
}

