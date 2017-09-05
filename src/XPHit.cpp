#include <iomanip>

#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"

#include "XPHit.hpp"

G4ThreadLocal G4Allocator<XPHit> *XPHitAllocator = 0;

XPHit::XPHit() : G4VHit() {}

XPHit::~XPHit() {}

XPHit::XPHit(const XPHit & /*right*/) : G4VHit() {}

XPHit &XPHit::operator=(const XPHit & /*right*/) { return *this; }

int XPHit::operator==(const XPHit & /*right*/) const { return 0; }
