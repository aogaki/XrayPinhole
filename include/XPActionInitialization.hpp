#ifndef XPActionInitialization_h
#define XPActionInitialization_h 1

#include <G4VUserActionInitialization.hh>
#include <globals.hh>


class XPActionInitialization : public G4VUserActionInitialization
{
public:
   XPActionInitialization();
   virtual ~XPActionInitialization();

   virtual void BuildForMaster() const;
   virtual void Build() const;

private:
      
};

#endif
