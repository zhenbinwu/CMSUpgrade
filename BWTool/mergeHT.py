#!/usr/bin/env python
# encoding: utf-8

# File        : mergeHT.py
# Author      : Ben Wu
# Contact     : benwu@fnal.gov
# Date        : 2014 Sep 25
#
# Description :


import os
import re
import glob
import multiprocessing
import subprocess



DET = "Snowmass"
pros = dict()

def GetList(filename):
    file = open("Merge_List", "r")
    for line_ in file.readlines():
        line = line_.strip()
        #print line
        out = re.match("(\w+)_14TEV_HT(\d+)_\d*_([^_]*)_%s.root" % DET, line)
        if out != None:
            pros["%s_14TEV_HT%s_%s_%s.root" % (out.group(1), out.group(2), out.group(3), DET)] = [out.group(1), out.group(2), out.group(3), DET]
    return pros

def GetProList(rootlist):
  for line_ in rootlist:
    line = line_.strip()
    #print line
    out = re.match("(\w+)_14TEV_HT(\d+)_\d*_([^_]*)_%s.root" % DET, line)
    if out != None:
      pros["%s_14TEV_HT%s_%s_%s.root" % (out.group(1), out.group(2), out.group(3), DET)] = [out.group(1), out.group(2), out.group(3), DET]
  return pros

def Hadd(pro):
    wildname = "%s_14TEV_HT%s_*_%s_%s.root" % (pros[pro][0], pros[pro][1], pros[pro][2], DET)
    #print "Hadd %s %s; mv %s merged" % (pro, wildname, wildname)
    #subprocess.call("/home/benwu/CMSUpgrade_TP/BWTool/HTadd %s %s" % (pro, wildname), shell=True)
    subprocess.call("/home/benwu/CMSUpgrade_TP/BWTool/HTadd %s %s; mv %s merged" % (pro, wildname, wildname), shell=True)


def multiHadd(pros):
    pool = multiprocessing.Pool(None)
    r = pool.map(Hadd, pros.keys())

if __name__ == "__main__":
    if not os.path.exists("merged"):
        os.mkdir('merged')
    globout=glob.glob('*.root')
    multiHadd(GetProList(sorted(globout)))

