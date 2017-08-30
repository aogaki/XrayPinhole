#include <TFile.h>

#include <G4SystemOfUnits.hh>
#include <G4AutoLock.hh>
#include <G4ParticleTable.hh>
#include <G4ChargedGeantino.hh>
#include <G4IonTable.hh>
#include <Randomize.hh>
#include <g4root.hh>

#include "XPPrimaryGeneratorAction.hpp"

static G4int nEveInPGA = 0;
G4Mutex mutexInPGA = G4MUTEX_INITIALIZER;


XPPrimaryGeneratorAction::XPPrimaryGeneratorAction()
   : G4VUserPrimaryGeneratorAction(),
     fParticleGun(nullptr)
{
   fParticleGun = new G4ParticleGun(1);

   fEnergy = 10.*keV;
   fDivergence = G4ThreeVector(0., 0., 1.);
   
   auto particleTable = G4ParticleTable::GetParticleTable();
   auto particle = particleTable->FindParticle("gamma");
   fParticleGun->SetParticleDefinition(particle);
   fParticleGun->SetParticlePosition(G4ThreeVector(0., 0., 0.));
   fParticleGun->SetParticleEnergy(fEnergy);
   fParticleGun->SetParticleMomentumDirection(fDivergence);
}

XPPrimaryGeneratorAction::~XPPrimaryGeneratorAction()
{
   delete fParticleGun;
}

void XPPrimaryGeneratorAction::GeneratePrimaries(G4Event *event)
{
   GenEnergy();
   fParticleGun->SetParticleEnergy(fEnergy);

   GenDivergence();
   fParticleGun->SetParticleMomentumDirection(fDivergence);

   fParticleGun->GeneratePrimaryVertex(event);

   G4AutoLock lock(&mutexInPGA);
   if (nEveInPGA++ % 10000 == 0)
      G4cout << nEveInPGA - 1 << " events done" << G4endl;
}

void XPPrimaryGeneratorAction::GenDivergence()
{
   G4double theta = acos(1. - G4UniformRand() * (1. - cos(20.*deg)));
   G4double phi = G4UniformRand() * 2. * CLHEP::pi;
   G4double vx = sin(theta) * cos(phi);
   G4double vy = sin(theta) * sin(phi);
   G4double vz = cos(theta);
   fDivergence = G4ThreeVector(vx, vy, vz);
}

void XPPrimaryGeneratorAction::GenEnergy()
{
   //fEnergy = (100. * G4UniformRand())*MeV;
   fEnergy = CLHEP::RandExponential::shoot(20)*MeV;
}
