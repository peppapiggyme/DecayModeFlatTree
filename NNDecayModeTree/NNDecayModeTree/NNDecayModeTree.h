#ifndef NNDECAYMODETREE_NNDECAYMODETREE_H
#define NNDECAYMODETREE_NNDECAYMODETREE_H

#include "xAODTau/TauJet.h"
#include "EventLoop/Algorithm.h"

#include <TTree.h>
#include <TFile.h>

#include <vector>

/**
 * This class only works when the element links are available
 * 
 * so far the operate* of element link doesn't work
 * and it is not finished (just two testing variables for now)
 * 
 * > link not in the file?
 * 
 * > Caused by renaming e.g. "TauChargedParticleFlowObjectsFix"?
 *
 * Will add the old approach in another class...
 */ 

class NNDecayModeTree : public EL::Algorithm
{
public:
  NNDecayModeTree();
  ~NNDecayModeTree();

public:
  virtual EL::StatusCode setupJob(EL::Job &job);
  virtual EL::StatusCode fileExecute();
  virtual EL::StatusCode histInitialize();
  virtual EL::StatusCode changeInput(bool firstFile);
  virtual EL::StatusCode initialize();
  virtual EL::StatusCode execute();
  virtual EL::StatusCode postExecute();
  virtual EL::StatusCode finalize();
  virtual EL::StatusCode histFinalize();

  ClassDef(NNDecayModeTree, 1);

private:
  int counter = 0; //!
  void resetAll();
  void initAll();
  std::string m_outputPath;                      //!
  TTree *treeDM = nullptr;                       //!
  TFile *fileDM = nullptr;                       //!
  unsigned long long m_mcEventNumber = 0;        //!
  std::vector<float> *m_chargedPFO_pt = nullptr; //!
};

#endif
