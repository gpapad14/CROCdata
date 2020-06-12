void CreateTXTfilesCROC(TString filename, TString dir, int ichan, TString ichanSTR, TString& inDir, int Nexclude) {
			// Find where the data you want start.
			ifstream inp;		
			inp.open(dir+filename);
			//cout<<dir+filename<<endl;

			//----------------------------------------
			// Get the number of samples there are
			int Nraw=0;
			double x;
			while(inp>>x) {	Nraw++; }
			if( !(Nraw%4==0) ) { cout<<"ERROR ! ! ! Number of raw data not multiple of 4."<<endl; }
			int Nmeasur = Nraw/4;
			//cout<<Nraw<<", "<< Nmeasur<<endl;
			inp.close();

		//=======================================================

			inp.open(dir+filename);
			//----------------------------------------
			// Exclude the first "Nexclude" samples.
			for(int j=0; j<Nexclude; j++) { inp>>x; } 
			int Nclean = Nmeasur - Nexclude;

			//----------------------------------------	
			// Create the new .txt file with only the samples that you want.
			TString nameFile = "Ch"+ichanSTR+"_"+filename;
			//nameFile += ".txt"; // .txt ending exists already in the filename.
			// Write the .txt file
			inDir+="/";
			ofstream outp(dir+inDir + nameFile);
			outp<<Nclean<<std::endl; // First element in the .txt file is the number of measurements.
			
			int Npoints=0;
			for(int i=0; i<Nraw; i++) {
				inp>>x;
				if(i%4==ichan) {
					if(Npoints>=Nexclude) {
						//cout<<i<<endl;
						outp << x << std::endl;
						//cout<<x<<endl;
					}
					Npoints++;
				}

			}

			outp.close();

		//----------------------------------------
		// Close the original file.
			inp.close();
	
}