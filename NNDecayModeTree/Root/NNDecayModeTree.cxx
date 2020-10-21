#include "NNDecayModeTree/NNDecayModeTree.h"

#include "AsgTools/MessageCheck.h"
#include "EventLoop/Job.h"
#include "EventLoop/StatusCode.h"
#include "EventLoop/IWorker.h"

#include "xAODEventInfo/EventInfo.h"
#include "xAODTau/TauJetContainer.h"
#include "xAODPFlow/PFOContainer.h"

using std::vector;

ClassImp(NNDecayModeTree)

NNDecayModeTree::NNDecayModeTree()
    : m_outputPath(std::string("test.root"))
{
}

NNDecayModeTree::~NNDecayModeTree()
{
  delete m_chargedPFO_pt;
  treeDM = nullptr;
}

EL::StatusCode NNDecayModeTree::setupJob(EL::Job &job)
{
  job.useXAOD();
  return EL::StatusCode::SUCCESS;
}

EL::StatusCode NNDecayModeTree::histInitialize()
{
  fileDM = wk()->getOutputFile("TREE");
  treeDM = new TTree("tree", "flat tree");
  treeDM->SetDirectory(fileDM);
  treeDM->SetMaxTreeSize(500'000'000);
  treeDM->SetAutoSave(0);

  // initialize all branches and vectors
  initAll();

  return EL::StatusCode::SUCCESS;
}

EL::StatusCode NNDecayModeTree::fileExecute()
{
  return EL::StatusCode::SUCCESS;
}

EL::StatusCode NNDecayModeTree::initialize()
{
  return EL::StatusCode::SUCCESS;
}

EL::StatusCode NNDecayModeTree::execute()
{
  ANA_CHECK_SET_TYPE(EL::StatusCode);

  // reset all values
  resetAll();

  // EventInfo
  const xAOD::EventInfo *ei = nullptr;
  ANA_CHECK(evtStore()->retrieve(ei, "EventInfo"));

  // TauJets
  const xAOD::TauJetContainer *taus = nullptr;
  ANA_CHECK(evtStore()->retrieve(taus, "TauJets"));

  const xAOD::PFOContainer *chargedPFO = nullptr;
  ANA_CHECK(evtStore()->retrieve(chargedPFO, "TauChargedParticleFlowObjects"));

  m_mcEventNumber = ei->mcEventNumber();

  for (const auto& tau : *taus)
  {
    std::clog << "-> " << counter++ << "-th tau\n";
    std::clog << "nChargedPFOs  :  " << tau->nChargedPFOs() << '\n';
    std::clog << "nNeutralPFOs  :  " << tau->nNeutralPFOs() << '\n';
    std::clog << "nShotPFOs     :  " << tau->nShotPFOs() << '\n';
    m_chargedPFO_pt->push_back(tau->pt());


    /// This causes crash, ElementLink operator*() doesn't work
    // static SG::AuxElement::Accessor< xAOD::TauJet::PFOLinks_t > chargedPFOAcc( "chargedPFOLinks" );

    // for (std::size_t i = 0; i < tau->nChargedPFOs(); i++) 
    // {
    //   std::clog << "  pt ChargedPFO = " << (*chargedPFOAcc(*tau)[i])->pt() << '\n';
    // }
    treeDM->Fill();
  }

  return EL::StatusCode::SUCCESS;
}

EL::StatusCode NNDecayModeTree::postExecute()
{
  return EL::StatusCode::SUCCESS;
}

EL::StatusCode NNDecayModeTree::finalize()
{
  return EL::StatusCode::SUCCESS;
}

EL::StatusCode NNDecayModeTree::histFinalize()
{
  if (fileDM)
  {
    // EL OutputStream does this for me
    // fileDM->Write();
    // fileDM->Close();
    // delete fileDM;
    fileDM = nullptr;
  }

  return EL::StatusCode::SUCCESS;
}

EL::StatusCode NNDecayModeTree::changeInput(bool /*firstFile*/)
{
  return EL::StatusCode::SUCCESS;
}

void NNDecayModeTree::resetAll()
{
  m_mcEventNumber = (unsigned long long)0;
  m_chargedPFO_pt->clear();
}

void NNDecayModeTree::initAll()
{
  treeDM->Branch("TauJets.mcEventNumber", &m_mcEventNumber);
  m_chargedPFO_pt = new vector<float>();
  treeDM->Branch("ChargedPFO.pt", &m_chargedPFO_pt);
}
