// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

//////////////////////////////////////////////////////////////////////////
// STimelineHeader

class STimelineHeader : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(STimelineHeader)
		: _SlateUnitsPerFrame(1)
		, _FlipbookBeingEdited((class UPaperFlipbook*)NULL)
		, _PlayTime(0)
	{}

	SLATE_ATTRIBUTE(float, SlateUnitsPerFrame)
		SLATE_ATTRIBUTE(class UPaperFlipbook*, FlipbookBeingEdited)
		SLATE_ATTRIBUTE(float, PlayTime)

		SLATE_END_ARGS()

		void Construct(const FArguments& InArgs)
	{
			SlateUnitsPerFrame = InArgs._SlateUnitsPerFrame;
			FlipbookBeingEdited = InArgs._FlipbookBeingEdited;
			PlayTime = InArgs._PlayTime;

			NumFramesFromLastRebuild = 0;

			ChildSlot
				[
					SNew(SOverlay)

					+ SOverlay::Slot()
					[
						SAssignNew(MainBoxPtr, SHorizontalBox)
					]

					+ SOverlay::Slot()
						.Padding(TAttribute<FMargin>(this, &STimelineHeader::GetPlayTimePadding))
						[
							SNew(SBox)
							.WidthOverride(SlateUnitsPerFrame.Get())
							[
								SNew(SImage)
							]
						]
				];

			Rebuild();
		}

	void Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime) override
	{
		SCompoundWidget::Tick(AllottedGeometry, InCurrentTime, InDeltaTime);

		UPaperFlipbook* Flipbook = FlipbookBeingEdited.Get();
		int32 NewNumFrames = (Flipbook != NULL ? Flipbook->GetNumFrames() : 0);
		if (NewNumFrames != NumFramesFromLastRebuild)
		{
			Rebuild();
		}
	}

private:
	void Rebuild()
	{
		MainBoxPtr->ClearChildren();

		UPaperFlipbook* Flipbook = FlipbookBeingEdited.Get();
		float LocalSlateUnitsPerFrame = SlateUnitsPerFrame.Get();
		if (Flipbook && LocalSlateUnitsPerFrame > 0)
		{
			const int32 NumFrames = Flipbook->GetNumFrames();
			for (int32 FrameIdx = 0; FrameIdx < NumFrames; ++FrameIdx)
			{
				MainBoxPtr->AddSlot()
					.AutoWidth()
					[
						SNew(SBox)
						.WidthOverride(LocalSlateUnitsPerFrame)
						.HAlign(HAlign_Center)
						[
							SNew(STextBlock)
							.Text(FText::AsNumber(FrameIdx).ToString())
						]
					];
			}

			NumFramesFromLastRebuild = NumFrames;
		}
		else
		{
			NumFramesFromLastRebuild = 0;
		}
	}

	FMargin GetPlayTimePadding() const
	{
		float PlayTimePadding = PlayTime.Get() - SlateUnitsPerFrame.Get() / 2;
		return FMargin(PlayTimePadding, 0, 0, 0);
	}


private:
	TAttribute<float> SlateUnitsPerFrame;
	TAttribute<class UPaperFlipbook*> FlipbookBeingEdited;
	TAttribute<float> PlayTime;

	TSharedPtr<SHorizontalBox> MainBoxPtr;

	int32 NumFramesFromLastRebuild;
};
