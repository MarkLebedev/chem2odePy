# Project description

The main purpose of chem2odePy is to streamline and optimize preprocessing steps for ISM chemistry simulation. This package aims to encapsulate all preparation steps and integrate with ODE solvers specific for the task. 

# Package architecture/structure

Currently, the package includes 3 main modules for simulation preparation:
1. Chemical network YAML standard
2. Chemical network to ODE system translator
3. ODE system to Jacobian Matrix translator

## YAML standard

*chemistry_yaml_template.yaml* describes a standardized way of writing down chemical networks using YAML. This standard is infinitely scalable and allows for flexible further integrations. It is intuitive enough for manual construction and can be easily parsed programmatically. *chemistry.yaml* file presents an example of an abstract chemistry network with 3 species and 2 reversible reactions.

## Network to ODE translator

*chem2ode.py* file contains Python code of the first translator, which uses the chemistry, described in *chemistry.yaml*, to produce *ode.txt*. *ode.txt* contains the ODE system, written as required by the Livermore ODE solver, which is currently used. The *ode.txt* provided shows the ODE system derived from the example chemical network.

## ODE to Jacobian translator

*ode2jacobian.py* file contains Python code of the second translator, which uses both the chemical network (*chemistry.yaml*) and ODE system (*ode.txt*) to produce *jacobian.txt*. *jacobian.txt* contains the Jacobian Matrix written as required by the Livermore ODE solver, which is currently used. The *jacobian.txt* provided shows the Jacobian Matrix derived from the example network and ODE. 

# Build and user manual

## Preprocessing

For easy use, the package has been compiled for both Windows and Linux (see *Build* folder). To generate the ODE and Jacobian, first fill out the *chemistry.yaml* file with the needed chemical network, then run the appropriate files for your operating system in sequence (*chem2ode_Win.exe* and *ode2jacobian_Win.exe* for Windows or *chem2ode_Linux* and *ode2jacobian_Linux* for Linux). After running the files, you should have *ode.txt* and *jacobian.txt* files in the same folder. 

## Livermore solver integration

Currently, the package only provides preformatted files for solver integration, so you need to open *driver.f90* and paste the resulting ODE and Jacobian into the FEX and JEX subroutine calls respectively. Also, in the main text body, you need to provide other variables, like initial values and tolerances for each specie. After that, the *compile* bash file can be used to compile the *dlsode.f* and *driver.f90* files into an executable *program* file using ifort compiler.

# Future development

Below is the list of future features that are to be considered and developed:
- [ ] Scalability and performance testing
- [ ] Solver variables configurator
- [ ] Automatic Livermore Solver integration
- [ ] Unified ODE and Jacobian standard (not solver-specific)
- [ ] ODE and Jacobian standard to solver-specific syntax translators
  - [ ] Livermore Solver
  - [ ] Radau5 Solver
- [ ] Hydrodynamics integration
