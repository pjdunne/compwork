{
  int ninputs=4;
  double meantime[ninputs][3];
  double stdev[ninputs][3];
  double efficiency[ninputs];
  double efficiencyerr[ninputs];
  double sysefficiency[ninputs];
  double sysefficiencyerr[ninputs];
  double number[ninputs];
  double numbererr[ninputs];
  for(int i =0;i<ninputs;i++){
    //Open input file
    number[i] = pow(10,i);
    numbererr[i]=0;
    //std::cout<<Number<<std::endl;
    ostringstream convert;
    convert<<number[i];
    string nevents=convert.str();
    std::ifstream inputinfofile((nevents+"eventsoutput.txt").c_str());

    //Read times into vectors
    std::string line;
    std::vector<double> times[3];
    
    int j=0;
    while(std::getline(inputinfofile,line)){
      j++;
      if(j==1)continue;//ignore header line
      std::istringstream iss(line);
      double numofevents,cputime,realtime,systime;
      iss>>numofevents>>cputime>>realtime>>systime;
      times[0].push_back(cputime);
      times[1].push_back(realtime);
      times[2].push_back(systime);
    }

    //calculate mean and std dev
    for(int l=0;l<3;l++){
      double sum=0;
      double sq_sum=0;
      for(int k=0;k<times[l].size();k++){
	sum+=times[l].at(k);
	sq_sum+=times[l].at(k)*times[l].at(k);
      }
      meantime[i][l] = sum / times[l].size();    
      stdev[i][l] = sqrt(sq_sum / times[l].size() - meantime[i][l] * meantime[i][l]);
      string timetype;
      if(l==0)timetype="cpu";
      else if (l==1)timetype="real";
      else if (l==2)timetype="system";
      //std::cout<<"With "<<nevents<<" events average "<<timetype<< " time is: "<<meantime[i][l]<<"+/-"<<stdev[i][l]<<std::endl;
    }
    efficiency[i]=meantime[i][0]/meantime[i][1];
    efficiencyerr[i]=sqrt(pow(stdev[i][0]/meantime[i][0],2)+pow(stdev[i][1]/meantime[i][1],2));
    sysefficiency[i]=meantime[i][0]/meantime[i][2];
    sysefficiencyerr[i]=sqrt(pow(stdev[i][0]/meantime[i][0],2)+pow(stdev[i][2]/meantime[i][2],2));
  }

  TCanvas c1;
  c1.SetLogx();  
  TGraphErrors *efficiencygraph = new TGraphErrors(ninputs,&number[0],&efficiency[0],&numbererr[0],&efficiencyerr[0]);
  efficiencygraph->GetXaxis()->SetTitle("No. of events processed");
  efficiencygraph->GetYaxis()->SetTitle("CPU time/Real time");
  efficiencygraph->GetYaxis()->SetTitleOffset(1.5);
  efficiencygraph->SetMarkerColor(4);
  efficiencygraph->SetMarkerStyle(21);
  efficiencygraph->SetLineColor(1);
  efficiencygraph->SetTitle("");
  //efficiencygraph->GetYaxis()->SetRangeUser(0.,1.1);
  efficiencygraph->Draw("ALP");
  c1->SaveAs("realtimeefficiency.pdf");

  TCanvas c2;
  c2.SetLogx();  
  TGraphErrors *sysefficiencygraph = new TGraphErrors(ninputs,&number[0],&sysefficiency[0],&numbererr[0],&sysefficiencyerr[0]);
  sysefficiencygraph->GetXaxis()->SetTitle("No. of events processed");
  sysefficiencygraph->GetYaxis()->SetTitle("CPU time/Real time");
  sysefficiencygraph->GetYaxis()->SetTitleOffset(1.5);
  sysefficiencygraph->SetMarkerColor(4);
  sysefficiencygraph->SetMarkerStyle(21);
  sysefficiencygraph->SetLineColor(1);
  sysefficiencygraph->SetTitle("");
  //sysefficiencygraph->GetYaxis()->SetRangeUser(0.,1.1);
  sysefficiencygraph->Draw("ALP");
  c2->SaveAs("systimeefficiency.pdf");
}
