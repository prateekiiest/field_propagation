//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
// $Id: NTSTFieldMessenger.hh,v 1.2 2007-10-26 09:51:28 gcosmo Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

#ifndef NTSTFieldMessenger_h
#define NTSTFieldMessenger_h 1

#include "globals.hh"
#include "G4UImessenger.hh"

class NTSTFieldSetup;
class G4UIdirectory;
class G4UIcmdWithAString;
class G4UIcmdWithAnInteger;
class G4UIcmdWithADouble;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithoutParameter;


class NTSTFieldMessenger: public G4UImessenger
{
  public:
    NTSTFieldMessenger(NTSTFieldSetup* );
   ~NTSTFieldMessenger();
    
    void SetNewValue(G4UIcommand*, G4String);
    void SetNewValue(G4UIcommand*, G4int);
    
  private:

    NTSTFieldSetup*             fEMfieldSetup;
    
    G4UIdirectory*             NTSTfieldDir;

    G4UIcmdWithAnInteger*      StepperCmd;
    G4UIcmdWithAnInteger*      FieldNameCmd;
    G4UIcmdWithADouble* GradFieldCmd;
    G4UIcmdWithADouble* MaxEpsilonCmd;
    G4UIcmdWithADouble* MinEpsilonCmd;
    G4UIcmdWithADoubleAndUnit* MinStepCmd;
    G4UIcmdWithoutParameter*   UpdateCmd;
    G4UIcmdWithoutParameter*   UpdateStepperCmd;
    G4UIcmdWithoutParameter*   fieldStat;
    G4UIcmdWithoutParameter*   chordFinderStat;
};

#endif
