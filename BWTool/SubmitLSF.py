#!/bin/env python
# Example PBS cluster job submission in Python

import os
import time
import glob
import re
import subprocess


###############################
RunProxy  = True
DelDir    = '/afs/cern.ch/work/b/benwu/CMSUpgrade_TP/Delphes/'
DelExe    = 'DelFill'
Directory = '/afs/cern.ch/work/b/benwu/CMSUpgrade_TP/Output/TestSL6AGAIN'
Analysis  = 'DM_5_30'
UserEMAIL = 'benwu@fnal.gov'
Detectors = [
    #'Snowmass',
    'PhaseI',
    #'PhaseII3',
    #'PhaseII4'
]

PileUps   = [
    'NoPileUp',
    #'50PileUp',
    #'140PileUp',
]

Processes = [
    #'Stopv01_14TeV',
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
    #'LL_14TEV_HT1',
    #'LL_14TEV_HT2',
    #'LL_14TEV_HT3',
    #'LL_14TEV_HT4',
    #'LL_14TEV_HT5',
    #'LL_14TEV_HT6',

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

def BSUB(Analysis, Process, Pileup, Detector):

    # Open a pipe to the qsub command.
    #output, input = popen2('echo')
    p = subprocess.Popen('bsub', shell=True, stdin=subprocess.PIPE, stdout=subprocess.PIPE, close_fds=True)
    output, input = (p.stdout, p.stdin)

    job_name = "%s_%s_%s_%s" % (Analysis, Process, Detector, Pileup)
    command1 = "%s/%s %s %s %s %s" % (DelDir, DelExe, Pileup, Process, Analysis, Detector)
    que = '1nd'

    ## -J Job name
    ## -o output -e error
    ## -q Que:
    ### 8nm (8 minutes)
    ### 1nh (1 hour)
    ### 8nh
    ### 1nd (1day)
    ### 2nd
    ### 1nw (1 week)
    ### 2nw
    job_string = """#!/bin/tcsh
    #BSUB -q %s
    #BSUB -J %s
    #BSUB -o ./%s_stdout
    #BSUB -e ./%s_stderr
    date
    ## Need this to fix the problem with batch node
    setenv LD_LIBRARY_PATH ''
    cd %s/..
    source setup.csh
    cd %s
    %s
    date""" % (que, job_name, job_name, job_name, DelDir, Directory, command1)

    # Send job_string to qsub
    input.write(job_string)
    input.close()

    # Print your job and the response to the screen
    print job_string
    print output.read()

    time.sleep(0.1)

def my_process():
    ## Some checking first
    my_CheckFile()

    ## Create the output directory
    try:
        os.makedirs(Directory)
    except OSError:
        pass

    ## Update condor files
    if not os.path.exists("%s/Filelist.tgz" % Directory):
      print "Copying Filelist to %s" % Directory
      os.system("cd %s; tar -czf %s/Filelist.tgz FileList" % (DelDir, Directory))
      os.chdir(Directory)
      if os.path.exists("Filelist"):
        os.system("rm -rf Filelist")
      os.system("tar -xzf Filelist.tgz")

    for pro in Processes:
      for pu in PileUps:
        for det in Detectors:
          for splitpro in SplitPro(det, pu, pro):
            BSUB(Analysis, splitpro, pu, det)

def SplitPro(detector, pileup, pro):
    globout=glob.glob('%s/FileList/%s/%s*%s.list'  % (Directory, detector, pro, pileup))
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


def my_CheckFile():
    ## Check the Delphes Dir
    if os.path.isdir(DelDir):
        pass
    else:
        print "Please input the path to Delphes"
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
        print "Please compile HTadd, which is needed for merging output"
        pass


if __name__ == "__main__":
    my_process()
