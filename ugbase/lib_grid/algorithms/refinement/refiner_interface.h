// created by Sebastian Reiter
// s.b.reiter@googlemail.com
// 24.01.2011 (m,d,y)

#ifndef __H__UG__REFINER_INTERFACE__
#define __H__UG__REFINER_INTERFACE__

#include "refinement_callbacks.h"

namespace ug
{
///	\addtogroup lib_grid_algorithms_refinement
///	@{

///	refinement-marks allow to specify how an element shall be processed during refinement.
//	Make sure not to use refinement marks with a value of 128 or higher! Those
enum RefinementMark{
	RM_NONE = 0,		///< no refinement is performed
	RM_REGULAR,			///< regular refinement is performed
	RM_ANISOTROPIC,		///< anisotropic refinement is performed
	RM_COARSEN,			///< the element is coarsened (only valid for adaptive multi-grid refinement)
	RM_MAX				///< the highest constant in RefinementMark. Should always be smaller than 128!
};

///	The refiner interface allows to mark elements for refinement and to call refine.
/**	A refiner always operates on a grid. A grid thus has to be assigned
 * before refinement starts. Please take a look at the specializations
 * of IRefiner, for more information.
 */
class IRefiner
{
	public:
		IRefiner(IRefinementCallback* refCallback = NULL) :
			m_refCallback(refCallback)	{}

		virtual ~IRefiner()	{}

		void set_refinement_callback(IRefinementCallback* refCallback)
			{m_refCallback = refCallback;}

		IRefinementCallback* get_refinement_callback()
			{return m_refCallback;}

	///	has to return the associated grid. Pure virtual
		virtual Grid* get_associated_grid() = 0;

	///	clears all marks. Default implementation is empty
		virtual void clear_marks()	{}

	///	Marks a element for refinement. Default implementation is empty
	/**	\{ */
		virtual bool mark(VertexBase* v, RefinementMark refMark = RM_REGULAR)	{return false;}
		virtual bool mark(EdgeBase* e, RefinementMark refMark = RM_REGULAR)		{return false;}
		virtual bool mark(Face* f, RefinementMark refMark = RM_REGULAR)			{return false;}
		virtual bool mark(Volume* v, RefinementMark refMark = RM_REGULAR)		{return false;}
	/**	\} */

	///	Returns the mark of a given element. Default returns RM_REGULAR
	/**	\{ */
		virtual RefinementMark get_mark(VertexBase* v)	{return RM_REGULAR;}
		virtual RefinementMark get_mark(EdgeBase* e)	{return RM_REGULAR;}
		virtual RefinementMark get_mark(Face* f)		{return RM_REGULAR;}
		virtual RefinementMark get_mark(Volume* v)		{return RM_REGULAR;}
	/**	\} */

	///	marks all elements between iterBegin and iterEnd.
	/**	the value-type of TIterator has to be a pointer to a type derived
	 * 	from either EdgeBase, Face or Volume.*/
		template <class TIterator>
		void mark(const TIterator& iterBegin, const TIterator& iterEnd,
				  RefinementMark refMark = RM_REGULAR)
			{
				TIterator iter = iterBegin;
				while(iter != iterEnd){
					mark(*iter, refMark);
					++iter;
				}
			}

	/// Performs refinement on the marked elements. Pure virtual.
		virtual void refine() = 0;

	///	Performs coarsening on the elements marked RM_COARSEN.
	/**	Note that coarsening is not supported by all refiners. Normally only
	 * MultiGrid-Refiner do support coarsening.
	 *
	 * coarsen returns false, if no elements have been coarsened, true if at
	 * least one has been coarsened.
	 *
	 * Since the default implementation does not perform coarsening, it returns false.
	 */
		virtual bool coarsen()		{return false;}

	protected:
		IRefinementCallback*	m_refCallback;
};

/// @}	// end of add_to_group command

}//	end of namespace

#endif
