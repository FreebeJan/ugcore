/*
 * lagrangep1.h
 *
 *  Created on: 16.02.2010
 *      Author: andreasvogel
 */


#include "./lagrangep1.h"

namespace ug{

/// \cond HIDDEN_SYMBOLS

///////////////////////////////////////
// ReferenceEdge
///////////////////////////////////////

template<>
LagrangeP1<ReferenceEdge,1>::shape_type
LagrangeP1<ReferenceEdge,1>::
shape(size_t i, const position_type& x) const
{
	switch(i)
	{
	case 0: return (1.-x[0]);
	case 1: return x[0];
	default: throw(UG_ERROR_InvalidShapeFunctionIndex(i));
	}
};

template<>
void
LagrangeP1<ReferenceEdge,1>::
evaluate_grad(size_t i, const position_type& x, grad_type& value) const
{
	switch(i)
	{
	case 0: value[0] = -1.0; break;
	case 1: value[0] = 	1.0; break;
	default: throw(UG_ERROR_InvalidShapeFunctionIndex(i));
	}
}

template<>
bool LagrangeP1<ReferenceEdge,1>::
position(size_t i, position_type& value) const
{
	switch(i)
	{
	case 0: value[0] =  0.0; return true;
	case 1: value[0] = 	1.0; return true;
	default: throw(UG_ERROR_InvalidShapeFunctionIndex(i));
	}
	return true;
}

///////////////////////////////////////
// ReferenceTriangle
///////////////////////////////////////

template<>
LagrangeP1<ReferenceTriangle,1>::shape_type
LagrangeP1<ReferenceTriangle,1>::
shape(size_t i, const position_type& x) const
{
	switch (i)
	{
	case 0: return(1.0-x[0]-x[1]);
	case 1: return(x[0]);
	case 2: return(x[1]);
	default: throw(UG_ERROR_InvalidShapeFunctionIndex(i));
	}
};

template<>
void
LagrangeP1<ReferenceTriangle,1>::
evaluate_grad(size_t i, const position_type& x, grad_type& value) const
{
	switch(i)
	{
	case 0: value[0] = -1.0;
			value[1] = -1.0; break;
	case 1: value[0] = 	1.0;
			value[1] =  0.0; break;
	case 2: value[0] =  0.0;
			value[1] =  1.0; break;
	default: throw(UG_ERROR_InvalidShapeFunctionIndex(i));
	}
}

template<>
bool LagrangeP1<ReferenceTriangle,1>::
position(size_t i, position_type& value) const
{
	switch(i)
	{
	case 0: value[0] =  0.0;
			value[1] =  0.0; return true;
	case 1: value[0] = 	1.0;
			value[1] =  0.0; return true;
	case 2: value[0] =  0.0;
			value[1] =  1.0; return true;
	default: throw(UG_ERROR_InvalidShapeFunctionIndex(i));
	}
	return true;
}

///////////////////////////////////////
// ReferenceQuadrilateral
///////////////////////////////////////

template<>
LagrangeP1<ReferenceQuadrilateral,1>::shape_type
LagrangeP1<ReferenceQuadrilateral,1>::
shape(size_t i, const position_type& x) const
{
	switch(i)
	{
	case 0: return((1.0-x[0])*(1.0-x[1]));
	case 1: return(x[0]*(1.0-x[1]));
	case 2: return(x[0]*x[1]);
	case 3: return((1.0-x[0])*x[1]);
	default: throw(UG_ERROR_InvalidShapeFunctionIndex(i));
	}
};

template<>
void
LagrangeP1<ReferenceQuadrilateral,1>::
evaluate_grad(size_t i, const position_type& x, grad_type& value) const
{
	switch(i)
	{
	case 0: value[0] = -1.0 + x[1];
			value[1] = -1.0 + x[0]; break;
	case 1: value[0] = 	1.0 - x[1];
			value[1] =      - x[0]; break;
	case 2: value[0] =        x[1];
			value[1] =        x[0]; break;
	case 3: value[0] =      - x[1];
			value[1] =  1.0 - x[0]; break;
	default: throw(UG_ERROR_InvalidShapeFunctionIndex(i));
	}
}

template<>
bool
LagrangeP1<ReferenceQuadrilateral,1>::
position(size_t i, position_type& value) const
{
	switch(i)
	{
	case 0: value[0] =  0.0;
			value[1] =  0.0; return true;
	case 1: value[0] = 	1.0;
			value[1] =  0.0; return true;
	case 2: value[0] =  1.0;
			value[1] =  1.0; return true;
	case 3: value[0] =  0.0;
			value[1] =  1.0; return true;
	default: throw(UG_ERROR_InvalidShapeFunctionIndex(i));
	}
	return true;
}

///////////////////////////////////////
// ReferenceTetrahedron
///////////////////////////////////////

template<>
LagrangeP1<ReferenceTetrahedron,1>::shape_type
LagrangeP1<ReferenceTetrahedron,1>::
shape(size_t i, const position_type& x) const
{
	switch(i)
	{
	case 0: return(1.0-x[0]-x[1]-x[2]);
	case 1: return(x[0]);
	case 2: return(x[1]);
	case 3: return(x[2]);
	default: throw(UG_ERROR_InvalidShapeFunctionIndex(i));
	}
};

template<>
void
LagrangeP1<ReferenceTetrahedron,1>::
evaluate_grad(size_t i, const position_type& x, grad_type& value) const
{
	switch(i)
	{
	case 0: value[0] = -1.0;
			value[1] = -1.0;
			value[2] = -1.0; break;
	case 1: value[0] = 	1.0;
			value[1] =  0.0;
			value[2] =  0.0; break;
	case 2: value[0] =  0.0;
			value[1] =  1.0;
			value[2] =  0.0; break;
	case 3: value[0] =  0.0;
			value[1] =  0.0;
			value[2] =  1.0; break;
	default: throw(UG_ERROR_InvalidShapeFunctionIndex(i));
	}
}

template<>
bool LagrangeP1<ReferenceTetrahedron,1>::
position(size_t i, position_type& value) const
{
	switch(i)
	{
	case 0: value[0] =  0.0;
			value[1] =  0.0;
			value[2] =  0.0; return true;
	case 1: value[0] = 	1.0;
			value[1] =  0.0;
			value[2] =  0.0; return true;
	case 2: value[0] =  0.0;
			value[1] =  1.0;
			value[2] =  0.0; return true;
	case 3: value[0] =  0.0;
			value[1] =  0.0;
			value[2] =  1.0; return true;
	default: throw(UG_ERROR_InvalidShapeFunctionIndex(i));
	}
	return true;
}

///////////////////////////////////////
// ReferencePyramid
///////////////////////////////////////

template<>
LagrangeP1<ReferencePyramid,1>::shape_type
LagrangeP1<ReferencePyramid,1>::
shape(size_t i, const position_type& x) const
{
	switch(i)
	{
	  case 0 :
		if (x[0] > x[1])
		  return((1.0-x[0])*(1.0-x[1]) + x[2]*(x[1]-1.0));
		else
		  return((1.0-x[0])*(1.0-x[1]) + x[2]*(x[0]-1.0));
	  case 1 :
		if (x[0] > x[1])
		  return(x[0]*(1.0-x[1])       - x[2]*x[1]);
		else
		  return(x[0]*(1.0-x[1])       - x[2]*x[0]);
	  case 2 :
		if (x[0] > x[1])
		  return(x[0]*x[1]             + x[2]*x[1]);
		else
		  return(x[0]*x[1]             + x[2]*x[0]);
	  case 3 :
		if (x[0] > x[1])
		  return((1.0-x[0])*x[1]       - x[2]*x[1]);
		else
		  return((1.0-x[0])*x[1]       - x[2]*x[0]);
	  case 4 : return(x[2]);
	default: throw(UG_ERROR_InvalidShapeFunctionIndex(i));
	}
};

template<>
void
LagrangeP1<ReferencePyramid,1>::
evaluate_grad(size_t i, const position_type& x, grad_type& value) const
{
	switch(i)
	{
	  case 0:
		if (x[0] > x[1])
		  {
			value[0] = -(1.0-x[1]);
			value[1] = -(1.0-x[0]) + x[2];
			value[2] = -(1.0-x[1]);
			break;
		  }
		else
		  {
			value[0] = -(1.0-x[1]) + x[2];
			value[1] = -(1.0-x[0]);
			value[2] = -(1.0-x[0]);
			break;
		  }
	  case 1:
		if (x[0] > x[1])
		  {
			value[0] = (1.0-x[1]);
			value[1] = -x[0] - x[2];
			value[2] = -x[1];
			break;
		  }
		else
		  {
			value[0] = (1.0-x[1]) - x[2];
			value[1] = -x[0];
			value[2] = -x[0];
			break;
		  }
	  case 2:
		if (x[0] > x[1])
		  {
			value[0] = x[1];
			value[1] = x[0] + x[2];
			value[2] = x[1];
			break;
		  }
		else
		  {
			value[0] = x[1] + x[2];
			value[1] = x[0];
			value[2] = x[0];
			break;
		  }
	  case 3:
		if (x[0] > x[1])
		  {
			value[0] = -x[1];
			value[1] = 1.0-x[0] - x[2];
			value[2] = -x[1];
			break;
		  }
		else
		  {
			value[0] = -x[1] - x[2];
			value[1] = 1.0-x[0];
			value[2] = -x[0];
			break;
		  }
      case 4:
		value[0] = 0.0;
		value[1] = 0.0;
		value[2] = 1.0;
		break;
	default: throw(UG_ERROR_InvalidShapeFunctionIndex(i));
	}
}

template<>
bool LagrangeP1<ReferencePyramid,1>::
position(size_t i, position_type& value) const
{
	static const DimReferenceElement<3>& refElem
		= DimReferenceElementFactory<3>::get_reference_element(ROID_PYRAMID);

	value = refElem.corner(i);
	return true;
}

///////////////////////////////////////
// ReferencePrism
///////////////////////////////////////

template<>
LagrangeP1<ReferencePrism,1>::shape_type
LagrangeP1<ReferencePrism,1>::
shape(size_t i, const position_type& x) const
{
	switch(i)
	{
	case 0 : return((1.0-x[0]-x[1])*(1.0-x[2]));
	case 1 : return(x[0]*(1.0-x[2]));
	case 2 : return(x[1]*(1.0-x[2]));
	case 3 : return((1.0-x[0]-x[1])*x[2]);
	case 4 : return(x[0]*x[2]);
	case 5 : return(x[1]*x[2]);
	default: throw(UG_ERROR_InvalidShapeFunctionIndex(i));
	}
};

template<>
void
LagrangeP1<ReferencePrism,1>::
evaluate_grad(size_t i, const position_type& x, grad_type& value) const
{
	switch(i)
	{
	  case 0:
		value[0] = -(1.0-x[2]);
		value[1] = -(1.0-x[2]);
		value[2] = -(1.0-x[0]-x[1]);
		break;
	  case 1:
		value[0] = (1.0-x[2]);
		value[1] = 0.0;
		value[2] = -x[0];
		break;
	  case 2:
		value[0] = 0.0;
		value[1] = (1.0-x[2]);
		value[2] = -x[1];
		break;
	  case 3:
		value[0] = -x[2];
		value[1] = -x[2];
		value[2] = 1.0-x[0]-x[1];
      break;
    case 4:
		value[0] = x[2];
		value[1] = 0.0;
		value[2] = x[0];
		break;
    case 5:
		value[0] = 0.0;
		value[1] = x[2];
		value[2] = x[1];
		break;
	default: throw(UG_ERROR_InvalidShapeFunctionIndex(i));
	}
}

template<>
bool LagrangeP1<ReferencePrism,1>::
position(size_t i, position_type& value) const
{
	static const DimReferenceElement<3>& refElem
		= DimReferenceElementFactory<3>::get_reference_element(ROID_PRISM);

	value = refElem.corner(i);
	return true;
}

///////////////////////////////////////
// ReferenceHexahedron
///////////////////////////////////////

template<>
LagrangeP1<ReferenceHexahedron,1>::shape_type
LagrangeP1<ReferenceHexahedron,1>::
shape(size_t i, const position_type& x) const
{
	switch(i)
	{
	case 0: return((1.0-x[0])*(1.0-x[1])*(1.0-x[2]));
	case 1: return((x[0])*(1.0-x[1])*(1.0-x[2]));
	case 2: return((x[0])*(x[1])*(1.0-x[2]));
	case 3: return((1.0-x[0])*(x[1])*(1.0-x[2]));
	case 4: return((1.0-x[0])*(1.0-x[1])*(x[2]));
	case 5: return((x[0])*(1.0-x[1])*(x[2]));
	case 6: return((x[0])*(x[1])*(x[2]));
	case 7: return((1.0-x[0])*(x[1])*(x[2]));
	default: throw(UG_ERROR_InvalidShapeFunctionIndex(i));
	}
};

template<>
void
LagrangeP1<ReferenceHexahedron,1>::
evaluate_grad(size_t i, const position_type& x, grad_type& value) const
{
	switch(i)
	{
	  case 0:
		value[0] = -(1.0-x[1])*(1.0-x[2]);
		value[1] = -(1.0-x[0])*(1.0-x[2]);
		value[2] = -(1.0-x[0])*(1.0-x[1]);
		break;
	  case 1:
		value[0] = (1.0-x[1])*(1.0-x[2]);
		value[1] = -(x[0])*(1.0-x[2]);
		value[2] = -(x[0])*(1.0-x[1]);
		break;
	  case 2:
		value[0] = (x[1])*(1.0-x[2]);
		value[1] = (x[0])*(1.0-x[2]);
		value[2] = -x[0]*x[1];
		break;
	  case 3:
		value[0] = -(x[1])*(1.0-x[2]);
		value[1] = (1.0-x[0])*(1.0-x[2]);
		value[2] = -(1.0-x[0])*(x[1]);
      break;
    case 4:
		value[0] = -(1.0-x[1])*(x[2]);
		value[1] = -(1.0-x[0])*(x[2]);
		value[2] = (1.0-x[0])*(1.0-x[1]);
		break;
	  case 5:
		value[0] = (1.0-x[1])*x[2];
		value[1] = -(x[0])*x[2];
		value[2] = (x[0])*(1.0-x[1]);
		break;
	  case 6:
		value[0] = (x[1])*x[2];
		value[1] = (x[0])*x[2];
		value[2] = x[0]*x[1];
		break;
	  case 7:
		value[0] = -(x[1])*x[2];
		value[1] = (1.0-x[0])*x[2];
		value[2] = (1.0-x[0])*x[1];
      break;
	default: throw(UG_ERROR_InvalidShapeFunctionIndex(i));
	}
}

template<>
bool LagrangeP1<ReferenceHexahedron,1>::
position(size_t i, position_type& value) const
{
	static const DimReferenceElement<3>& refElem
		= DimReferenceElementFactory<3>::get_reference_element(ROID_HEXAHEDRON);

	value = refElem.corner(i);
	return true;
}

/// \endcond

}

