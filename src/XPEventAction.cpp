#include <G4Event.hh>
#include <G4SDManager.hh>
#include <g4root.hh>

#include "XPEventAction.hpp"


XPEventAction::XPEventAction()
   : G4UserEventAction(),
     fHitsCollectionID(-1)
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
   if (fHitsCollectionID == -1)
      fHitsCollectionID = G4SDManager::GetSDMpointer()->GetCollectionID("HC");
   
   XPHitsCollection *hc = GetHitsCollection(fHitsCollectionID, event);
   const G4int kHit = hc->entries();
   for (G4int iHit = 0; iHit < kHit; iHit++) {
      XPHit *newHit = (*hc)[iHit];
   }

}

