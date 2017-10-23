# nano-babel

```

Usage: nanobabel [action] [options]

  all actions:
    minimize       Molecular energy minimization
    bonding        Compute bonding information of molecule
    convert        Rewrite molecule file in a different format
    hydrogen       Add or remove hydrogens from a molecule file
    forcefields    List supported forcefields
    formats        List supported file formats
    descriptors    List supported descriptors
    plugins        List supported plugins
    config         Current configuration informations

  minimize options:
    -i  FILE       Input file (default=input.pdb)
    -o  FILE       Output file (default=output.pdb)
    -cx FILE       Forcefield constraints file
    -ff FFID       Select a forcefield (default=UFF)
    -h             Add hydrogen atoms
    -n  steps      Specify the maximum number of steps (default=2500)
    -l  steps      Specify the interval number of steps between structure updates
    -dd PATH       Set the path for the data directory

  bonding options:
    -i  FILE       Input file (default=input.pdb)
    -o  FILE       Output file (default=output.pdb)
    -h             Add hydrogen atoms
    -dd PATH       Set the path for the data directory

  convert options:
    -i  FILE       Input file (default=input.pdb)
    -o  FILE       Output file (default=output.pdb)
    -h             Add hydrogen atoms
    -dd PATH       Set the path for the data directory

  hydrogen options:
    -i  FILE       Input file (default=input.pdb)
    -o  FILE       Output file (default=output.pdb)
    -add           Add hydrogen atoms
    -del           Delete hydrogen atoms
    -dd PATH       Set the path for the data directory

```