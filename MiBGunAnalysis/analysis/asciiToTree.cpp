#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <cctype>
#include <algorithm>

#include "AndCommon.h"

#include "TFile.h"
#include "TTree.h"
#include "TString.h"




int main( int argc, char* argv[] ) {

  if( argc != 2 ) {

    std::cout << "USAGE: ./asciiToTree [filename]" << std::endl;
    exit(1);

  }

  std::string fullFileName(argv[1]);
  std::string dataset = AndCommon::removePathAndSuffix(fullFileName);
  std::string fileName = dataset + ".dat";
  std::string pathAndFile = "./data/" + fileName;


  std::ifstream fs(pathAndFile.c_str());
  if( !fs.good() ) {
    std::cout << "-> No file called '" << fileName << "' found in './data/'. Exiting." << std::endl;
    exit(1);
  }

  std::cout << "-> Opened ascii data file: " << pathAndFile << std::endl;

  std::string outfileName = "./data/" + dataset + ".root";;
  TFile* outfile = TFile::Open( outfileName.c_str(), "recreate" );
  TTree* tree = new TTree( "tree", "" );


  int ev=-1;
  int nch;
  float base     ;
  float amp      ;
  float charge   ;
  float letime   ;
  float tetime   ;
  float ratecount;
  float pshape   [1024];
  float sampling_p=-1;  

  tree->Branch( "ev"        , &ev        , "ev/I"            );
  tree->Branch( "nch"       , &nch       , "nch/I"           );
  tree->Branch( "base"      , &base      , "base/F"          );
  tree->Branch( "amp"       , &amp       , "amp/F"           );
  tree->Branch( "charge"    , &charge    , "charge/F"        );
  tree->Branch( "letime"    , &letime    , "letime/F"        );
  tree->Branch( "tetime"    , &tetime    , "tetime/F"        );
  tree->Branch( "ratecount" , &ratecount , "ratecount/F"     );
  tree->Branch( "pshape"    , pshape     , "pshape[1024]/F"  );
  tree->Branch( "sampling_p", &sampling_p, "sampling_p/F"    );


  std::string line;
  bool wasReadingEvent = false;
  bool readyForPulseShape = false;
  int ch = -1;


  if( fs.good() ) {

    std::cout << "-> Starting parsing file." << std::endl;
    nch=0;

    while( getline(fs,line) ) {
      auto it = std::unique(line.begin(), line.end(),[](char const &lhs, char const &rhs) {
                return (lhs == rhs) && (lhs == ' ');
            }); 
      line.erase(it, line.end());
      std::string delimiter = " ";
      size_t pos = 0;
      std::vector<std::string> words;
      std::string word;
      while ((pos = line.find(delimiter)) != std::string::npos) {
        word = line.substr(0, pos);
        line.erase(0, pos + delimiter.length());
        words.push_back(word);
      }

      if( words[0]=="===" && words[1]=="DATA" && words[2]=="SAMPLES") {
        sampling_p    = atof(words[15].c_str());  
      }

      if( words[0]=="===" && words[1]=="EVENT" && wasReadingEvent ) {

        if( ev % 1000 == 0 ) std::cout << "   ... analyzing event: " << ev << std::endl;

        tree->Fill();
 
        nch = 0;
        ch = -1;
        wasReadingEvent = false;

      } else if( words[0]=="===" && words[1]=="CH:" ) {

        wasReadingEvent = true;
        readyForPulseShape = true;

        nch += 1;

        ch            = atoi(words[2].c_str());
        base          = atof(words[8].c_str());
        amp           = atof(words[11].c_str());
        charge        = atof(words[14].c_str());
        letime        = atof(words[17].c_str());
        tetime        = atof(words[20].c_str());
        ratecount     = atof(words[23].c_str());

      } else if( readyForPulseShape && ch>=0 ) {
  
        for( unsigned i=0; i<words.size(); ++i ) 
          pshape[i] = atof(words[i].c_str());

        readyForPulseShape = false;
   
      }

      if( words[0]=="===" && words[1]=="EVENT" && wasReadingEvent==false) {
        ev = atoi(words[2].c_str());
      }

    } // while get lines

  } // if file is good

  if( wasReadingEvent )
    {
      std::cout << "   ... analyzing event: " << ev << std::endl;
      tree->Fill();
    }

  fs.close();

  tree->Write();
  outfile->Close();

  std::cout << "-> Tree saved in: " << outfile->GetName() << std::endl;

  return 0;

}
