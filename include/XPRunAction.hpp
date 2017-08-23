#ifndef XPRunAction_h
#define XPRunAction_h 1

#include <G4UserRunAction.hh>
#include <G4Run.hh>


class XPRunAction: public G4UserRunAction
{
public:
   XPRunAction();
   virtual ~XPRunAction();

   virtual void BeginOfRunAction(const G4Run *);
   virtual void EndOfRunAction(const G4Run *);

private:

};

#endif
