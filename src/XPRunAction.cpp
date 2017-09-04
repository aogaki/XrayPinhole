#include <g4root.hh>

#include "XPRunAction.hpp"


XPRunAction::XPRunAction()
   : G4UserRunAction()
{
   auto anaMan = G4AnalysisManager::Instance();
   //anaMan->SetNtupleMerging(true);
   anaMan->SetVerboseLevel(1);
   G4String fileName = "result";
   anaMan->SetFileName(fileName);
   
   anaMan->CreateNtuple("CsI", "deposit energy information");
   anaMan->CreateNtupleIColumn(0, "EventID");
   anaMan->CreateNtupleIColumn(0, "TrackID");
   anaMan->CreateNtupleIColumn(0, "PDGCode");    
   anaMan->CreateNtupleDColumn(0, "DepositEnergy");
   anaMan->CreateNtupleSColumn(0, "VertexName");

   anaMan->CreateNtupleDColumn(0, "x");
   anaMan->CreateNtupleDColumn(0, "y");
   anaMan->CreateNtupleDColumn(0, "z");

   anaMan->CreateNtuple("Mirror", "incident gamma information");
   anaMan->CreateNtupleIColumn(1, "EventID");
   anaMan->CreateNtupleIColumn(1, "TrackID");
   anaMan->CreateNtupleDColumn(1, "KineticEnergy");

   anaMan->CreateNtupleDColumn(1, "x");
   anaMan->CreateNtupleDColumn(1, "y");
   anaMan->CreateNtupleDColumn(1, "z");

   anaMan->CreateNtupleDColumn(1, "Px");
   anaMan->CreateNtupleDColumn(1, "Py");
   anaMan->CreateNtupleDColumn(1, "Pz");

   anaMan->FinishNtuple();
}

XPRunAction::~XPRunAction()
{
   delete G4AnalysisManager::Instance();
}

void XPRunAction::BeginOfRunAction(const G4Run *)
{
   G4AnalysisManager *anaMan = G4AnalysisManager::Instance();
   anaMan->OpenFile();
}

void XPRunAction::EndOfRunAction(const G4Run *)
{
   G4AnalysisManager *anaMan = G4AnalysisManager::Instance();
   anaMan->Write();
   anaMan->CloseFile();
}

