#!/bin/env python
# Example PBS cluster job submission in Python

import os
from popen2 import popen2
import time
import glob
import re


###############################
DelDir    = '/afs/cern.ch/work/b/benwu/CMSUpgrade/Delphes/'
DelExe    = 'DelFill'
Directory = 'TEST_5_30'
UserEMAIL = 'benwu@fnal.gov'
PileUps   = [
    #'NoPileUp',
    #'50PileUp',
    '140PileUp',
]

Detector = [
    #'PhaseI',
    'PhaseII3',
    #'PhaseII4',
]

Projects  = [
    #'Wino100_14TeV',
    #'Wino200_14TeV',
    #'Wino500_14TeV',
    #'WJETS_13TEV',
    #'ZJETS_13TEV',
    #'TTBAR_13TEV',
    'B_14TEV_HT1' ,
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
    
##~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ 33TEV ~~~~~
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

def BSUB():
    for pu in PileUps:
        for dec in Detector:
            for pro in Projects:
                for splitpro in SplitPro(dec, pu, pro):
                    cms = 'bsub '
                    cms += ' -q 8nh '
                    cms += "%s/%s/RunCERN.csh " % (DelDir, Directory)
                    cms += "%s " % splitpro
                    cms += "%s " % Directory
                    cms += "%s " % pu
                    cms += "%s " % dec
                    print cms
                    stat = os.popen(cms).read()
                    print stat

def my_CheckFile():
    ## Check the Delphes Dir
    if os.path.isdir(DelDir):
        pass
    else:
        print "Please input the path to Delphes"
        quit()


    ## Check RunHT.csh file
    if os.path.isfile("RunCERN.csh") and os.access("RunCERN.csh", os.X_OK):
        #print "Found RunCERN.csh"
        pass
    else:
        print "Please locate RunCERN.csh"
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

def SplitPro(detector, pileup, pro):
    globout=glob.glob('FileList/%s/%s*%s.list'  % (detector, pro, pileup))
    testout=[]
    for out in globout:
        file = out.split('/')[-1]
        #print file
        match = re.match(r'(%s.*)_%s\.list' % (pro, pileup), file)
        #match = re.match('%s*%s' % (pro, pileup), file)
        if match != None:
            #print match.group(0)
            print match.group(1)
            testout.append(match.group(1))
    return testout

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
    RunHTFile = outdir + "/" + "RunCERN.csh"
    #with open(RunHTFile, "wt") as outfile:
    outfile = open(RunHTFile, "wt")
    for line in open("RunCERN.csh", "r"):
        line = line.replace("DELDIR", DelDir)
        line = line.replace("DELEXE", DelExe)
        outfile.write(line)
    os.system("chmod 755 %s" % RunHTFile)

    ## Update condor files
    os.system("cp mergeHT.csh %s" % outdir)
    os.chdir(DelDir)
    os.system("tar -czf FileList.tgz ./FileList")
    os.system("mv FileList.tgz %s" % outdir)
    os.chdir(outdir)
    print "Untaring Filelist ....... "
    os.system("tar -xzf FileList.tgz")
    BSUB()

if __name__ == "__main__":
    my_process()

