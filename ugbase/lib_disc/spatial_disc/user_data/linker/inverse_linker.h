/*
 * Copyright (c) 2013-2015:  G-CSC, Goethe University Frankfurt
 * Authors: Ivo Muha, Andreas Vogel
 * 
 * This file is part of UG4.
 * 
 * UG4 is free software: you can redistribute it and/or modify it under the
 * terms of the GNU Lesser General Public License version 3 (as published by the
 * Free Software Foundation) with the following additional attribution
 * requirements (according to LGPL/GPL v3 §7):
 * 
 * (1) The following notice must be displayed in the Appropriate Legal Notices
 * of covered and combined works: "Based on UG4 (www.ug4.org/license)".
 * 
 * (2) The following notice must be displayed at a prominent place in the
 * terminal output of covered works: "Based on UG4 (www.ug4.org/license)".
 * 
 * (3) The following bibliography is recommended for citation and must be
 * preserved in all covered files:
 * "Reiter, S., Vogel, A., Heppner, I., Rupp, M., and Wittum, G. A massively
 *   parallel geometric multigrid solver on hierarchically distributed grids.
 *   Computing and visualization in science 16, 4 (2013), 151-164"
 * "Vogel, A., Reiter, S., Rupp, M., Nägel, A., and Wittum, G. UG4 -- a novel
 *   flexible software system for simulating pde based models on high performance
 *   computers. Computing and visualization in science 16, 4 (2013), 165-179"
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 */

/*
 * andreasvogel used scale_add_linker ass template
 */

#ifndef __H__UG__LIB_DISC__SPATIAL_DISC__INVERSE_LINKER__
#define __H__UG__LIB_DISC__SPATIAL_DISC__INVERSE_LINKER__

#include "linker.h"

namespace ug{


////////////////////////////////////////////////////////////////////////////////
// Scaled adding of Data
////////////////////////////////////////////////////////////////////////////////

/**
 * This linker recombines the data like
 *
 * l = a / b
 *
 * \tparam		dim			world dimension
 * \tparam		TDataScale 	type of scaling data
 */
template <int dim>
class InverseLinker
	: public StdDataLinker<InverseLinker<dim>, number, dim>
{
	public:
	//	type of base class
		typedef StdDataLinker<InverseLinker<dim>, number, dim> base_type;

	public:
	///	constructor
		InverseLinker() {}

	///	constructor
		InverseLinker(const InverseLinker& linker);

		void divide(SmartPtr<CplUserData<number, dim> > dividend,
		         SmartPtr<CplUserData<number, dim> > divisor);
		void divide(number dividend,
		         SmartPtr<CplUserData<number, dim> > divisor);
		void divide(SmartPtr<CplUserData<number, dim> > dividend,
		         number divisor);
		void divide(number dividend,
		         number divisor);

		inline void evaluate (number& value,
		                      const MathVector<dim>& globIP,
		                      number time, int si) const;

		template <int refDim>
		inline void evaluate(number vValue[],
		                     const MathVector<dim> vGlobIP[],
		                     number time, int si,
		                     GridObject* elem,
		                     const MathVector<dim> vCornerCoords[],
		                     const MathVector<refDim> vLocIP[],
		                     const size_t nip,
		                     LocalVector* u,
		                     const MathMatrix<refDim, dim>* vJT = NULL) const;

		template <int refDim>
		void eval_and_deriv(number vValue[],
		                    const MathVector<dim> vGlobIP[],
		                    number time, int si,
		                    GridObject* elem,
		                    const MathVector<dim> vCornerCoords[],
		                    const MathVector<refDim> vLocIP[],
		                    const size_t nip,
		                    LocalVector* u,
		                    bool bDeriv,
		                    int s,
		                    std::vector<std::vector<number> > vvvDeriv[],
		                    const MathMatrix<refDim, dim>* vJT = NULL) const;

	protected:
	///	divisor at ip of input
		const number& divisor_value(size_t i, size_t s, size_t ip) const
		{
			UG_ASSERT(i < m_vpDivisorData.size(), "Input not needed");
			UG_ASSERT(m_vpDivisorData[i].valid(), "Input invalid");
			return m_vpDivisorData[i]->value(this->series_id(2*i,s), ip);
		}

	///	divisor of data at input at ip
		const number& divisor_deriv(size_t i, size_t s, size_t ip, size_t fct, size_t dof) const
		{
			UG_ASSERT(i < m_vpDependData.size(), "Input not needed");
			UG_ASSERT(m_vpDependData[i].valid(), "Input invalid");
			return m_vpDependData[i]->deriv(this->series_id(2*i,s), ip, fct, dof);
		}

	///	dividend at ip of input
		const number& dividend_value(size_t i, size_t s, size_t ip) const
		{
			UG_ASSERT(i < m_vpDividendData.size(), "Input not needed");
			UG_ASSERT(m_vpDividendData[i].valid(), "Input invalid");
			return m_vpDividendData[i]->value(this->series_id(2*i+1,s), ip);
		}

	///	derivative of dividend at input at ip
		const number& dividend_deriv(size_t i, size_t s, size_t ip, size_t fct, size_t dof) const
		{
			UG_ASSERT(i < m_vpDividendDependData.size(), "Input not needed");
			UG_ASSERT(m_vpDividendDependData[i].valid(), "Input invalid");
			return m_vpDividendDependData[i]->deriv(this->series_id(2*i+1,s), ip, fct, dof);
		}

	///	returns number of functions the divisor depends on
		size_t divisor_num_fct(size_t i) const {return base_type::input_num_fct(2*i);}

	///	returns the number in the common FctGrp for a fct of an divisor
		size_t divisor_common_fct(size_t i, size_t fct) const	{return base_type::input_common_fct(2*i, fct);}

	///	returns number of functions the dividend depends on
		size_t dividend_num_fct(size_t i) const {return base_type::input_num_fct(2*i+1);}

	///	returns the number in the common FctGrp for a fct of a dividend
		size_t dividend_common_fct(size_t i, size_t fct) const	{return base_type::input_common_fct(2*i+1, fct);}

	protected:
	///	data Dividend
		std::vector<SmartPtr<CplUserData<number, dim> > > m_vpDividendData;

	///	data Dividend casted to dependend data
		std::vector<SmartPtr<DependentUserData<number, dim> > > m_vpDividendDependData;

	///	data Divisor
		std::vector<SmartPtr<CplUserData<number, dim> > > m_vpDivisorData;

	///	data Divisor casted to dependend data
		std::vector<SmartPtr<DependentUserData<number, dim> > > m_vpDependData;
};

} // end namespace ug

#include "inverse_linker_impl.h"

#endif /* __H__UG__LIB_DISC__SPATIAL_DISC__INVERSE_LINKER__ */
