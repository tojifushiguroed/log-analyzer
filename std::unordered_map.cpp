#include <iostream>
#include <fstream>
#include <unordered_map>
#include <string>
#include <vector>
#include <algorithm>
#include <chrono>

using namespace std;
using namespace chrono;
std::unordered_map<string, int > myMap;
string key;
string foundKey= "";




int sumvalue(const unordered_map<string, int>& myMap){
    int sum =0;
    for(const auto&entry: myMap ){
        sum +=entry.second;
    }
    return sum;
}

string findkey(ifstream& kaynak){
    string line;
        
    while (getline(kaynak, line) && !kaynak.eof()){
        size_t baslangıcPozisyonu= line.find("GET");
        if(baslangıcPozisyonu != string::npos){

        size_t endposition= line.find("HTTP");
        if (endposition != string::npos) {
            
            key = line.substr(baslangıcPozisyonu+3, endposition - (baslangıcPozisyonu+3)); // GET VE HTTP boyutunu aldım.
            myMap[key]++;
            foundKey=key;
                    }
            }
    }
    return foundKey;
}

void shellSort(vector<pair<string, int> >& arr) {
    int n = arr.size();
    for (int gap = n/2; gap > 0; gap /= 2) {
        for (int i = gap; i < n; i += 1) {
            auto temp = arr[i];
            int j;           
            for (j = i; j >= gap && arr[j - gap].second < temp.second; j -= gap)
                arr[j] = arr[j - gap];
            arr[j] = temp;
        }
    }
}

void orderandprint(const unordered_map<string, int>& myMap) {
    // Anahtar-değer çiftlerini vektöre kopyala
    vector<pair<string, int> > sortedPairs(myMap.begin(), myMap.end());

    // Shell sort
    shellSort(sortedPairs);

    // En büyük 10 değeri yazdır
    size_t topN = min<size_t>(10, sortedPairs.size());
    for (size_t i = 0; i < topN; ++i) {
        std::cout << "Key: " << sortedPairs[i].first << " Value: " << sortedPairs[i].second << std::endl;
    }
}
int main(){
    auto start = high_resolution_clock::now(); // Start time measurement

    ifstream log;
    string line;

    log.open("/Users/egolboyu/Desktop/access_log.txt"); // kaynak kodunu kendi bilgisayarınıza göre yazın gençler ..
    if (!log.is_open()) {
           cerr << "File not opened!" << endl;
           return -1; // Hata durumunda programı sonlandır ve -1 değerini döndürür.
       }
    string result= findkey(log);
    if (!result.empty()) {
        orderandprint(myMap);
    }
    else {
        std::cout << "Key not found!" << std::endl;
    }

    int sum = sumvalue(myMap);
    cout<< "Sum of all lines : " << sum <<endl;

    log.close();

    auto stop = high_resolution_clock::now(); // Stop time measurement
    auto duration = duration_cast<milliseconds>(stop - start);
    cout << "Total Elapsed Time : " << duration.count() << " millisecond." << endl;

    return 0;
}
