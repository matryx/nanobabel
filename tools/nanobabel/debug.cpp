
#include "nanobabel.h"

using namespace OpenBabel;

void debugResidues(OBMol *mol)
{
  int i = 0;
  for (OBResidueIterator ritr = mol->BeginResidues(), aend = mol->EndResidues(); ritr != aend; ++ritr)
  {
    OBResidue *residue = *ritr;
    if (residue)
    {
      char chain = residue->GetChain();
      std::string name = residue->GetName();
      int serial = residue->GetIdx();
      log("ResidueDebug: " + toString(chain) + " residue: " + name + "->" + toString(serial));
    }
    i++;
  }
  log("Total residues: " + toString(i));
}

void debugAtoms(OBMol *mol)
{
  int i = 0;
  for (OBAtomIterator aitr = mol->BeginAtoms(), aend = mol->EndAtoms(); aitr != aend; ++aitr)
  {
    OBAtom *atom = *aitr;
    if (atom)
    {
      int atomicNum = atom->GetAtomicNum();
      int atomicIso = atom->GetIsotope();
      int atomicIdx = atom->GetIndex();
      log("AtomDebug: " + toString(atomicNum) + " iso: " + toString(atomicIso) + " idx: " + toString(atomicIdx));
    }
    i++;
  }
  log("Total atoms: " + toString(i));
}

void debugBonds(OBMol *mol)
{
  int i = 0;
  for (OBBondIterator bitr = mol->BeginBonds(), aend = mol->EndBonds(); bitr != aend; ++bitr)
  {
    OBBond *bond = *bitr;
    if (bond)
    {
      OBAtom *a1 = bond->GetBeginAtom();
      OBAtom *a2 = bond->GetEndAtom();
      int order = bond->GetBondOrder();
      int a1idx = a1->GetIndex();
      int a2idx = a2->GetIndex();
      log("BondDebug: " + toString(order) + " atoms: " + toString(a1idx) + "->" + toString(a2idx));
    }
    i++;
  }
  log("Total bonds: " + toString(i));
}
