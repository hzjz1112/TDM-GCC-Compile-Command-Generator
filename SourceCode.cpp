/*
 * TDM-GCC��������
 * Copyright (c) 2025 hzjz1112
 * License: MIT (����Ŀ��Ŀ¼LICENSE�ļ�)
 */
#include <iostream>
#include <filesystem>
#include <string>
#include <vector>
#include <cctype>
#include <process.h>

namespace fs = std::filesystem;

// ת���ַ���ΪСд
std::string toLower(const std::string& str) {
    std::string lowerStr;
    for (char c : str) {
        lowerStr += tolower(c);
    }
    return lowerStr;
}

// ��׼��C++��׼����
std::string normalizeStd(const std::string& input) {
    std::string lower = toLower(input);
    if (lower.find("c++") != std::string::npos || lower.find("cpp") != std::string::npos) {
		if (lower.find("98") != std::string::npos) return "-std=c++98";
		if (lower.find("03") != std::string::npos) return "-std=c++03";
        if (lower.find("11") != std::string::npos) return "-std=c++11";
        if (lower.find("14") != std::string::npos) return "-std=c++14";
        if (lower.find("17") != std::string::npos) return "-std=c++17";
        if (lower.find("20") != std::string::npos) return "-std=c++20";
        if (lower.find("23") != std::string::npos || lower.find("2b") != std::string::npos) return "-std=c++23";
    }
    return "-std=c++17"; // Ĭ��ֵ
}

// ��ȡ�û������Y/N��ת��Ϊbool
bool getYesNo(const std::string& prompt) {
    while (true) {
        std::cout << prompt << " (Y/N): ";
        std::string input;
        std::getline(std::cin, input);
        if (!input.empty()) {
            char choice = tolower(input[0]);
            if (choice == 'y') return true;
            if (choice == 'n') return false;
        }
        std::cout << "��Ч���룬������Y��N\n";
    }
}

// ִ�б�������
void executeCompilation(const std::string& command, const fs::path& workingDir) {
    std::cout << "\n=== ִ������ ===\n" << command << "\n";
    std::cout << "����Ŀ¼: " << workingDir << "\n\n";

    // �л�������Ŀ¼
    fs::current_path(workingDir);

    // ִ�б�������
    int result = system(command.c_str());

    if (result == 0) {
        std::cout << "\n����ɹ����!\n";
    } else {
        std::cout << "\n����ʧ�ܣ��������: " << result << "\n";
    }
}

