//used for /20191029/NOISE/
// also /20191113/clampone_RCtime/NEW/
// also /20191113/clampone_RCtime/OLD/

#include "ScanDirectory.h"
#include "croc_noise.h"
void noiseDir() {
	TString dir="20191113/NOISE_ALL/"; // Noise_10kHz Noise_50kHz Noise_200kHz_fastTS
	TString extension = ".txt";
	int Nfiles;
	TString* filenameList = ScanDirectory(dir,extension,Nfiles);
	cout<<"# Number of files found = "<<Nfiles<<endl;

	for(int i=0; i<Nfiles; i++) {
		TString filename=filenameList[i];
		//cout<<"# Filename: "<<filename<<endl;
		//if(filename=="G30_O120_ALL_20191029_170556.txt") {
		
		// NEW gain=63
		//if(filename=="G63_O120_noIn_ALL_NEW_20191113_142300.txt") {
		// OLD gain=63
		if(true) {
			croc_noise(filename);
		}
	}

}