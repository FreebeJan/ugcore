/*
 * mg_solver.cpp
 *
 *  Created on: 02.08.2011
 *      Author: andreasvogel
 */

#include "mg_solver.h"

namespace ug{


////////////////////////////////////////////////////////////////////////////////
// SelectNonShadowsAdjacentToShadowsOnLevel
////////////////////////////////////////////////////////////////////////////////

void SelectNonShadowsAdjacentToShadowsOnLevel(BoolMarker& sel,
                                              const SurfaceView& surfView,
                                              int level)
{
	PROFILE_FUNC_GROUP("gmg");
//	vectors for associated elements
	std::vector<VertexBase*> vAssVertex;
	std::vector<EdgeBase*> vAssEdge;
	std::vector<Face*> vAssFace;
	std::vector<Volume*> vAssVolume;

//	get grid
	Grid& grid = *sel.grid();

//	get multigrid
	MultiGrid& mg = *dynamic_cast<MultiGrid*>(&grid);

//	check multigrid
	if(&mg == NULL)
		UG_THROW("SelectNonShadowsAdjacentToShadowsOnLevel: No "
					"Multigrid given, selection ob level not possible.");

//	check level
	if(level >= (int) mg.num_levels() || level < 0)
		UG_THROW("SelectNonShadowsAdjacentToShadowsOnLevel: Requested "
						"level "<<level<<" does not exist in Multigrid.");

//	iterator type
	geometry_traits<VertexBase>::const_iterator iter, iterEnd;

	iterEnd = mg.end<VertexBase>(level);

//	loop all base elems
	for(iter = mg.begin<VertexBase>(level); iter != iterEnd; ++iter)
	{
	//	get element
		VertexBase* shadow = *iter;

	//	check if element is a shadow
		if(!surfView.is_shadowed(shadow)) continue;

	//	get adjacent elements
		CollectAssociated(vAssEdge, grid, shadow);

		vAssVertex.clear();
		for(size_t i = 0; i < vAssEdge.size(); ++i)
			CollectAssociated(vAssVertex, grid, vAssEdge[i], false);

		vAssEdge.clear();
		vAssFace.clear();
		vAssVolume.clear();
		for(size_t i = 0; i < vAssVertex.size(); ++i)
		{
			CollectAssociated(vAssEdge, grid, vAssVertex[i], false);
			CollectAssociated(vAssFace, grid, vAssVertex[i], false);
			CollectAssociated(vAssVolume, grid, vAssVertex[i], false);
		}

	//	select associated elements
		for(size_t i = 0; i < vAssVertex.size(); ++i)
			if(surfView.is_surface_element(vAssVertex[i]))
				sel.mark(vAssVertex[i]);
		for(size_t i = 0; i < vAssEdge.size(); ++i)
			if(surfView.is_surface_element(vAssEdge[i]))
				sel.mark(vAssEdge[i]);
		for(size_t i = 0; i < vAssFace.size(); ++i)
			if(surfView.is_surface_element(vAssFace[i]))
				sel.mark(vAssFace[i]);
		for(size_t i = 0; i < vAssVolume.size(); ++i)
			if(surfView.is_surface_element(vAssVolume[i]))
				sel.mark(vAssVolume[i]);
	}
}

} // end namespace ug
