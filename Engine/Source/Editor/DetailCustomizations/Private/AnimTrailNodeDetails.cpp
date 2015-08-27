// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "DetailCustomizationsPrivatePCH.h"

#include "AnimTrailNodeDetails.h"
#include "AnimGraphNode_Trail.h"

#define LOCTEXT_NAMESPACE "FAnimTrailNodeDetails"

/////////////////////////////////////////////////////////////////////////

TSharedRef<IDetailCustomization> FAnimTrailNodeDetails::MakeInstance()
{
	return MakeShareable( new FAnimTrailNodeDetails );
}

void FAnimTrailNodeDetails::CustomizeDetails( IDetailLayoutBuilder& DetailBuilder )
{	
	TArray<TWeakObjectPtr<UObject> > SelectedObjects;	//the objects we're showing details for
	DetailBuilder.GetObjectsBeingCustomized(SelectedObjects);

	//we only do fancy customization if we have one vehicle component selected
	if(SelectedObjects.Num() != 1)
	{
		return;
	}
	else if(UAnimGraphNode_Trail* InGraphNode = Cast<UAnimGraphNode_Trail>(SelectedObjects[0].Get()))
	{
		TrailRelaxCurveEditor = FTrailRelaxCurveEditor(InGraphNode);
	}
	else
	{
		return;
	}

	//Trail Relax curve
	IDetailCategoryBuilder& TrailCategory = DetailBuilder.EditCategory("Trail");
	TSharedRef<IPropertyHandle> TrailRelaxCurveHandle = DetailBuilder.GetProperty("Node.TrailRelaxationCurve");
	TSharedPtr<class SCurveEditor> TrailRelaxCurveWidget;

	TrailCategory.AddProperty(TrailRelaxCurveHandle).CustomWidget()
	.NameContent()
		[
			TrailRelaxCurveHandle->CreatePropertyNameWidget()
		]
	.ValueContent()
		.MinDesiredWidth(125.f * 3.f)
		[
			SAssignNew(TrailRelaxCurveWidget, SCurveEditor)
			.ViewMinInput(0.f)
			.ViewMaxInput(70000.f)
			.ViewMinOutput(0.f)
			.ViewMaxOutput(1.f)
			.TimelineLength(7000.f)
			.HideUI(false)
			.DesiredSize(FVector2D(512, 128))
			.ZoomToFitVertical(true)
			.ZoomToFitHorizontal(true)
		];

	TrailRelaxCurveWidget->SetCurveOwner(&TrailRelaxCurveEditor);
}


TArray<FRichCurveEditInfoConst> FAnimTrailNodeDetails::FTrailRelaxCurveEditor::GetCurves() const
{
	TArray<FRichCurveEditInfoConst> Curves;
	Curves.Add(&GraphNodeOwner->Node.TrailRelaxationCurve.EditorCurveData);

	return Curves;
}

TArray<FRichCurveEditInfo> FAnimTrailNodeDetails::FTrailRelaxCurveEditor::GetCurves()
{
	TArray<FRichCurveEditInfo> Curves;
	Curves.Add(&GraphNodeOwner->Node.TrailRelaxationCurve.EditorCurveData);

	return Curves;
}

void FAnimTrailNodeDetails::FTrailRelaxCurveEditor::ModifyOwner()
{
	if(GraphNodeOwner)
	{
		GraphNodeOwner->Modify();
	}
}

void FAnimTrailNodeDetails::FTrailRelaxCurveEditor::MakeTransactional()
{
	if(GraphNodeOwner)
	{
		GraphNodeOwner->SetFlags(GraphNodeOwner->GetFlags() | RF_Transactional);
	}
}

bool FAnimTrailNodeDetails::FTrailRelaxCurveEditor::IsValidCurve(FRichCurveEditInfo CurveInfo)
{
	return CurveInfo.CurveToEdit == &GraphNodeOwner->Node.TrailRelaxationCurve.EditorCurveData;
}

FAnimTrailNodeDetails::FTrailRelaxCurveEditor::FTrailRelaxCurveEditor(UAnimGraphNode_Trail * InGraphNode)
{
	GraphNodeOwner = InGraphNode;
}

#undef LOCTEXT_NAMESPACE
