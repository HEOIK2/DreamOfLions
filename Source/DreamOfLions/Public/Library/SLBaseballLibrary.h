#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "SLBaseballLibrary.generated.h"


UCLASS()
class DREAMOFLIONS_API USLBaseballLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public: 
	static FString GenerateSecretNumber();
	static bool IsValidGuess(const FString& Input);
	static FString Judge(const FString& Secret, const FString& Guess);
	static FString GetGuessError(const FString& Input);
};
