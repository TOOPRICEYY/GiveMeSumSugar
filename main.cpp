// Project UID db1f506d06d84ab787baf250c265e24e
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <map>


using namespace std;


vector<vector<string>> input_csv(istream&);

int main(int argc, char * argv[]) {
cout.precision(3);
//bool debug = false;


if(argc<3||argc>4){
    cout << "Usage: main.exe TRAIN_FILE TEST_FILE [--debug]" << endl;return 1;}
string temp = "";

if(argc==4){temp = argv[3];if(temp!="--debug"){
    cout << "Usage: main.exe TRAIN_FILE TEST_FILE [--debug]" << endl;return 1;}}
else{
    //debug = true;
}
string training_file = argv[1];
string testing_file  = argv[2];

//TRAINING

ifstream fi;
fi.open(training_file);
if(!fi.is_open()){cout << "Error opening file: " << training_file << endl; return 1;}
auto parsed = input_csv(fi);

parsed.erase(parsed.begin(),parsed.begin()+1);

std::map<string, std::map<string,double>> categories;

for(auto & val : parsed){
    stringstream ss;
    ss << val[3];
    string a;
    while(ss>>a) ++(categories[val[2]][a]);
}

//cout << categories["euchre"]["the"] << endl;

fi.close();





fi.open(testing_file);
if(!fi.is_open()){cout << "Error opening file: " << testing_file << endl; return 1;}

//

}



vector<vector<string>> input_csv(istream &os){
    vector<vector<string>> vec;
    string s;

    while(getline(os, s)){
        string temp = "";
        vector<string> temp_vec;
        for(unsigned int i = 0; i<s.length(); i++){
            if(s.at(i)==','){
                temp_vec.push_back(temp);
                //cout << temp << " ";
                temp = "";
            }else{
                temp+=s.at(i);
            }

        }
        if(temp!="") temp_vec.push_back(temp);
        //cout << temp << endl;
        vec.push_back(temp_vec);
    }
    return vec;
}
