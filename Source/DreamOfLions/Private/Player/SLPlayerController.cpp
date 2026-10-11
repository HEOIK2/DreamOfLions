#include "Player/SLPlayerController.h"
#include "UI/SLChatWidget.h"
#include "Game/SLGameModeBase.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"

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

	if (IsValid(NoticeWidgetClass))
	{
		NoticeWidgetInstance = CreateWidget<UUserWidget>(this, NoticeWidgetClass);
		if (IsValid(NoticeWidgetInstance))
		{
			NoticeWidgetInstance->AddToViewport(1);   // 채팅창보다 위에
		}
	}
}

void ASLPlayerController::SendChat(const FString& Msg)
{
	ServerSendInput(Msg);
}

void ASLPlayerController::ClientShowNotice_Implementation(const FString& Notice)
{
	if (IsValid(NoticeWidgetInstance) == false)
	{
		return;
	}
	UTextBlock* NoticeText = Cast<UTextBlock>(NoticeWidgetInstance->GetWidgetFromName(TEXT("TextBlock_Notice")));
	if (IsValid(NoticeText))
	{
		NoticeText->SetText(FText::FromString(Notice));
	}
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
