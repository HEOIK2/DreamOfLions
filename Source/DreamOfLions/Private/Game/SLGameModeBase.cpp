#include "Game/SLGameModeBase.h"
#include "Player/SLPlayerController.h"
#include "Player/SLPlayerState.h"
#include "Library/SLBaseballLibrary.h"

void ASLGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	SecretNumber = USLBaseballLibrary::GenerateSecretNumber();
	UE_LOG(LogTemp, Warning, TEXT("[Secret] %s"), *SecretNumber);   // 테스트용 정답 확인

}


	void ASLGameModeBase::HandleChatInput(ASLPlayerController* Sender, const FString& Msg)
{
	ASLPlayerState* PS = Sender->GetPlayerState<ASLPlayerState>();
	if (IsValid(PS) == false)
	{
		return;
	}
	const FString Name = PS->PlayerNameString;

	// 숫자가 하나도 없으면 일반 채팅
	bool bHasDigit = false;
	for (const TCHAR C : Msg)
	{
		if (FChar::IsDigit(C))
		{
			bHasDigit = true;
			break;
		}
	}
	if (bHasDigit == false)
	{
		BroadcastLine(Name + TEXT(": ") + Msg);
		return;
	}

	// 입력 검사 실패, 본인에게만 사유 안내, 기회 유지
	const FString Error = USLBaseballLibrary::GetGuessError(Msg);
	if (Error.IsEmpty() == false)
	{
		Sender->ClientReceiveChat(TEXT("[Notice] ") + Error + TEXT(" Try again."));
		return;
	}

	// 기회 소진
	if (PS->CurrentGuessCount >= PS->MaxGuessCount)
	{
		Sender->ClientReceiveChat(TEXT("[Notice] You have no chances left."));
		return;
	}

	// 시도 +1 → 판정 → 전원 방송
	PS->CurrentGuessCount++;
	const FString Result = USLBaseballLibrary::Judge(SecretNumber, Msg);
	BroadcastLine(FString::Printf(TEXT("%s %s: %s -> %s"),
		*Name, *PS->GetGuessCountString(), *Msg, *Result));

	// 승부 판단
	JudgeGame(Sender, Result);
}


void ASLGameModeBase::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	ASLPlayerState* PS = NewPlayer->GetPlayerState<ASLPlayerState>();
	if (IsValid(PS))
	{
		++JoinedPlayerCount;
		PS->PlayerNameString = FString::Printf(TEXT("Player%d"), JoinedPlayerCount);
	}
}

void ASLGameModeBase::BroadcastLine(const FString& Line)
{
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		ASLPlayerController* PC = Cast<ASLPlayerController>(It->Get());
		if (IsValid(PC))
		{
			PC->ClientReceiveChat(Line);
		}
	}
}

void ASLGameModeBase::ResetGame()
{
	SecretNumber = USLBaseballLibrary::GenerateSecretNumber();
	UE_LOG(LogTemp, Warning, TEXT("[Secret] %s"), *SecretNumber);

	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		ASLPlayerState* PS = It->Get()->GetPlayerState<ASLPlayerState>();
		if (IsValid(PS))
		{
			PS->CurrentGuessCount = 0;
		}
	}
}

void ASLGameModeBase::BroadcastNotice(const FString& Notice)
{
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		ASLPlayerController* PC = Cast<ASLPlayerController>(It->Get());
		if (IsValid(PC))
		{
			PC->ClientShowNotice(Notice);
		}
	}
}

void ASLGameModeBase::JudgeGame(ASLPlayerController* Sender, const FString& Result)
{
	ASLPlayerState* PS = Sender->GetPlayerState<ASLPlayerState>();

	// 승리
	if (Result == TEXT("3S0B"))
	{
		BroadcastNotice(PS->PlayerNameString + TEXT(" Wins!"));
		BroadcastLine(TEXT("[System] New game started."));
		ResetGame();
		return;
	}

	// 무승부: 기회 남은 사람이 한 명이라도 있으면 계속
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		ASLPlayerState* OtherPS = It->Get()->GetPlayerState<ASLPlayerState>();
		if (IsValid(OtherPS) && OtherPS->CurrentGuessCount < OtherPS->MaxGuessCount)
		{
			return;
		}
	}
	BroadcastNotice(TEXT("Draw!"));
	BroadcastLine(FString::Printf(TEXT("[System] The answer was %s. New game started."), *SecretNumber));
	ResetGame();
}
