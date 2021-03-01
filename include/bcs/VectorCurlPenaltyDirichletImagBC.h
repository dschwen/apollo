//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#pragma once

#include "VectorIntegratedBC.h"

class VectorCurlPenaltyDirichletImagBC : public VectorIntegratedBC
{
public:
  static InputParameters validParams();

  VectorCurlPenaltyDirichletImagBC(const InputParameters & parameters);

protected:
  Real _penalty;
  const VectorVariableValue & _v;
  virtual Real computeQpResidual();
  virtual Real computeQpJacobian();
  virtual Real computeQpOffDiagJacobian(unsigned int jvar) override;
  // Holds the solution curl at the current quadrature points
  const unsigned int _v_id;    
  const Function * const _function;
  const Function & _function_x;
  const Function & _function_y;
  const Function & _function_z;

};