int main() {
    std::cout << "=== TDM-GCC �������������� ===\n"; 
    std::cout << "��ȡTDM-GCC��"<<std::endl;
    std::cout << "�ٷ����ӣ�https://jmeubank.github.io/tdm-gcc/download/"<<std::endl;
    std::cout << "123���̣������Լ�������https://www.123865.com/s/6Mp1jv-7QUBh"<<std::endl;

    // 1. ��ȡԴ�ļ�Ŀ¼
    fs::path sourceDir;
    while (true) {
        std::cout << "������CPPԴ�ļ�����Ŀ¼: ";
        std::string dirInput;
        std::getline(std::cin, dirInput);
        
        sourceDir = fs::path(dirInput);
        if (fs::exists(sourceDir) && fs::is_directory(sourceDir)) {
            break;
        }
        std::cout << "Ŀ¼�����ڻ���Ч������������\n";
    }

    // 2. ��ȡC++��׼
    std::cout << "\n������C++��׼(ֱ������һ�����֣���11, 17�ȣ��൱��C++11��C++17��Ĭ��ʹ��C++17): ";
    std::string stdInput;
    std::getline(std::cin, stdInput);
    std::string stdOption = normalizeStd(stdInput);

    // 3. ��ȡԴ�ļ���
    std::string sourceFile;
    while (true) {
        std::cout << "\n������Դ�ļ���(��.cpp��׺): ";
        std::getline(std::cin, sourceFile);
        
        if (sourceFile.size() > 4 && 
            toLower(sourceFile.substr(sourceFile.size() - 4)) == ".cpp") {
            if (fs::exists(sourceDir / sourceFile)) {
                break;
            }
            std::cout << "�ļ������ڣ�����������\n";
        } else {
            std::cout << "�ļ���������.cpp��β\n";
        }
    }

    // 4. ��ȡ����ļ���
    std::string outputFile;
    while (true) {
        std::cout << "\n����������ļ���(��.exe��׺): ";
        std::getline(std::cin, outputFile);
        
        if (outputFile.size() > 4 && 
            toLower(outputFile.substr(outputFile.size() - 4)) == ".exe") {
            break;
        }
        std::cout << "�ļ���������.exe��β\n";
    }

    // 5. ͷ�ļ�·������
    std::vector<std::string> includePaths;
    std::cout << "\n��Ҫ��Ӷ����ͷ�ļ�����·����";
    if (getYesNo("")) {
        std::cout << "������Ҫ��ӵ�ͷ�ļ�·����ֱ�Ӱ��س��������룬����0��N������:\n";
        std::string path;
        while (true) {
            std::cout << "ͷ�ļ�·�� " << includePaths.size() + 1 << ": ";
            std::getline(std::cin, path);
            
            if (path.empty() || toLower(path) == "0" || toLower(path) == "n") {
                break;
            }
            includePaths.push_back(path);
        }
    }

    // 6. ���ļ�����
    std::vector<std::string> libraryPaths;
    std::vector<std::string> libraries;
    std::cout << "\n��Ҫ���Ӷ���Ŀ���";
    if (getYesNo("")) {
        // ����������
        std::cout << "������Ҫ���ӵĿ���������libǰ׺��.a/.lib��׺��ֱ�Ӱ��س��������룩:\n";
        std::string lib;
        while (true) {
            std::cout << "���� " << libraries.size() + 1 << ": ";
            std::getline(std::cin, lib);
            
            if (lib.empty()) {
                break;
            }
            libraries.push_back(lib);
        }

        // ��·������
        std::cout << "\n��Ҫָ�����ļ�����·����";
        if (getYesNo("")) {
            std::cout << "��������ļ�·����ֱ�Ӱ��س��������룩:\n";
            std::string libPath;
            while (true) {
                std::cout << "��·�� " << libraryPaths.size() + 1 << ": ";
                std::getline(std::cin, libPath);
                
                if (libPath.empty()) {
                    break;
                }
                libraryPaths.push_back(libPath);
            }
        }
    }

    // 7. ����ѡ��
    bool enableWarnings = getYesNo("\n�������о���(-Wall)?");
    bool enableO2 = getYesNo("����O2�Ż�?");
    bool hideConsole = getYesNo("���ؿ���̨����(-mwindows)?");

    // ������������
    std::string command = "g++ " + stdOption + " ";
    
    if (enableWarnings) command += "-Wall ";
    if (enableO2) command += "-O2 ";
    if (hideConsole) command += "-mwindows ";
    
    // ���ͷ�ļ�·��
    for (const auto& path : includePaths) {
        command += "-I\"" + path + "\" ";
    }
    
    // ��ӿ��ļ�·��
    for (const auto& path : libraryPaths) {
        command += "-L\"" + path + "\" ";
    }
    
    // ���Դ�ļ�
    command += "\"" + sourceFile + "\" ";
    
    // �������ļ�
    command += "-o \"" + outputFile + "\" ";
    
    // ��ӿ�
    for (const auto& lib : libraries) {
        command += "-l" + lib + " ";
    }

    // ִ�б���
    std::cout << "\n=== ���ɵı������� ===\n" << command << "\n\n";
    if (getYesNo("ȷ��ִ�б���?")) {
        executeCompilation(command, sourceDir);
    } else {
        std::cout << "������ȡ��\n";
    }

    return 0;
}
