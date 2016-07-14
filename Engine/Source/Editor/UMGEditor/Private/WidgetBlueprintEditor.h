// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Editor/Kismet/Public/BlueprintEditor.h"
#include "ISequencer.h"
#include "PreviewScene.h"
#include "WidgetReference.h"

class ISequencer;
class UWidgetAnimation;
class UUserWidget;

struct FNamedSlotSelection
{
	FWidgetReference NamedSlotHostWidget;
	FName SlotName;
};

/**
 * widget blueprint editor (extends Blueprint editor)
 */
class UMGEDITOR_API FWidgetBlueprintEditor : public FBlueprintEditor
{
public:
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnHoveredWidgetSet, const FWidgetReference&)
	DECLARE_MULTICAST_DELEGATE(FOnHoveredWidgetCleared);

	DECLARE_MULTICAST_DELEGATE(FOnSelectedWidgetsChanging)
	DECLARE_MULTICAST_DELEGATE(FOnSelectedWidgetsChanged)

	/** Called after the widget preview has been updated */
	DECLARE_MULTICAST_DELEGATE(FOnWidgetPreviewUpdated)
public:
	FWidgetBlueprintEditor();

	virtual ~FWidgetBlueprintEditor();

	void InitWidgetBlueprintEditor(const EToolkitMode::Type Mode, const TSharedPtr< IToolkitHost >& InitToolkitHost, const TArray<UBlueprint*>& InBlueprints, bool bShouldOpenInDefaultsMode);

	/** FBlueprintEditor interface */
	virtual void Tick(float DeltaTime) override;
	virtual void PostUndo(bool bSuccessful) override;
	virtual void PostRedo(bool bSuccessful) override;
	virtual void Compile() override;

	/** FGCObjectInterface */
	virtual void AddReferencedObjects( FReferenceCollector& Collector ) override;

	/** @return The widget blueprint currently being edited in this editor */
	class UWidgetBlueprint* GetWidgetBlueprintObj() const;

	/** @return The preview widget. */
	UUserWidget* GetPreview() const;

	/** @return The preview scene that owns the preview widget. */
	FPreviewScene* GetPreviewScene();

	/**  */
	bool IsSimulating() const;

	/**  */
	void SetIsSimulating(bool bSimulating);

	/** Causes the preview to be destroyed and a new one to be created next tick */
	void InvalidatePreview(bool bViewOnly = false);

	/** Immediately rebuilds the preview widget. */
	void RefreshPreview();

	/** Creates a widget reference using the template. */
	FWidgetReference GetReferenceFromTemplate(UWidget* TemplateWidget);

	/** Creates a widget reference using the preview.  Which is used to lookup the stable template pointer. */
	FWidgetReference GetReferenceFromPreview(UWidget* PreviewWidget);

	/** @return The sequencer used to create widget animations */
	TSharedPtr<ISequencer>& GetSequencer();

	/** Changes the currently viewed animation in Sequencer to the new one*/
	void ChangeViewedAnimation( UWidgetAnimation& InAnimationToView );

	/** Updates the current animation if it is invalid */
	const UWidgetAnimation* RefreshCurrentAnimation();

	/** Sets the currently selected set of widgets */
	void SelectWidgets(const TSet<FWidgetReference>& Widgets, bool bAppendOrToggle);

	/** Sets the currently selected set of objects */
	void SelectObjects(const TSet<UObject*>& Objects);

	/** Sets the selected named slot */
	void SetSelectedNamedSlot(TOptional<FNamedSlotSelection> SelectedNamedSlot);

	/** Removes removed widgets from the selection set. */
	void CleanSelection();

	/** @return The selected set of widgets */
	const TSet<FWidgetReference>& GetSelectedWidgets() const;

	/** @return The selected named slot */
	TOptional<FNamedSlotSelection> GetSelectedNamedSlot() const;

	/** @return The selected set of Objects */
	const TSet< TWeakObjectPtr<UObject> >& GetSelectedObjects() const;

	TSharedPtr<class FWidgetBlueprintEditorToolbar> GetWidgetToolbarBuilder() { return WidgetToolbar; }

	/** Migrate a property change from the preview GUI to the template GUI. */
	void MigrateFromChain(FEditPropertyChain* PropertyThatChanged, bool bIsModify);

	/** Event called when an undo/redo transaction occurs */
	DECLARE_EVENT(FWidgetBlueprintEditor, FOnWidgetBlueprintTransaction)
	FOnWidgetBlueprintTransaction& GetOnWidgetBlueprintTransaction() { return OnWidgetBlueprintTransaction; }

	/** Creates a sequencer widget */
	TSharedRef<SWidget> CreateSequencerWidget();

	/**
	 * The widget we're now hovering over in any particular context, allows multiple views to 
	 * synchronize feedback on where that widget is in their representation.
	 */
	void SetHoveredWidget(FWidgetReference& InHoveredWidget);

	void ClearHoveredWidget();

	/** @return The widget that is currently being hovered over (either in the designer or hierarchy) */
	const FWidgetReference& GetHoveredWidget() const;

	void AddPostDesignerLayoutAction(TFunction<void()> Action);

	TArray< TFunction<void()> >& GetQueuedDesignerActions();

	/** Get the current designer flags that are in effect for the current user widget we're editing. */
	EWidgetDesignFlags::Type GetCurrentDesignerFlags() const;

