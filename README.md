# ccxpre
A command line program that can be used to easily configure a CalculiX project. Abaqus input file generated using Gmsh is configured to CalculiX input format if given as input to ccxpre.

## Example
To create a dummy CalculiX input file use,

`ccxpre -d test`

To configure an existing Abaqus input file generated using Gmsh such that only say surface elements are to be included and rename the element type to say CAX4, and to create a CalculiX input file use,

`ccxpre test_mesh.inp P CAX4`
