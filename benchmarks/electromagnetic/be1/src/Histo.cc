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
//---------------------------------------------------------------------------
//
// ClassName:   Histo - Generic histogram/ntuple manager class
//
//
// Author:      V.Ivanchenko 30.10.03
//
//----------------------------------------------------------------------------
//

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

#include "Histo.hh"

#ifdef G4ANALYSIS_USE

#include "AIDA/AIDA.h"
#include "HistoMessenger.hh"

#endif

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

Histo::Histo()
{
  verbose    = 0;
  histName   = "testem9";
  histType   = "hbook";
  nHisto     = 0;
  defaultAct = 1;
  tupleName  = "tuple9";
  tupleId    = "100";
  tupleList  = "";
  ntup = 0;
  messenger  = 0;

#ifdef G4ANALYSIS_USE
  tree = 0;
  af   = 0; 
  messenger = new HistoMessenger(this);
#endif
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

Histo::~Histo()
{
#ifdef G4ANALYSIS_USE
  delete messenger;
  delete af;
#endif
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void Histo::book()
{
#ifdef G4ANALYSIS_USE
  G4cout << "### Histo books " << nHisto << " histograms " << G4endl;
  // Creating the analysis factory
  if(!af) af = AIDA_createAnalysisFactory();
  // Creating the tree factory
  AIDA::ITreeFactory* tf = af->createTreeFactory();

  // Creating a tree mapped to a new hbook file.

  G4String nam = histName + "." + histType;

  tree = tf->create(nam,histType,false,true,"uncompress");
  delete tf;
  if(tree) {
    G4cout << "Tree store  : " << tree->storeName() << G4endl;
  } else {
    G4cout << "ERROR: Tree store " << histName  << " is not created!" << G4endl;
    return;
  }
  // Creating a histogram factory, whose histograms will be handled by the tree
  AIDA::IHistogramFactory* hf = af->createHistogramFactory( *tree );

  // Creating an 1-dimensional histograms in the root directory of the tree
  for(G4int i=0; i<nHisto; i++) {
    if(active[i]) 
      histo[i] = hf->createHistogram1D(ids[i], titles[i], bins[i], xmin[i], xmax[i]);
  }
  delete hf;
  // Creating a tuple factory, whose tuples will be handled by the tree
  if(tupleList != "") {
     AIDA::ITupleFactory* tpf = af->createTupleFactory( *tree );
     ntup = tpf->create(tupleId, tupleName, tupleList);
     delete tpf;
  }
#endif
} 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void Histo::save()
{
#ifdef G4ANALYSIS_USE
  // Write histogram file
  if(tree) {
    tree->commit();
    G4cout << "Closing the tree..." << G4endl;
    tree->close();
    G4cout << "Histograms and Ntuples are saved" << G4endl;
    delete tree;
    tree = 0;
  }
#endif
} 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void Histo::add1D(const G4String& id, const G4String& name, G4int nb, 
                  G4double x1, G4double x2, G4double u)
{
  if(verbose > 0) {
    G4cout << "New histogram will be booked: #" << id << "  <" << name 
           << "  " << nb << "  " << x1 << "  " << x2 << "  " << u 
           << G4endl;
  }
  nHisto++;
  x1 /= u;
  x2 /= u;
  active.push_back(defaultAct);
  bins.push_back(nb);
  xmin.push_back(x1);
  xmax.push_back(x2);
  unit.push_back(u);
  ids.push_back(id);
  titles.push_back(name);
  histo.push_back(0);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void Histo::setHisto1D(G4int i, G4int nb, G4double x1, G4double x2, G4double u)
{
  if(i>=0 && i<nHisto) {
    if(verbose > 0) {
      G4cout << "Update histogram: #" << i  
             << "  " << nb << "  " << x1 << "  " << x2 << "  " << u 
             << G4endl;
    }
    bins[i] = nb;
    xmin[i] = x1;
    xmax[i] = x2;
    unit[i] = u;
  } else {
    G4cout << "Histo::setHisto1D: WARNING! wrong histogram index " << i << G4endl;
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void Histo::fill(G4int i, G4double x, G4double w)
{
  if(verbose > 1) {
    G4cout << "fill histogram: #" << i << " at x= " << x 
           << "  weight= " << w
           << G4endl;   
  }
#ifdef G4ANALYSIS_USE  
  if(!tree) return;
  if(i>=0 && i<nHisto) {
    if(active[i]) histo[i]->fill((x/unit[i]), w);
  } else {
    G4cout << "Histo::fill: WARNING! wrong histogram index " << i << G4endl;
  }
#endif
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void Histo::scale(G4int i, G4double x)
{
  if(verbose > 0) {
    G4cout << "Scale histogram: #" << i << " by factor " << x << G4endl;   
  }
#ifdef G4ANALYSIS_USE  
  if(!tree) return;
  if(i>=0 && i<nHisto) {
    histo[i]->scale(x);
  } else {
    G4cout << "Histo::scale: WARNING! wrong histogram index " << i << G4endl;
  }
#endif
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void Histo::addTuple(const G4String& w1, const G4String& w2, const G4String& w3)
{
  tupleId = w1;
  tupleName = w2;
  tupleList = w3;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void Histo::fillTuple(const G4String& parname, G4double x)
{
  if(verbose > 1) {
    G4cout << "fill tuple by parameter <" << parname << "> = " << x << G4endl; 
  }
#ifdef G4ANALYSIS_USE  
  if(ntup) ntup->fill(ntup->findColumn(parname), (float)x);
#endif
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void Histo::addRow()
{
#ifdef G4ANALYSIS_USE
  if(ntup) ntup->addRow();
#endif
} 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void Histo::setFileName(const G4String& nam) 
{
  histName = nam;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void Histo::setFileType(const G4String& nam) 
{
  if(nam == "root" || nam == "hbook" || nam == "aida") histType = nam;
  else if(nam == "xml" || nam == "XML") histType = "aida";
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

