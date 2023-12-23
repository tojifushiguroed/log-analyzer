#include <iostream>
#include <fstream>
#include <chrono>
#include <vector>
#include <algorithm>
using namespace std;
using namespace chrono;

struct Data {
    string key;
    int value;
    Data* next = nullptr; // Zincirleme için kullanılan bağlı liste

    Data(const string& key, int value) : key(key), value(value) {}
};

class SimpleHashTable {
private:
    int size;
    int capacity;
    vector<Data*> hashtable;

    size_t hash(const string& key) {
        size_t hash = 0;
        for (char ch : key) {
            hash = hash * 31 + ch;
        }
        return hash;
    }

    void resizeTable() {
        // Implement the resize function here
    }

public:
    SimpleHashTable(int capacity) : size(0), capacity(capacity) {
        hashtable.resize(capacity, nullptr);
    }

    void insert(const string& key) {
        if (size == capacity) {
            resizeTable();
        }

        size_t index = hash(key) % capacity;
        while (hashtable[index] != nullptr && hashtable[index]->key != key) {
            index = (index + 1) % capacity;
        }

        if (hashtable[index] == nullptr) {
            hashtable[index] = new Data{key, 1};
            size++;
        } else {
            hashtable[index]->value++;
        }

        // Genişletme kontrolü
        if (size > capacity / 2) {
            resizeTable();
        }
    }

    Data* find(const string& key) {
        size_t index = hash(key) % capacity;
        while (hashtable[index] != nullptr && hashtable[index]->key != key) {
            index = (index + 1) % capacity;
        }
        return hashtable[index];
    }

    void printTopKeys() const {
        for (size_t i = 0; i < 10; ++i) {
            size_t maxIndex = 0;
            int maxValue = 0;

            for (size_t j = 0; j < capacity; ++j) {
                if (hashtable[j] != nullptr && hashtable[j]->value > maxValue) {
                    maxIndex = j;
                    maxValue = hashtable[j]->value;
                }
            }

            if (maxValue > 0) {
                cout << "Key: " << hashtable[maxIndex]->key << " Value: " << maxValue << endl;
                hashtable[maxIndex]->value = 0;  // Anahtarı işaretleyerek tekrar sayılmamasını sağla
            }
        }
    }

    int getTotalCount() const {
        int total = 0;
        for (size_t i = 0; i < capacity; ++i) {
            Data* current = hashtable[i];
            while (current != nullptr) {
                total += current->value;
                current = current->next;
            }
        }
        return total;
    }
};

void processKeys(ifstream& source, SimpleHashTable& myHashTable) {
    string line;

    while (getline(source, line, '\n')) {
        size_t start_position = line.find(' ') + 1;
        size_t end_position = line.find("HTTP");

        if (start_position != string::npos && end_position != string::npos) {
            string key = line.substr(start_position, end_position - start_position);

            // Remove spaces
            key.erase(remove_if(key.begin(), key.end(), ::isspace), key.end());

            if (!key.empty()) {
                myHashTable.insert(key);
            }
        }
    }
}

int main() {
auto start = high_resolution_clock::now();
ifstream log("/Users/egolboyu/Desktop/access_log.txt");
if (!log.is_open()) {
	cerr << "Dosya açma hatası!" << endl;
	return -1;
}
SimpleHashTable HashTable(724000); // Provide the desired capacity as an argument

// Anahtarları bul ve ekleyerek işle
processKeys(log, HashTable);

log.clear();
log.seekg(0, ios::beg);
    
    // Toplu anahtarları yazdır
    HashTable.printTopKeys();
    
    // Toplam sayıyı yazdır
    int totalCount = HashTable.getTotalCount();
    cout << "Sum: " << totalCount << endl;

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(stop - start);
    cout << "Total Elapsed Time :" << duration.count() << " millisecond." << endl;
    return 0;
}