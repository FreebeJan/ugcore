// created by Sebastian Reiter
// s.b.reiter@googlemail.com
// y09 m08 d17

#ifndef __H__LIB_GRID__DISTRIBUTED_GRID__
#define __H__LIB_GRID__DISTRIBUTED_GRID__

#include <map>
#include <vector>
#include "lib_grid/lg_base.h"
#include "parallel_grid_layout.h"

namespace ug
{

/// \addtogroup lib_grid_parallelization
/// @{

///	the states with which elements are marked in ug::DistributedGridManager
/**	Note that the constants are directly related to the constants enumerated
 * in InterfaceNodeTypes.
 * Please also note that the constants are currently stored in bytes. No
 * value usig more thatn 8 bits is thus allowed.
 */
enum ElementStatusTypes
{
	ES_NONE = INT_NONE,
	ES_H_MASTER = INT_H_MASTER,
	ES_H_SLAVE = INT_H_SLAVE,
	ES_V_MASTER = INT_V_MASTER,
	ES_V_SLAVE = INT_V_SLAVE,

	//ES_GHOST = 1 << 5,//currently unused
	ES_SCHEDULED_FOR_INTERFACE = 1 << 6,
	ES_IN_INTERFACE = 1 << 7
};

///	manages the layouts and interfaces which are associated with a distributed grid.
/**
 * This class is work in progress. It not yet works in all situations.
 * The following things can be performed on a distributed grid:
 * 	- New elements can be created. If a vertex or an edge is created as
 * 		a child of an element that lies in an interface, it will be
 *		added to the corresponding interfaces.
 *		Note that you have to call begin_ordered_element_insertion() before
 *		and end_ordered_element_insertion() after you add elements.
 *	- Elements can be replaced. Interface entries are updated on the fly.
 *
 * The following things do not yet work properly:
 *	- Face-interfaces are not yet handled correctly during element-insertion.
 *	- When you erase elements, interfaces and layouts are not updated.
 *		That means if an interface element is deleted, a dangling pointer
 *		will remain in the interfaces - this may lead to bad errors later on.
 *		Methods that allow correct element removal have to be added somewhen soon.
 *		Until then you will have to update the grid_layout_map manually
 *		and inform the DistributedGridManager about changes by calling
 *		DistributedGridManager::grid_layouts_changed(false). 
 */
class DistributedGridManager : public GridObserver
{	
	public:
		DistributedGridManager();
		DistributedGridManager(MultiGrid& grid);
		virtual ~DistributedGridManager();
		
	//	assignment
		void assign(MultiGrid& grid);
			
		inline MultiGrid* get_assigned_grid()	{return m_pGrid;}
		inline const MultiGrid* get_assigned_grid()	const {return m_pGrid;}

	//	layout access
	/**	if you change the layout externally, be sure to call
	 *	DistributedGrid::layout_changed() afterwards.*/
		inline GridLayoutMap& grid_layout_map()				{return m_gridLayoutMap;}
		inline const GridLayoutMap& grid_layout_map() const	{return m_gridLayoutMap;}	
		
	///	call this method if you altered the layout externally.
	/**	This should be done as seldom as possible.
	 *	If you only added elements you may set addedElemsOnly to true.
	 *	The complexity in this case is proportional to the number of elements
	 *	in the layout.
	 *	If you removed elements or if you are unsure what operations have been
	 *	performed on the layout, you have to set addedElemsOnly to false
	 *	(the default value). Complexity in this case is proportional to the
	 *	number of elements in the underlying grid (or numer of elements in
	 *	the layout - whichever is higher).*/
	 	void grid_layouts_changed(bool addedElemsOnly = false);
		
	///	returns the status of the given object.
	/**	The status gives information about the type of interfaces in which a node
	 * lies. The returned value is an or combination of the constants enumerated
	 * in InterfaceNodeTypes and ElementStatusTypes.
	 * \sa contains_status
	 * \{
	 */
		byte get_status(GeometricObject* go) const;
		inline byte get_status(VertexBase* vrt)	const	{return elem_info(vrt).get_status();}
		inline byte get_status(EdgeBase* edge) const	{return elem_info(edge).get_status();}
		inline byte get_status(Face* face) const		{return elem_info(face).get_status();}
		inline byte get_status(Volume* vol) const		{return elem_info(vol).get_status();}
	/**	\} */

