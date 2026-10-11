#include "UI/SLChatWidget.h"
#include "Player/SLPlayerController.h"
#include "Components/EditableTextBox.h"
#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"
#include "Blueprint/WidgetTree.h"


void USLChatWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	// 엔터 같은 확정 입력이 오면 내 ㅎ마수를 불러줘를 등록한다.
	EditableTextBox_ChatInput->OnTextCommitted.AddUniqueDynamic(this, &USLChatWidget::OnChatInputCommitted);

}

void USLChatWidget::OnChatInputCommitted(const FText& Text, ETextCommit::Type CommitMethod)
{
	if (CommitMethod != ETextCommit::OnEnter || Text.IsEmpty()) // 엔터가 아니거나 빈칸이면 무시
	{
		return;
	}

	ASLPlayerController* PC = Cast<ASLPlayerController>(GetOwningPlayer()); // 이 위젯의 주인 PC
	if (IsValid(PC))
	{
		PC->SendChat(Text.ToString());
	}

	EditableTextBox_ChatInput->SetText(FText()); // 입력창 비우기

}

void USLChatWidget::AddChatLine(const FString& Line)
{
	UTextBlock* NewLine = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass());
	NewLine->SetText(FText::FromString(Line));
	ScrollBox_ChatLog->AddChild(NewLine);
	ScrollBox_ChatLog->ScrollToEnd();
}