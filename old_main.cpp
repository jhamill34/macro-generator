#include <iostream> // cin and cout 
#include <fstream>	// file input and output
#include <string>	// 
#include <sstream>
#include <vector>
#include <regex>

using namespace std;

vector<string> tokenize(const string &str, char delim){
	vector<string> tokens;
	stringstream ss(str);
	string temp;

	while(getline(ss, temp, delim)){
		tokens.push_back(temp);
	}

	return tokens;
}


int main(int argc, char * argv[]){
	ifstream inputfile;
	string part;
	vector<string> tokens;

	inputfile.open(argv[1]);
	if(!inputfile){
		cout << "File does not exist" << endl;
	}else{
		while(inputfile >> part){
			tokens = tokenize(part, ' ');

			int i;
			for(i = 0; i < tokens.size(); i++){
				cout << tokens[i] << endl;
			}
		}
	}

	return 0;
}

