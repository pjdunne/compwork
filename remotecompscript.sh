#!/bin/bash

evtoproc=$1
niterations=$2
filetoprocess="file:MyOutputFile.root"
outfile=${1}eventsoutput.txt
#filetoprocess="root://cmssrv32.fnal.gov:1094//store/mc/Summer12_DR53X/VBF_HToZZTo4Nu_M-120_8TeV-pythia6/AODSIM/PU_S10_START53_V7A-v1/00000/1C2FB96C-2B11-E211-A8D2-001E67396A8B.root"
printf "%-10s %-10s %-10s %-10s\n" "NEvents" "TotCPUT" "TotRealT" "TotSysT" > $outfile
for (( iteration=1; iteration<=$niterations; iteration++ ))
  do
  #RUN CONFIG
  tstart=`date +%s`
  cmsRun compwork_5_3_7_cfg.py release=53X nevents=$evtoproc filetoproc=$filetoprocess &> tmp.txt #| tee tmp.txt
  tfin=`date +%s`
  #cat exampleoutput.txt > tmp.txt #TEST USING SAMPLE OUTPUT
  
  #GET TIME INFORMATION
  perevcputime=`grep "TimeReport CPU/event =" tmp.txt | awk '{print $4}'`
  perevrealtime=`grep "TimeReport CPU/event =" tmp.txt | awk '{print $7}'`
  nevents=`grep "TrigReport Events total =" tmp.txt | awk '{print $5}'`
  totalcputime=$(expr $perevcputime*$nevents|bc)
  totalrealtime=$(expr $perevrealtime*$nevents|bc)

  
  #cat datetmp.txt | awk '{print $4}' | sed 's/:/ /g' > tmp2.txt
  #cat datetmp2.txt | awk '{print $4}' | sed 's/:/ /g' > tmp3.txt
  #hstart=`cat tmp2.txt|awk '{print $1}'`
  #mstart=`cat tmp2.txt|awk '{print $2}'`
  #sstart=`cat tmp2.txt|awk '{print $3}'`
  #startsec=$(expr 3600*$hstart+60*$mstart+$sstart|bc)
  #hfin=`cat tmp3.txt|awk '{print $1}'`
  #mfin=`cat tmp3.txt|awk '{print $2}'`
  #sfin=`cat tmp3.txt|awk '{print $3}'`
  #endsec=$(expr 3600*$hfin+60*$mfin+$sfin|bc)
  totalsystime=$(expr $tfin-$tstart|bc)
  
  #if [ `cat datetmp.txt | awk '{print $3}'` -ne `cat datetmp.txt | awk '{print $3}'` ]
  #    then
  #    totalsystime=$(expr $totalsystime+86400|bc)
  #fi

  #OUTPUT
  echo Nevents: $nevents, TotCPUt: $totalcputime s, TotRealt: $totalrealtime s, TotSyst: $totalsystime s
  #echo $nevents $totalcputime $totalrealtime $totalsystime >> output.txt
  printf "%-10d %-10f %-10f %-10f\n" "$nevents" "$totalcputime" "$totalrealtime" "$totalsystime" >> $outfile
  rm tmp.txt 
  #rm datetmp.txt datetmp2.txt
done