#ifndef XPDetectorConstruction_h
#define XPDetectorConstruction_h 1

#include <vector>

#include <G4VUserDetectorConstruction.hh>
#include <G4VPhysicalVolume.hh>
#include <G4Material.hh>
#include <G4VisAttributes.hh>
#include <G4GenericMessenger.hh>
#include <G4LogicalVolume.hh>


class XPDetectorConstruction : public G4VUserDetectorConstruction
{
public:
   XPDetectorConstruction();
   virtual ~XPDetectorConstruction();

   virtual G4VPhysicalVolume *Construct();
   virtual void ConstructSDandField();

private:
   G4bool fCheckOverlap;

   // Parameters
   void SetParameters();
   
   // Materials
   void DefineMaterials();
   G4Material *fVacuumMat;

   // Camera (out frame)
   G4LogicalVolume *GetCamera();
   G4Material *fCameraMat;
   G4Material *fBottomMat;
   G4double fCameraT;
   G4double fBottomT;
   G4double fBottomD;
   G4double fInsideL;
   
   // Be filters
   G4LogicalVolume *GetFilter1(); // before pinhole
   G4LogicalVolume *GetFilter2(); // before CsI
   G4Material *fFilterMat;
   G4double fFilter1T;
   G4double fFilter1D;
   G4double fFilter2T;
   G4double fFilter2D;
   
   // Pin hole
   G4LogicalVolume *GetPinhole();
   G4Material *fPinholeMat;
   G4double fPinholeT;
   G4double fPinholeD; // diameter of pinhole.  NOT pinhole plate diameter
   
   // Magnet
   void ConstructBField();
   G4LogicalVolume *GetMagnet();
   G4LogicalVolume *fMagnetLV;
   G4Material *fMagnetMat;
   G4double fMagnetL;
   G4double fMagnetGap;
   
   // W block
   G4LogicalVolume *GetBlock();
   G4Material *fBlockMat;
   G4double fBlockT;
   
   // Mirror
   G4LogicalVolume *GetMirror();
   G4Material *fMirrorMat;
   G4Material *fCoatingMat;
   G4double fMirrorT;
   G4double fCoatingT;
   
   // CsI scintillator
   G4LogicalVolume *GetScreen();
   G4Material *fScreenMat;
   G4double fScreenT;
   G4double fScreenD;

   void CalPositions(); // Calculating the positions based on the mirror Z position
   G4double fScreenY;
   G4double fMirrorY;
   G4double fMirrorZ;
   G4double fBlockEdge; // the bottom position of the block
   
   std::vector<G4VisAttributes *> fVisAttributes;
   
};

#endif
