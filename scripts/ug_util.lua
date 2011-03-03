-- Create util namespace
util = util or {}

-- Creates a domain for the given dimension. 1, 2, 3 are supported.
function util.CreateDomain(dim)
	if dim == 1 then
		return Domain1d()
	elseif dim == 2 then
		return Domain2d()
	elseif dim == 3 then
		return Domain3d()
	end
	return nil
end

-- returns the standard path at which grids are stored
function util.GetGridPath()
	return ug_get_data_path().."/grids/"
end

-- loads a domain.
-- If the file can not be found, the method tries to find it in ugs data path.
function util.LoadDomain(domain, gridName)
	local dim = domain:get_dim()

	local tname = gridName
	if ug_file_exists(tname) == false then
		tname = ug_get_data_path().."/grids/" .. gridName
		if ug_file_exists(tname) == false then
			return false
		end
	end

	return LoadDomain(domain, tname, 0)
end


function util.GlobalRefineParallelDomain(domain)
	local dim = domain:get_dim()
	if dim == 1 then
		return GlobalRefineParallelDomain1d(domain)
	elseif dim == 2 then
		return GlobalRefineParallelDomain2d(domain)
	elseif dim == 3 then
		return GlobalRefineParallelDomain3d(domain)
	end
	return false
end

	
-- Creates an Approxmiation space of correct dimesion
function util.CreateApproximationSpace(domain)
	local approxSpace = nil
	if dim == 1 then
		approxSpace = ApproximationSpace1d()
	elseif dim == 2 then
		approxSpace = ApproximationSpace2d()
	elseif dim == 3 then
		approxSpace = ApproximationSpace3d()
	end
	approxSpace:assign_domain(domain)
	
	return approxSpace
end

-- creates Neumann Boundary
function util.CreateNeumannBoundary(approxSpace, subsets)
	local domain = approxSpace:get_domain()
	local dim = domain:get_dim()
	local neumannDisc
	if dim == 1 then
		neumannDisc = FV1NeumannBoundary1d()
	elseif dim == 2 then
		neumannDisc = FV1NeumannBoundary2d()
	elseif dim == 3 then
		neumannDisc = FV1NeumannBoundary3d()
	else
		return nil
	end

	neumannDisc:set_domain(domain)
	neumannDisc:set_pattern(approxSpace)
	neumannDisc:set_subsets(subsets)
	return neumannDisc
end

-- creates Dirichlet Boundary
function util.CreateDirichletBoundary(approxSpace)
	local domain = approxSpace:get_domain()
	local dim = domain:get_dim()
	local dirichlet
	if dim == 1 then
		dirichlet = DirichletBND1d()
	elseif dim == 2 then
		dirichlet = DirichletBND2d()
	elseif dim == 3 then
		dirichlet = DirichletBND3d()
	else
		return nil
	end

	dirichlet:set_domain(domain)
	dirichlet:set_pattern(approxSpace)
	return dirichlet
end

-- creates Inner Boundary
function util.CreateInnerBoundary(approxSpace, functions, subsets)
	local domain = approxSpace:get_domain()
	local dim = domain:get_dim()
	local innerDisc
	if dim == 1 then
		innerDisc = FV1InnerBoundary1d()
	elseif dim == 2 then
		innerDisc = FV1InnerBoundary2d()
	elseif dim == 3 then
		innerDisc = FV1InnerBoundary3d()
	else
		return nil
	end

	innerDisc:set_domain(domain)
	innerDisc:set_pattern(approxSpace)
	innerDisc:set_subsets(subsets)
	innerDisc:set_functions(functions)
	return innerDisc
end

-- creates FV1ConvDiff
function util.CreateFV1ConvDiff(approxSpace, functions, subsets)
	local domain = approxSpace:get_domain()
	local dim = domain:get_dim()
	local elemDisc
	if dim == 1 then
		elemDisc = FV1ConvectionDiffusion1d()
	elseif dim == 2 then
		elemDisc = FV1ConvectionDiffusion2d()
	elseif dim == 3 then
		elemDisc = FV1ConvectionDiffusion3d()
	else
	return nil
	end
	
	elemDisc:set_domain(domain)
	elemDisc:set_pattern(approxSpace)
	elemDisc:set_subsets(subsets)
	elemDisc:set_functions(functions)
	return elemDisc
end

function util.CreateFE1ConvDiff(approxSpace, functions, subsets)
	local domain = approxSpace:get_domain()
	local dim = domain:get_dim()
	local elemDisc
	if dim == 1 then
		elemDisc = FE1ConvectionDiffusion1d()
	elseif dim == 2 then
		elemDisc = FE1ConvectionDiffusion2d()
	elseif dim == 3 then
		elemDisc = FE1ConvectionDiffusion3d()
	else
		return nil
	end
	
	elemDisc:set_domain(domain)
	elemDisc:set_pattern(approxSpace)
	elemDisc:set_subsets(subsets)
	elemDisc:set_functions(functions)
	return elemDisc
