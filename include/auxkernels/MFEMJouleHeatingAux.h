#pragma once

#include "MFEMAuxSolver.h"

class JouleHeatingCoefficient : public hephaestus::CoupledCoefficient
{
private:
  mfem::Coefficient * sigma;
  mfem::ParGridFunction * joule_heating_gf;
  std::string conductivity_coef_name;
  // std::string var_name;

public:
  JouleHeatingCoefficient(const hephaestus::InputParameters & params)
    : hephaestus::CoupledCoefficient(params),
      conductivity_coef_name(params.GetParam<std::string>("ConductivityCoefName")){};

  void Init(const mfem::NamedFieldsMap<mfem::ParGridFunction> & variables,
            hephaestus::DomainProperties & domain_properties)
  {
    // To ensure conductivity on subdomains is converted into global coefficient
    // Hephaestus update for domain_properties initialisation could address this
    if (!domain_properties.scalar_property_map.Has(conductivity_coef_name))
    {
      domain_properties.scalar_property_map.Register(
          conductivity_coef_name,
          new mfem::PWCoefficient(
              domain_properties.getGlobalScalarProperty(std::string(conductivity_coef_name))),
          true);
    }

    hephaestus::CoupledCoefficient::Init(variables, domain_properties);
    std::cout << "Intialising JouleHeating";
    sigma = domain_properties.scalar_property_map.Get(conductivity_coef_name);

    joule_heating_gf = variables.Get("joule_heating");
  }

  virtual double Eval(mfem::ElementTransformation & T, const mfem::IntegrationPoint & ip)
  {
    mfem::Vector E;
    double thisSigma;
    gf->GetVectorValue(T, ip, E);
    thisSigma = sigma->Eval(T, ip);
    return thisSigma * (E * E);
  }

  void Solve(double t)
  {
    this->SetTime(t);
    joule_heating_gf->ProjectCoefficient(*this);
  }
};

class MFEMJouleHeatingAux : public MFEMAuxSolver
{
public:
  static InputParameters validParams();

  MFEMJouleHeatingAux(const InputParameters & parameters);
  virtual ~MFEMJouleHeatingAux();

  virtual void execute() override {}
  virtual void initialize() override {}
  virtual void finalize() override {}

  virtual hephaestus::AuxSolver * getAuxSolver() override;
  virtual void storeCoefficients(hephaestus::DomainProperties & domain_properties) override;

protected:
  hephaestus::InputParameters joule_heating_params;
  JouleHeatingCoefficient joule_heating_aux;
};
