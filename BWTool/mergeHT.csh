#!/bin/tcsh

set DET       = $1

mkdir -p merged

foreach Dec (`echo $DET`)
  foreach PU (NoPileUp 140PileUp)
    foreach HT (1 2  3 4 5 6 7 )
      foreach pro (B W Z H Photon MetDiMuon MetDiEle \
        Wev Wmv Wtv Wlv Whad SysWmv SysWev  \
          Zee Zmm Ztt Zll Zvv Zhad SysZee SysZmm \
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

foreach Dec (`echo $DET`)
  foreach PU (NoPileUp 140PileUp)
    foreach pro (Wino100 Wino200 Wino500)
      HTadd ${pro}_14TeV_${PU}_${Dec}.root ${pro}_14TeV_*_${PU}_${Dec}.root
      mv ${pro}_14TeV_*_${PU}_${Dec}.root merged/
    end
  end
end
