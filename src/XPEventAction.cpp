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
   if (fHCScreenID == -1)
      fHCScreenID = G4SDManager::GetSDMpointer()->GetCollectionID("ScreenHC");

   G4int eventID = event->GetEventID();

   G4AnalysisManager *anaMan = G4AnalysisManager::Instance();
   
   XPHitsCollection *hc = GetHitsCollection(fHCScreenID, event);
   const G4int kHit = hc->entries();
   for (G4int iHit = 0; iHit < kHit; iHit++) {
      XPHit *newHit = (*hc)[iHit];

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

}

