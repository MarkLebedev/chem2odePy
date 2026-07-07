# Project description

The main purpose of chem2odePy is to streamline and optimize preprocessing steps for ISM chemistry simulation. This package aims to encapsulate all preparation steps and integrate with ODE solvers specific for the task. 

# Package architecture/structure

![img.png](img.png)

Currently, the package includes 3 main modules for simulation preparation:
1. Chemical network YAML standard
2. Chemical network to ODE system translator
3. ODE system to Jacobian Matrix translator
4. Solver Fortran code generator

## YAML standard

*chemistry_yaml_template.yaml* describes a standardized way of writing down chemical networks using YAML. This standard is infinitely scalable and allows for flexible further integrations. It is intuitive enough for manual construction and can be easily parsed programmatically. *chemistry.yaml* file presents an example of an abstract chemistry network with 3 species and 2 reversible reactions.

## Network to ODE translator

*chem2ode.py* file contains Python code of the first translator, which uses the chemistry, described in *chemistry.yaml*, to produce *ode.txt*. *ode.txt* contains the ODE system, written as required by the Livermore ODE solver, which is currently used. The *ode.txt* provided shows the ODE system derived from the example chemical network.

## ODE to Jacobian translator

*ode2jacobian.py* file contains Python code of the second translator, which uses both the chemical network (*chemistry.yaml*) and ODE system (*ode.txt*) to produce *jacobian.txt*. *jacobian.txt* contains the Jacobian Matrix written as required by the Livermore ODE solver, which is currently used. The *jacobian.txt* provided shows the Jacobian Matrix derived from the example network and ODE. 

## Solver Fortran code generator

*driver_config_generator.py* file contains Python code that generates a config file with soma variables needed for the simulations. The resulting file (*driver_config.yaml*) includes some default values that can be corrected.
*driver_constructor.py* file contains Python code that generates Fortran code for the Livermore Solver. 

**Note:** This module is not yet included into the build (next paragraph)

# Build and user manual

## Preprocessing

For easy use, the package has been compiled for both Windows and Linux (see *Build* folder). To generate the ODE and Jacobian, first fill out the *chemistry.yaml* file with the needed chemical network, then run the appropriate files for your operating system in sequence (*chem2ode_Win.exe* and *ode2jacobian_Win.exe* for Windows or *chem2ode_Linux* and *ode2jacobian_Linux* for Linux). After running the files, you should have *ode.txt* and *jacobian.txt* files in the same folder. For windows, use *Full_Win.exe* to launch both files (better performance).

## Livermore solver integration

The *driver_constructor.py* file generates a *driver.f90* Fortran file that can be compiled (alongside *dlsode.f*) using the *compile* bash file into an executable *program* file using ifort compiler.

# Future development

Below is the list of future features that are to be considered and developed:
- [x] Scalability and performance testing
- [x] Solver variables configurator
- [x] Automatic Livermore Solver integration
  - [ ] Include in the build
- [ ] Unified ODE and Jacobian standard (not solver-specific)
- [ ] ODE and Jacobian standard to solver-specific syntax translators
  - [ ] Livermore Solver
  - [ ] Radau5 Solver
- [ ] Hydrodynamics integration
