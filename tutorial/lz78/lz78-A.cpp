#include <iostream>
#include <map>
#include <fstream>
#include <vector>
#include <sstream>

#include <string.h>
#include <time.h>

#include <libcdsBasics.h>
#include <Array.h>
#include <cppUtils.h>
#include <BitSequence.h>
#include <BitSequenceRG.h>


using namespace std;
using namespace cds_utils;
using namespace cds_static;


//CARGA ARCHIVO
char *LoadFile(const char *name, uint *content_length) {
  ifstream in(name);
  if (!in.good()) {
    return NULL;
  }

  in.seekg(0, ios_base::end);
  size_t length = in.tellg();
  in.seekg(0, ios_base::beg);

  *content_length = length;
  char *ret = loadValue<char>(in, *content_length);
  in.close();
  return ret;
}

//CUENTA LOS PARRAFOS QUE CONTENGAN EL PATRON

int suma( vector<int> pattern, int pos,  Array *prev){
	int occ=0;
	if (pos == 0) {
		return 0;
	}
	for( int i = 0; i < pattern.size(); i++) {
	 	if(pattern[i]==pos)
			occ++;
		}
	pos--;			
	return occ+suma(pattern, prev->getField(pos),prev);	
}

int suma1( vector<int> pattern, int pos,  Array *prev){
	int occ=0;
	if (pos == 0) {
		return 0;
	}
	for( int i = 0; i < pattern.size(); i++) {
	 	if(pattern[i]==pos){
			occ++;
			return 1;
		}
		}			
	return 0;	
}

//LOCATE DE UNA POSICION
int locate1(int pos,Array *prev)
{
	if (pos == 0) {
		return 1;
	}
	pos--;
	return 1+locate1(prev->getField(pos),prev);
}

int locate(int i,Array *prev)
{
	int k=0;	
	for (int j = 0; j < i-1; j++) 
		k+=locate1(prev->getField(j),prev);  	
	return k;
}

int lzbus(const char* pattern,size_t patternlenght, Array *prev, Array *news) 
{
	
	map <int  ,vector<int> > map1;
	int occ=0; 
	
	//GENERA LOS PREFIJOS Y SE GUARDAN EN MAP1
	
	for(int k=0;k<patternlenght;k++)	map1[k];		
	//RECORRE TODO EL INDEX			
					
	for (int i = 0; i < prev->getLength(); i++) {
		
		//SE REVISA SI LA LETRA DEL PARRAFO ES IGUAL A LA ULTIMA LETRA DE LOS SUFIJOS
		//cout<<i+1<<" ("<<prev->getField(i)<<","<<(unsigned char)news->getField(i)<<")"<<endl;
		for(int k=patternlenght; k>0 ;k--){			
			if((unsigned char)news->getField(i)== pattern[k-1])
			{				
						if( k-1==0)
						{					
							map1[k-1].push_back(i+1);
							break;						
						}
						map<int, vector<int> >::iterator map2;						
						map2=map1.find(k-2);			
						for(int r=0; r < map2->second.size(); r++)
						{
							if(	prev->getField(i)	==	map2->second[r]	)
							{
								map1[k-1].push_back(i+1);
								if( k-1 == (patternlenght-1))	{occ++;}
								break;							
							}						
						}						
			}			
		}

		occ+=suma1(map1.find(patternlenght-1)->second,prev->getField(i),prev);
												
	}
	
	/*for (map<int, vector<int> >::iterator it = map1.begin(); it != map1.end(); ++it) {  
		for(unsigned int i = 0; i < it->second.size(); i++) {
			cout << it->first <<"->"<< it->second[i]  << endl;
		}   
	}*/
	
	
						
	return occ;
}



int main(int argc, char **argv) {
	float tiempo=0;	
	clock_t t1;
	
	if (argc != 5) {
		cout << "usage: " << argv[0] << "<input-index> <output> <pattern-file> <pattern-lenght>" << endl;
		return 0;
	}
	char *name_input = argv[1];
	char *name_output = argv[2];

	ifstream in(name_input);
	if (!in.good()) {
		cerr << "Error reading input file" << endl;
		return 1;
	}

	Array *prev = new Array(in);
	Array *news = new Array(in);

	in.close();
	
	char *name_input1 = argv[3];
	uint size = 0;
	char *texto = LoadFile(name_input1, &size);
	
	uint m;
	{
    stringstream ss;
    ss << argv[4];
    ss >> m;
	}

	char * pattern = new char[m+1];
	int pos=0;
	
	while(pos!=size){	
		
		for(uint k=0;k<m;k++)
			  pattern[k] = texto[pos+k];
		 pattern[m] = 0;
		cout << pattern <<"|"<<m<< endl;


		t1 = clock();
		cout<<lzbus(pattern,m, prev, news)<<endl;
		t1 = clock()-t1;
		
		tiempo += ((float)t1)/CLOCKS_PER_SEC;
		cout<<"Tiempo promedio(pos/(m+1)):"<< tiempo/(pos/(m+1)) <<endl;
		pos+=m+1; 
	}
	cout<<"Patron Largo:"<<m<<endl;
	cout<<"Tiempo promedio(10000):"<< tiempo/10000 <<endl;
    cout<<"Tiempo promedio(pos/(m+1)):"<< tiempo/(pos/(m+1)) <<endl;
	
	
	return 0;
}
