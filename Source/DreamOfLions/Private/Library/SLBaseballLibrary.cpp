#include "Library/SLBaseballLibrary.h"

FString USLBaseballLibrary::GenerateSecretNumber()
{
	FMath::RandInit(static_cast<int32>(FDateTime::Now().GetTicks())); // 매번 다른 난수

	TArray<int32> Numbers; // 숫자 바구니 1-9
	for (int32 i = 1; i <= 9; ++i)
	{
		Numbers.Add(i);
	}

	FString Result;
	for (int32 i = 0; i < 3; ++i)
	{
		const int32 Index = FMath::RandRange(0, Numbers.Num() - 1); // 랜덤 위치
		Result.Append(FString::FromInt(Numbers[Index])); // 결과에 붙이기
		Numbers.RemoveAt(Index); // 뽑은 건 빼서 중복 방지
	}
	return Result;
}

// 3글자, 1~9만, 중복 없음
bool USLBaseballLibrary::IsValidGuess(const FString& Input)
{
	return GetGuessError(Input).IsEmpty();
}

// 자리와 숫자가 같으면 S, 숫자만 있으면 B, 둘 다 0이면 OUT
FString USLBaseballLibrary::Judge(const FString& Secret, const FString& Guess)
{
	int32 Strike = 0;
	int32 Ball = 0;

	for (int32 i = 0; i < 3; ++i)
	{
		if (Guess[i] == Secret[i])
		{
			++Strike;
		}
		else if (Secret.Contains(FString::Chr(Guess[i])))
		{
			++Ball;
		}
	}

	if (Strike == 0 && Ball == 0)
	{
		return TEXT("OUT");
	}
	return FString::Printf(TEXT("%dS%dB"), Strike, Ball);
}

FString USLBaseballLibrary::GetGuessError(const FString& Input)
{
	if (Input.Len() != 3)
	{
		return TEXT("Please enter a 3-digit number.");
	}

	TSet<TCHAR> Digits;
	for (const TCHAR C : Input)
	{
		if (C < TEXT('1') || C > TEXT('9'))
		{
			return TEXT("Use digits 1-9 only. (no letters, no 0)");
		}
		Digits.Add(C);
	}

	if (Digits.Num() != 3)
	{
		return TEXT("No duplicate digits allowed.");
	}
	return FString();
}
