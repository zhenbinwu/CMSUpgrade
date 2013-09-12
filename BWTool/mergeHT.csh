#!/bin/tcsh

mkdir -p merged
foreach Dec (PhaseI PhaseII3 PhaseII4)
  foreach PU (NoPileUp 140PileUp)
    foreach HT (1 2  3 4 5 6 7 )
      foreach pro (B W Z H Photon MetDiMuon MetDiEle \
        Wev Wmv Wtv Wlv Whad SysWmv SysWev  \
          Zee Zmm Ztt Zll Zvv Zhad SysZee SysZmm \
          Wino100 Wino200 Wino500 \
          EleEle MuMu NvNv TauTau  LL Lep SysEE SysMM \
          TT TTFLep TTHad  TTSLep)
        foreach B ('' J JJ)
          #echo  ${pro}${B}_14TEV_HT${HT}_${PU}_${Dec}.root 
          #if (`ls|wc` != 0) then
            HTadd ${pro}${B}_14TEV_HT${HT}_${PU}_${Dec}.root ${pro}${B}_14TEV_HT${HT}_*_${PU}_${Dec}.root
            mv ${pro}${B}_14TEV_HT${HT}_*_${PU}_${Dec}.root merged/
          #endif
        end
      end
    end
  end
end
