// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EnemyFSM.generated.h"

// ����� ���� ���� 
// UENUM ��ũ�δ� �𸮾󿡼� 'EEnemyState' ��� ���ο� ������ Ÿ���� ������� ���� �˷��ݴϴ�. BlueprintType ���ڸ� �Ҵ��Ͽ� �������Ʈ������ ����� �� �ֵ��� �մϴ�.
UENUM(BlueprintType)
// UENUM ��ũ�θ� �̿��Ͽ� �𸮾󿡼� 'EEnemyState'��� ���ο� ������ Ÿ���� ������� ���� �˷��ְ� ���´� 5�����̱� ������ enum class�� ����� ũ��� uint8�� �Ҵ��� �ݴϴ�.
// ����� enum Ÿ���� �̸� �տ� ���ξ� 'E'�� �ٿ��־�� �մϴ�. �׷��� 'EEnemyState'��� �̸��� ����մϴ�. 
enum class EEnemyState : uint8
{
	Idel,
	Move,
	Attack,
	Damage,
	Die,
};



UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TPSPROJECT_API UEnemyFSM : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UEnemyFSM();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	// ���� ���¸� ����� �ɹ� ���� mstate�� �� ���¸� ó���� �Լ��� public �����ڷ� ������ �ְڽ��ϴ�. mState ������ �������Ʈ���� ������ �� �ֵ��� BlueprintReadOnly �Ӽ��� �߰��մϴ�
	UPROPERTY(visibleAnywhere, BlueprintReadOnly, Category = FSM)
		EEnemyState mState = EEnemyState::Idel;

	// ��� ����
	void IdleState();
	// �̵� ����
	void MoveState();
	// ���� ����
	void AttackState();
	// �ǰ� ����
	void DamageState();
	// ���� ����
	void DieState();

	// ��� �ð�
	UPROPERTY(EditDefaultsOnly, Category = FSM)
		float idleDelayTime = 2;
	// ��� �ð�
	float currentTime = 0;

	// Ÿ�� 
	UPROPERTY(VisibleAnywhere, Category = FSM)
		class ATPSPlayer* target;

	// ���� ����
	// Ÿ�� ������ �̵��Ϸ��ŵ� �������� ���� ������ �ʿ��ϴ� 
	// Ÿ�� ���� = ������ - ���� ��ġ
	UPROPERTY()
		class AEnemy* me;

	// ���� ����
	UPROPERTY(EditAnywhere, Category = FSM)
		float attackRange = 150.0f;

	UPROPERTY(EditAnywhere, Category = FSM)
		float attackDelayTime = 2.0f;

	// �ǰ� �˸� �̺�Ʈ �Լ�
	void OnDamageProcess();

	// ü��
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = FSM)
		int32 hp = 3;

	// �ǰ� ��� �ð�
	UPROPERTY(EditAnywhere, Category = FSM)
		float damageDelayTime = 2.0f;

	// �Ʒ��� ������� �ӵ�
	UPROPERTY(EditAnywhere, Category = FSM)
		float dieSpeed = 50.0f;
};

 