public:
	/** Fires whenever a new widget is being hovered over */
	FOnHoveredWidgetSet OnHoveredWidgetSet;

	/** Fires when there is no longer any widget being hovered over */
	FOnHoveredWidgetCleared OnHoveredWidgetCleared;	

	/** Fires whenever the selected set of widgets changing */
	FOnSelectedWidgetsChanged OnSelectedWidgetsChanging;

	/** Fires whenever the selected set of widgets changes */
	FOnSelectedWidgetsChanged OnSelectedWidgetsChanged;

	/** Notification for when the preview widget has been updated */
	FOnWidgetPreviewUpdated OnWidgetPreviewUpdated;

	/** Command list for handling widget actions in the WidgetBlueprintEditor */
	TSharedPtr< FUICommandList > DesignerCommandList;

	/** Paste Metadata */
	FVector2D PasteDropLocation;

protected:
	// Begin FBlueprintEditor
	virtual void RegisterApplicationModes(const TArray<UBlueprint*>& InBlueprints, bool bShouldOpenInDefaultsMode, bool bNewlyCreated = false) override;
	virtual FGraphAppearanceInfo GetGraphAppearance(class UEdGraph* InGraph) const override;
	virtual void AppendExtraCompilerResults(TSharedPtr<class IMessageLogListing> ResultsListing) override;
	// End FBlueprintEditor

private:
	bool CanDeleteSelectedWidgets();
	void DeleteSelectedWidgets();

	bool CanCopySelectedWidgets();
	void CopySelectedWidgets();

	bool CanPasteWidgets();
	void PasteWidgets();

	bool CanCutSelectedWidgets();
	void CutSelectedWidgets();

private:
	/** Called whenever the blueprint is structurally changed. */
	virtual void OnBlueprintChangedImpl(UBlueprint* InBlueprint, bool bIsJustBeingCompiled = false ) override;

	/** Called when objects need to be swapped out for new versions, like after a blueprint recompile. */
	void OnObjectsReplaced(const TMap<UObject*, UObject*>& ReplacementMap);

	/** Destroy the current preview GUI object */
	void DestroyPreview();

	/** Tick the current preview GUI object */
	void UpdatePreview(UBlueprint* InBlueprint, bool bInForceFullUpdate);

	/** Populates the sequencer add menu. */
	void OnGetAnimationAddMenuContent(FMenuBuilder& MenuBuilder, TSharedRef<ISequencer> Sequencer);

	/** Adds the supplied UObject to the current animation. */
	void AddObjectToAnimation(UObject* ObjectToAnimate);

	/** Gets the extender to use for sequencers context sensitive menus and toolbars. */
	TSharedRef<FExtender> GetContextSensitiveSequencerExtender( const TSharedRef<FUICommandList> CommandList, const TArray<UObject*> ContextSensitiveObjects );

	/** Extends the sequencer add track menu. */
	void ExtendSequencerAddTrackMenu( FMenuBuilder& AddTrackMenuBuilder, const TArray<UObject*> ContextObjects );

	/** Add an animation track for the supplied slot to the current animation. */
	void AddSlotTrack( UPanelSlot* Slot );

	/** Add an animation track for the supplied material property path to the current animation. */
	void AddMaterialTrack( UWidget* Widget, TArray<UProperty*> MaterialPropertyPath, FText MaterialPropertyDisplayName );

	/** Handler which is called whenever sequencer movie scene data changes. */
	void OnMovieSceneDataChanged();

private:
	/** The preview scene that owns the preview GUI */
	FPreviewScene PreviewScene;

	/** Sequencer for creating and previewing widget animations */
	TSharedPtr<ISequencer> Sequencer;

	/** Overlay used to display UI on top of sequencer */
	TWeakPtr<SOverlay> SequencerOverlay;

	/** A text block which is displayed in the overlay when no animation is selected. */
	TWeakPtr<STextBlock> NoAnimationTextBlock;

	/** The Blueprint associated with the current preview */
	UWidgetBlueprint* PreviewBlueprint;

	/** The currently selected preview widgets in the preview GUI */
	TSet< FWidgetReference > SelectedWidgets;

	/** The currently selected objects in the designer */
	TSet< TWeakObjectPtr<UObject> > SelectedObjects;

	/** The currently selected named slot */
	TOptional<FNamedSlotSelection> SelectedNamedSlot;

	/** The preview GUI object */
	mutable TWeakObjectPtr<UUserWidget> PreviewWidgetPtr;

	/** Delegate called when a undo/redo transaction happens */
	FOnWidgetBlueprintTransaction OnWidgetBlueprintTransaction;

	/** The toolbar builder associated with this editor */
	TSharedPtr<class FWidgetBlueprintEditorToolbar> WidgetToolbar;

	/** The widget references out in the ether that may need to be updated after being issued. */
	TArray< TWeakPtr<FWidgetHandle> > WidgetHandlePool;

	/** The widget currently being hovered over */
	FWidgetReference HoveredWidget;

	/** The preview becomes invalid and needs to be rebuilt on the next tick. */
	bool bPreviewInvalidated;

	/**  */
	bool bIsSimulateEnabled;

	/**  */
	bool bIsRealTime;

	TArray< TFunction<void()> > QueuedDesignerActions;

	/** The currently viewed animation, if any. */
	TWeakObjectPtr<UWidgetAnimation> CurrentAnimation;

	FDelegateHandle SequencerExtenderHandle;

	/** Messages we want to append to the compiler results. */
	TArray< TSharedRef<class FTokenizedMessage> > DesignerCompilerMessages;

	/** When true the animation data in the generated class should be replaced with the current animation data. */
	bool bRefreshGeneratedClassAnimations;
};
