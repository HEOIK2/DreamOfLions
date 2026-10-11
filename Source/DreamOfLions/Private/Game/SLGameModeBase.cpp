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

	//  숫자 3글자가 아니면 그냥 채팅
	if (Msg.Len() != 3 || Msg.IsNumeric() == false)
	{
		BroadcastLine(Name + TEXT(": ") + Msg);
		return;
	}

	//  규칙 위반(0, 중복) → 본인에게만 안내, 기회 유지
	if (USLBaseballLibrary::IsValidGuess(Msg) == false)
	{
		Sender->ClientReceiveChat(TEXT("[Notice] Use 3 different digits from 1-9. (No chance used)"));
		return;
	}

	//  기회 소진 → 본인에게만 안내
	if (PS->CurrentGuessCount >= PS->MaxGuessCount)
	{
		Sender->ClientReceiveChat(TEXT("[Notice] You have no chances left."));
		return;
	}

	//  정상 추측: 시도 +1 → 판정 → 전원 방송
	PS->CurrentGuessCount++;
	const FString Result = USLBaseballLibrary::Judge(SecretNumber, Msg);
	BroadcastLine(FString::Printf(TEXT("%s(%d/%d): %s -> %s"),
		*Name, PS->CurrentGuessCount, PS->MaxGuessCount, *Msg, *Result));

	//  승리
	if (Result == TEXT("3S0B"))
	{
		BroadcastLine(Name + TEXT(" wins! New game started."));
		ResetGame();
		return;
	}

	//  무승부: 전원 기회 소진
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		ASLPlayerState* OtherPS = It->Get()->GetPlayerState<ASLPlayerState>();
		if (IsValid(OtherPS) && OtherPS->CurrentGuessCount < OtherPS->MaxGuessCount)
		{
			return;   // 아직 기회 남은 사람이 있음
		}
	}
	BroadcastLine(FString::Printf(TEXT("Draw! The answer was %s. New game started."), *SecretNumber));
	ResetGame();
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
