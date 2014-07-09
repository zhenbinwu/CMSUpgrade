#!/bin/env python
# Example PBS cluster job submission in Python

import os
import time
import glob
import re
import subprocess


###############################
RunProxy  = True
DelDir    = '/home/benwu/CMSUpgrade_TP/Delphes'
DelExe    = 'DelFill'
Directory = '/home/benwu/CMSUpgrade_TP/Output/Sbottom300'
Analysis  = 'DM_5_30'
UserEMAIL = 'benwu@fnal.gov'
Detectors = [
    'Snowmass',
    #'PhaseI',
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
    ##'WJETS_13TEV',
    ##'ZJETS_13TEV',
    ##'TTBAR_13TEV',
    'Sbottom300_2p_14TeV', 
    'Sbottom300_QCD0_14TeV', 
    'Sbottom300_QED0_14TeV',    
    'Sbottom300_QCD2QED2_14TeV', 

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

def QSUB(Analysis, Process, Pileup, Detector):

    # Open a pipe to the qsub command.
    #output, input = popen2('echo')
    p = subprocess.Popen('qsub', shell=True, stdin=subprocess.PIPE, stdout=subprocess.PIPE, close_fds=True)
    output, input = (p.stdout, p.stdin)

    job_name = "%s_%s_%s_%s" % (Analysis, Process, Detector, Pileup)

    walltime = "168:00:00"
    processors = "nodes=1:ppn=1"

    command1 = "%s/%s %s %s %s %s" % (DelDir, DelExe, Pileup, Process, Analysis, Detector)


    job_string = """#!/bin/tcsh
    #PBS -N %s
    #PBS -l walltime=%s
    #PBS -l %s
    #PBS -d %s
    #PBS -o ./%s_stdout
    #PBS -e ./%s_stderr
    date
    cd %s/..
    source setup.csh
    setenv X509_USER_PROXY ~/.x509_user_proxy
    cd $PBS_O_WORKDIR
    pwd
    %s
    date""" % (job_name, walltime, processors, Directory, job_name, job_name, DelDir, command1)

    # Send job_string to qsub
    input.write(job_string)
    input.close()

    # Print your job and the response to the screen
    print job_string
    print output.read()

    time.sleep(0.1)


def CheckQStat():
    while True:
        stat = os.popen("qstat -u $USER | wc").read()
        total = stat.split()[0]
        if total == '':
            break
        total = int(total)

        if total != 0 and total % 100 == 0:
            time.sleep(1800)
        else:
            time.sleep(30)

        if total < 300:
            break
        else:
            time.sleep(1800)

        print "Waiting for next submittion : %d" % total


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
      print pro
      for pu in PileUps:
        print pu
        for det in Detectors:
          print det
          for splitpro in SplitPro(det, pu, pro):
            print splitpro
            QSUB(Analysis, splitpro, pu, det)

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


    ## Check current CMSSW setup
    if not os.environ.get("SCRAM_ARCH"):
      print "Please setup Delphes or cmssoft first!"
      quit()

    if RunProxy:
      proxyfile = os.path.expanduser("~/.x509_user_proxy")
      if not os.path.exists(proxyfile) or (time.time() - os.stat(proxyfile).st_ctime) / 60/24 > 1:
        print "Proxy file is at least one day old. Requesting new proxy"
        os.system("voms-proxy-init -valid 168:00 -voms cms; cp $X509_USER_PROXY $HOME/.x509_user_proxy")


if __name__ == "__main__":
    my_process()
