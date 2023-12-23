#include <iostream>
#include <fstream>
#include <chrono>
using namespace std;
using namespace chrono;

struct Data {
    string key;
    int value;
    Data* next; // Zincirleme için kullanılan bağlı liste
};

class SimpleHashTable {
private:
    Data** hashtable;
    size_t capacity;
    size_t size; // Tablodaki eleman sayısı

    size_t customHashFunction(const string& key) const {
        size_t hash = 0;
        for (char ch : key) {
            hash = hash * 31 + ch;
        }
        return hash;
    }

    void resizeTable() {
        size_t newCapacity = capacity * 2; // Genişletme katsayısı
        Data** newTable = new Data*[newCapacity]();

        // Eski verileri yeni tabloya taşı
        for (size_t i = 0; i < capacity; ++i) {
            Data* current = hashtable[i];
            while (current != nullptr) {
                size_t newIndex = customHashFunction(current->key) % newCapacity;

                // Yeni index boşsa doğrudan ekle, değilse zincirle
                if (newTable[newIndex] == nullptr) {
                    newTable[newIndex] = new Data{current->key, current->value, nullptr};
                } else {
                    Data* newNode = new Data{current->key, current->value, newTable[newIndex]};
                    newTable[newIndex] = newNode;
                }

                current = current->next;
            }
        }

        // Eski tabloyu temizle
        clearTable(hashtable, capacity);

        // Yeni tabloyu kullanmaya başla
        hashtable = newTable;
        capacity = newCapacity;
    }

    void clearTable(Data** table, size_t size) {
        for (size_t i = 0; i < size; ++i) {
            Data* current = table[i];
            while (current != nullptr) {
                Data* next = current->next;
                delete current;
                current = next;
            }
        }
        delete[] table;
    }

public:
    explicit SimpleHashTable(size_t initialSize = 10)
        : hashtable(new Data*[initialSize]()), capacity(initialSize), size(0) {}

    ~SimpleHashTable() {
        clearTable(hashtable, capacity);
    }

    void insert(const string& key) {
        size_t index = customHashFunction(key) % capacity;
        Data* current = hashtable[index];

        // Anahtarı tabloda bul
        while (current != nullptr) {
            if (current->key == key) {
                current->value++;
                return;
            }
            current = current->next;
        }

        // Anahtar tabloda yoksa yeni bir veri oluştur ve ekle
        Data* newNode = new Data{key, 1, hashtable[index]};
        hashtable[index] = newNode;
        size++;

        // Genişletme kontrolü
        if (size > capacity / 2) {
            resizeTable();
        }
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
    SimpleHashTable myHashTable;
    
    // Anahtarları bul ve ekleyerek işle
    processKeys(log, myHashTable);

    log.clear();
    log.seekg(0, ios::beg);
    
    // Toplu anahtarları yazdır
    myHashTable.printTopKeys();
    
    // Toplam sayıyı yazdır
    int totalCount = myHashTable.getTotalCount();
    cout << "Sum: " << totalCount << endl;

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(stop - start);
    cout << "Total Elapsed Time :" << duration.count() << " millisecond." << endl;
    return 0;
}