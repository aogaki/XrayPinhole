#include <G4HCofThisEvent.hh>
#include <G4SDManager.hh>
#include <G4TouchableHistory.hh>
#include <G4Step.hh>
#include <G4ios.hh>
#include <g4root.hh>
#include <G4SystemOfUnits.hh>
#include <G4Material.hh>
#include <G4VProcess.hh>

#include "XPSD.hpp"
#include "XPHit.hpp"


XPSD::XPSD(const G4String &name,
             const G4String &hitsCollectionName)
   : G4VSensitiveDetector(name)
{
   collectionName.insert(hitsCollectionName);
}

XPSD::~XPSD()
{}

void XPSD::Initialize(G4HCofThisEvent *hce)
{
   fHitsCollection
      = new XPHitsCollection(SensitiveDetectorName, collectionName[0]);

   G4int hcID
      = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
   hce->AddHitsCollection(hcID, fHitsCollection);
}

G4bool XPSD::ProcessHits(G4Step *step, G4TouchableHistory */*history*/)
{
   G4double depositEnergy = step->GetTotalEnergyDeposit();
   if(depositEnergy == 0.) return false;

   XPHit *newHit = new XPHit();
   newHit->SetDepositEnergy(depositEnergy);
   
   G4Track *track = step->GetTrack();   
   G4String vertexName = track->GetLogicalVolumeAtVertex()->GetName();
   newHit->SetVertexName(vertexName);
   
   G4ParticleDefinition *particle = track->GetDefinition();
   G4int pdgCode = particle->GetPDGEncoding();
   newHit->SetPDGCode(pdgCode);

   G4int trackID = track->GetTrackID();
   newHit->SetTrackID(trackID);
   
   G4StepPoint *postStepPoint = step->GetPostStepPoint();
   G4ThreeVector position =  postStepPoint->GetPosition();
   newHit->SetPosition(position);

   fHitsCollection->insert(newHit);
   
   return true;
}