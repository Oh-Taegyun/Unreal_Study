// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

DECLARE_LOG_CATEGORY_EXTERN(TPS, Log, All);

// 디버깅 하기 위해 로그를 찍어볼때 어디에서 해당 내용이 출력 되는지 확인하는데 도움을 주는 메크로 함수
#define CALLINFO (FString(__FUNCTION__) + TEXT("(") + FString::FromInt(__LINE__) + TEXT(")"))
#define PRINT_CALLINFO() UE_LOG(TPS, Warning, TEXT("%s"), *CALLINFO)

// 원하는 로그 데이터를 출력할 수 있는 매크로
// 사용법 PRINT_LOG(TEXT("원하는 텍스트 1"), TEXT("원하는 텍스트 2"), ...)
#define PRINT_LOG(fmt, ...) UE_LOG(TPS, Warning, TEXT("%s"), *CALLINFO, *FString::Printf(fmt, ##__VA_ARGS__)