	///	returns true if the status of the given object contains the given status.
	/**	status can be an or-combination of constants enumerated in InterfaceNodeTypes
	 * and ElementStatusTypes.
	 */
		template <class TGeomObj>
		bool contains_status(TGeomObj* o, byte status) const	{return (get_status(o) & status) == status;}

	///	returns true if the element is a ghost
	/**	ghost elements are vertical masters that are in no other interfaces.
	 *	Those elements shouldn't be refined.*/
	 	template<class TElem>
		inline bool is_ghost(TElem* elem) const;

	//	element creation
	///	call this method before you start creating new elements in the associated grid.
	/** You shouldn't add new interfaces to the associated communication-set
	 *  between begin_ and end_element_creation.*/
		void begin_ordered_element_insertion();
		
	///	call this method when you're done with element creation.
	/**	Elements will not be added to the associated \sa GridCommunicationSet
	 *  until this method is called.*/
		void end_ordered_element_insertion();
		
	///	returns true if an element is in one or more interfaces
		template <class TElem>
		bool is_interface_element(TElem* elem);
		
	/**	returns a list of pairs (procID, index) that tells for each element
	 *	where in which interfaces it lies.*/
	 	template <class TElem>
	 	void collect_interface_entries(
						std::vector<std::pair<int, size_t> >& vEntriesOut,
						TElem* elem);


	///ONLY FOR DEBUG PURPOSES
		void enable_ordered_element_insertion(bool bEnable)	{m_bOrderedInsertionEnabled = bEnable;}
		
	////////////////////////////////
	//	grid callbacks
		virtual void grid_to_be_destroyed(Grid* grid);
		
	//	vertex callbacks
		virtual void vertex_created(Grid* grid, VertexBase* vrt,
									GeometricObject* pParent = NULL,
									bool replacesParent = false);

		virtual void edge_created(Grid* grid, EdgeBase* e,
									GeometricObject* pParent = NULL,
									bool replacesParent = false);
		
		virtual void face_created(Grid* grid, Face* f,
									GeometricObject* pParent = NULL,
									bool replacesParent = false);

		virtual void volume_created(Grid* grid, Volume* v,
									GeometricObject* pParent = NULL,
									bool replacesParent = false);

	protected:
	///	performs registration and deregistration at a grid.
	/**	call set_grid(NULL) to unregister the observer from a grid.*/
		void set_grid(Grid* grid);
		
		template <class TGeomObj>
		void reset_elem_infos();
		
		template <class TElem>
		void set_preliminary_ghost_states();

		void update_ghost_states();

		template <class TGeomObj, class TLayoutMap>
		void update_elem_info(TLayoutMap& layoutMap, int nodeType,
							  byte newStatus, bool addStatus = false);

		template <class TGeomObj>
		void update_all_elem_infos();
							  
	///	vertex_created, edge_created, ... callbacks call this method.
		template <class TElem>
		void handle_created_element(TElem* pElem, GeometricObject* pParent,
									bool replacesParent);
		
		template <class TElem, class TScheduledElemMap, class TParent>
		void schedule_element_for_insertion(TScheduledElemMap& elemMap,
												TElem* elem,
												TParent* pParent);

		void clear_scheduled_elements();
		
		template <class TScheduledElemMap>
		void perform_ordered_element_insertion(TScheduledElemMap& elemMap);
		
		template <class TElem>
		void add_element_to_interface(TElem* pElem, int procID);
			
	protected:
		template <class TGeomObj>
		class ElementInfo
		{
			public:
			//	types
				typedef typename GridLayoutMap::template Types<TGeomObj>
						::Interface		Interface;
				typedef typename Interface::iterator InterfaceElemIter;
				//typedef std::pair<Interface*, InterfaceElemIter> Entry;

				struct Entry{
					Entry()	{}
					Entry(Interface* intfc, InterfaceElemIter intfcElemIter, int intfcType) :
						m_interface(intfc), m_interfaceElemIter(intfcElemIter),
						m_interfaceType(intfcType)	{}

					Interface* 			m_interface;
					InterfaceElemIter	m_interfaceElemIter;
					int					m_interfaceType;
				};

