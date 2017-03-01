// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "MeshEditorStyle.h"
#include "Styling/SlateStyleRegistry.h"


#define IMAGE_BRUSH( RelativePath, ... ) FSlateImageBrush( StyleSet->RootToContentDir( RelativePath, TEXT( ".png" ) ), __VA_ARGS__ )
#define BOX_BRUSH( RelativePath, ... ) FSlateBoxBrush( StyleSet->RootToContentDir( RelativePath, TEXT( ".png" ) ), __VA_ARGS__ )
#define TTF_CORE_FONT( RelativePath, ... ) FSlateFontInfo( StyleSet->RootToCoreContentDir( RelativePath, TEXT( ".ttf" ) ), __VA_ARGS__ )

TSharedPtr<FSlateStyleSet> FMeshEditorStyle::StyleSet = nullptr;

void FMeshEditorStyle::Initialize()
{
	if( StyleSet.IsValid() )
	{
		return;
	}

	StyleSet = MakeShared<FSlateStyleSet>( GetStyleSetName() );

	// @todo mesheditor: put content in the module itself?
	StyleSet->SetContentRoot( FPaths::EngineContentDir() / TEXT( "Editor/Slate") );
	StyleSet->SetCoreContentRoot( FPaths::EngineContentDir() / TEXT( "Slate" ) );

	const FVector2D Icon20x20( 20.0f, 20.0f );
	const FVector2D Icon40x40( 40.0f, 40.0f );
	const FVector2D Icon512x512(512.0f, 512.0f);
	const FLinearColor DimBackground = FLinearColor( FColor( 64, 64, 64 ) );
	const FLinearColor DimBackgroundHover = FLinearColor( FColor( 80, 80, 80 ) );
	const FLinearColor LightBackground = FLinearColor( FColor( 128, 128, 128 ) );
	const FLinearColor HighlightedBackground = FLinearColor( FColor( 255, 192, 0 ) );

	// Icons for the mode panel tabs
	StyleSet->Set( "LevelEditor.MeshEditorMode", new IMAGE_BRUSH( "Icons/icon_Mode_MeshEdit_40px", Icon40x40 ) );
	StyleSet->Set( "LevelEditor.MeshEditorMode.Small", new IMAGE_BRUSH( "Icons/icon_Mode_MeshEdit_40px", Icon20x20 ) );
	StyleSet->Set( "LevelEditor.MeshEditorMode.Selected", new IMAGE_BRUSH( "Icons/icon_Mode_MeshEdit_40px", Icon40x40 ) );
	StyleSet->Set( "LevelEditor.MeshEditorMode.Selected.Small", new IMAGE_BRUSH( "Icons/icon_Mode_MeshEdit_40px", Icon20x20 ) );

	StyleSet->Set( "EditingMode.Entry", FCheckBoxStyle()
		.SetCheckBoxType( ESlateCheckBoxType::ToggleButton )
		.SetUncheckedImage( BOX_BRUSH( "Common/Selection", 8.0f / 32.0f, DimBackground ) )
		.SetUncheckedPressedImage( BOX_BRUSH( "Common/Selection", 8.0f / 32.0f, LightBackground  ) )
		.SetUncheckedHoveredImage( BOX_BRUSH( "Common/Selection", 8.0f / 32.0f, DimBackgroundHover ) )
		.SetCheckedImage( BOX_BRUSH( "Common/Selection", 8.0f / 32.0f, LightBackground ) )
		.SetCheckedHoveredImage( BOX_BRUSH( "Common/Selection", 8.0f / 32.0f, LightBackground ) )
		.SetCheckedPressedImage( BOX_BRUSH( "Common/Selection", 8.0f / 32.0f, LightBackground ) )
		.SetPadding( 0 ) );

	StyleSet->Set( "EditingMode.Entry.Text", FTextBlockStyle()
		.SetFont( TTF_CORE_FONT( "Fonts/Roboto-Bold", 10 ) )
		.SetColorAndOpacity( FLinearColor( 1.0f, 1.0f, 1.0f, 0.9f ) )
		.SetShadowOffset( FVector2D( 1, 1 ) )
		.SetShadowColorAndOpacity( FLinearColor( 0, 0, 0, 0.9f ) )
		.SetHighlightColor( FLinearColor( 0.02f, 0.3f, 0.0f ) )
		.SetHighlightShape( BOX_BRUSH( "Common/TextBlockHighlightShape", FMargin(3.f/8.f) ) ) );

	StyleSet->Set( "SelectionMode.Entry", FCheckBoxStyle()
		.SetCheckBoxType( ESlateCheckBoxType::ToggleButton )
		.SetUncheckedImage( BOX_BRUSH( "Common/Button", 8.0f / 32.0f, DimBackground ) )
		.SetUncheckedPressedImage( BOX_BRUSH( "Common/Button", 8.0f / 32.0f, LightBackground  ) )
		.SetUncheckedHoveredImage( BOX_BRUSH( "Common/Button", 8.0f / 32.0f, DimBackgroundHover ) )
		.SetCheckedImage( BOX_BRUSH( "Common/Button", 8.0f / 32.0f, HighlightedBackground ) )
		.SetCheckedHoveredImage( BOX_BRUSH( "Common/Button", 8.0f / 32.0f, HighlightedBackground ) )
		.SetCheckedPressedImage( BOX_BRUSH( "Common/Button", 8.0f / 32.0f, HighlightedBackground ) )
		.SetPadding( 6 ) );

	StyleSet->Set( "SelectionMode.Entry.Text", FTextBlockStyle()
		.SetFont( TTF_CORE_FONT( "Fonts/Roboto-Regular", 10 ) )
		.SetColorAndOpacity( FLinearColor( 1.0f, 1.0f, 1.0f, 0.9f ) )
		.SetHighlightColor( FLinearColor( 0.02f, 0.3f, 0.0f ) )
		.SetHighlightShape( BOX_BRUSH( "Common/TextBlockHighlightShape", FMargin(3.f/8.f) ) ) );
	// @todo mesheditor: add icons for other modes / actions

	StyleSet->Set("MeshEditorMode.AddSubdivision", new IMAGE_BRUSH("Icons/VREditor/T_Radial_Sub_Add", Icon512x512));
	StyleSet->Set("MeshEditorMode.RemoveSubdivision", new IMAGE_BRUSH("Icons/VREditor/T_Radial_Sub_Minus", Icon512x512));
	StyleSet->Set("MeshEditorMode.PropagateChanges", new IMAGE_BRUSH("Icons/VREditor/Z_Radial_Mesh_Instance", Icon512x512));
	StyleSet->Set("MeshEditorMode.EditInstance", new IMAGE_BRUSH("Icons/VREditor/Z_Radial_Mesh_Non_Instance", Icon512x512));

	StyleSet->Set("MeshEditorMode.PolyDelete", new IMAGE_BRUSH("Icons/VREditor/T_Radial_Poly_Delete", Icon512x512));
	StyleSet->Set("MeshEditorMode.PolyExtrude", new IMAGE_BRUSH("Icons/VREditor/T_Radial_Poly_Extrude", Icon512x512));
	StyleSet->Set("MeshEditorMode.PolyInset", new IMAGE_BRUSH("Icons/VREditor/T_Radial_Poly_Inset", Icon512x512));
	StyleSet->Set("MeshEditorMode.PolyMove", new IMAGE_BRUSH("Icons/VREditor/T_Radial_Poly_Move", Icon512x512));

	StyleSet->Set("MeshEditorMode.VertexExtend", new IMAGE_BRUSH("Icons/VREditor/T_Radial_Vertex_Extend", Icon512x512));
	StyleSet->Set("MeshEditorMode.VertexMove", new IMAGE_BRUSH("Icons/VREditor/T_Radial_Vertex_Move", Icon512x512));
	StyleSet->Set("MeshEditorMode.VertexWeld", new IMAGE_BRUSH("Icons/VREditor/T_Radial_Vertex_Weld", Icon512x512));
	StyleSet->Set("MeshEditorMode.VertexRemove", new IMAGE_BRUSH("Icons/VREditor/T_Radial_Vertex_Remove", Icon512x512));
	StyleSet->Set("MeshEditorMode.VertexDelete", new IMAGE_BRUSH("Icons/VREditor/T_Radial_Vertex_Delete", Icon512x512));

	StyleSet->Set("MeshEditorMode.EdgeDelete", new IMAGE_BRUSH("Icons/VREditor/T_Radial_Edge_Delete", Icon512x512));
	StyleSet->Set("MeshEditorMode.EdgeExtend", new IMAGE_BRUSH("Icons/VREditor/T_Radial_Edge_Extend", Icon512x512));
	StyleSet->Set("MeshEditorMode.EdgeInsert", new IMAGE_BRUSH("Icons/VREditor/T_Radial_Edge_Insert", Icon512x512));
	StyleSet->Set("MeshEditorMode.EdgeMove", new IMAGE_BRUSH("Icons/VREditor/T_Radial_Edge_Move", Icon512x512));
	StyleSet->Set("MeshEditorMode.EdgeRemove", new IMAGE_BRUSH("Icons/VREditor/T_Radial_Edge_Remove", Icon512x512));
	StyleSet->Set("MeshEditorMode.SelectLoop", new IMAGE_BRUSH("Icons/VREditor/T_Radial_Edge_Select_Loop", Icon512x512));

	FSlateStyleRegistry::RegisterSlateStyle( *StyleSet.Get() );
}


void FMeshEditorStyle::Shutdown()
{
	if( StyleSet.IsValid() )
	{
		FSlateStyleRegistry::UnRegisterSlateStyle( *StyleSet.Get() );
		ensure( StyleSet.IsUnique() );
		StyleSet.Reset();
	}
}


FName FMeshEditorStyle::GetStyleSetName()
{
	static FName StyleName( "MeshEditorStyle" );
	return StyleName;
}
