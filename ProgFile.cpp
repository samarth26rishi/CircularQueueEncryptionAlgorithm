#include <iostream>
#include <vector>
#include <string>
#include <sstream>
using namespace std;

class CircularQueue {
    int front, rear, size;
    char *arr;
public:
    CircularQueue(int s) {
        size = s;
        arr = new char[size];
        front = -1;
        rear = -1;
    }

    void enqueue(char data) {
        if (front == -1) front = 0;
        rear = (rear + 1) % size;
        arr[rear] = data;
    }

    void rotateLeft(int k) {
        k = k % size;
        for (int i = 0; i < k; i++) {
            char temp = arr[front];
            front = (front + 1) % size;
            rear = (rear + 1) % size;
            arr[rear] = temp;
        }
    }


    char getAt(int index) {
        return arr[(front + index) % size];
    }
};


vector<int> generateFibsUpTo(int maxVal) {
    vector<int> fibs = {1, 2};
    while (fibs.back() <= maxVal) {
        fibs.push_back(fibs.back() + fibs[fibs.size() - 2]);
    }
    if (fibs.back() > maxVal) fibs.pop_back();
    return fibs;
}

string toFibonacciRepresentation(int num) {
    vector<int> fibs = generateFibsUpTo(num);
    string fibRep(fibs.size(), '0');

    for (int i = fibs.size() - 1; i >= 0; i--) {
        if (fibs[i] <= num) {
            num -= fibs[i];
            fibRep[i] = '1';
        }
    }
    return fibRep;
}

int fromFibonacciRepresentation(const string &fibRep) {
    if (fibRep.empty()) return 0;

    vector<int> fibs = {1, 2};
    for (int i = 2; i < fibRep.size(); i++)
        fibs.push_back(fibs[i - 1] + fibs[i - 2]);

    int val = 0;
    for (int i = 0; i < fibRep.size(); i++)
        if (fibRep[i] == '1') val += fibs[i];

    return val;
}

string encrypt(string text, string keyword, int queueSize) {
    string encryptedBinary = "";

    int start = 0;
    for (char c : keyword) start += (int)c;
    start = start % queueSize;

    for (size_t i = 0; i < text.length(); i += queueSize) {
        size_t chunkSize = min((size_t)queueSize, text.length() - i);
        CircularQueue cq(chunkSize);

        for (size_t j = 0; j < chunkSize; j++)
            cq.enqueue(text[i + j]);

        cq.rotateLeft(start);

        for (size_t j = 0; j < chunkSize; j++) {
            char ch = cq.getAt(j);
            encryptedBinary += toFibonacciRepresentation((int)ch) + " ";
        }
    }

    if (!encryptedBinary.empty()) encryptedBinary.pop_back();
    return encryptedBinary;
}


string decrypt(string encryptedBinary, string keyword, int queueSize) {
    string result = "";

    vector<string> fibChunks;
    stringstream ss(encryptedBinary);
    string temp;
    while (ss >> temp) fibChunks.push_back(temp);

    int start = 0;
    for (char c : keyword) start += (int)c;
    start = start % queueSize;

    for (size_t i = 0; i < fibChunks.size(); i += queueSize) {
        size_t chunkSize = min((size_t)queueSize, fibChunks.size() - i);
        CircularQueue cq(chunkSize);

        for (size_t j = 0; j < chunkSize; j++)
            cq.enqueue((char)fromFibonacciRepresentation(fibChunks[i + j]));

        cq.rotateLeft((chunkSize - start) % chunkSize);

        for (size_t j = 0; j < chunkSize; j++)
            result += cq.getAt(j);
    }

    return result;
}


int main() {
    string text, keyword;
    int queueSize;

    cout << "Enter text: ";
    getline(cin, text);

    cout << "Enter keyword: ";
    cin >> keyword;

    cout << "Enter queue size: ";
    cin >> queueSize;

    string encrypted = encrypt(text, keyword, queueSize);
    cout << "\nEncrypted Binary (Fibonacci Representation):\n" << encrypted << endl;

    string decrypted = decrypt(encrypted, keyword, queueSize);
    cout << "\nDecrypted Text:\n" << decrypted << endl;

    return 0;
}