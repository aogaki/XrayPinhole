#include <G4FieldManager.hh>
#include <G4TransportationManager.hh>
#include <G4SystemOfUnits.hh>
#include <G4AutoLock.hh>

#include "XPMagneticField.hpp"


XPMagneticField::XPMagneticField()
   : G4MagneticField(),
     fFieldManager(nullptr)
{
   fFieldManager = new G4FieldManager();
   fFieldManager->SetDetectorField(this);    
   fFieldManager->CreateChordFinder(this);

   fBy = 1.0*tesla;
}

XPMagneticField::~XPMagneticField()
{
   delete fFieldManager;
}

void XPMagneticField::GetFieldValue(const G4double point[4], G4double *bField) const
{
   bField[0] = bField[2] = 0.;
   bField[1] = fBy;
}

G4FieldManager*  XPMagneticField::GetFieldManager() const
{
   return fFieldManager;
}
