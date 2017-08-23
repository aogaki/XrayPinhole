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

private:

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
