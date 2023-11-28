﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/GMCAbilityComponent.h"


// Sets default values for this component's properties
UGMC_AbilityComponent::UGMC_AbilityComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

bool UGMC_AbilityComponent::TryActivateAbility(FGMCAbilityData InAbilityData)
{
	// UE_LOG(LogTemp, Warning, TEXT("Trying To Activate Ability: %d"), AbilityData.GrantedAbilityIndex);
	if (AbilityData.GrantedAbilityIndex >= 0 && AbilityData.GrantedAbilityIndex <= GrantedAbilities.Num() - 1)
	{
		UGMCAbility* Ability = GrantedAbilities[AbilityData.GrantedAbilityIndex]->GetDefaultObject<UGMCAbility>();

		// Check to make sure there's enough resource to use
		if (!CanAffordAbilityCost(Ability)) return false;
		
		Ability->Execute(InAbilityData, this);

		return true;
		// UE_LOG(LogTemp, Warning, TEXT("Ability: %d Activated | %s"), AbilityData.GrantedAbilityIndex, *AbilityData.AbilityActivationID.ToString());
	}

	return false;
}

void UGMC_AbilityComponent::QueueAbility(FGMCAbilityData InAbilityData)
{
	// UE_LOG(LogTemp, Warning, TEXT("Queued Ability: %d"), AbilityData.GrantedAbilityIndex);
	QueuedAbilities.Push(InAbilityData);
}

void UGMC_AbilityComponent::BindReplicationData_Implementation()
{
	Super::BindReplicationData_Implementation();

	// Attribute Binds
	BindSinglePrecisionFloat(Attributes.Stamina,
		EGMC_PredictionMode::ServerAuth_Output_ClientValidated,
		EGMC_CombineMode::CombineIfUnchanged,
		EGMC_SimulationMode::Periodic_Output,
		EGMC_InterpolationFunction::Linear);

	BindSinglePrecisionFloat(Attributes.MaxStamina,
		EGMC_PredictionMode::ServerAuth_Output_ClientValidated,
		EGMC_CombineMode::CombineIfUnchanged,
		EGMC_SimulationMode::Periodic_Output,
		EGMC_InterpolationFunction::Linear);

	BindSinglePrecisionFloat(Attributes.Health,
		EGMC_PredictionMode::ServerAuth_Output_ClientValidated,
		EGMC_CombineMode::CombineIfUnchanged,
		EGMC_SimulationMode::Periodic_Output,
		EGMC_InterpolationFunction::Linear);

	BindSinglePrecisionFloat(Attributes.MaxHealth,
		EGMC_PredictionMode::ServerAuth_Output_ClientValidated,
		EGMC_CombineMode::CombineIfUnchanged,
		EGMC_SimulationMode::Periodic_Output,
		EGMC_InterpolationFunction::Linear);

	// AbilityData Binds
	// These are mostly client-inputs made to the server as Ability Requests
	BindInt(AbilityData.AbilityActivationID,
		EGMC_PredictionMode::ClientAuth_Input,
		EGMC_CombineMode::CombineIfUnchanged,
		EGMC_SimulationMode::None,
		EGMC_InterpolationFunction::TargetValue);

	BindInt(AbilityData.GrantedAbilityIndex,
		EGMC_PredictionMode::ClientAuth_Input,
		EGMC_CombineMode::CombineIfUnchanged,
		EGMC_SimulationMode::None,
		EGMC_InterpolationFunction::TargetValue);

	BindCompressedVector(AbilityData.TargetVector0,
		EGMC_PredictionMode::ClientAuth_Input,
		EGMC_CombineMode::CombineIfUnchanged,
		EGMC_SimulationMode::None,
		EGMC_InterpolationFunction::TargetValue);

	BindCompressedVector(AbilityData.TargetVector1,
		EGMC_PredictionMode::ClientAuth_Input,
		EGMC_CombineMode::CombineIfUnchanged,
		EGMC_SimulationMode::None,
		EGMC_InterpolationFunction::TargetValue);

	BindCompressedVector(AbilityData.TargetVector2,
		EGMC_PredictionMode::ClientAuth_Input,
		EGMC_CombineMode::CombineIfUnchanged,
		EGMC_SimulationMode::None,
		EGMC_InterpolationFunction::TargetValue);

	BindActorReference(AbilityData.TargetActor,
		EGMC_PredictionMode::ClientAuth_Input,
		EGMC_CombineMode::CombineIfUnchanged,
		EGMC_SimulationMode::None,
		EGMC_InterpolationFunction::TargetValue);

	BindActorComponentReference(AbilityData.TargetComponent,
		EGMC_PredictionMode::ClientAuth_Input,
		EGMC_CombineMode::CombineIfUnchanged,
		EGMC_SimulationMode::None,
		EGMC_InterpolationFunction::TargetValue);

	BindBool(AbilityData.bProcessed,
		EGMC_PredictionMode::ClientAuth_Input,
		EGMC_CombineMode::CombineIfUnchanged,
		EGMC_SimulationMode::None,
		EGMC_InterpolationFunction::TargetValue);
	}

void UGMC_AbilityComponent::GenPredictionTick_Implementation(float DeltaTime)
{
	Super::GenPredictionTick_Implementation(DeltaTime);

	if (AbilityData.bProcessed == false)
	{
		TryActivateAbility(AbilityData);
		UE_LOG(LogTemp, Warning, TEXT("%d: Processing Ability..."), GetOwner()->GetLocalRole());
		AbilityData.bProcessed = true;
	}
}

void UGMC_AbilityComponent::PreLocalMoveExecution_Implementation(const FGMC_Move& LocalMove)
{
	Super::PreLocalMoveExecution_Implementation(LocalMove);
	if (QueuedAbilities.Num() > 0)
	{
		AbilityData = QueuedAbilities.Pop();
		AbilityData.bProcessed = false;
	}
}

bool UGMC_AbilityComponent::CanAffordAbilityCost(UGMCAbility* Ability)
{
	if (Attributes.Stamina >= Ability->StaminaCost) return true;
	return false;
}

void UGMC_AbilityComponent::ApplyAbilityCost(UGMCAbility* Ability)
{
	Attributes.Stamina -= Ability->StaminaCost;
}

