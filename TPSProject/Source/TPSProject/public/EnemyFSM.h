// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemyFSM.generated.h"

// 사용할 상태 정의
UENUM(BlueprintType)


// UENUM 매크로를 이용하여 언리얼에서 'EEnemyState'라는 새로운 열거형 타입이 만들어진 것을 알려주고 상태는 5가지이기 때문에 enum class가 사용할 크기는 uint8로 할당해 줍니다.
// 참고로 enum 타입은 이름 앞에 접두어 'E'를 붙여주어야 합니다. 그래서 'EEnemyState'라는 이름을 사용합니다. 
enum class EEnemyState : uint8
{
	Idel,
	Move,
	Attack,
	Damage,
	Die,
};

// BlueprintType 인자를 할당하여 블루프린트에서도 사용할 수 있도록 합니다.
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
	// 현재 상태를 기억할 맴버 변수 mstate와 각 상태를 처리할 함수를 public 접근자로 선언해 주겠습니다. mState 변수는 블루프린트에서 접근할 수 있도록 BlueprintReadOnly 속성을 추가합니다
	UPROPERTY(visibleAnywhere, BlueprintReadOnly, Category = FSM)
		EEnemyState mState = EEnemyState::Idel;

};
