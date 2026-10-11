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
	void HandleChatInput(ASLPlayerController* Sender, const FString& Msg);

};