				typedef std::list<Entry>				EntryList;
				typedef typename EntryList::iterator	EntryIterator;
				typedef typename EntryList::const_iterator	ConstEntryIterator;
				
			//	methods
				ElementInfo()	: m_status(ES_NONE)			{}
				
				void reset()								{m_status = ES_NONE; m_entries.clear();}
				
				void add_entry(Interface* interface,
								InterfaceElemIter iter,
								int intfcType)				{m_entries.push_back(Entry(interface, iter, intfcType));}
				
				void remove_entry(Interface* interface)		{m_entries.erase(find_entry(interface));}
				
				inline EntryIterator entries_begin()		{return m_entries.begin();}
				inline EntryIterator entries_end()			{return m_entries.end();}
				
				inline ConstEntryIterator entries_begin() const	{return m_entries.begin();}
				inline ConstEntryIterator entries_end() const	{return m_entries.end();}

				size_t get_local_id(EntryIterator iter) const	{return iter->m_interface->get_local_id(iter->m_interfaceElemIter);}
				size_t get_local_id(ConstEntryIterator iter) const	{return iter->m_interface->get_local_id(iter->m_interfaceElemIter);}
				int get_target_proc(EntryIterator iter) const	{return iter->m_interface->get_target_proc();}
				int get_target_proc(ConstEntryIterator iter) const	{return iter->m_interface->get_target_proc();}
				Interface* get_interface(EntryIterator iter)	{return iter->m_interface;}
				int get_interface_type(EntryIterator iter) const	{return iter->m_interfaceType;}
				int get_interface_type(ConstEntryIterator iter) const	{return iter->m_interfaceType;}

				EntryIterator find_entry(Interface* interface)
					{	for(EntryIterator iter = entries_begin(); iter != entries_end(); ++iter){
							if(iter->m_interface == interface)
								return iter;
						}
						return entries_end();
					}
				
				void set_status(byte status)				{m_status = status;}
				byte get_status() const						{return m_status;}
				
			protected:
				EntryList	m_entries;
				byte		m_status;
		};
		
		typedef ElementInfo<VertexBase>	ElemInfoVrt;
		typedef ElementInfo<EdgeBase>	ElemInfoEdge;
		typedef ElementInfo<Face>		ElemInfoFace;
		typedef ElementInfo<Volume>		ElemInfoVol;
		
		typedef Attachment<ElemInfoVrt>		AElemInfoVrt;
		typedef Attachment<ElemInfoEdge>	AElemInfoEdge;
		typedef Attachment<ElemInfoFace>	AElemInfoFace;
		typedef Attachment<ElemInfoVol>		AElemInfoVol;
		
	///	Used to schedule an element for insertion during ordered-insertion-mode.
	/**	For each interface in which the parent lies, an instance of
	 *	this class is added to scheduledElementMap of the parents type.
	 *	The local ID of the parent will be used as key.
	 *	The type of layout into which the element shall be inserted can
	 *	be retrieved from the status of the associated geomObj.*/
		struct ScheduledElement
		{
			ScheduledElement(GeometricObject* obj, int procID) :
				geomObj(obj), connectedProcID(procID)				{}

			GeometricObject*	geomObj;
			int					connectedProcID;
		};
		
		typedef std::multimap<size_t, ScheduledElement>	ScheduledElemMap;
							
	protected:
		inline ElemInfoVrt& elem_info(VertexBase* ele)	{return m_aaElemInfoVRT[ele];}
		inline ElemInfoEdge& elem_info(EdgeBase* ele)	{return m_aaElemInfoEDGE[ele];}
		inline ElemInfoFace& elem_info(Face* ele)		{return m_aaElemInfoFACE[ele];}
		inline ElemInfoVol& elem_info(Volume* ele)		{return m_aaElemInfoVOL[ele];}

		inline const ElemInfoVrt& elem_info(VertexBase* ele) const	{return m_aaElemInfoVRT[ele];}
		inline const ElemInfoEdge& elem_info(EdgeBase* ele) const	{return m_aaElemInfoEDGE[ele];}
		inline const ElemInfoFace& elem_info(Face* ele) const		{return m_aaElemInfoFACE[ele];}
		inline const ElemInfoVol& elem_info(Volume* ele) const		{return m_aaElemInfoVOL[ele];}

