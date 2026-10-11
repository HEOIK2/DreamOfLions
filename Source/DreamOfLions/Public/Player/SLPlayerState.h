#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "SLPlayerState.generated.h"


UCLASS()
class DREAMOFLIONS_API ASLPlayerState : public APlayerState
{
	GENERATED_BODY()
	

public:
	ASLPlayerState();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	FString GetGuessCountString() const;

	UPROPERTY(Replicated)
	FString PlayerNameString;

	UPROPERTY(Replicated)
	int32 CurrentGuessCount;

	int32 MaxGuessCount;

};
