#include <bits/stdc++.h>
using namespace std;

#define cntFrame 8      // Broj okvira (0-7)
#define cntProcess 3    // Broj procesa (1-4) 
#define cntPage 4       // Broj stranica po procesu (0-3)

struct Frame {
    int processId;
    int pageNum;
    int refBit;
};

Frame frames[cntFrame];
int clockHand = 3;
int memory[cntFrame][256]; // Simulirana memorija 256B po okviru

void initMemory() {
    // Početno stanje sam napravio prema primjeru ispisa
    frames[0] = {2, 0, 0};
    frames[1] = {1, 2, 0};
    frames[2] = {1, 3, 0};
    frames[3] = {3, 0, 1};
    frames[4] = {2, 3, 0};
    frames[5] = {3, 1, 0};
    frames[6] = {3, 2, 1};
    frames[7] = {1, 0, 1};
}

void printFrames() {
    cout << "frames:";
    for (int i = 0; i < cntFrame; i++) {
        cout << " " << i << ":[" << frames[i].processId << ":" << frames[i].pageNum << "]";
    }
    cout << endl;
}

void printClock() {
    cout << "clock:";
    for (int i = 0; i < cntFrame; i++) {
        cout << " " << frames[i].refBit;
        if (i < cntFrame - 1) cout << "-";
    }
    cout << endl;
}

// Nisam išao s ^ jer je komplicirano bezveze haha
void printHand() {
    cout << "hand " << clockHand << endl;
}

// Ovdje pronalazim okvir za određeni proces i stranicu
int findFrame(int processId, int pageNum) {
    for (int i = 0; i < cntFrame; i++) {
        if (frames[i].processId == processId && frames[i].pageNum == pageNum) {
            return i;
        }
    }
    return -1; // Nije pronađeno
}

// Clock algoritam za pronalaženje okvira, stavio sam da se nakon proalaska victim framea hand se pomakne za 1
int clockAlgorithm() {
    while (true) {
        if (frames[clockHand].refBit == 0) {
            int victim = clockHand;
            clockHand = (clockHand + 1) % cntFrame;
            return victim;
        } else {
            frames[clockHand].refBit = 0;
            clockHand = (clockHand + 1) % cntFrame;
        }
    }
}

void processRequest(int processId, string operation, int value, int logicalAddr) {
    cout << "process " << processId << " " << operation;
    if (operation == "WRITE") cout << "(0x" << hex << uppercase << value << ")";
    cout << " LA=0x" << hex << uppercase << setfill('0') << setw(4) << logicalAddr << endl;
    
    // Izvuci page number i offset
    int pageNum = (logicalAddr >> 8) & 0xFF;
    int offset = logicalAddr & 0xFF;
    
    // Provjeri je li stranica validna za proces, ako nije samo ide novi upis i killamo proces
    if (pageNum >= cntPage) {
        cout << "MEMORY FAULT: page " << pageNum << " not allocated, terminating process " << processId << endl;
        return;
    }
    
    printFrames();
    
    int frameNum = findFrame(processId, pageNum);
    
    if (frameNum != -1) {
        // HIT
        cout << "HIT: " << frameNum << endl;
        
        cout << "paging: process " << processId << ", page=" << pageNum << " => frame=" << frameNum;
        cout << ", 0x" << hex << uppercase << setfill('0') << setw(4) << logicalAddr << " => 0x0" << frameNum << setfill('0') << setw(2) << hex << offset << endl;
        
        if (operation == "READ") {
            int readValue = memory[frameNum][offset];
            cout << "read value at 0x0" << frameNum << setfill('0') << setw(2) << hex << offset << " => 0x" << hex << uppercase << readValue << endl;
        } else {
            cout << "save (0x" << hex << uppercase << value << ") at 0x0" << frameNum << setfill('0') << setw(2) << hex << offset << endl;
            memory[frameNum][offset] = value;
        }
        
        frames[frameNum].refBit = 1;
        printClock();
        printHand();
        
    } else {
        // MISS
        cout << "MISS (page " << pageNum << " not in memory)" << endl;
        
        cout << "clock before:";
        for (int i = 0; i < cntFrame; i++) {
            cout << " " << frames[i].refBit;
            if (i < cntFrame - 1) cout << "-";
        }
        cout << endl;
        
        cout << "hand before:" << clockHand << endl;
        
        // Clock algoritam
        int victimFrame = clockAlgorithm();
        
        cout << "clock after:";
        for (int i = 0; i < cntFrame; i++) {
            cout << " " << frames[i].refBit;
            if (i < cntFrame - 1) cout << "-";
        }
        cout << endl;
        
        cout << "hand after:" << clockHand << endl;
        
        cout << "use frame " << victimFrame << ":" << endl;
        cout << "- save to disk: process " << frames[victimFrame].processId << ", page " << frames[victimFrame].pageNum << endl;
        cout << "- load from disk: process " << processId << ", page " << pageNum << endl;
        
        // Zamijeni stranicu
        frames[victimFrame].processId = processId;
        frames[victimFrame].pageNum = pageNum;
        frames[victimFrame].refBit = 0;
        
        printFrames();
        
        cout << "restarting instruction:" << endl;
        
        // Ponovi zahtjev
        cout << "process " << processId << " " << operation;
        if (operation == "WRITE") cout << "(0x" << hex << uppercase << value << ")";
        cout << " LA=0x" << hex << uppercase << setfill('0') << setw(4) << logicalAddr << endl;
        
        printFrames();
        
        cout << "HIT: " << victimFrame << endl;
        
        cout << "paging: page=" << pageNum << " => frame=" << victimFrame;
        cout << ", 0x" << hex << uppercase << setfill('0') << setw(4) << logicalAddr << " => 0x0" << victimFrame << setfill('0') << setw(2) << hex << offset << endl;
        
        if (operation == "READ") {
            int readValue = memory[victimFrame][offset];
            cout << "read value at 0x0" << victimFrame << setfill('0') << setw(2) << hex << offset << " => 0x" << hex << uppercase << readValue << endl;
        } else {
            cout << "save (0x" << hex << uppercase << value << ") at 0x0" << victimFrame << setfill('0') << setw(2) << hex << offset << endl;
            memory[victimFrame][offset] = value;
        }
        
        frames[victimFrame].refBit = 1;
        printClock();
        printHand();
    }
    
    cout << endl;
}

int main() {
    initMemory();
    
    int processId;
    string operation;
    int value = 0;
    int logicalAddr;
    
    while (true) {
        cout << "Unesi broj procesa (1-4): ";
        cin >> processId;
        
        if (processId < 1 || processId > 4) {
            cout << "Neispravan broj procesa!" << endl;
            continue;
        }
        
        cout << "Unesi READ ili WRITE: ";
        cin >> operation;
        
        if (operation != "READ" && operation != "WRITE") {
            cout << "Neispravna operacija!" << endl;
            continue;
        }
        
        if (operation == "WRITE") {
            cout << "Unesi vrijednost: ";
            cin >> hex >> value;
        }
        
        cout << "Unesi logicku adresu: ";
        cin >> hex >> logicalAddr;
        
        processRequest(processId, operation, value, logicalAddr);
    }
    
    return 0;
}