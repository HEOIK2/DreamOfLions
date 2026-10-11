#include "Player/SLPlayerState.h"
#include "Net/UnrealNetwork.h"

ASLPlayerState::ASLPlayerState()
	: PlayerNameString(TEXT("None")), CurrentGuessCount(0), MaxGuessCount(3)
{
	bReplicates = true;
}

void ASLPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ThisClass, PlayerNameString);
	DOREPLIFETIME(ThisClass, CurrentGuessCount)
}

FString ASLPlayerState::GetGuessCountString() const
{
	return FString::Printf(TEXT("[%d/%d]"), CurrentGuessCount, MaxGuessCount);
}
