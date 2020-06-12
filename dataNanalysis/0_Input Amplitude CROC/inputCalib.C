void inputCalib() {
	gStyle->SetOptFit(1111);	

int N=8;
double setvalue[N];
setvalue[0] = 1;
setvalue[1] = 3;
setvalue[2] = 5;
setvalue[3] = 7;
setvalue[4] = 9;
setvalue[5] = 10;
setvalue[6] = 11;
setvalue[7] = 15;

double measured[N];
measured[0] = 0.989;
measured[1] = 2.965;
measured[2] = 4.936;
measured[3] = 6.907;
measured[4] = 8.881;
measured[5] = 9.899;
measured[6] = 10.884;
measured[7] = 14.842;
	

TGraphErrors* graph = new TGraphErrors(N);
for(int i=0; i<N; i++) {
	graph->SetPoint(i,setvalue[i],measured[i]);
	graph->SetPointError(i,0.0, 0.013);
}



		TCanvas* cgraph = new TCanvas("cgraph","input_amplitude_calibration", 800,600);
		cgraph->Draw();
		TPad * pad1 = new TPad("pad1" , "pad1" , 0,0 ,1.,0.3) ; 
		pad1->Draw() ;
		//cgraph->cd() ; 
		TPad * pad2 = new TPad("pad2" , "pad2" , 0,0.3 ,1.,1.) ; 
		pad2->SetBottomMargin(0.02);
		pad2->Draw() ;
		//cgraph->cd() ;
		pad2->cd() ;

		pad2->SetGrid();
		pad2->SetRightMargin(0.05);
		graph->SetTitle("Input amplitude calibration");
		graph->GetYaxis()->SetTitle("input amplitude measured (ADU)");
		graph->GetXaxis()->SetLabelSize(0);
		//graph->GetYaxis()->SetRangeUser(0, 131000);
		graph->SetMarkerStyle(20);
		graph->Draw("AP");
		graph->Fit("pol1");

		gPad->Update();
		TPaveStats *params = (TPaveStats*)graph->FindObject("stats");
			params->SetX1NDC(0.7);
			params->SetX2NDC(0.99);
			params->SetY1NDC(0.1);
			params->SetY2NDC(0.3);
double offset = graph->GetFunction("pol1")->GetParameter(0);
double slope  = graph->GetFunction("pol1")->GetParameter(1);
double offsetErr = graph->GetFunction("pol1")->GetParError(0);
double slopeErr  = graph->GetFunction("pol1")->GetParError(1);

TGraphErrors* residuals = new TGraphErrors(N);
double ymean,fitErr, yErr;
for(int i=0; i<N; i++) {
	ymean=slope*setvalue[i]+offset;
	fitErr = sqrt( pow(setvalue[i]*slopeErr,2) + pow(offset,2) );
	//cout<< setprecision(2) <<yErr<<endl;
	residuals->SetPoint(i, setvalue[i], measured[i] - ymean);
	yErr = sqrt( pow(fitErr,2) + pow(0.013,2) );
	residuals->SetPointError(i, 0.0, 0.013);
	//residuals->SetPointError(i, 0.0, yErr);
}


		pad1->cd() ;
		pad1->SetGrid();
		pad1->SetTopMargin(0.01);
		pad1->SetRightMargin(0.05);
		pad1->SetBottomMargin(0.2);
		//pad1->SetLeftMargin(0.14);
		residuals->SetTitle("");
		residuals->SetMarkerStyle(20);
		residuals->GetXaxis()->SetTitle("input amplitude set (mV)");
		residuals->GetXaxis()->SetTitleSize(0.09);
		residuals->GetXaxis()->SetLabelSize(0.09);

		residuals->GetYaxis()->SetTitleSize(0.09);
		residuals->GetYaxis()->SetTitle("residuals (ADU)");
		residuals->GetYaxis()->SetTitleOffset(0.4);
		residuals->GetYaxis()->SetLabelSize(0.08);
		
		residuals->Draw("AP");
		gStyle->SetOptFit(0);	
		residuals->Fit("pol0","Q","");

//	cgraph->SaveAs("input_amplitude_calibration.pdf","");
//	cgraph->SaveAs("input_amplitude_calibration.png","");



}

