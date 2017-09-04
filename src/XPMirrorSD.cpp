#include <G4HCofThisEvent.hh>
#include <G4SDManager.hh>
#include <G4TouchableHistory.hh>
#include <G4Step.hh>
#include <G4ios.hh>
#include <g4root.hh>
#include <G4SystemOfUnits.hh>
#include <G4Material.hh>
#include <G4VProcess.hh>

#include "XPMirrorSD.hpp"
#include "XPHit.hpp"


XPMirrorSD::XPMirrorSD(const G4String &name,
             const G4String &hitsCollectionName)
   : G4VSensitiveDetector(name)
{
   collectionName.insert(hitsCollectionName);
}

XPMirrorSD::~XPMirrorSD()
{}

void XPMirrorSD::Initialize(G4HCofThisEvent *hce)
{
   fHitsCollection
      = new XPHitsCollection(SensitiveDetectorName, collectionName[0]);

   G4int hcID
      = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
   hce->AddHitsCollection(hcID, fHitsCollection);
}

G4bool XPMirrorSD::ProcessHits(G4Step *step, G4TouchableHistory */*history*/)
{
   // Checking enter or not
   G4StepPoint *preStepPoint = step->GetPreStepPoint();
   if(preStepPoint->GetStepStatus() != fGeomBoundary)
      return false;

   // Checking the direction
   G4ThreeVector momentum =  preStepPoint->GetMomentum();
   if(momentum.y() > 0.)
      return false;

   // Checking gamma or not
   G4Track *track = step->GetTrack();
   G4ParticleDefinition *particle = track->GetDefinition();
   G4int pdgCode = particle->GetPDGEncoding();
   if(pdgCode != 22) // 22 is gamma
      return false;

   XPHit *newHit = new XPHit();

   G4int trackID = track->GetTrackID();
   newHit->SetTrackID(trackID);

   G4double kineticEnergy = preStepPoint->GetKineticEnergy();
   newHit->SetKineticEnergy(kineticEnergy);
   
   newHit->SetMomentum(momentum);

   G4ThreeVector position =  preStepPoint->GetPosition();
   newHit->SetPosition(position);
   
   fHitsCollection->insert(newHit);
   return true;
}
