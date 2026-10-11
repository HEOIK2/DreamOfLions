#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SLPlayerController.generated.h"

class USLChatWidget;

UCLASS()
class DREAMOFLIONS_API ASLPlayerController : public APlayerController
{
	GENERATED_BODY()
	

public:
	virtual void BeginPlay() override;

	void SendChat(const FString& Msg);

	UFUNCTION(Client, Reliable)
	void ClientReceiveChat(const FString& Msg); // GameMode가 부르니까 public.

protected:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<USLChatWidget> ChatWidgetClass; // 에디터 상에 BP에서 WBP_Chat을 지정할 칸

	UPROPERTY()
	TObjectPtr<USLChatWidget> ChatWidgetInstance; // 실제로 만들어진 위젯

	UFUNCTION(Server, Reliable)
	void ServerSendInput(const FString& Msg);
};
