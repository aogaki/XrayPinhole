#include <G4Tubs.hh>
#include <G4Box.hh>
#include <G4NistManager.hh>
#include <G4SystemOfUnits.hh>
#include <G4LogicalVolume.hh>
#include <G4PVPlacement.hh>
#include <G4SDManager.hh>
#include <G4LogicalVolumeStore.hh>
#include <G4PVReplica.hh>
#include <G4RunManager.hh>
#include <G4SubtractionSolid.hh>
#include <G4Trap.hh>
#include <G4Cons.hh>
#include <G4UnionSolid.hh>
#include <G4IntersectionSolid.hh>
#include <G4UnionSolid.hh>
#include <G4AutoDelete.hh>

#include "XPDetectorConstruction.hpp"
#include "XPScreenSD.hpp"
#include "XPMirrorSD.hpp"
#include "XPMagneticField.hpp"


XPDetectorConstruction::XPDetectorConstruction()
   : fVacuumMat(nullptr),
     fCameraMat(nullptr),
     fBottomMat(nullptr),
     fFilterMat(nullptr),
     fPinholeMat(nullptr),
     fMagnetMat(nullptr),
     fBlockMat(nullptr),
     fMirrorMat(nullptr),
     fCoatingMat(nullptr),
     fScreenMat(nullptr)
{
   fCheckOverlap = true;

   SetParameters();
   DefineMaterials();
}

XPDetectorConstruction::~XPDetectorConstruction()
{
   for(auto &pointer: fVisAttributes) delete pointer;
}

void XPDetectorConstruction::SetParameters()
{
   fCameraT = 5.*mm;
   
   fFilter1T = 500.*um;
   fFilter2T = 250.*um;

   fFilter1D = 200.*mm;

   fPinholeT = 50.*um;
   //fPinholeT = 5.*mm;
   fPinholeD = 5.*um;
   
   fBlockT = 5.*mm;

   fCoatingT = 5.*um;
   fMirrorT = 1.*mm + fCoatingT;
   
   fScreenT = 0.15*mm;
   fFilter2D = fScreenD = 26.*mm;

   fBottomT = 5.*mm;
   fBottomD = 200.*mm;

   fInsideL = 1.5*m;

   fMagnetGap = 40.*mm;
   fMagnetL = 50.*mm;

   fMirrorZ = -250.*mm; // z position in the camera.  NOT global position
   CalPositions();
}

void XPDetectorConstruction::CalPositions()
{
   G4double mirrorZ = (fInsideL / 2. + fMirrorZ);
   fMirrorY = -mirrorZ * tan(3.*mrad);
   fMirrorY -= fMirrorT / 2.;
   
   const G4double sourceD = 10.*um;
   const G4double sourceY = 30.*mm * tan(0.003) - sourceD / 2.;// Lower edge of the source
   const G4double holeY = 0.0025*um; // Upper edge of the hole
   const G4double gra = holeY - sourceY / 30.*mm;

   fBlockEdge = gra * (fInsideL / 2. + fMirrorZ) + holeY;

   const G4double mirrorFace = fMirrorY + fMirrorT;
   fScreenY = mirrorFace * (1. - (fInsideL - mirrorZ) / (mirrorZ));
}

void XPDetectorConstruction::DefineMaterials()
{
   G4NistManager *manager = G4NistManager::Instance();

   // NIST database materials
   fVacuumMat = manager->FindOrBuildMaterial("G4_Galactic");
   fCameraMat = manager->FindOrBuildMaterial("G4_Al");
   fBottomMat = manager->FindOrBuildMaterial("G4_GLASS_PLATE");
   fFilterMat = manager->FindOrBuildMaterial("G4_Be");
   fPinholeMat = manager->FindOrBuildMaterial("G4_W");
   fBlockMat = manager->FindOrBuildMaterial("G4_W");
   fMagnetMat = manager->FindOrBuildMaterial("G4_Fe");
   fCoatingMat = manager->FindOrBuildMaterial("G4_Pt");
   fMirrorMat =  manager->FindOrBuildMaterial("G4_Si");
   fScreenMat = manager->FindOrBuildMaterial("G4_CESIUM_IODIDE");
}

