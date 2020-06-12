#import "ScanDirectory.h"
#import "CreateTXTfilesCROC.h"

void croc_separateChannels() {

	TString dir="20191023/Ch0/Bandwidth_onTop_10kHz/"; // Noise_10kHz Noise_50kHz Noise_200kHz_fastTS
	TString extension = ".txt";
	int Nfiles;
	TString* filenameList = ScanDirectory(dir,extension,Nfiles);

	bool RecreateTXTfiles=true;

	int Nexclude=100;

//===========================================================
// Create basic .txt files with the same name as the original files and store them in directory "inFolder".
	TString inDir0 = "Ch0";
	TString inDir1 = "Ch1";
	TString inDir2 = "Ch2";
	TString inDir3 = "Ch3";
	// Check if the folder exists. If it exists (=-1) then the .txt files also exist. gSystem->mkdir(inFolder) == 0 / -1
	if( RecreateTXTfiles && gSystem->mkdir(dir+inDir0) == -1 ) { 
		cout<< "$ Deleting folder . . . DONE"<<endl;; 
		gSystem->Exec("rm -r "+dir+inDir0);
		gSystem->Exec("rm -r "+dir+inDir1);
		gSystem->Exec("rm -r "+dir+inDir2);
		gSystem->Exec("rm -r "+dir+inDir3); 
	}
	if( gSystem->mkdir(dir+inDir0) == 0 && gSystem->mkdir(dir+inDir1) == 0 && gSystem->mkdir(dir+inDir2) == 0 && gSystem->mkdir(dir+inDir3) == 0 ) {
		int ifile;
		for(ifile=0; ifile<Nfiles; ifile++) {
		//for(ifile=1; ifile<2; ifile++) {
			TString filename=filenameList[ifile];
			//cout<<filename<<endl;
			CreateTXTfilesCROC(filename, dir, 0, "0", inDir0, Nexclude);
			//CreateTXTfilesCROC(filename, dir, 1, "1", inDir1, Nexclude);
			//CreateTXTfilesCROC(filename, dir, 2, "2", inDir2, Nexclude);
			//CreateTXTfilesCROC(filename, dir, 3, "3", inDir3, Nexclude);
			cout<<"$ Create .txt files "<< filename<<". . . DONE"<<endl;
		}
		cout<<"$ Create .txt files. . . COMPLETED"<<endl;
	}
	

}