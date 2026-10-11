#include "Game/SLGameModeBase.h"
#include "Player/SLPlayerController.h"

void ASLGameModeBase::HandleChatInput(ASLPlayerController* Sender, const FString& Msg)
{
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		ASLPlayerController* PC = Cast<ASLPlayerController>(It->Get());
		if (IsValid(PC))
		{
			PC->ClientReceiveChat(Msg);
		}
	}
}
