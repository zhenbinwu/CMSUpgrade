#!/bin/tcsh

#mkdir -p merged
foreach Dec (PhaseI PhaseII3 PhaseII4)
  foreach PU (NoPileUp 140PileUp)
    foreach HT (1 2  3 4 5 6 7 )
      foreach pro (Zvv ZvvJ ZvvJJ Wlv WlvJ WlvJJ  \
        Wev WevJ WevJJ  Wmv WmvJ WmvJJ Wtv WtvJ WtvJJ  \
          SysZmm SysZmmJ SysZmmJJ SysZee SysZeeJ SysZeeJJ \
            SysWev SysWevJ SysWevJJ SysWmv SysWmvJ SysWmvJJ \
              EleEle MuMu NvNv TauTau  LL Lep SysEE SysMM \
              TT TTFLep TTHad  TTSLep)
              #echo  ${pro}_14TEV_HT${HT}_${PU}_${Dec}.root 
              HTadd ${pro}_14TEV_HT${HT}_${PU}_${Dec}.root ${pro}_14TEV_HT${HT}_*_${PU}_${Dec}.root
              mv ${pro}_14TEV_HT${HT}_*_${PU}_${Dec}.root merged
      end
    end
  end
end
