//
// Copyright (C) Valve Corporation. All rights reserved.
//

#include "PhononProbeVolumeDetails.h"

#include "PhononCommon.h"
#include "SteamAudioEditorModule.h"
#include "SteamAudioSettings.h"

#include "DetailLayoutBuilder.h"
#include "DetailCategoryBuilder.h"
#include "DetailWidgetRow.h"
#include "IDetailsView.h"
#include "IDetailCustomization.h"
#include "PhononProbeVolume.h"
#include "PhononProbeComponent.h"
#include "TickableNotification.h"
#include "Async/Async.h"
#include "PhononScene.h"
#include "LevelEditorViewport.h"
#include "Settings/LevelEditorPlaySettings.h"
#include "Settings/LevelEditorViewportSettings.h"
#include "PropertyCustomizationHelpers.h"
#include "IDetailChildrenBuilder.h"
#include "IDetailPropertyRow.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Layout/SBox.h"
#include "Editor.h"

namespace SteamAudio
{
	static TSharedPtr<FTickableNotification> GGenerateProbesTickable = MakeShareable(new FTickableNotification());

	static void GenerateProbesProgressCallback(float Progress)
	{
		FFormatNamedArguments Arguments;
		Arguments.Add(TEXT("GenerateProbesProgress"), FText::AsPercent(Progress));
		GGenerateProbesTickable->SetDisplayText(FText::Format(NSLOCTEXT("SteamAudio", "ComputingProbeLocationsText",
			"Computing probe locations ({GenerateProbesProgress} complete)"), Arguments));
	}

	TSharedRef<IDetailCustomization> FPhononProbeVolumeDetails::MakeInstance()
	{
		return MakeShareable(new FPhononProbeVolumeDetails);
	}

	FText FPhononProbeVolumeDetails::GetTotalDataSize()
	{
		return PrettyPrintedByte(PhononProbeVolume->ProbeDataSize);
	}

	void FPhononProbeVolumeDetails::CustomizeDetails(IDetailLayoutBuilder& DetailLayout)
	{
		const TArray<TWeakObjectPtr<UObject>>& SelectedObjects = DetailLayout.GetSelectedObjects();

		for (int32 ObjectIndex = 0; ObjectIndex < SelectedObjects.Num(); ++ObjectIndex)
		{
			const TWeakObjectPtr<UObject>& CurrentObject = SelectedObjects[ObjectIndex];
			if (CurrentObject.IsValid())
			{
				APhononProbeVolume* CurrentCaptureActor = Cast<APhononProbeVolume>(CurrentObject.Get());
				if (CurrentCaptureActor)
				{
					PhononProbeVolume = CurrentCaptureActor;
					break;
				}
			}
		}

		DetailLayout.HideCategory("BrushSettings");

		DetailLayout.EditCategory("ProbeGeneration").AddProperty(GET_MEMBER_NAME_CHECKED(APhononProbeVolume, PlacementStrategy));
		DetailLayout.EditCategory("ProbeGeneration").AddProperty(GET_MEMBER_NAME_CHECKED(APhononProbeVolume, HorizontalSpacing));
		DetailLayout.EditCategory("ProbeGeneration").AddProperty(GET_MEMBER_NAME_CHECKED(APhononProbeVolume, HeightAboveFloor));
		DetailLayout.EditCategory("ProbeGeneration").AddCustomRow(NSLOCTEXT("PhononProbeVolumeDetails", "Generate Probes", "Generate Probes"))
			.NameContent()
			[
				SNullWidget::NullWidget
			]
			.ValueContent()
			[
				SNew(SHorizontalBox)
				+ SHorizontalBox::Slot()
				.AutoWidth()
				[
					SNew(SButton)
					.ContentPadding(2)
					.VAlign(VAlign_Center)
					.HAlign(HAlign_Center)
					.OnClicked(this, &FPhononProbeVolumeDetails::OnGenerateProbes)
					[
						SNew(STextBlock)
						.Text(NSLOCTEXT("PhononProbeVolumeDetails", "Generate Probes", "Generate Probes"))
						.Font(IDetailLayoutBuilder::GetDetailFont())
					]
				]
			];

			TSharedPtr<IPropertyHandle> BakedDataProperty = DetailLayout.GetProperty(GET_MEMBER_NAME_CHECKED(APhononProbeVolume, BakedDataInfo));
			TSharedRef<FDetailArrayBuilder> BakedDataBuilder = MakeShareable(new FDetailArrayBuilder(BakedDataProperty.ToSharedRef()));
			BakedDataBuilder->OnGenerateArrayElementWidget(FOnGenerateArrayElementWidget::CreateSP(this, &FPhononProbeVolumeDetails::OnGenerateBakedDataInfo));
			DetailLayout.EditCategory("ProbeVolumeStatistics").AddProperty(GET_MEMBER_NAME_CHECKED(APhononProbeVolume, NumProbes));

			auto ProbeDataSize = DetailLayout.GetProperty(GET_MEMBER_NAME_CHECKED(APhononProbeVolume, ProbeDataSize));
			TAttribute<FText> TotalDataSize = TAttribute<FText>::Create(TAttribute<FText>::FGetter::CreateSP(this, &FPhononProbeVolumeDetails::GetTotalDataSize));
		
			DetailLayout.EditCategory("ProbeVolumeStatistics").AddProperty(ProbeDataSize).CustomWidget()
				.NameContent()
				[
					SNew(STextBlock)
					.Text(NSLOCTEXT("PhononProbeVolumeDetails", "Probe Data Size", "Probe Data Size"))
					.Font(IDetailLayoutBuilder::GetDetailFont())
				]
				.ValueContent()
				[
					SNew(STextBlock)
					.Text(TotalDataSize)
					.Font(IDetailLayoutBuilder::GetDetailFont())
				];
			DetailLayout.EditCategory("ProbeVolumeStatistics").AddCustomBuilder(BakedDataBuilder);
	}

