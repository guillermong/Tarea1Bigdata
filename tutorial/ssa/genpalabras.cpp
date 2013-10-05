#include "ssa.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>

using namespace std;


int main(int argc, char ** argv) {
  if(argc!=3) {
    cout << "usage: " << argv[0] << " <text> <largopatron>" << endl;
    return 0;
  }

  fstream input(argv[1],ios::in | ios::binary);
  if(!input.is_open()) {
    cout << "Error opening file: " << argv[1] << endl;
    return -1;
  }

  input.seekg(0,ios::end);
  uint n=input.tellg();
  uchar * text = new uchar[n+1];

  input.seekg(0,ios::beg);
  input.read((char*)text,sizeof(uchar)*n);
  input.close();
  //for(uint i=0;i<n;i++) text[i]++;
  text[n] = 0;
  
  uint m;
  {
    stringstream ss;
    ss << argv[2];
    ss >> m;
  }
  
  char * pattern = new char[m+1];
  ofstream out("patrones");
  for(uint i=0;i<1000;i++) {
	  uint pos = rand()%(n-m-1);
	//  while(strcmp(text[pos]," ") && strcmp(text[pos+m-1]," "))
	//		pos = rand()%(n-m-1); 
    for(uint k=0;k<m;k++)
      pattern[k] = text[pos+k];
    pattern[m] = '\n';
    //cout << pattern <<endl;
    out.write(pattern,m+1);
  }
  out.close();
  return 0;
}
