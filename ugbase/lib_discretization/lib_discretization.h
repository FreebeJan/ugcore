
// include all files from the lib_discretization module

/////////////////////
// basics
/////////////////////

// domain description
#include "lib_discretization/domain.h"

// degree of freedom managers
#include "lib_discretization/dof_manager/dof_manager.h"
#include "lib_discretization/dof_manager/p1conform_dof_manager/p1conform_dof_manager.h"
#include "lib_discretization/dof_manager/general_dof_manager/general_dof_manager.h"

// function spaces
#include "lib_discretization/function_spaces/grid_function_space.h"

// reference elements
#include "lib_discretization/reference_element/reference_elements.h"

// quadratures
#include "lib_discretization/quadrature/quadrature.h"

// local shape functions
#include "lib_discretization/local_shape_function_set/local_shape_function_set_factory.h"

// assembling interface
#include "lib_discretization/assemble.h"

////////////////////////
// spacial discretizations
////////////////////////

// plug in discs
#include "lib_discretization/domain_discretization/plug_in_disc/convection_diffusion_equation/convection_diffusion_assemble.h"

// domain discretization
#include "lib_discretization/domain_discretization/plug_in_domain_discretization.h"



//#include "numericalsolution.h"

//#include "differentialoperator.h"
//#include "rhs.h"
//#include "dirichletbndcond.h"

/*#include "spacialdiscretization.h"
#include "subsetdiscretization.h"
#include "systemdiscretization.h"
#include "equation.h"*/

//#include "vtkoutput.h"
//#include "timestep.h"
//#include "newton.h"
//#include "elementdata.h"
