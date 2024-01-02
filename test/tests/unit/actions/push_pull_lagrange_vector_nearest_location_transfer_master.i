[Mesh]
  type = GeneratedMesh
  dim = 3
  nx = 5
  ny = 5
  nz = 5
[]

[Problem]
  solve = false
[]

[AuxVariables]
  [received_vector]
    family = LAGRANGE_VEC
    order = FIRST
  []

  [expected_vector]
    family = LAGRANGE_VEC
    order = FIRST
  []
[]

[Functions]
  [set_vector]
    type = ParsedVectorFunction
    expression_x = '100*x*x'
    expression_y = '100*y*y'
    expression_z = '100*z*z'
  []
[]

[ICs]
  [set_vector]
    type = VectorFunctionIC
    variable = expected_vector
    function = set_vector
  []
[]

[MultiApps]
  [sub_app]
    type = TransientMultiApp
    positions = '0 0 0'
    input_files = 'push_pull_lagrange_vector_nearest_location_transfer_subapp.i'
    execute_on = timestep_begin
  []
[]

[VectorTransfers]
  # Transfer expected variable to subapp.
  [push]
    type = MultiAppGeneralFieldNearestLocationTransfer
    to_multi_app = sub_app 
    source_variable = expected_vector 
    variable = received_vector_sub_app
  []

  # Transfer back.
  [pull]
    type = MultiAppGeneralFieldNearestLocationTransfer
    from_multi_app = sub_app
    source_variable = received_vector_sub_app
    variable = received_vector
  []
[]

[Postprocessors]
  [vector_l2_difference]
    type = ElementVectorL2Difference
    variable = received_vector
    other_variable = expected_vector
    execute_on = timestep_end
  []
[]

[Executioner]
  type = Transient
  dt = 1.0
  start_time = 0.0
  end_time = 1.0
[]

[Outputs]
  csv = true
[]