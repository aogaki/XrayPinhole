#ifndef XPSD_h
#define XPSD_h 1

#include <G4VSensitiveDetector.hh>
#include <G4ThreeVector.hh>
#include <G4LogicalVolume.hh>

#include "XPHit.hpp"


class G4Step;
class G4HCofThisEvent;
class G4TouchableHistory;

class XPSD: public G4VSensitiveDetector
{
public:
   XPSD(const G4String &name,
            const G4String &hitsCollectionName);
   virtual ~XPSD();

   virtual void Initialize(G4HCofThisEvent *hce);
   virtual G4bool ProcessHits(G4Step *step, G4TouchableHistory *history);

private:
   XPHitsCollection *fHitsCollection;
};

#endif
