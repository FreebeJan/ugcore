/*
 * periodic_boundary_manager.cpp
 *
 *  Created on: 05.12.2012
 *      Author: marscher
 */
#include "periodic_boundary_manager.h"

namespace ug {

PeriodicBoundaryManager::PeriodicBoundaryManager() :
				m_pGrid(NULL)/*, m_pSH(NULL)*/ {}

PeriodicBoundaryManager::~PeriodicBoundaryManager()
{
	// set grid to NULL to detach groups from grid
	set_grid(NULL);
}

void PeriodicBoundaryManager::set_grid(Grid* g)
{
	// group attachments
	Attachment<Group<Vertex>*> aGroupVRT;
	Attachment<Group<EdgeBase>*> aGroupEDG;
	Attachment<Group<Face>*> aGroupFCE;

	// periodic info attachments
	Attachment<PeriodicStatus> aPeriodicStatus;

	// detach groups and unregister observer
	if(m_pGrid) {
		m_pGrid->detach_from_vertices(aGroupVRT);
		m_pGrid->detach_from_edges(aGroupEDG);
		m_pGrid->detach_from_faces(aGroupFCE);

		m_pGrid->detach_from_vertices(aPeriodicStatus);
		m_pGrid->detach_from_edges(aPeriodicStatus);
		m_pGrid->detach_from_faces(aPeriodicStatus);

		m_pGrid->unregister_observer(this);
	}

	m_pGrid = dynamic_cast<MultiGrid*>(g);

	// attach groups and register observer
	if(m_pGrid != NULL) {
		m_pGrid->attach_to_vertices_dv(aGroupVRT, NULL);
		m_pGrid->attach_to_edges_dv(aGroupEDG, NULL);
		m_pGrid->attach_to_faces_dv(aGroupFCE, NULL);

		m_pGrid->attach_to_vertices_dv(aPeriodicStatus, P_NOT_PERIODIC);
		m_pGrid->attach_to_edges_dv(aPeriodicStatus, P_NOT_PERIODIC);
		m_pGrid->attach_to_faces_dv(aPeriodicStatus, P_NOT_PERIODIC);

		// access grid with those attachments
		m_aaGroupVRT.access(*m_pGrid, aGroupVRT);
		m_aaGroupEDG.access(*m_pGrid, aGroupEDG);
		m_aaGroupFCE.access(*m_pGrid, aGroupFCE);

		m_aaPeriodicStatusVRT.access(*m_pGrid, aPeriodicStatus);
		m_aaPeriodicStatusEDG.access(*m_pGrid, aPeriodicStatus);
		m_aaPeriodicStatusFCE.access(*m_pGrid, aPeriodicStatus);

		uint options = OT_GRID_OBSERVER | OT_VERTEX_OBSERVER | OT_EDGE_OBSERVER |
				OT_FACE_OBSERVER;
		m_pGrid->register_observer(this, options);
	}
}

Grid* PeriodicBoundaryManager::get_grid() const {
	return m_pGrid;
}

//void PeriodicBoundaryManager::set_subset_handler(ISubsetHandler* sh) {
//	m_pSH = sh;
//	// very small memory overhead buys constant access time in match() methods
//	m_vIdentifier.resize(m_pSH->num_subsets());
//}

// group accessors
template <>
Grid::AttachmentAccessor<Vertex, Attachment<PeriodicBoundaryManager::Group<Vertex>* > >&
PeriodicBoundaryManager::get_group_accessor() {
	return m_aaGroupVRT;
}

template <>
Grid::AttachmentAccessor<EdgeBase, Attachment<PeriodicBoundaryManager::Group<EdgeBase>* > >&
PeriodicBoundaryManager::get_group_accessor() {
	return m_aaGroupEDG;
}

template <>
Grid::AttachmentAccessor<Face, Attachment<PeriodicBoundaryManager::Group<Face>* > >&
PeriodicBoundaryManager::get_group_accessor() {
	return m_aaGroupFCE;
}

template <>
const Grid::AttachmentAccessor<Vertex, Attachment<PeriodicBoundaryManager::Group<Vertex>* > >&
PeriodicBoundaryManager::get_group_accessor() const {
	return m_aaGroupVRT;
}

template <>
const Grid::AttachmentAccessor<EdgeBase, Attachment<PeriodicBoundaryManager::Group<EdgeBase>* > >&
PeriodicBoundaryManager::get_group_accessor() const {
	return m_aaGroupEDG;
}

template <>
const Grid::AttachmentAccessor<Face, Attachment<PeriodicBoundaryManager::Group<Face>* > >&
PeriodicBoundaryManager::get_group_accessor() const {
	return m_aaGroupFCE;
}

template <>
const Grid::AttachmentAccessor<Volume, Attachment<PeriodicBoundaryManager::Group<Volume>* > >&
PeriodicBoundaryManager::get_group_accessor() const {
	UG_THROW("not impled");
}

// periodic status accessors
template <>
const Grid::AttachmentAccessor<Vertex, Attachment<PeriodicBoundaryManager::PeriodicStatus> >&
PeriodicBoundaryManager::get_periodic_status_accessor() const {
	return m_aaPeriodicStatusVRT;
}

template <>
Grid::AttachmentAccessor<Vertex, Attachment<PeriodicBoundaryManager::PeriodicStatus> >&
PeriodicBoundaryManager::get_periodic_status_accessor() {
	return m_aaPeriodicStatusVRT;
}

template <>
const Grid::AttachmentAccessor<EdgeBase, Attachment<PeriodicBoundaryManager::PeriodicStatus> >&
PeriodicBoundaryManager::get_periodic_status_accessor() const {
	return m_aaPeriodicStatusEDG;
}

template <>
Grid::AttachmentAccessor<EdgeBase, Attachment<PeriodicBoundaryManager::PeriodicStatus> >&
PeriodicBoundaryManager::get_periodic_status_accessor() {
	return m_aaPeriodicStatusEDG;
}

template <>
Grid::AttachmentAccessor<Face, Attachment<PeriodicBoundaryManager::PeriodicStatus> >&
PeriodicBoundaryManager::get_periodic_status_accessor() {
	return m_aaPeriodicStatusFCE;
}

template <>
const Grid::AttachmentAccessor<Face, Attachment<PeriodicBoundaryManager::PeriodicStatus> >&
PeriodicBoundaryManager::get_periodic_status_accessor() const {
	return m_aaPeriodicStatusFCE;
}

template <>
Grid::AttachmentAccessor<Volume, Attachment<PeriodicBoundaryManager::PeriodicStatus> >&
PeriodicBoundaryManager::get_periodic_status_accessor() {
	UG_THROW("not impled");
}

template <>
const Grid::AttachmentAccessor<Volume, Attachment<PeriodicBoundaryManager::PeriodicStatus> >&
PeriodicBoundaryManager::get_periodic_status_accessor() const {
	UG_THROW("not impled");
}

///// grid observer methods
void PeriodicBoundaryManager::grid_to_be_destroyed(Grid* grid) {

	/// delete groups
	for (VertexIterator iter = m_pGrid->begin<Vertex>();
			iter != m_pGrid->end<Vertex>(); ++iter) {
		if(is_master(*iter)) delete m_aaGroupVRT[*iter];
	}

	for (EdgeBaseIterator iter = m_pGrid->begin<EdgeBase>();
			iter != m_pGrid->end<EdgeBase>(); ++iter) {
		if(is_master(*iter)) delete m_aaGroupEDG[*iter];
	}

	for (FaceIterator iter = m_pGrid->begin<Face>();
			iter != m_pGrid->end<Face>(); ++iter) {
		if(is_master(*iter)) delete m_aaGroupFCE[*iter];
	}

	set_grid(NULL);
}

void PeriodicBoundaryManager::vertex_created(Grid* grid, Vertex* vrt,
		GridObject* pParent, bool replacesParent) {
	handle_creation_cast_wrapper(vrt, pParent, replacesParent);
}

void PeriodicBoundaryManager::edge_created(Grid* grid, EdgeBase* e, GridObject* pParent,
		bool replacesParent) {
	handle_creation_cast_wrapper(e, pParent, replacesParent);
}

void PeriodicBoundaryManager::face_created(Grid* grid, Face* f, GridObject* pParent,
		bool replacesParent) {
	handle_creation_cast_wrapper(f, pParent, replacesParent);
}

void PeriodicBoundaryManager::vertex_to_be_erased(Grid* grid, Vertex* vrt,
		Vertex* replacedBy) {
	handle_deletion(vrt, replacedBy);
}

void PeriodicBoundaryManager::edge_to_be_erased(Grid* grid, EdgeBase* e,
		EdgeBase* replacedBy) {
	handle_deletion(e, replacedBy);
}

void PeriodicBoundaryManager::face_to_be_erased(Grid* grid, Face* f,
		Face* replacedBy) {
	handle_deletion(f, replacedBy);
}

/**
 * performs following checks on all elements in goc1 and goc2
 * 1. all elements given are periodic
 * 2. masters of groups are valid (master pointer of group valid + have at least one child)
 * 3. no duplicate master, slave pairs exists
 * 4.
 */
bool PeriodicBoundaryManager::check_periodicity(
		const GridObjectCollection& goc1,
		const GridObjectCollection& goc2, ISubsetHandler* sh) {

	Group<Vertex>::unique_pairs s_vert;
	Group<EdgeBase>::unique_pairs s_edge;
	Group<Face>::unique_pairs s_face;

	UG_ASSERT(goc1.num_levels() == goc2.num_levels(),
			"collections have different mg levels!")

	for (size_t lvl = 0; lvl < goc1.num_levels(); lvl++) {
		// check faces
		check_elements_periodicity<Face>(goc1.begin<Face>(lvl),
				goc1.end<Face>(lvl), s_face, sh);
		check_elements_periodicity<Face>(goc2.begin<Face>(lvl),
				goc2.end<Face>(lvl), s_face, sh);

		// check edges
		check_elements_periodicity<EdgeBase>(goc1.begin<EdgeBase>(lvl),
				goc1.end<EdgeBase>(lvl), s_edge, sh);
		check_elements_periodicity<EdgeBase>(goc2.begin<EdgeBase>(lvl),
				goc2.end<EdgeBase>(lvl), s_edge, sh);

		// check vertices
		check_elements_periodicity<Vertex>(goc1.begin<Vertex>(lvl),
				goc1.end<Vertex>(lvl), s_vert, sh);
		check_elements_periodicity<Vertex>(goc2.begin<Vertex>(lvl),
				goc2.end<Vertex>(lvl), s_vert, sh);
	}

	return true;
}

//void PeriodicBoundaryManager::set_identifier(SmartPtr<IIdentifier> i, size_t si) {
//	UG_ASSERT(m_vIdentifier.capacity() >= si, "identifier vector not big enough")
//	m_vIdentifier[si] = i;
//}

} // end of namespace ug
