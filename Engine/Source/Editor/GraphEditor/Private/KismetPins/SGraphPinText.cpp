// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.


#include "KismetPins/SGraphPinText.h"
#include "ScopedTransaction.h"
#include "STextPropertyEditableTextBox.h"

namespace
{
	/** Allows STextPropertyEditableTextBox to edit a graph pin */
	class FEditableTextGraphPin : public IEditableTextProperty
	{
	public:
		FEditableTextGraphPin(UEdGraphPin* InGraphPinObj)
			: GraphPinObj(InGraphPinObj)
		{
		}

		virtual bool IsMultiLineText() const override
		{
			return true;
		}

		virtual bool IsPassword() const override
		{
			return false;
		}

		virtual bool IsReadOnly() const override
		{
			return GraphPinObj->bDefaultValueIsReadOnly;
		}

		virtual bool IsDefaultValue() const override
		{
			FString TextAsString;
			if (FTextStringHelper::WriteToString(TextAsString, GraphPinObj->DefaultTextValue))
			{
				return TextAsString.Equals(GraphPinObj->AutogeneratedDefaultValue, ESearchCase::CaseSensitive);
			}
			return false;
		}

		virtual FText GetToolTipText() const override
		{
			return FText::GetEmpty();
		}

		virtual int32 GetNumTexts() const override
		{
			return 1;
		}

		virtual FText GetText(const int32 InIndex) const override
		{
			check(InIndex == 0);
			return GraphPinObj->DefaultTextValue;
		}

		virtual void SetText(const int32 InIndex, const FText& InText) override
		{
			check(InIndex == 0);
			const FScopedTransaction Transaction(NSLOCTEXT("GraphEditor", "ChangeTxtPinValue", "Change Text Pin Value"));
			GraphPinObj->Modify();
			GraphPinObj->GetSchema()->TrySetDefaultText(*GraphPinObj, InText);
		}

#if USE_STABLE_LOCALIZATION_KEYS
		virtual void GetStableTextId(const int32 InIndex, const ETextPropertyEditAction InEditAction, const FString& InTextSource, const FString& InProposedNamespace, const FString& InProposedKey, FString& OutStableNamespace, FString& OutStableKey) const override
		{
			check(InIndex == 0);
			return StaticStableTextId(GraphPinObj->GetOwningNodeUnchecked(), InEditAction, InTextSource, InProposedNamespace, InProposedKey, OutStableNamespace, OutStableKey);
		}
#endif // USE_STABLE_LOCALIZATION_KEYS

		virtual void RequestRefresh() override
		{
		}

	private:
		UEdGraphPin* GraphPinObj;
	};
}

void SGraphPinText::Construct(const FArguments& InArgs, UEdGraphPin* InGraphPinObj)
{
	SGraphPin::Construct(SGraphPin::FArguments(), InGraphPinObj);
}

TSharedRef<SWidget>	SGraphPinText::GetDefaultValueWidget()
{
	return SNew(STextPropertyEditableTextBox, MakeShareable(new FEditableTextGraphPin(GraphPinObj)))
		.Style(FEditorStyle::Get(), "Graph.EditableTextBox")
		.Visibility(this, &SGraphPin::GetDefaultValueVisibility)
		.ForegroundColor(FSlateColor::UseForeground())
		.WrapTextAt(400)
		.MinDesiredWidth(18.0f)
		.MaxDesiredHeight(200);
}
