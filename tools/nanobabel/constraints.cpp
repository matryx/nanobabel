
#include "nanobabel.h"

using namespace OpenBabel;

std::string constrainMinimizationChainKey(std::string chainName)
{
  return "CHAIN:" + chainName;
}

std::string constrainMinimizationResidueKey(std::string chainName, std::string residueName, int residueSerial)
{
  return "RESIDUE:" + chainName + ":" + residueName + ":" + toString(residueSerial);
}

std::string constrainMinimizationAtomKey(int atomSerial)
{
  return "ATOM:" + toString(atomSerial);
}

bool applyConstraints(OBMol *mol, OBFFConstraints *constraints, std::string constraintFileContent)
{
  // Constraints mapping
  std::map<std::string, bool> chainsLockeds;
  std::map<std::string, bool> residuesLockeds;
  std::map<std::string, bool> atomsLockeds;
  // Read constrain file
  std::vector<std::string> lines = split(constraintFileContent, '\n');
  for (int i = 0; i < lines.size(); i++)
  {
    std::string line = lines[i];
    std::vector<std::string> parts = split(line, ':');
    if (parts.size() >= 2)
    {
      std::string type = parts[0];
      std::string constraint = parts[1];
      // Chain level lock
      if (type == "CHAIN" && constraint == "FIXED" && parts.size() == 3)
      {
        std::string chainName = parts[2];
        std::string chainKey = constrainMinimizationChainKey(chainName);
        chainsLockeds[chainKey] = true;
      }
      // Residue level lock
      if (type == "RESIDUE" && constraint == "FIXED" && parts.size() == 5)
      {
        std::string chainName = parts[2];
        std::string residueName = parts[3];
        int residueSerial = atoi(parts[4].c_str());
        std::string residueKey = constrainMinimizationResidueKey(chainName, residueName, residueSerial);
        residuesLockeds[residueKey] = true;
      }
      // Atom level lock
      if (type == "ATOM" && constraint == "FIXED" && parts.size() == 3)
      {
        int atomSerial = atoi(parts[2].c_str());
        std::string atomKey = constrainMinimizationAtomKey(atomSerial);
        atomsLockeds[atomKey] = true;
      }
    }
  }
  // Loop over which atoms should be locked
  int constrained = 0;
  for (OBAtomIterator aitr = mol->BeginAtoms(), aend = mol->EndAtoms(); aitr != aend; ++aitr)
  {
    OBAtom *atom = *aitr;
    if (atom)
    {
      int atomSerial = atom->GetIdx();
      // Try apply atom constraint
      std::string atomKey = constrainMinimizationAtomKey(atomSerial);
      if (atomsLockeds.count(atomKey) > 0)
      {
        constraints->AddAtomConstraint(atom->GetIdx());
        constrained += 1;
        continue;
      }
      OBResidue *residue = atom->GetResidue();
      if (residue)
      {
        std::string chainName = std::string(1, residue->GetChain());
        std::string residueName = std::string(residue->GetName());
        int residueSerial = residue->GetNum();
        // Try apply residue constraints
        std::string residueKey = constrainMinimizationResidueKey(chainName, residueName, residueSerial);
        if (residuesLockeds.count(residueKey) > 0)
        {
          constraints->AddAtomConstraint(atom->GetIdx());
          constrained += 1;
          continue;
        }
        // Try apply chain constraints
        std::string chainKey = constrainMinimizationChainKey(chainName);
        if (chainsLockeds.count(chainKey) > 0)
        {
          constraints->AddAtomConstraint(atom->GetIdx());
          constrained += 1;
          continue;
        }
      }
    }
  }
  // Done
  log("Constrained: " + toString(constrained) + " of " + toString(mol->NumAtoms()) + " atoms");
  return true;
}