G4VPhysicalVolume *XPDetectorConstruction::Construct()
{
   // world volume
   G4double worldW = 5.*m;
   G4double worldH = 5.*m;
   G4double worldL = 5.*m;

   G4Box *worldS = new G4Box("World", worldW / 2., worldH / 2., worldL / 2.);
   G4LogicalVolume *worldLV = new G4LogicalVolume(worldS, fVacuumMat, "World");

   G4VisAttributes *visAttributes = new G4VisAttributes(G4Colour::White());
   visAttributes->SetVisibility(false);
   worldLV->SetVisAttributes(visAttributes);
   fVisAttributes.push_back(visAttributes);

   G4VPhysicalVolume *worldPV
      = new G4PVPlacement(nullptr, G4ThreeVector(), worldLV, "World", 0,
                          false, 0, fCheckOverlap);
      
   // Camera
   G4double cameraY = 0.*mm;
   G4double cameraZ = 780.*mm;
   G4LogicalVolume *cameraLV = GetCamera();
   G4RotationMatrix *cameraRot = new G4RotationMatrix();
   //cameraRot->rotateX(-45.*deg);
   //cameraY = -780.*mm * sin(45.*deg);
   //cameraZ = 780.*mm * cos(45.*deg);
   G4ThreeVector cameraPos(0., cameraY, cameraZ);
   new G4PVPlacement(cameraRot, cameraPos, cameraLV, "Camera", worldLV,
                     false, 0, fCheckOverlap);

   return worldPV;
}

G4LogicalVolume *XPDetectorConstruction::GetCamera()
{
   // out frame of camera
   G4double topD = fFilter1D + 2 * fCameraT;
   G4double bottomD = fBottomD + 2 * fCameraT;
   G4double cameraL = fInsideL + fBottomT;
   G4Cons *cameraS = new G4Cons("Camera", 0., topD / 2., 0., bottomD / 2., cameraL / 2., 0., CLHEP::twopi);
   G4LogicalVolume *cameraLV = new G4LogicalVolume(cameraS, fCameraMat, "Camera");

   // Bottom of camera
   G4Tubs *bottomS = new G4Tubs("Bottom", 0., fBottomD / 2., fBottomT / 2., 0., CLHEP::twopi);
   G4LogicalVolume *bottomLV = new G4LogicalVolume(bottomS, fBottomMat, "Bottom");
   G4ThreeVector bottomPos(0., 0., cameraL / 2. - fBottomT / 2.);
   new G4PVPlacement(nullptr, bottomPos, bottomLV, "Bottom", cameraLV,
                     false, 0, fCheckOverlap);

   // insdie of camera filled by vacuum
   G4double insideTopD = fFilter1D;
   G4Cons *insideS = new G4Cons("Inside", 0., insideTopD / 2., 0., fBottomD / 2.,
                                fInsideL / 2., 0., CLHEP::twopi);
   G4LogicalVolume *insideLV = new G4LogicalVolume(insideS, fVacuumMat, "Inside");
   G4ThreeVector insidePos(0., 0., -fBottomT / 2.);
   new G4PVPlacement(nullptr, insidePos, insideLV, "Inside", cameraLV,
                     false, 0, fCheckOverlap);

   // Upstream Be filter
   G4LogicalVolume *filter1LV = GetFilter1();
   G4double filter1Z = -fInsideL / 2. + fFilter1T / 2.;
   G4ThreeVector filter1Pos(0., 0., filter1Z);
   new G4PVPlacement(nullptr, filter1Pos, filter1LV, "Filter1", insideLV,
                     false, 0, fCheckOverlap);

   // Pinhole plate
   G4LogicalVolume *pinholeLV = GetPinhole();
   G4double pinholeZ = -fInsideL / 2. + fFilter1T + fPinholeT / 2.;
   G4ThreeVector pinholePos(0., 0., pinholeZ);
   new G4PVPlacement(nullptr, pinholePos, pinholeLV, "Pinhole", insideLV,
                     false, 0, fCheckOverlap);
   
   // Magnet
   fMagnetLV = GetMagnet();
   G4double magnetZ = -fInsideL / 2. + fFilter1T + fPinholeT + fMagnetL / 2.;
   G4ThreeVector magnetPos(0., 0., magnetZ);
   new G4PVPlacement(nullptr, magnetPos, fMagnetLV, "Magnet", insideLV,
                     false, 0, fCheckOverlap);
   
   // Downstream Be filter
   G4LogicalVolume *filter2LV = GetFilter2();
   G4double filter2Z = fInsideL / 2. - fScreenT - fFilter2T / 2.;
   G4ThreeVector filter2Pos(0., fScreenY, filter2Z);
   new G4PVPlacement(nullptr, filter2Pos, filter2LV, "Filter2", insideLV,
                     false, 0, fCheckOverlap);
   
   // CsI screen
   G4LogicalVolume *screenLV = GetScreen();
   G4double screenZ = fInsideL / 2. - fScreenT / 2.;
   G4ThreeVector screenPos(0., fScreenY, screenZ);
   new G4PVPlacement(nullptr, screenPos, screenLV, "Screen", insideLV,
                     false, 0, fCheckOverlap);
   
   // W block
   G4LogicalVolume *blockLV = GetBlock();
   G4double blockZ = fMirrorZ + fBlockT / 2.;
   G4ThreeVector blockPos(0., 0., blockZ);
   new G4PVPlacement(nullptr, blockPos, blockLV, "Block", insideLV,
                     false, 0, fCheckOverlap);

   // Mirror 
   G4LogicalVolume *mirrorLV = GetMirror();
   G4ThreeVector mirrorPos(0., fMirrorY, fMirrorZ);
   G4RotationMatrix *mirrorRot = new G4RotationMatrix();
   mirrorRot->rotateX(90.*deg);
   new G4PVPlacement(mirrorRot, mirrorPos, mirrorLV, "Mirror", insideLV,
                     false, 0, fCheckOverlap);
   
   
   G4VisAttributes *visAttributes = new G4VisAttributes(G4Colour::White());
   visAttributes->SetVisibility(true);
   visAttributes->SetColor(1., 1., 1., 0.2);
   cameraLV->SetVisAttributes(visAttributes);
   fVisAttributes.push_back(visAttributes);

   visAttributes = new G4VisAttributes(G4Colour::White());
   visAttributes->SetVisibility(false);
   insideLV->SetVisAttributes(visAttributes);
   fVisAttributes.push_back(visAttributes);

   visAttributes = new G4VisAttributes(G4Colour::White());
   visAttributes->SetVisibility(true);
   bottomLV->SetVisAttributes(visAttributes);
   fVisAttributes.push_back(visAttributes);
   
   return cameraLV;
}

