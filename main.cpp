// Project UID db1f506d06d84ab787baf250c265e24e
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <map>
#include <cmath>



using namespace std;


vector<vector<string>> input_csv(istream&);
void debug_func(bool , pair<map<string, map<string,double>>,
map<string, map<string,double>>> &,  map<string,double> &);
double prob_calc(string , const vector<string> &,
pair<map<string, map<string,double>>, map<string, map<string,double>>> &, 
map<string,double> &);
double prob_calc_one_word(string , string ,  pair<map<string, map<string,double>>,
map<string, map<string,double>>> &,  map<string,double> &);

int main(int argc, char * argv[]) {
cout.precision(3);
bool debug = false;


if(argc<3||argc>4){
    cout << "Usage: main.exe TRAIN_FILE TEST_FILE [--debug]" << endl;return 1;}
string temp = "";

if(argc==4){temp = argv[3];if(temp!="--debug"){
    cout << "Usage: main.exe TRAIN_FILE TEST_FILE [--debug]" << endl;return 1;}
else{
    debug = true;
}}
string training_file = argv[1];
string testing_file  = argv[2];

//TRAINING

ifstream fi;
fi.open(training_file);
if(!fi.is_open()){cout << "Error opening file: " << training_file << endl; return 1;}
auto parsed = input_csv(fi);parsed.erase(parsed.begin(),parsed.begin()+1);
map<string, map<string,double>> vocab;
map<string, map<string,double>> categories;
map<string,double> cat_counts;


if(debug) cout << "training data:"<<endl;
int i = 0;
for(auto & val : parsed){ //input all parsed into vocab
    stringstream ss;
    ++i;
    ++cat_counts[val[2]];
    ss << val[3];
    string a;
    map<string, double> words_to_add; // impliments to stop duplicating additions
    while(ss>>a) {words_to_add[a]=1;}
    for(auto m : words_to_add){++(vocab[m.first][val[2]]); 
    ++(categories[val[2]][m.first]);}
    if(debug) cout << "  label = " << val[2] << ", content = " << val[3] << endl;
}
cat_counts["total"] = i;
cout<<"trained on " << i << " examples" << endl;
if(debug)cout<<"vocabulary size = " << vocab.size()<<endl;
cout<<endl;

pair<map<string, map<string,double>>,
map<string, map<string,double>>> bundle = {vocab, categories};

debug_func(debug,bundle,cat_counts); // prints all log probs for each category

fi.close();



//Testing

fi.open(testing_file);
if(!fi.is_open()){cout << "Error opening file: " << testing_file << endl; return 1;}
parsed = input_csv(fi);parsed.erase(parsed.begin(),parsed.begin()+1);
cout << "test data:" << endl;
i = 0;
int correct  = 0;
for(auto & val : parsed){ //input all parsed into vocab
    stringstream ss;
    ss << val[3];
    string a;
    ++i;

    vector<string> words;
    while(ss>>a) words.push_back(a);


    double lowest = -1000;
    string winer = "";
    for(auto & cat : cat_counts){
        double temp = prob_calc(cat.first,words,bundle,cat_counts);
        if(temp>lowest){
            winer = cat.first;
            lowest = temp;
        }
    }
    cout << "  correct = " << val[2] << ", predicted = " << winer 
    << ", log-probability score = " << lowest << endl;
    cout <<"  content = " <<val[3] << endl<<endl;
    if(val[2]==winer) ++correct;
}

cout << "performance: " <<correct<<" / "<<i<<" posts predicted correctly"<<endl;


}








void debug_func(bool debug_me, pair<map<string, map<string,double>>,
map<string, map<string,double>>> &bundle,  map<string,double> &cat_counts){
    if(!debug_me) return;

    cout<<"classes:"<<endl;
    for(auto n : cat_counts){
        if(n.first=="total") continue;
        cout <<"  "<<n.first<<", "<<n.second<<" examples, log-prior = "<<
        log(n.second/cat_counts["total"])<<endl;
    }


    cout << "classifier parameters:"<<endl;
    for(auto n : bundle.second){
        for(auto w : n.second){
            cout<<"  "<<n.first<<":"<<w.first<<
            ", count = "<<w.second<<", log-likelihood = " 
            <<prob_calc_one_word(n.first,w.first,bundle,cat_counts)<<endl;
            //if(n.first=="euchre"&&w.first=="the"){exit(1);}
        }
    }
    cout<<endl;
}
double add_up_counts(map<string,double> m){
    unsigned int t = 0;
    for(auto n : m){
        t+= n.second;
    }
    return t;
}

double prob_calc_one_word(string category,string word,pair<map<string,
map<string,double>>,map<string, map<string,double>>> &bundle, 
map<string,double> &cat_counts){
    if(add_up_counts(bundle.first[word])==0) {
        return log(1/cat_counts["total"]);
        }// if word doesnt show up in training
    if(bundle.second[category][word]==0) {
        return log(add_up_counts(bundle.first[word])/cat_counts["total"]); 
    } //shows up in training but not in this category
    //cout<<" "<<bundle.second[category][word]/cat_counts[category]<<" ";
    return log(bundle.second[category][word]/cat_counts[category]);
    
}


double prob_calc(string category, const vector<string> &words,
 pair<map<string, map<string,double>>,map<string, map<string,double>>> &bundle, 
 // can make more efficent by generating a map to refer to for all words
 map<string,double> &cat_counts){
    double total = log(cat_counts[category]/cat_counts["total"]);
   
    for(string word : words){
        total+=prob_calc_one_word(category,word,bundle,cat_counts);
    }

    return total;

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
