// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerAnim.h"
#include "TPSPlayer.h"
#include <GameFramework/CharacterMovementComponent.h>

void UPlayerAnim::NativeUpdateAnimation(float DeltaSeconds)
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
	}

	// �÷��̾ ���� ���߿� �ִ��� ���θ� ����ϰ� �ʹ�.
	auto movement = player->GetCharacterMovement();
	isInAir = movement->IsFalling();
}

void UPlayerAnim::PlayAttackAnim()
{
	Montage_Play(attackAnimMontage);
}