	void FPhononProbeVolumeDetails::OnGenerateBakedDataInfo(TSharedRef<IPropertyHandle> PropertyHandle, int32 ArrayIndex, IDetailChildrenBuilder& ChildrenBuilder)
	{
		auto& BakedDataRow = ChildrenBuilder.AddProperty(PropertyHandle);
		auto& BakedDataInfo = PhononProbeVolume->BakedDataInfo[ArrayIndex];

		BakedDataRow.ShowPropertyButtons(false);
		BakedDataRow.CustomWidget(false)
			.NameContent()
			[
				SNew(STextBlock)
				.Text(FText::FromName(BakedDataInfo.Name))
				.Font(IDetailLayoutBuilder::GetDetailFont())
			]
			.ValueContent()
			[
				SNew(SHorizontalBox)
				+ SHorizontalBox::Slot()
				[
					SNew(SBox)
					.MinDesiredWidth(200)
					.VAlign(VAlign_Center)
					[
						SNew(STextBlock)
						.Text(PrettyPrintedByte(BakedDataInfo.Size))
						.Font(IDetailLayoutBuilder::GetDetailFont())
					]
				]
				+ SHorizontalBox::Slot()
					.AutoWidth()
				[
					PropertyCustomizationHelpers::MakeDeleteButton(FSimpleDelegate::CreateSP(this, &FPhononProbeVolumeDetails::OnClearBakedDataClicked, ArrayIndex))
				]
			];
	}

	void FPhononProbeVolumeDetails::OnClearBakedDataClicked(const int32 ArrayIndex)
	{
		IPLhandle ProbeBox = nullptr;
		PhononProbeVolume->LoadProbeBoxFromDisk(&ProbeBox);
		iplDeleteBakedDataByName(ProbeBox, TCHAR_TO_ANSI(*PhononProbeVolume->BakedDataInfo[ArrayIndex].Name.ToString().ToLower()));
		PhononProbeVolume->BakedDataInfo.RemoveAt(ArrayIndex);
		PhononProbeVolume->UpdateProbeData(ProbeBox);
		iplDestroyProbeBox(&ProbeBox);
	}