G4LogicalVolume *XPDetectorConstruction::GetPinhole()
{
   G4Tubs *pinholeS = new G4Tubs("Pinhole", fPinholeD / 2., fFilter1D / 2., fPinholeT / 2., 0., CLHEP::twopi);
   G4LogicalVolume *pinholeLV = new G4LogicalVolume(pinholeS, fPinholeMat, "Pinhole");

   G4VisAttributes *visAttributes = new G4VisAttributes(G4Colour::Green());
   visAttributes->SetVisibility(true);
   pinholeLV->SetVisAttributes(visAttributes);
   fVisAttributes.push_back(visAttributes);

   return pinholeLV;
}

G4LogicalVolume *XPDetectorConstruction::GetFilter1()
{
   G4Tubs *filter1S = new G4Tubs("Filter1", 0., fFilter1D / 2., fFilter1T / 2., 0., CLHEP::twopi);
   G4LogicalVolume *filter1LV = new G4LogicalVolume(filter1S, fFilterMat, "Filter1");

   G4VisAttributes *visAttributes = new G4VisAttributes(G4Colour::Cyan());
   visAttributes->SetVisibility(true);
   filter1LV->SetVisAttributes(visAttributes);
   fVisAttributes.push_back(visAttributes);

   return filter1LV;
}

G4LogicalVolume *XPDetectorConstruction::GetFilter2()
{
   G4Tubs *filter2S = new G4Tubs("Filter2", 0., fFilter2D / 2., fFilter2T / 2., 0., CLHEP::twopi);
   G4LogicalVolume *filter2LV = new G4LogicalVolume(filter2S, fFilterMat, "Filter2");

   G4VisAttributes *visAttributes = new G4VisAttributes(G4Colour::Cyan());
   visAttributes->SetVisibility(true);
   filter2LV->SetVisAttributes(visAttributes);
   fVisAttributes.push_back(visAttributes);

   return filter2LV;
}

G4LogicalVolume *XPDetectorConstruction::GetBlock()
{
   //G4double blockD = 20.*mm;
   G4double blockD = (fBottomD + fFilter1D) / 2.;
   G4Tubs *bulkS = new G4Tubs("Bulk", 0., blockD / 2., fBlockT / 2., 0., CLHEP::twopi);
   G4Trap *cutS = new G4Trap("Cut", blockD, fBlockT, blockD, blockD - fBlockT * tan(0.003));
   G4ThreeVector cutPos(0., blockD / 2. + fBlockEdge, 0.); // hardcoding
   G4RotationMatrix *cutRot = new G4RotationMatrix();
   cutRot->rotateY(90.*deg);
   cutRot->rotateZ(90.*deg);
   G4IntersectionSolid *blockS = new G4IntersectionSolid("Block", bulkS, cutS, cutRot, cutPos);
   G4LogicalVolume *blockLV = new G4LogicalVolume(blockS, fFilterMat, "Block");

   G4VisAttributes *visAttributes = new G4VisAttributes(G4Colour::Magenta());
   visAttributes->SetVisibility(true);
   blockLV->SetVisAttributes(visAttributes);
   fVisAttributes.push_back(visAttributes);

   return blockLV;
}

