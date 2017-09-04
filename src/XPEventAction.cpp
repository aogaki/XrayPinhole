#include <G4Event.hh>
#include <G4SDManager.hh>
#include <g4root.hh>

#include "XPEventAction.hpp"


XPEventAction::XPEventAction()
   : G4UserEventAction(),
     fHCScreenID(-1),
     fHCMirrorID(-1)
{}

XPEventAction::~XPEventAction()
{}

XPHitsCollection *XPEventAction::GetHitsCollection(G4int hcID, const G4Event *event)
const
{
   XPHitsCollection *hitsCollection 
      = static_cast<XPHitsCollection *>(
         event->GetHCofThisEvent()->GetHC(hcID));
  
   if ( ! hitsCollection ) {
      G4ExceptionDescription msg;
      msg << "Cannot access hitsCollection ID " << hcID;
      // check how to use G4Exception
      G4Exception("XPEventAction::GetHitsCollection()",
                  "XPCode0003", FatalException, msg);
   }         

   return hitsCollection;
}

void XPEventAction::BeginOfEventAction(const G4Event *)
{}

void XPEventAction::EndOfEventAction(const G4Event *event)
{
   G4int eventID = event->GetEventID();

   G4AnalysisManager *anaMan = G4AnalysisManager::Instance();

   // Get the CsI screen info
   if (fHCScreenID == -1)
      fHCScreenID = G4SDManager::GetSDMpointer()->GetCollectionID("ScreenHC");

   XPHitsCollection *hcScreen = GetHitsCollection(fHCScreenID, event);
   const G4int kScreenHit = hcScreen->entries();
   for (G4int iHit = 0; iHit < kScreenHit; iHit++) {
      XPHit *newHit = (*hcScreen)[iHit];

      anaMan->FillNtupleIColumn(0, 0, eventID); // EventID

      G4int trackID = newHit->GetTrackID();
      anaMan->FillNtupleIColumn(0, 1, trackID);

      G4int PDGCode = newHit->GetPDGCode();
      anaMan->FillNtupleIColumn(0, 2, PDGCode);

      G4double depositEnergy = newHit->GetDepositEnergy();
      anaMan->FillNtupleDColumn(0, 3, depositEnergy);

      G4String vertexName = newHit->GetVertexName();
      anaMan->FillNtupleSColumn(0, 4, vertexName);

      G4ThreeVector position = newHit->GetPosition();
      anaMan->FillNtupleDColumn(0, 5, position.x());
      anaMan->FillNtupleDColumn(0, 6, position.y());
      anaMan->FillNtupleDColumn(0, 7, position.z());

      anaMan->AddNtupleRow(0);
   }

   // Get the mirror info
   if (fHCMirrorID == -1)
      fHCMirrorID = G4SDManager::GetSDMpointer()->GetCollectionID("MirrorHC");

   XPHitsCollection *hcMirror = GetHitsCollection(fHCMirrorID, event);
   const G4int kMirrorHit = hcMirror->entries();
   for (G4int iHit = 0; iHit < kMirrorHit; iHit++) {
      XPHit *newHit = (*hcMirror)[iHit];

      anaMan->FillNtupleIColumn(1, 0, eventID); // EventID

      G4int trackID = newHit->GetTrackID();
      anaMan->FillNtupleIColumn(1, 1, trackID);

      G4double kineticEnergy = newHit->GetKineticEnergy();
      anaMan->FillNtupleDColumn(1, 2, kineticEnergy);

      G4ThreeVector position = newHit->GetPosition();
      anaMan->FillNtupleDColumn(1, 3, position.x());
      anaMan->FillNtupleDColumn(1, 4, position.y());
      anaMan->FillNtupleDColumn(1, 5, position.z());

      G4ThreeVector momentum = newHit->GetMomentum();
      anaMan->FillNtupleDColumn(1, 6, momentum.x());
      anaMan->FillNtupleDColumn(1, 7, momentum.y());
      anaMan->FillNtupleDColumn(1, 8, momentum.z());

      anaMan->AddNtupleRow(1);
   }

}

