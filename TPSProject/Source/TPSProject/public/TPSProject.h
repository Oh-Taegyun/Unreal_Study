// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

DECLARE_LOG_CATEGORY_EXTERN(TPS, Log, All);

// ����� �ϱ� ���� �α׸� ���� ��𿡼� �ش� ������ ��� �Ǵ��� Ȯ���ϴµ� ������ �ִ� ��ũ�� �Լ�
#define CALLINFO (FString(__FUNCTION__) + TEXT("(") + FString::FromInt(__LINE__) + TEXT(")"))
#define PRINT_CALLINFO() UE_LOG(TPS, Warning, TEXT("%s"), *CALLINFO)

// ���ϴ� �α� �����͸� ����� �� �ִ� ��ũ��
// ���� PRINT_LOG(TEXT("���ϴ� �ؽ�Ʈ 1"), TEXT("���ϴ� �ؽ�Ʈ 2"), ...)
#define PRINT_LOG(fmt, ...) UE_LOG(TPS, Warning, TEXT("%s"), *CALLINFO, *FString::Printf(fmt, ##__VA_ARGS__)
