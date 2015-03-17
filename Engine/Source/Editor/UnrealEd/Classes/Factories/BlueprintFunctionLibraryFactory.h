// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

/**
 *
 */

#pragma once
#include "BlueprintFunctionLibraryFactory.generated.h"

UCLASS(hidecategories=Object, collapsecategories)
class UBlueprintFunctionLibraryFactory : public UBlueprintFactory
{
	GENERATED_UCLASS_BODY()

	// UFactory interface
	virtual FText GetDisplayName() const override;
	virtual FName GetNewAssetThumbnailOverride() const override;
	virtual uint32 GetMenuCategories() const override;
	virtual FText GetToolTip() const override;
	virtual FString GetToolTipDocumentationExcerpt() const override;
	virtual UObject* FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn, FName CallingContext) override;
	virtual bool ConfigureProperties() override;
	virtual FString GetDefaultNewAssetName() const override;
	// End of UFactory interface
};



