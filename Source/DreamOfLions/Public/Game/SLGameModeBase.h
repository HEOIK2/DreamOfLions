#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SLGameModeBase.generated.h"

class ASLPlayerController;

UCLASS()
class DREAMOFLIONS_API ASLGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public: 
	virtual void BeginPlay() override;

	void HandleChatInput(ASLPlayerController* Sender, const FString& Msg);

	virtual void PostLogin(APlayerController* NewPlayer) override;

protected:
	int32 JoinedPlayerCount = 0;

	FString SecretNumber;

	void BroadcastLine(const FString& Line);
	void ResetGame();
	void BroadcastNotice(const FString& Notice);
	void JudgeGame(ASLPlayerController* Sender, const FString& Result);
};
