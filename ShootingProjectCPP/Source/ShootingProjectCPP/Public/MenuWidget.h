// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MenuWidget.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTINGPROJECTCPP_API UMenuWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UButton* button_Restart;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UButton* button_Quit;

	UFUNCTION()
		void Restart();
	
	UFUNCTION()
		void Quit();

protected:
	// 델리게이트에 함수를 바인딩하기 위해서 사용하는 함수
	virtual void NativeConstruct() override;
};
