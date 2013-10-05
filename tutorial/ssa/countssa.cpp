#include "ssa.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include<string.h>
#include<time.h> 

uchar *LoadFile(const char *name, uint *content_length) {
  ifstream in(name);
  if (!in.good()) {
    return NULL;
  }

  in.seekg(0, ios_base::end);
  size_t length = in.tellg();
  in.seekg(0, ios_base::beg);

  *content_length = length;
 
  uchar *ret = loadValue<uchar>(in, *content_length);
  in.close();
  return ret;
}



int main(int argc, char ** argv) {
	
  float tiempo=0 ;
	clock_t t1;
	
  if(argc!=5) {
    cout << "usage: " << argv[0] << " <text> <index-file> <pattern-file> <pattern-length>" << endl;
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
  
  ifstream ssainput(argv[2]);
  ssa * _ssa = new ssa(ssainput);
  ssainput.close();
  
    uint m;
  {
    stringstream ss;
    ss << argv[4];
    ss >> m;
  }
  
  char *name_input = argv[3];
  uint size = 0;
  uchar *texto = LoadFile(name_input, &size);

	
  uchar * pattern = new uchar[m+1];

  uint pos = 0;
  while(pos!=size){  
	
  
	  for(uint k=0;k<m;k++)
		  pattern[k] = texto[pos+k];
	  pattern[m] = 0;
	 
	 
	  //cout << "pattern :" << pattern <<"|";
	  
	  t1 = clock();
	  uint occ = _ssa->count(pattern,m);
	  t1 = clock() -t1;
	  
	  //cout << "occ:" <<occ << endl;
	  tiempo += ((float)t1)/CLOCKS_PER_SEC;
	  
		pos+=m+1;
  }
  cout<<"Patron Largo:"<<m<<endl;
  cout<<"Tiempo promedio(100000):"<< tiempo/10000 <<endl;
  cout<<"Tiempo promedio(pos/(m+1)):"<< tiempo/(pos/(m+1)) <<endl;

  
  return 0;
}
