# ccxpre
A command line program that helps configure a CalculiX project, and clean mesh files. Abaqus input file generated using Gmsh can be configured or converted to CalculiX input format if given as input to ccxpre.

## Examples
To create a boilerplate CalculiX input file use,

`ccxpre -b new.inp`

To create a CalculiX input file and, configure an existing Abaqus input file generated using Gmsh such that only say surface elements are to be included and rename the element type to say CAX4 use,

`ccxpre -i my_mesh.inp -e SUR=CAX4`

## Install

## Future work
