#!/usr/bin/env python
# coding: utf-8

import os


Command = '~/work/CMSUpgrade/Delphes/RunHT.csh'
Directory = 'Test_5_30'
Projects = [
    'Wino200_14TeV', 
    'Wino500_14TeV', 
    'WJETS_13TEV',
    'ZJETS_13TEV',
    'TTBAR_13TEV',
    'B_14TEV_HT1' ,
    'BJ_14TEV_HT1',
    'BJ_14TEV_HT2',
    'BJ_14TEV_HT3',
    'BJ_14TEV_HT4',
    'BJ_14TEV_HT5',
    'BJ_14TEV_HT6',
    'BJ_14TEV_HT7',
    'BJJ_14TEV_HT1',
    'BJJ_14TEV_HT2',
    'BJJ_14TEV_HT3',
    'BJJ_14TEV_HT4',
    'BJJ_14TEV_HT5',
    'TT_14TEV_HT1',
    'TT_14TEV_HT2',
    'TT_14TEV_HT3',
    'TT_14TEV_HT4',
    'TT_14TEV_HT5',
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
]

def my_process():
    pids = set()

    for pro in Projects:
        cmd = '%s %s %s' % (Command, pro, Directory)
        to_run = ('ssh cmslpc-sl5.fnal.gov "%s" > %s_%s.log 2>&1 &' %   \
                  (cmd, Directory, pro))
        print to_run
        os.system(to_run)
        os.system("sleep 30")

if __name__ == "__main__":
    os.system("kinit -r 8d")
    my_process();
