#include "Player/SLPlayerController.h"
#include "UI/SLChatWidget.h"
#include "Game/SLGameModeBase.h"


void ASLPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (IsLocalController() == false) // 서버에 있는 "남의 PC"면 위젯 안 만듦
	{
		return;
	}

	SetInputMode(FInputModeUIOnly());
	bShowMouseCursor = true;

	if (IsValid(ChatWidgetClass))
	{
		ChatWidgetInstance = CreateWidget<USLChatWidget>(this, ChatWidgetClass);
		if (IsValid(ChatWidgetInstance))
		{
			ChatWidgetInstance->AddToViewport();
		}
	}

}

void ASLPlayerController::SendChat(const FString& Msg)
{
	ServerSendInput(Msg);
}

void ASLPlayerController::ServerSendInput_Implementation(const FString& Msg)
{
	ASLGameModeBase* GM = GetWorld()->GetAuthGameMode<ASLGameModeBase>();
	if (IsValid(GM))
	{
		GM->HandleChatInput(this, Msg);
	}
}

void ASLPlayerController::ClientReceiveChat_Implementation(const FString& Msg)
{
	if (IsValid(ChatWidgetInstance))
	{
		ChatWidgetInstance->AddChatLine(Msg);
	}
}