	protected:
		MultiGrid*		m_pGrid;
		GridLayoutMap	m_gridLayoutMap;
		
		bool m_bOrderedInsertionEnabled;///<only for debug purposes
		
		bool m_bOrderedInsertionMode;
		
		AElemInfoVrt	m_aElemInfoVrt;
		AElemInfoEdge	m_aElemInfoEdge;
		AElemInfoFace	m_aElemInfoFace;
		AElemInfoVol	m_aElemInfoVol;
		
		Grid::VertexAttachmentAccessor<AElemInfoVrt>	m_aaElemInfoVRT;
		Grid::EdgeAttachmentAccessor<AElemInfoEdge>		m_aaElemInfoEDGE;
		Grid::FaceAttachmentAccessor<AElemInfoFace>		m_aaElemInfoFACE;
		Grid::VolumeAttachmentAccessor<AElemInfoVol>	m_aaElemInfoVOL;
		
		ScheduledElemMap	m_vrtMap;	///< holds all elements that were scheduled by vertices
		ScheduledElemMap	m_edgeMap;	///< holds all elements that were scheduled by edges
		ScheduledElemMap	m_faceMap;	///< holds all elements that were scheduled by faces
		ScheduledElemMap	m_volMap;	///< holds all elements that were scheduled by volumes

};

/// @}

#ifdef __OLD_IMPLEMENTATION__
////////////////////////////////////////////////////////////////////////
///	Helps to create new interface-elements in the correct order.
/**
 * Between calls to begin_ordered_element_insertion()
 * and end_ordered_element_insertion(),
 * instances of this class will collect all created elements that have
 * been created from parent-elements that lie on an interface.
 * On end_element_creation() those elements will be added to interfaces
 * of the associated \sa GridCommunicationSet. The order in which those
 * elements are inserted is the same as the order that their parents
 * have in their interfaces.
 */
class DistributedGrid : public GridObserver
{		
	public:
		DistributedGrid();
		DistributedGrid(Grid& grid);
		
		virtual ~DistributedGridObserver();
		
	//	assignment
		void assign(Grid& grid);
			
	//	layout access
	/**	if you change the layout externally, be sure to call
	 *	DistributedGrid::layout_changed() afterwards.*/
		inline GridLayoutMap& grid_layout_map()				{return m_gridLayoutMap;}
		inline const GridLayoutMap& grid_layout_map() const	{return m_gridLayoutMap;}	
		
	///	call this method if you altered the layout externally.
	/**	This should be done as seldom as possible.
	 *	If you only added elements you may set addedElemsOnly to true.
	 *	The complexity in this case is proportional to the number of elements
	 *	in the layout.
	 *	If you removed elements or if you are unsure what operations have been
	 *	performed on the layout, you have to set addedElemsOnly to false
	 *	(the default value). Complexity in this case is proportional to the
	 *	number of elements in the underlying grid (or numer of elements in
	 *	the layout - whichever is higher).*/
	 	void grid_layouts_changed(bool addedElemsOnly = false);
		
	//	element creation
	///	call this method before you start creating new elements in the associated grid.
	/** You shouldn't add new interfaces to the associated communication-set
	 *  between begin_ and end_element_creation.*/
		void begin_ordered_element_insertion();
		
	///	call this method when you're done with element creation.
	/**	Elements will not be added to the associated \sa GridCommunicationSet
	 *  until this method is called.*/
		void end_ordered_element_insertion();
		
	//	element-status
		inline bool check_status(VertexBase* vrt, byte status)
			{return ((get_status(vrt) & status) == status);}
		
		inline bool check_status(EdgeBase* edge, byte status)
			{return ((get_status(edge) & status) == status);}

		inline bool check_status(Face* face, byte status)
			{return ((get_status(face) & status) == status);}

		inline bool check_status(Volume* vol, byte status)
			{return ((get_status(vol) & status) == status);}
			
		inline byte get_status(VertexBase* vrt)	{return elem_info(vrt).status;}
		inline byte get_status(EdgeBase* edge)	{return elem_info(edge).status;}
		inline byte get_status(Face* face)		{return elem_info(face).status;}
		inline byte get_status(Volume* vol)		{return elem_info(vol).status;}
		
