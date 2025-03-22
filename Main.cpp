#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <queue>
#include <filesystem>

#include "VMParser.hpp"

namespace fs = std::filesystem;
using namespace std;

string fnameOut, moduleName;
VMParser vmp;
queue<string> queue1;
bool bootstrapAdded = false;

void writeBootstrapCode(ofstream &fOut) {
    fOut << "@256\nD=A\n@SP\nM=D\n";
    fOut << "@paplu.RET_0\nD=A\n@SP\nA=M\nM=D\n@SP\nM=M+1\n";
    fOut << "@LCL\nD=M\n@SP\nA=M\nM=D\n@SP\nM=M+1\n";
    fOut << "@ARG\nD=M\n@SP\nA=M\nM=D\n@SP\nM=M+1\n";
    fOut << "@THIS\nD=M\n@SP\nA=M\nM=D\n@SP\nM=M+1\n";
    fOut << "@THAT\nD=M\n@SP\nA=M\nM=D\n@SP\nM=M+1\n";
    fOut << "@SP\nD=M\n@LCL\nM=D\n";
    fOut << "@5\nD=D-A\n@ARG\nM=D\n";
    fOut << "@Sys.init\n0;JMP\n";
    fOut << "(paplu.RET_0)\n";
}

void processFile(const string &fnameIn, ofstream &fOut, bool multipleFiles) {
    ifstream fIn(fnameIn);
    if (!fIn.is_open()) {
        throw runtime_error("Unable to open file: " + fnameIn);
    }

    moduleName = fs::path(fnameIn).stem().string();
    
    if (multipleFiles && !bootstrapAdded) {
        writeBootstrapCode(fOut);
        bootstrapAdded = true;
    }
    
    string line;
    while (getline(fIn, line)) {
        int idx = line.find("//");
        string lineRmComm = line.substr(0, idx);
        if (lineRmComm.empty()) continue;

        auto first_non_space = lineRmComm.begin();
        while (first_non_space != lineRmComm.end() && isspace(*first_non_space)) {
            ++first_non_space;
        }
        lineRmComm.erase(lineRmComm.begin(), first_non_space);
        queue1.push(lineRmComm);
    }

    while (!queue1.empty()) {
        string it = queue1.front();
        queue1.pop();
        string wtof = vmp.parseVMLine(it);
        fOut << wtof;
    }

    fIn.close();
}

int main(int argc, char const *argv[]) {
    if (argc < 2) {
        throw runtime_error("Usage: VMTranslator <input .vm file | directory containing .vm files> [output.asm]");
    }

    fs::path inputPath = argv[1];
    ofstream fOut;
    
    if (argc == 3) {
        fnameOut = argv[2];
    } else {
        fnameOut = inputPath.stem().string() + ".asm";
    }

    fOut.open(fnameOut, ofstream::trunc);
    if (!fOut.is_open()) {
        throw runtime_error("Unable to open output file: " + fnameOut);
    }

    bool multipleFiles = fs::is_directory(inputPath) && distance(fs::directory_iterator(inputPath), fs::directory_iterator{}) > 1;

    if (fs::is_regular_file(inputPath) && inputPath.extension() == ".vm") {
        processFile(inputPath.string(), fOut, false);
    } else if (fs::is_directory(inputPath)) {
        for (const auto &entry : fs::directory_iterator(inputPath)) {
            if (entry.path().extension() == ".vm") {
                processFile(entry.path().string(), fOut, multipleFiles);
            }
        }
    } else {
        throw runtime_error("Invalid input: Must be a .vm file or a directory containing .vm files");
    }

    fOut << vmp.parseVMLine("end");
    fOut.close();
    cout << "Output written to " << fnameOut << endl;
    return 0;
}
