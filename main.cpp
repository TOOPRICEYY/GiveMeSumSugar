// Project UID db1f506d06d84ab787baf250c265e24e
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <map>
#include <cmath>
#include "csvstream.h"



using namespace std;


vector<map<string, string>> input_csv(istream&);
void debug_func(bool , pair<map<string, map<string,int>>,
map<string, map<string,int>>> &,  map<string,int> &);
double prob_calc(string , const vector<string> &,
pair<map<string, map<string,int>>, map<string, map<string,int>>> &, 
map<string,int> &);
double prob_calc_one_word(string , string ,  pair<map<string, map<string,int>>,
map<string, map<string,int>>> &,  map<string,int> &);

class Learn{
    private:
    
        map<string, map<string,int>> vocab;
        map<string, map<string,int>> categories;
        map<string,int> cat_counts;
        pair<map<string, map<string,int>>,
        map<string, map<string,int>>> bundle = {vocab, categories};
        vector<map<string, string>> parsed_training;
        vector<map<string, string>> parsed_testing;
        bool debug =  false;


    public:

    Learn(ifstream &training, ifstream &testing, bool debug): debug(debug){
        parsed_training = input_csv(training);
        parsed_testing = input_csv(testing);
    }

    void train(){
        if(debug) cout << "training data:"<<endl;
        unsigned int i = 0;
        for(auto & val : parsed_training){ //input all parsed into vocab
            stringstream ss;
            ++i;
            ++cat_counts[val["tag"]];
            ss << val["content"];
            string a;
            map<string, int> words_to_add; // impliments to stop duplicating additions
            while(ss>>a) {words_to_add[a]=1;}
            for(auto m : words_to_add){++(vocab[m.first][val["content"]]); 
            ++(categories[val["tag"]][m.first]);}
            if(debug) cout << "  label = " << val["tag"] << 
            ", content = " << val["content"] << endl;
        }
        cat_counts["total"] = i;
        cout<<"trained on " << i << " examples" << endl;
        if(debug)cout<<"vocabulary size = " << vocab.size()<<endl;
        cout<<endl;

        bundle = {vocab, categories};

        debug_func(debug,bundle,cat_counts); // prints all log probs for each category
    }

    void test(){
        cout << "test data:" << endl;
        int i = 0;
        int correct  = 0;
        for(auto & val : parsed_testing){ //input all parsed into vocab
            stringstream ss;
            ss << val["content"];
            string a;
            ++i;

            vector<string> words;
            while(ss>>a) words.push_back(a);


            double lowest = -100000;
            string winer = "";
            for(auto & cat : cat_counts){
                if(cat.first == "total") continue;
                double temp = prob_calc(cat.first,words,bundle,cat_counts);
                if(temp>lowest){
                    winer = cat.first;
                    lowest = temp;
                }
            }
            cout << "  correct = " << val["tag"] << ", predicted = " << winer 
            << ", log-probability score = " << lowest << endl;
            cout <<"  content = " <<val["content"] << endl<<endl;

            if((val["tag"])==winer) ++correct;
        }

        cout << "performance: " <<correct<<" / "<<i<<" posts predicted correctly"<<endl;
    }

};

int main(int argc, char * argv[]) {
cout.precision(3);
bool debug = false;


if(argc<3||argc>4){
    cout << "Usage: main.exe TRAIN_FILE TEST_FILE [--debug]" << endl;return 1;}
string temp = "";

if(argc==4){temp = argv[3];if(temp!="--debug"){
    cout << "Usage: main.exe TRAIN_FILE TEST_FILE [--debug]" << endl;
    debug = true;return 1;}
else{
    debug = true;
}}
string training_file = argv[1];
string testing_file  = argv[2];

//TRAINING

ifstream fi;
ifstream fi2;
fi.open(training_file);
if(!fi.is_open()){cout << "Error opening file: " << training_file << endl; return 1;}
fi2.open(testing_file);
if(!fi.is_open()){cout << "Error opening file: " << testing_file << endl; return 1;}

Learn learn(fi,fi2,debug);

fi.close();
fi2.close();

//for( auto & val : parsed){ cout<<val["tag"]<<", "<<val["content"]<<endl;}

learn.train();
learn.test();
//Testing


}








void debug_func(bool debug_me, pair<map<string, map<string,int>>,
map<string, map<string,int>>> &bundle,  map<string,int> &cat_counts){
    if(!debug_me) return;

    cout<<"classes:"<<endl;
    for(auto n : cat_counts){
        if(n.first=="total") continue;
        cout <<"  "<<n.first<<", "<<n.second<<" examples, log-prior = "<<
        log(double(n.second)/cat_counts["total"])<<endl;
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
double add_up_counts(map<string,int> m){
    double t = 0;
    for(auto n : m){
        t+= n.second;
    }
    return t;
}

double prob_calc_one_word(string category, string word,  
pair<map<string, map<string,int>>,
map<string, map<string,int>>> &bundle,  
map<string,int> &cat_counts){
    if(bundle.second[category][word]>0){ 
    return log(double(bundle.second[category][word])/cat_counts[category]);
    }
    if(add_up_counts(bundle.first[word])==0){
    return log(1.0/cat_counts["total"]);}// if word doesnt show up in training
    return log(double(add_up_counts(bundle.first[word]))/cat_counts["total"]); 
    
    
}


double prob_calc(string category, const vector<string> &words,
pair<map<string, map<string,int>>, 
map<string, map<string,int>>> &bundle, 
map<string,int> &cat_counts){
    
    double total = log(double(cat_counts[category])/cat_counts["total"]);
    map<string, int> words_to_add; // impliments to stop duplicating additions
    for(auto & word : words) {words_to_add[word]=1;}
    for(auto & word : words_to_add){
        total+=prob_calc_one_word(category,word.first,bundle,cat_counts);
    }

    return total;
    
    return 1;

}

vector<map<string, string>> input_csv(istream &os){
    csvstream csv(os);
    vector<map<string, string>> vec;
    map<string, string> m_temp;

    while(csv>>m_temp){
        vec.push_back(m_temp);
    }
    return vec;
}
