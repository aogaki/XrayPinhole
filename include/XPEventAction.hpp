#ifndef XPEventAction_h
#define XPEventAction_h 1

#include <G4UserEventAction.hh>

#include "XPHit.hpp"

class XPEventAction : public G4UserEventAction
{
public:
   XPEventAction();
   virtual ~XPEventAction();

   virtual void BeginOfEventAction(const G4Event *);
   virtual void EndOfEventAction(const G4Event *);

private:
   XPHitsCollection *GetHitsCollection(G4int hcID, const G4Event *event) const;
   
   G4int fHCScreenID;
   G4int fHCMirrorID;
};

#endif