end


-- saves matrix for connection viewer
function SaveMatrixForConnectionViewer(gridFunc, linOp, fileName)
	local dim = gridFunc:get_dim()
	if dim == 1 then
		SaveMatrixForConnectionViewer1d(gridFunc, linOp, fileName)
	elseif dim == 2 then
		SaveMatrixForConnectionViewer2d(gridFunc, linOp, fileName)
	elseif dim == 3 then
		SaveMatrixForConnectionViewer3d(gridFunc, linOp, fileName)
	else
		return false
	end
	
	return true
end

-- saves vector for connection viewer
function SaveVectorForConnectionViewer(gridFunc, fileName)
	local dim = gridFunc:get_dim()
	if dim == 1 then
		SaveVectorForConnectionViewer1d(gridFunc, fileName)
	elseif dim == 2 then
		SaveVectorForConnectionViewer2d(gridFunc, fileName)
	elseif dim == 3 then
		SaveVectorForConnectionViewer3d(gridFunc, fileName)
	else
	return false
	end

	return true
end

-- writes grid function in vtk format
function WriteGridFunctionToVTK(gridFunc, fileName)
	local dim = gridFunc:get_dim()
	if dim == 1 then
		WriteGridFunctionToVTK1d(gridFunc, fileName)
	elseif dim == 2 then
		WriteGridFunctionToVTK2d(gridFunc, fileName)
	elseif dim == 3 then
		WriteGridFunctionToVTK3d(gridFunc, fileName)
	else
	return false
	end
	
	return true
end

-- applies the linear solver
function ApplyLinearSolver(linOp, u, b, linSolver)
	local dim = u:get_dim()
	if dim == 1 then
		ApplyLinearSolver1d(linOp, u, b, linSolver)
	elseif dim == 2 then
		ApplyLinearSolver2d(linOp, u, b, linSolver)
	elseif dim == 3 then
		ApplyLinearSolver3d(linOp, u, b, linSolver)
	else
	return false
	end
	
	return true
end

-- create Geometric Multigrid
function util.CreateGeometricMultiGrid(approxSpace)
	local dim = approxSpace:get_domain():get_dim()
	local gmg = nil;
	if dim == 1 then
		gmg = GeometricMultiGridPreconditioner1d()
	elseif dim == 2 then
		gmg = GeometricMultiGridPreconditioner2d()
	elseif dim == 3 then
		gmg = GeometricMultiGridPreconditioner3d()
	else
		gmg = nil
	end
	
	gmg:set_approximation_space(approxSpace)
	return gmg
end

-- create Prolongation / Restriction
function util.CreateP1Prolongation(approxSpace)
	local dim = approxSpace:get_domain():get_dim()
	local transfer = nil;
	if dim == 1 then
		transfer = P1ProlongationOperator1d()
	elseif dim == 2 then
		transfer = P1ProlongationOperator2d()
	elseif dim == 3 then
		transfer = P1ProlongationOperator3d()
	else
	transfer = nil
	end
	
	transfer:set_approximation_space(approxSpace)
	return transfer
end

-- create Projection
function util.CreateP1Projection(approxSpace)
	local dim = approxSpace:get_domain():get_dim()
	local project = nil;
	if dim == 1 then
		project = P1ProjectionOperator1d()
	elseif dim == 2 then
		project = P1ProjectionOperator2d()
	elseif dim == 3 then
		project = P1ProjectionOperator3d()
	else
	project = nil
	end
	
	project:set_approximation_space(approxSpace)
	return project
end


-- creates a Lua User Matrix using a lua function and returns the Provider
function util.CreateLuaUserMatrix(funcName, dim)
	local mat = nil
	if dim == 1 then
		mat = LuaUserMatrix1d()
	elseif dim == 2 then
		mat = LuaUserMatrix2d()
	elseif dim == 3 then
		mat = LuaUserMatrix3d()
	end
		
	mat:set_lua_callback(funcName)
	return mat
end

-- creates a Const User Matrix using a lua function and returns the Provider
function util.CreateConstDiagUserMatrix(diagVal, dim)
	local mat = nil
	if dim == 1 then
		mat = ConstUserMatrix1d()
	elseif dim == 2 then
		mat = ConstUserMatrix2d()
	elseif dim == 3 then
		mat = ConstUserMatrix3d()
	end

	mat:set_diag_tensor(diagVal)
	return mat
end

