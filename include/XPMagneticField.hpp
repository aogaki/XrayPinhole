#ifndef XPMagneticField_h
#define XPMagneticField_h 1

#include <G4MagneticField.hh>
#include <G4FieldManager.hh>

#include <G4SystemOfUnits.hh>


class XPMagneticField : public G4MagneticField
{
public:
   XPMagneticField();   
   virtual ~XPMagneticField();  
      
   virtual void GetFieldValue(const G4double point[4], G4double *bfield) const;
      
   // Get the field manager
   G4FieldManager *GetFieldManager() const; 
    
private:
   // Field manager 
   G4FieldManager *fFieldManager;
   
   // Magnetic Field information
   G4double fBy;
};

#endif
