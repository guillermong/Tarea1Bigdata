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
	for( int i = 1; i < pattern.size(); i++) {
	 	if(pattern[i]==pos)
			occ++;
		}
	pos--;			
	return occ+suma(pattern, prev->getField(pos),prev);	
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
	
	map <const char * ,vector<int> > map1;
	vector<const char *> a;
	vector<int> loc;
	int pos=0,occ=0; 
	
	//GENERA LOS PREFIJOS Y SE GUARDAN EN MAP1
	
	for(int k=0;k<=patternlenght;k++){
			char * p = new char[k];
			for(int j=0;j<=k;j++)
			{
				  p[j] = pattern[j];
			 }
			p[k] = 0;
			a.push_back(p);
			map1[a[k]].push_back(-2);
	}		
	//RECORRE TODO EL INDEX			
					
	for (int i = 0; i < prev->getLength(); i++) {
		
		//SE REVISA SI LA LETRA DEL PARRAFO ES IGUAL A LA ULTIMA LETRA DE LOS SUFIJOS
		//cout<<i+1<<" ("<<prev->getField(i)<<","<<(unsigned char)news->getField(i)<<")"<<endl;
		for(int k=1;k<=patternlenght;k++){
			if((unsigned char)news->getField(i)==a[k][k-1])
			{					
					if(a[k][k-1]=='h')
					{						
						map1[a[k]].push_back(i+1);						
					}else 
					{
						map<const char*, vector<int> >::iterator map2;						
						map2=map1.find(a[k-1]);	
										
						for(int r=1;r<map2->second.size();r++)
							{
							if(	prev->getField(i)	==	map2->second[r]	)
							{
								map1[a[k]].push_back(i+1);
								if( strcmp( a[k] , pattern ) == 0 )
								{
									occ++;	
								}							
							}
						
						}
					}			
			}
		}		
		occ+=suma(map1.find(a[patternlenght])->second,prev->getField(i),prev);
	}		
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
		lzbus(pattern,m, prev, news);
		t1 = clock()-t1;
		
		tiempo += ((float)t1)/CLOCKS_PER_SEC;
		pos+=m+1; 
	}
	cout<<"Patron Largo:"<<m<<endl;
	cout<<"Tiempo promedio(10000):"<< tiempo/10000 <<endl;
    cout<<"Tiempo promedio(pos/(m+1)):"<< tiempo/(pos/(m+1)) <<endl;
	
	
	return 0;
}