-- creates a Lua User Vector using a lua function and returns the Provider
function util.CreateLuaUserVector(funcName, dim)
	local vec = nil
	if dim == 1 then
		vec = LuaUserVector1d()
	elseif dim == 2 then
		vec = LuaUserVector2d()
	elseif dim == 3 then
		vec = LuaUserVector3d()
	end

	vec:set_lua_callback(funcName)
	return vec
end

-- creates a Const User Vector using a lua function and returns the Provider
function util.CreateConstUserVector(val, dim)
	local vec = nil
	if dim == 1 then
		vec = ConstUserVector1d()
	elseif dim == 2 then
		vec = ConstUserVector2d()
	elseif dim == 3 then
		vec = ConstUserVector3d()
	end
	
	vec:set_all_entries(val)
	return vec
end

-- creates a Lua User Number using a lua function and returns the Provider
function util.CreateLuaUserNumber(funcName, dim)
	local number = nil
	if dim == 1 then
		number = LuaUserNumber1d()
	elseif dim == 2 then
		number = LuaUserNumber2d()
	elseif dim == 3 then
		number = LuaUserNumber3d()
	end
	
	number:set_lua_callback(funcName)
	return number
end

-- creates a Const User Number using a lua function and returns the Provider
function util.CreateConstUserNumber(val, dim)
	local number = nil
	if dim == 1 then
		number = ConstUserNumber1d()
	elseif dim == 2 then
		number = ConstUserNumber2d()
	elseif dim == 3 then
		number = ConstUserNumber3d()
	end
	
	number:set(val)
	return number
end


-- creates a Lua Boundary Numver using a lua function and returns the Provider
function util.CreateLuaBoundaryNumber(funcName, dim)
	local number = nil
	if dim == 1 then
		number = LuaBoundaryNumber1d()
	elseif dim == 2 then
		number = LuaBoundaryNumber2d()
	elseif dim == 3 then
		number = LuaBoundaryNumber3d()
	end
	
	number:set_lua_callback(funcName)
	return number
end

-- creates a Const Boundary Number using a lua function and returns the Provider
function util.CreateConstBoundaryNumber(val, dim)
	local number = nil
	if dim == 1 then
		number = ConstBoundaryNumber1d()
	elseif dim == 2 then
		number = ConstBoundaryNumber2d()
	elseif dim == 3 then
		number = ConstBoundaryNumber3d()
	end
	
	number:set(val)
	return number
end


-- creates a GridFunctionDebugWriter
function util.CreateGridFunctionDebugWriter(dim)
	local writer = nil
	if dim == 1 then
		writer = GridFunctionDebugWriter1d()
	elseif dim == 2 then
		writer = GridFunctionDebugWriter2d()
	elseif dim == 3 then
		writer = GridFunctionDebugWriter3d()
	end

	return writer
end


-- creates a VTKWriter
function util.CreateVTKWriter(dim)
	local writer = nil
	if dim == 1 then
		writer = VTKOutput1d()
	elseif dim == 2 then
		writer = VTKOutput2d()
	elseif dim == 3 then
		writer = VTKOutput3d()
	end
	
	return writer
end

--------------------------------------------------------------------------------
-- some auxiliary functions
--------------------------------------------------------------------------------
-- function returns true if the number is a power of two
function util.isPowerOfTwo(n)
	local number compare = 1

	while (compare < n) do
		compare = compare*2
	end

	return compare==n
end

-- function returns true if the number is a natural number
function util.isNaturalNumber(n)
	if n-math.floor(n) == 0 then
		return true
	else
		return false
	end
end

--------------------------------------------------------------------------------
-- Command line functions
--------------------------------------------------------------------------------

-- returns parameter in ugargv after ugargv[i] == name
-- use with CommandLine to get parameters, like -dim 3
-- second parameter gets returned when parameter is not found
-- remember that GetParam(name) is GetParam(name, nil)
function util.GetParam(name, return_if_unavailable)
	local i
	for i = 1, ugargc-1 do
		if ugargv[i] == name then
			return ugargv[i+1]
		end
	end
	return return_if_unavailable; 
end


-- return the number for parameter 'name'
-- if parameter is not a number, returns return_if_unavailable
function util.GetParamNumber(name, return_if_unavailable)
	local param = util.GetParam(name, return_if_unavailable)
	local number = tonumber(param)
	if number == nil then
		print("WARNING: Parameter "..name.." is not a number, using "..return_if_unavailable.." instead\n") 
		return return_if_unavailable
	else
		return number
	end
end

-- returns if ugargv contains an option name
-- use with CommandLine to get option, like -useAMG
function util.HasParamOption(name)
	for i = 1, ugargc do
		if ugargv[i] == name then
			return true
		end
	end
	return false 
end