	//	grid callbacks
		virtual void registered_at_grid(Grid* grid);
		virtual void unregistered_from_grid(Grid* grid);
		virtual void elements_to_be_cleared(Grid* grid);
		
	//	vertex callbacks
		virtual void vertex_created(Grid* grid, VertexBase* vrt, GeometricObject* pParent = NULL);
		virtual void vertex_to_be_erased(Grid* grid, VertexBase* vrt);
		virtual void vertex_to_be_replaced(Grid* grid, VertexBase* vrtOld, VertexBase* vrtNew);

	//	edge callbacks
		virtual void edge_created(Grid* grid, EdgeBase* edge, GeometricObject* pParent = NULL);
		virtual void edge_to_be_erased(Grid* grid, EdgeBase* edge);
		virtual void edge_to_be_replaced(Grid* grid, EdgeBase* edgeOld, EdgeBase* edgeNew);

	//	face callbacks
		virtual void face_created(Grid* grid, Face* face, GeometricObject* pParent = NULL)				{}
		virtual void face_to_be_erased(Grid* grid, Face* face)			{}
		virtual void face_to_be_replaced(Grid* grid, Face* faceOld, Face* faceNew)	{}

	//	volume callbacks
		virtual void volume_created(Grid* grid, Volume* vol, GeometricObject* pParent = NULL)			{}
		virtual void volume_to_be_erased(Grid* grid, Volume* vol)		{}
		virtual void volume_to_be_replaced(Grid* grid, Volume* volOld, Volume* volNew)	{}
		
	protected:
		
		class ScheduledElement
		{
			public:
				ScheduledElement()	{}
				ScheduledElement(GeometricObject* obj, InterfaceNodeTypes nt,
								int procID) : pObj(obj),
											nodeType(nt),
											connectedProcID(procID)	{}
				GeometricObject*	pObj;
				InterfaceNodeTypes	nodeType;
				int					connectedProcID;
		};
		
		typedef VertexLayout::Interface::iterator	InterfaceElemIteratorVrt;
		typedef EdgeLayout::Interface::iterator		InterfaceElemIteratorEdge;
		typedef FaceLayout::Interface::iterator		InterfaceElemIteratorFace;
		typedef VolumeLayout::Interface::iterator	InterfaceElemIteratorVol;
		
		typedef std::multimap<InterfaceElemIteratorVrt,
							ScheduledElement,
							VertexLayout::Interface::cmp> 	ScheduledElemMapVrt;
							
		typedef std::multimap<InterfaceElemIteratorEdge,
							ScheduledElement,
							EdgeLayout::Interface::cmp> 	ScheduledElemMapEdge;
							
		typedef std::multimap<InterfaceElemIteratorFace,
							ScheduledElement,
							FaceLayout::Interface::cmp> 	ScheduledElemMapFace;
							
		typedef std::multimap<InterfaceElemIteratorVol,
							ScheduledElement,
							VolumeLayout::Interface::cmp> 	ScheduledElemMapVol;

/*
		typedef VertexCommunicationGroup::HNODE HVertex;
		typedef EdgeCommunicationGroup::HNODE HEdge;
		typedef FaceCommunicationGroup::HNODE HFace;
		typedef VolumeCommunicationGroup::HNODE HVolume;
*/		
		template <class TGeomObj>
		struct ElementInfo
		{
		//	types
			typedef typename GridLayoutMap::template Types<TGeomObj>
					::Interface::iterator InterfaceElemIter;
					
			struct ProcIterPair{
				ProcIterPair(int pID, InterfaceElemIter i) :
					procID(pID), iter(i)	{}
					
				int					procID;
				InterfaceElemIter	iter;
			};

			typedef std::list<ProcIterPair>	ProcIterPairList;			

		//	methods
			ElementInfo()		{}
			inline void reset()	{lstProcIterPairs.clear(); status = ES_NONE;}
			
		//	members
		///	holds pairs of (procID, elem-iterator).
			ProcIterPairList	lstProcIterPairs;
		///	the status of the element
			byte 	status;
		};
			
