#!/usr/bin/env python
import argparse
import os
import sys


def get_args():
    parser = argparse.ArgumentParser()
    parser.add_argument("directory",
                        help="Directory containing the samples (as downloaded with rucio)")
    parser.add_argument("pattern",
                        help="Pattern to select samples from `directory`")
    parser.add_argument("-o", "--outfile", metavar="OUTFILE",
                        default="DMtree.root",
                        help="Output ntuple name")
    parser.add_argument("-d", metavar="DIR", default="DMtreeSubmit",
                        help="Algorithm submit directory")
    parser.add_argument("-n", metavar="NUM", type=int,
                        help="Number of events to run")
    parser.add_argument("-e", "--eldriver", metavar="DRIVER", default="Direct", 
                        help="EventLoop driver")

    return parser.parse_args()


if __name__ == "__main__":
    args = get_args()

    # Check if submit dir already exists
    if os.path.exists(args.d):
        print("Submit directory already exists - aborting")
        sys.exit(1)

    import ROOT
    ROOT.gROOT.Macro("$ROOTCOREDIR/scripts/load_packages.C")
    ROOT.xAOD.Init().ignore()

    # Search for samples
    sh = ROOT.SH.SampleHandler()

    ROOT.SH.ScanDir() \
           .samplePattern(args.pattern) \
           .samplePostfix("_output.root") \
           .scan(sh, args.directory)

    sh.setMetaString("nc_tree", "CollectionTree")
    sh.setMetaDouble(ROOT.EL.Job.optEventsPerWorker, 1000000)
    sh.printContent()

    # Setup job
    job = ROOT.EL.Job()
    job.sampleHandler(sh)

    if args.n:
        job.options().setDouble(ROOT.EL.Job.optMaxEvents, args.n)

    alg = ROOT.NNDecayModeTree()
    alg.SetName("NNDecayModeTree")

    # Common options
    alg.m_outputName = args.outfile

    job.algsAdd(alg)

    job.outputAdd(ROOT.EL.OutputStream('TREE'))

    if args.eldriver == "Direct":
         driver = ROOT.EL.DirectDriver()
         driver.submit(job, args.d)
    elif args.eldriver == "Condor":
        driver = ROOT.EL.CondorDriver()
        driver.shellInit = "export ATLAS_LOCAL_ROOT_BASE=/cvmfs/atlas.cern.ch/repo/ATLASLocalRootBase && source ${ATLAS_LOCAL_ROOT_BASE}/user/atlasLocalSetup.sh"
        job.options().setDouble(ROOT.EL.Job.optFilesPerWorker, 2)
        driver.submitOnly(job, args.d)
    else:
        raise ValueError
