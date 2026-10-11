#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Types/SlateEnums.h"
#include "SLChatWidget.generated.h"


class UEditableTextBox;
class UScrollBox;

UCLASS()
class DREAMOFLIONS_API USLChatWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta = (BindWidget)) 
	TObjectPtr<UEditableTextBox> EditableTextBox_ChatInput;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UScrollBox> ScrollBox_ChatLog;

	void AddChatLine(const FString& Line);

protected:
	virtual void NativeConstruct() override; // 위젯이 생성될 때 자동 호출

	UFUNCTION()
	void OnChatInputCommitted(const FText& Text, ETextCommit::Type CommitMethod);

};