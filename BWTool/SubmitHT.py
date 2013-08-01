#!/usr/bin/env python
# coding: utf-8

import os
import re
import shutil

DelDir    = '/uscms/home/benwu/work/CMSUpgrade/Delphes'
DelExe    = 'DelFill'
Directory = 'TEST_5_30'
UserEMAIL = 'benwu@fnal.gov'
PileUps   = [
    'NoPileUp',
    #'50PileUp',
    #'140PileUp',
]
Projects  = [
    #'Wino200_14TeV',
    #'Wino500_14TeV',
    #'WJETS_13TEV',
    #'ZJETS_13TEV',
    #'TTBAR_13TEV',
    #'B_14TEV_HT1' ,
    #'BJ_14TEV_HT1',
    #'BJ_14TEV_HT2',
    #'BJ_14TEV_HT3',
    #'BJ_14TEV_HT4',
    #'BJ_14TEV_HT5',
    #'BJ_14TEV_HT6',
    #'BJ_14TEV_HT7',
    #'BJJ_14TEV_HT1',
    #'BJJ_14TEV_HT2',
    #'BJJ_14TEV_HT3',
    #'BJJ_14TEV_HT4',
    #'BJJ_14TEV_HT5',
    #'TT_14TEV_HT1',
    #'TT_14TEV_HT2',
    #'TT_14TEV_HT3',
    #'TT_14TEV_HT4',
    #'TT_14TEV_HT5',
    
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ 33TEV ~~~~~
    #'B_33TEV_HT1',
    #'BJ_33TEV_HT1',
    #'BJ_33TEV_HT2',
    #'BJ_33TEV_HT3',
    #'BJ_33TEV_HT4',
    #'BJ_33TEV_HT5',
    #'BJ_33TEV_HT6',
    #'BJ_33TEV_HT7',
    #'BJJ_33TEV_HT1',
    #'BJJ_33TEV_HT2',
    #'BJJ_33TEV_HT3',
    #'BJJ_33TEV_HT4',
    #'BJJ_33TEV_HT5',
    #'TT_33TEV_HT1',
    #'TT_33TEV_HT2',
    #'TT_33TEV_HT3',
    #'TT_33TEV_HT4',
    #'TT_33TEV_HT5',
    #'TT_33TEV_HT6',

    ## Below are the files haven't been consider yet
    #'BB_14TEV_HT1' ,
    #'BB_14TEV_HT2' ,
    #'BB_14TEV_HT3' ,
    #'BB_14TEV_HT4' ,
    #'BB_14TEV_HT5' ,
    #'BBB_14TEV_HT1',
    #'BBB_14TEV_HT2',
    #'BBB_14TEV_HT3',
    #'H_14TEV_HT1'  ,
    #'H_14TEV_HT2'  ,
    #'H_14TEV_HT3'  ,
    #'H_14TEV_HT4'  ,
    #'LL_14TEV_HT1' ,
    #'LL_14TEV_HT2' ,
    #'LL_14TEV_HT3' ,
    #'LL_14TEV_HT4' ,
    #'LL_14TEV_HT5' ,
    #'LL_14TEV_HT6' ,
    #'LLB_14TEV_HT1',
    #'LLB_14TEV_HT2',
    #'LLB_14TEV_HT3',
    #'TB_14TEV_HT1' ,
    #'TB_14TEV_HT2' ,
    #'TB_14TEV_HT3' ,
    #'TB_14TEV_HT4' ,
    #'TB_14TEV_HT5' ,
    #'TJ_14TEV_HT1' ,
    #'TJ_14TEV_HT2' ,
    #'TJ_14TEV_HT3' ,
    #'TJ_14TEV_HT4' ,
    #'TJ_14TEV_HT5' ,

#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ 33TEV ~~~~~
    #'BB_33TEV_HT1',
    #'BB_33TEV_HT2',
    #'BB_33TEV_HT3',
    #'BB_33TEV_HT4',
    #'BB_33TEV_HT5',
    #'BBB_33TEV_HT1',
    #'BBB_33TEV_HT2',
    #'BBB_33TEV_HT3',
    #'BBB_33TEV_HT4',
    #'H_33TEV_HT1',
    #'H_33TEV_HT2',
    #'H_33TEV_HT3',
    #'H_33TEV_HT4',
    #'H_33TEV_HT5',
    #'LL_33TEV_HT1',
    #'LL_33TEV_HT2',
    #'LL_33TEV_HT3',
    #'LL_33TEV_HT4',
    #'LL_33TEV_HT5',
    #'LLB_33TEV_HT1',
    #'LLB_33TEV_HT2',
    #'LLB_33TEV_HT3',
    #'TB_33TEV_HT1',
    #'TB_33TEV_HT2',
    #'TB_33TEV_HT3',
    #'TB_33TEV_HT4',
    #'TB_33TEV_HT5',
    #'TJ_33TEV_HT1',
    #'TJ_33TEV_HT2',
    #'TJ_33TEV_HT3',
    #'TJ_33TEV_HT4',
    #'TJ_33TEV_HT5',
]


def my_process():
    ## Some checking first
    my_CheckFile()

    ## Create the output directory
    outdir = DelDir + "/" + Directory
    try:
        os.mkdir(outdir)
    except OSError:
        pass

    ## Update RunHT.csh with DelDir and pileups
    RunHTFile = outdir + "/" + "RunHT.csh"
    PUtoRun = '"'
    for pu in PileUps:
        PUtoRun += "%s " % pu
    PUtoRun += '"'

    with open(RunHTFile, "wt") as outfile:
        for line in open("RunHT.csh", "r"):
            line = line.replace("PILEUPS", PUtoRun)
            line = line.replace("DELDIR", DelDir)
            line = line.replace("DELEXE", DelExe)
            outfile.write(line)

    ## Update condor files
    shutil.copy2("Delphes_condor", outdir)
    os.chdir(outdir)
    os.system("tar -czf FileList.tgz ../FileList")
    for pro in Projects:
        cond_file = pro + "_condor"
        print cond_file
        with open(cond_file, "wt") as out:
            for line in open("Delphes_condor", "r"):
                line = line.replace("USER@FNAL.GOV", UserEMAIL)
                line = line.replace("ProcessName", pro)
                line = line.replace("Tag_JetEta_JetPt", Directory)
                out.write(line)

        os.system("condor_submit " + cond_file)


def my_CheckFile():
    ## Check the Delphes Dir
    if os.path.isdir(DelDir):
        pass
    else:
        print "Please input the path to Delphes"
        quit()


    ## Check RunHT.csh file
    if os.path.isfile("RunHT.csh") and os.access("RunHT.csh", os.X_OK):
        #print "Found RunHT.csh"
        pass
    else:
        print "Please locate RunHT.csh"
        quit()

    ## Check DelFill to be execute
    DelFill = DelDir + "/" + DelExe
    if os.path.isfile(DelFill) and os.access(DelFill, os.X_OK):
        #print "Found DelFill"
        pass
    else:
        print "Please locate %s" % DelFill
        quit()

    ## Check HTadd
    if os.path.isfile("HTadd") and os.access("HTadd", os.X_OK):
        #print "Found HTadd"
        pass
    else:
        print "Please compile HTadd"
        return None

    ## Check Delphes_condor
    if os.path.isfile("Delphes_condor"):
        #print "Found HTadd"
        pass
    else:
        print "Please compile HTadd"
        quit()

if __name__ == "__main__":
    my_process()
