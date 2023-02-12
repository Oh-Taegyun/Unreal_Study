// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemyFSM.generated.h"

// ����� ���� ����
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

// BlueprintType ���ڸ� �Ҵ��Ͽ� �������Ʈ������ ����� �� �ֵ��� �մϴ�.
UCLASS(ClassGroup = (Custom), meta=(BlueprintSpawnableComponent))

class TPSPROJECT_API AEnemyFSM : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemyFSM();
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	// ���� ���¸� ����� �ɹ� ���� mstate�� �� ���¸� ó���� �Լ��� public �����ڷ� ������ �ְڽ��ϴ�. mState ������ �������Ʈ���� ������ �� �ֵ��� BlueprintReadOnly �Ӽ��� �߰��մϴ�
	UPROPERTY(visibleAnywhere, BlueprintReadOnly, Category = FSM)
		EEnemyState mState = EEnemyState::Idel;

};