		typedef ElementInfo<VertexBase>	ElemInfoVrt;
		typedef ElementInfo<EdgeBase>	ElemInfoEdge;
		typedef ElementInfo<Face>		ElemInfoFace;
		typedef ElementInfo<Volume>		ElemInfoVol;
		
		typedef util::Attachment<ElemInfoVrt>	AElemInfoVrt;
		typedef util::Attachment<ElemInfoEdge>	AElemInfoEdge;
		typedef util::Attachment<ElemInfoFace>	AElemInfoFace;
		typedef util::Attachment<ElemInfoVol>	AElemInfoVol;
		
	protected:	
		void clear_scheduled_elements();
		void resize_scheduled_element_map_vecs();
		
		inline ElemInfoVrt& elem_info(VertexBase* ele)	{return m_aaElemInfoVRT[ele];}
		inline ElemInfoEdge& elem_info(EdgeBase* ele)	{return m_aaElemInfoEDGE[ele];}
		inline ElemInfoFace& elem_info(Face* ele)		{return m_aaElemInfoFACE[ele];}
		inline ElemInfoVol& elem_info(Volume* ele)		{return m_aaElemInfoVOL[ele];}
		
		byte get_geom_obj_status(GeometricObject* go);
		
		inline void set_status(VertexBase* vrt, byte status){elem_info(vrt).status = status;}
		inline void set_status(EdgeBase* edge, byte status)	{elem_info(edge).status = status;}
		inline void set_status(Face* face, byte status)		{elem_info(face).status = status;}
		inline void set_status(Volume* vol, byte status)	{elem_info(vol).status = status;}
/*
		const std::vector<int>& get_interface_indices(GeometricObject* go);
		const std::vector<int>& get_interface_entry_indices(GeometricObject* go);
*/
		template <class TGeomObj>
		void reset_elem_infos();
		
		template <class TGeomObj, class TLayoutMap>
		void update_elem_info(TLayoutMap& layoutMap, int nodeType, byte newStatus);

	///	vertex_created, edge_created, ... callbacks call this method.
		template <class TElem>
		void handle_created_element(TElem* pElem,
									GeometricObject* pParent);
		
	///	vertex_to_be_erased, edge..., ... callbacks call this method.
		template <class TElem, class TCommGrp>
		void handle_erased_element(TElem* e, TCommGrp& commGrp);
	
	///	vertex_to_be_replaced, edge..., ... callbacks call this method.	
		template <class TElem, class TCommGrp>
		void handle_replaced_element(TElem* eOld, TElem* eNew,
									TCommGrp& commGrp);
									
		template <class TScheduledElemMap>
		void perform_ordered_element_insertion(TScheduledElemMap& elemMap);
		
		template <class TElem, class TCommGrp>
		void add_element_to_interface(TElem* pElem, TCommGrp& commGrp,
										int procID, InterfaceNodeTypes nodeType);
						
	protected:
		Grid*					m_pGrid;
		GridLayoutMap			m_gridLayoutMap;
		
		ScheduledElemMapVrt		m_vrtMap;	///< holds all elements that were scheduled by vertices
		ScheduledElemMapEdge	m_edgeMap;	///< holds all elements that were scheduled by edges
		ScheduledElemMapFace	m_faceMap;	///< holds all elements that were scheduled by faces
		ScheduledElemMapVol		m_volMap;	///< holds all elements that were scheduled by volumes
		
		bool	m_bOrderedInsertionMode;
		
		AElemInfoVrt	m_aElemInfoVrt;
		AElemInfoEdge	m_aElemInfoEdge;
		AElemInfoFace	m_aElemInfoFace;
		AElemInfoVol	m_aElemInfoVol;
		
		Grid::VertexAttachmentAccessor<AElemInfoVrt>	m_aaElemInfoVRT;
		Grid::EdgeAttachmentAccessor<AElemInfoEdge>		m_aaElemInfoEDGE;
		Grid::FaceAttachmentAccessor<AElemInfoFace>		m_aaElemInfoFACE;
		Grid::VolumeAttachmentAccessor<AElemInfoVol>	m_aaElemInfoVOL;
};

#endif //__OLD_IMPLEMENTATION__
}// end of namespace

////////////////////////////////
//	include implementation
#include "distributed_grid_impl.hpp"

#endif
