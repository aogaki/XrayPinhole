#include "XPActionInitialization.hpp"
#include "XPPrimaryGeneratorAction.hpp"
#include "XPRunAction.hpp"
#include "XPEventAction.hpp"


XPActionInitialization::XPActionInitialization()
   : G4VUserActionInitialization()
{}

XPActionInitialization::~XPActionInitialization()
{}

void XPActionInitialization::BuildForMaster() const
{
   SetUserAction(new XPRunAction());
}

void XPActionInitialization::Build() const
{
   SetUserAction(new XPPrimaryGeneratorAction());
   SetUserAction(new XPRunAction());
   SetUserAction(new XPEventAction());
}
