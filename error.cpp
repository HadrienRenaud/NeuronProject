#include "error.h"

bool err(string err, int opt){
	if(opt)
		std::cout << err << std::endl;
	ofstream file(ERROR,ios::app);//ios:app = append = ajouter à la fin du fichier
	if(!file)
		return false;
	file << err << std::endl;
	return true;
}
