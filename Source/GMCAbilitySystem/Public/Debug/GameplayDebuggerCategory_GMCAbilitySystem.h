// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "GameplayDebuggerCategory.h"
#include "GameplayTagContainer.h"
#if WITH_GAMEPLAY_DEBUGGER

/**
 * 
 */

class GMCABILITYSYSTEM_API FGameplayDebuggerCategory_GMCAbilitySystem : public FGameplayDebuggerCategory
{
public:
	FGameplayDebuggerCategory_GMCAbilitySystem();
	virtual void CollectData(APlayerController* OwnerPC, AActor* DebugActor) override;
	virtual void DrawData(APlayerController* OwnerPC, FGameplayDebuggerCanvasContext& CanvasContext) override;

	static TSharedRef<FGameplayDebuggerCategory> MakeInstance();

	/**
 * Splits a long string into multiple lines for better display in the Gameplay Debugger.
 * 
 * @param CanvasContext - The canvas context to print the text to
 * @param Header - Optional header text to display before the wrapped content
 * @param Content - The text content to wrap and display
 * @param MaxLineLength - Maximum length of each line before wrapping
 * @param IndentSubsequentLines - Whether to indent lines after the first one
 * @param IndentStr - The string to use for indentation (default is 4 spaces)
 */
	void DrawWrappedText(FGameplayDebuggerCanvasContext& CanvasContext, 
					 const FString& Header,
					 const FString& Content, 
					 int32 MaxLineLength = 100,
					 bool IndentSubsequentLines = true,
					 const FString& IndentStr = TEXT("    "));


protected:
	struct FRepData
	{
		// Put all data you want to display here
		FString ActorName;
		FString GrantedAbilities;
		int NBGrantedAbilities;
		FString ActiveTags;
		int NBActiveTags;
		FString Attributes;
		int NBAttributes;
		FString ActiveEffects;
		int NBActiveEffects;
		FString ActiveEffectData;
		int NBActiveEffectData;
		FString ActiveAbilities;
		int NBActiveAbilities;
        
		void Serialize(FArchive& Ar);
	};
    
	FRepData DataPack;
};

#endif	