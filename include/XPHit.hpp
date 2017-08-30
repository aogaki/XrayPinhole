#ifndef XPHit_h
#define XPHit_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4Types.hh"
#include "G4ThreeVector.hh"


class XPHit : public G4VHit
{
public:
   XPHit();
   virtual ~XPHit();
   XPHit(const XPHit &right);
   const XPHit &operator=(const XPHit &right);
   int operator==(const XPHit &right) const;

   inline void *operator new(size_t);
   inline void  operator delete(void *);

   // add setter/getter methods
   void SetTrackID(G4int id) {fTrackID = id;};
   G4int GetTrackID() {return fTrackID;};

   void SetPDGCode(G4int code) {fPDGCode = code;};
   G4int GetPDGCode() {return fPDGCode;};

   void SetDepositEnergy(G4double ene) {fDepositEnergy = ene;};
   G4double GetDepositEnergy() {return fDepositEnergy;};

   void SetVertexName(G4String name) {fVertexName = name;};
   G4String GetVertexName() {return fVertexName;};
   
   void SetPosition(G4ThreeVector pos) {fPosition = pos;};
   G4ThreeVector GetPosition() {return fPosition;};
   
private:
   G4int fTrackID;
   G4int fPDGCode;
   G4double fDepositEnergy;
   G4String fVertexName;
   G4ThreeVector fPosition;
};

typedef G4THitsCollection<XPHit> XPHitsCollection;

extern G4ThreadLocal G4Allocator<XPHit> *XPHitAllocator;

inline void *XPHit::operator new(size_t)
{
   if (!XPHitAllocator)
      XPHitAllocator = new G4Allocator<XPHit>;
   return (void *)XPHitAllocator->MallocSingle();
}

inline void XPHit::operator delete(void *hit)
{
   XPHitAllocator->FreeSingle((XPHit *) hit);
}

#endif
