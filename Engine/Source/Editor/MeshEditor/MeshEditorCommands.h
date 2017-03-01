// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "MeshEditorStyle.h"

#define LOCTEXT_NAMESPACE "MeshEditorCommands"

// Actions that can be invoked from this mode regardless of what type of elements are selected
class FMeshEditorCommonCommands : public TCommands<FMeshEditorCommonCommands>
{
public:
	FMeshEditorCommonCommands() : TCommands<FMeshEditorCommonCommands>
	(
		"MeshEditorCommon",
		LOCTEXT("MeshEditorGeneral", "Mesh Editor Common"),
		"MainFrame",
		FMeshEditorStyle::GetStyleSetName()
	)
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;
	// End of TCommands<> interface

public:

	/** Deletes selected mesh elements, including polygons partly defined by selected elements */
	TSharedPtr<FUICommandInfo> DeleteMeshElement;

	/** Increases the number of subdivision levels for the selected mesh. */
	TSharedPtr<FUICommandInfo> AddSubdivisionLevel;

	/** Decreases the number of subdivision levels for the selected mesh. */
	TSharedPtr<FUICommandInfo> RemoveSubdivisionLevel;

	/** Shows vertex normals */
	TSharedPtr<FUICommandInfo> ShowVertexNormals;

	/** Marquee select actions */
	TSharedPtr<FUICommandInfo> MarqueeSelectVertices;
	TSharedPtr<FUICommandInfo> MarqueeSelectEdges;
	TSharedPtr<FUICommandInfo> MarqueeSelectPolygons;

	/** Draw vertices */
	TSharedPtr<FUICommandInfo> DrawVertices;

	/** Frame selected elements */
	TSharedPtr<FUICommandInfo> FrameSelectedElements;

	/** Set mesh element selection modes */
	TSharedPtr<FUICommandInfo> SetVertexSelectionMode;
	TSharedPtr<FUICommandInfo> SetEdgeSelectionMode;
	TSharedPtr<FUICommandInfo> SetPolygonSelectionMode;
	TSharedPtr<FUICommandInfo> SetAnySelectionMode;

	/** Quadrangulate mesh */
	TSharedPtr<FUICommandInfo> QuadrangulateMesh;
};


// Actions that can be invoked from this mode when vertices are selected
class FMeshEditorVertexCommands : public TCommands<FMeshEditorVertexCommands>
{
public:
	FMeshEditorVertexCommands() : TCommands<FMeshEditorVertexCommands>
		(
			"MeshEditorVertex",
			LOCTEXT("MeshEditorVertex", "Mesh Editor Vertex"),
			"MeshEditorCommon",
			FMeshEditorStyle::GetStyleSetName()
			)
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;
	// End of TCommands<> interface

public:

	/** Sets the primary action to move vertices */
	TSharedPtr<FUICommandInfo> MoveVertex;

	/** Sets the primary action to extend vertices */
	TSharedPtr<FUICommandInfo> ExtendVertex;

	/** Sets the primary action to edit the vertex's corner sharpness */
	TSharedPtr<FUICommandInfo> EditVertexCornerSharpness;

	/** Removes the selected vertex if possible */
	TSharedPtr<FUICommandInfo> RemoveVertex;

	/** Welds the selected vertices */
	TSharedPtr<FUICommandInfo> WeldVertices;
};


// Actions that can be invoked from this mode when edges are selected
class FMeshEditorEdgeCommands : public TCommands<FMeshEditorEdgeCommands>
{
public:
	FMeshEditorEdgeCommands() : TCommands<FMeshEditorEdgeCommands>
		(
			"MeshEditorEdge",
			LOCTEXT("MeshEditorEdge", "Mesh Editor Edge"),
			"MeshEditorCommon",
			FMeshEditorStyle::GetStyleSetName()
			)
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;
	// End of TCommands<> interface

public:

	/** Sets the primary action to move edges */
	TSharedPtr<FUICommandInfo> MoveEdge;

	/** Sets the primary action to split edges */
	TSharedPtr<FUICommandInfo> SplitEdge;

	/** Sets the primary action to split edges and drag vertices */
	TSharedPtr<FUICommandInfo> SplitEdgeAndDragVertex;

	/** Sets the primary action to insert edge loops */
	TSharedPtr<FUICommandInfo> InsertEdgeLoop;

	/** Sets the primary action to extend edges */
	TSharedPtr<FUICommandInfo> ExtendEdge;

	/** Sets the primary action to edit the edge's crease sharpness */
	TSharedPtr<FUICommandInfo> EditEdgeCreaseSharpness;

	/** Removes the selected edge if possible */
	TSharedPtr<FUICommandInfo> RemoveEdge;

	/** Soften edge */
	TSharedPtr<FUICommandInfo> SoftenEdge;

	/** Harden edge */
	TSharedPtr<FUICommandInfo> HardenEdge;

	/** Select edge loop */
	TSharedPtr<FUICommandInfo> SelectEdgeLoop;
};


// Actions that can be invoked from this mode when polygons are selected
class FMeshEditorPolygonCommands : public TCommands<FMeshEditorPolygonCommands>
{
public:
	FMeshEditorPolygonCommands() : TCommands<FMeshEditorPolygonCommands>
		(
			"MeshEditorPolygon",
			LOCTEXT("MeshEditorPolygon", "Mesh Editor Polygon"),
			"MeshEditorCommon",
			FMeshEditorStyle::GetStyleSetName()
			)
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;
	// End of TCommands<> interface

public:

	/** Sets the primary action to move polygons */
	TSharedPtr<FUICommandInfo> MovePolygon;

	/** Sets the primary action to extrude polygons */
	TSharedPtr<FUICommandInfo> ExtrudePolygon;

	/** Sets the primary action to freely extrude polygons */
	TSharedPtr<FUICommandInfo> FreelyExtrudePolygon;

	/** Sets the primary action to inset polygons */
	TSharedPtr<FUICommandInfo> InsetPolygon;

	/** Sets the primary action to bevel polygons */
	TSharedPtr<FUICommandInfo> BevelPolygon;

	/** Flips the currently selected polygon(s) */
	TSharedPtr<FUICommandInfo> FlipPolygon;

	/** Triangulates the currently selected polygon(s) */
	TSharedPtr<FUICommandInfo> TriangulatePolygon;

	/** Tessellates selected polygons into smaller polygons */
	TSharedPtr<FUICommandInfo> TessellatePolygon;

	/** Assigns the highlighted material to the currently selected polygon(s) */
	TSharedPtr<FUICommandInfo> AssignMaterial;
};

#undef LOCTEXT_NAMESPACE