	FReply FPhononProbeVolumeDetails::OnGenerateProbes()
	{
		GGenerateProbesTickable->SetDisplayText(NSLOCTEXT("SteamAudio", "Generating probes...", "Generating probes..."));
		GGenerateProbesTickable->CreateNotification();

		// Grab a copy of the volume ptr as it will be destroyed if user clicks off of volume in GUI
		auto PhononProbeVolumeHandle = PhononProbeVolume.Get();

		Async<void>(EAsyncExecution::Thread, [PhononProbeVolumeHandle]()
		{
			// Load the scene
			UWorld* World = GEditor->LevelViewportClients[0]->GetWorld();
			IPLhandle PhononScene = nullptr;
			FPhononSceneInfo PhononSceneInfo;

			IPLSimulationSettings SimulationSettings;
			SimulationSettings.sceneType = IPL_SCENETYPE_PHONON;
			SimulationSettings.irDuration = GetDefault<USteamAudioSettings>()->IndirectImpulseResponseDuration;
			SimulationSettings.ambisonicsOrder = GetDefault<USteamAudioSettings>()->IndirectImpulseResponseOrder;
			SimulationSettings.maxConvolutionSources = 1024; // FIXME
			SimulationSettings.numBounces = GetDefault<USteamAudioSettings>()->BakedBounces;
			SimulationSettings.numRays = GetDefault<USteamAudioSettings>()->BakedRays;
			SimulationSettings.numDiffuseSamples = GetDefault<USteamAudioSettings>()->BakedSecondaryRays;

			GGenerateProbesTickable->SetDisplayText(NSLOCTEXT("SteamAudio", "Loading scene...", "Loading scene..."));

			// Attempt to load from disk, otherwise export
			if (!LoadSceneFromDisk(World, nullptr, SimulationSettings, &PhononScene, PhononSceneInfo))
			{
				TArray<IPLhandle> PhononStaticMeshes;

				if (!CreateScene(World, &PhononScene, &PhononStaticMeshes, PhononSceneInfo.NumTriangles))
				{
					GGenerateProbesTickable->QueueWorkItem(FWorkItem([](FText& DisplayText) {
						DisplayText = NSLOCTEXT("SteamAudio", "Unable to create scene.", "Unable to create scene.");
					}, SNotificationItem::CS_Fail, true));
					return;
				}

				//iplFinalizeScene(PhononScene, FinalizeSceneProgressCallback);
				iplFinalizeScene(PhononScene, nullptr);
				PhononSceneInfo.DataSize = iplSaveFinalizedScene(PhononScene, nullptr);
				bool SaveSceneSuccessful = SaveFinalizedSceneToDisk(World, PhononScene, PhononSceneInfo);

				if (SaveSceneSuccessful)
				{
					FSteamAudioEditorModule* Module = &FModuleManager::GetModuleChecked<FSteamAudioEditorModule>("SteamAudioEditor");
					if (Module != nullptr)
					{
						Module->SetCurrentPhononSceneInfo(PhononSceneInfo);
					}
				}

				for (IPLhandle PhononStaticMesh : PhononStaticMeshes)
				{
					iplDestroyStaticMesh(&PhononStaticMesh);
				}
			}

			// Place probes
			IPLhandle SceneCopy = PhononScene;
			TArray<IPLSphere> ProbeSpheres;
			PhononProbeVolumeHandle->PlaceProbes(PhononScene, GenerateProbesProgressCallback, ProbeSpheres);
			PhononProbeVolumeHandle->BakedDataInfo.Empty();

			// Clean up
			iplDestroyScene(&SceneCopy);

			// Update probe component with new probe locations
			{
				FScopeLock(&PhononProbeVolumeHandle->PhononProbeComponent->ProbeLocationsCriticalSection);
				auto& ProbeLocations = PhononProbeVolumeHandle->GetPhononProbeComponent()->ProbeLocations;
				ProbeLocations.Empty();
				ProbeLocations.SetNumUninitialized(ProbeSpheres.Num());
				for (auto i = 0; i < ProbeSpheres.Num(); ++i)
				{
					ProbeLocations[i] = SteamAudio::PhononToUnrealFVector(SteamAudio::FVectorFromIPLVector3(ProbeSpheres[i].center));
				}
			}
			
			// Notify UI that we're done
			GGenerateProbesTickable->QueueWorkItem(FWorkItem([](FText& DisplayText) {
				DisplayText = NSLOCTEXT("SteamAudio", "Probe placement complete.", "Probe placement complete."); 
			}, SNotificationItem::CS_Success, true));
		});

		return FReply::Handled();
	}
}