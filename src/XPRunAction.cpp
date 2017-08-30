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