G4LogicalVolume *XPDetectorConstruction::GetScreen()
{
   G4Tubs *screenS = new G4Tubs("Screen", 0., fScreenD / 2., fScreenT / 2., 0., CLHEP::twopi);
   G4LogicalVolume *screenLV = new G4LogicalVolume(screenS, fScreenMat, "Screen");

   G4VisAttributes *visAttributes = new G4VisAttributes(G4Colour::Yellow());
   visAttributes->SetVisibility(true);
   screenLV->SetVisAttributes(visAttributes);
   fVisAttributes.push_back(visAttributes);

   return screenLV;
}

G4LogicalVolume *XPDetectorConstruction::GetMirror()
{
   //G4double mirrorSize = 150.*mm;
   G4double mirrorW = 150.*mm;
   G4double mirrorL = 150.*mm;
   G4Box *mirrorS = new G4Box("mirror", mirrorW / 2., mirrorL / 2., fMirrorT / 2.);
   G4LogicalVolume *mirrorLV = new G4LogicalVolume(mirrorS, fMirrorMat, "Mirror");
   
   G4Box *coatingS = new G4Box("coating", mirrorW / 2., mirrorL / 2., fCoatingT / 2.);
   G4LogicalVolume *coatingLV = new G4LogicalVolume(coatingS, fCoatingMat, "Coating");
   G4ThreeVector coatingPos(0., 0., fMirrorT / 2. - fCoatingT / 2.);
   new G4PVPlacement(nullptr, coatingPos, coatingLV, "Coating", mirrorLV,
                     false, 0, fCheckOverlap);

   G4VisAttributes *visAttributes = new G4VisAttributes(G4Colour::Yellow());
   visAttributes->SetVisibility(true);
   mirrorLV->SetVisAttributes(visAttributes);
   fVisAttributes.push_back(visAttributes);

   return mirrorLV;
}

G4LogicalVolume *XPDetectorConstruction::GetMagnet()
{
   G4double yokeT = 10.*mm;
   G4double magnetH = fMagnetGap + 2 * yokeT;
   G4double magnetW = 150.*mm;
   G4Box *magnetS = new G4Box("magnet", magnetW / 2., magnetH / 2., fMagnetL / 2.);
   G4LogicalVolume *magnetLV = new G4LogicalVolume(magnetS, fMagnetMat, "Magnet");
   
   G4Box *cutS = new G4Box("cut", magnetW / 2. - yokeT, magnetH / 2. - yokeT, fMagnetL / 2.);
   G4LogicalVolume *cutLV = new G4LogicalVolume(cutS, fVacuumMat, "Cut");
   G4ThreeVector cutPos(0., 0., 0.);
   new G4PVPlacement(nullptr, cutPos, cutLV, "Cut", magnetLV,
                     false, 0, fCheckOverlap);

   G4VisAttributes *visAttributes = new G4VisAttributes(G4Colour::Red());
   visAttributes->SetVisibility(true);
   magnetLV->SetVisAttributes(visAttributes);
   fVisAttributes.push_back(visAttributes);

   visAttributes = new G4VisAttributes(G4Colour::Red());
   visAttributes->SetVisibility(true);
   cutLV->SetVisAttributes(visAttributes);
   fVisAttributes.push_back(visAttributes);

   return magnetLV;
   
}

void XPDetectorConstruction::ConstructSDandField()
{
   // Sensitive Detectors
   G4VSensitiveDetector *screenSD = new XPScreenSD("ScreenSD", "ScreenHC");
   G4SDManager::GetSDMpointer()->AddNewDetector(screenSD);
   
   G4VSensitiveDetector *mirrorSD = new XPMirrorSD("MirrorSD", "MirrorHC");
   G4SDManager::GetSDMpointer()->AddNewDetector(mirrorSD);
   
   G4LogicalVolumeStore *lvStore = G4LogicalVolumeStore::GetInstance();
   for(auto &&lv: *lvStore){
      if(lv->GetName().contains("Screen"))
         SetSensitiveDetector(lv->GetName(), screenSD);
      if(lv->GetName().contains("Coating")) // Mirror is better?
         SetSensitiveDetector(lv->GetName(), mirrorSD);
   }

   ConstructBField();
}

void XPDetectorConstruction::ConstructBField()
{
   // Create magnetic field and set it to Tube using the function
   XPMagneticField *magneticField = new XPMagneticField();
   fMagnetLV->SetFieldManager(magneticField->GetFieldManager(), true); 

   G4FieldManager *fieldManager = new G4FieldManager();
   fieldManager->SetDetectorField(magneticField);
   fieldManager->CreateChordFinder(magneticField);
   G4bool forceToAllDaughters = true;
   fMagnetLV->SetFieldManager(fieldManager, forceToAllDaughters);

   // Register the field and its manager for deleting
   G4AutoDelete::Register(magneticField);
   G4AutoDelete::Register(fieldManager);
}
