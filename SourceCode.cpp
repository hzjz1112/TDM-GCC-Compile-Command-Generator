/*
 * TDM-GCC编译助手
 * Copyright (c) 2025 hzjz1112
 * License: MIT (见项目根目录LICENSE文件)
 */
#include <iostream>
#include <filesystem>
#include <string>
#include <vector>
#include <cctype>
#include <process.h>

namespace fs = std::filesystem;

// 转换字符串为小写
std::string toLower(const std::string& str) {
    std::string lowerStr;
    for (char c : str) {
        lowerStr += tolower(c);
    }
    return lowerStr;
}

// 标准化C++标准输入
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
    return "-std=c++17"; // 默认值
}

// 获取用户输入的Y/N并转换为bool
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
        std::cout << "无效输入，请输入Y或N\n";
    }
}

// 执行编译命令
void executeCompilation(const std::string& command, const fs::path& workingDir) {
    std::cout << "\n=== 执行命令 ===\n" << command << "\n";
    std::cout << "工作目录: " << workingDir << "\n\n";

    // 切换到工作目录
    fs::current_path(workingDir);

    // 执行编译命令
    int result = system(command.c_str());

    if (result == 0) {
        std::cout << "\n编译成功完成!\n";
    } else {
        std::cout << "\n编译失败，错误代码: " << result << "\n";
    }
}

int main() {
    std::cout << "=== TDM-GCC 编译命令生成器 ===\n"; 
    std::cout << "获取TDM-GCC："<<std::endl;
    std::cout << "官方链接：https://jmeubank.github.io/tdm-gcc/download/"<<std::endl;
    std::cout << "123网盘（本人自己分享）：https://www.123865.com/s/6Mp1jv-7QUBh"<<std::endl;

    // 1. 获取源文件目录
    fs::path sourceDir;
    while (true) {
        std::cout << "请输入CPP源文件所在目录: ";
        std::string dirInput;
        std::getline(std::cin, dirInput);
        
        sourceDir = fs::path(dirInput);
        if (fs::exists(sourceDir) && fs::is_directory(sourceDir)) {
            break;
        }
        std::cout << "目录不存在或无效，请重新输入\n";
    }

    // 2. 获取C++标准
    std::cout << "\n请输入C++标准(直接输入一个数字，如11, 17等，相当于C++11和C++17，默认使用C++17): ";
    std::string stdInput;
    std::getline(std::cin, stdInput);
    std::string stdOption = normalizeStd(stdInput);

    // 3. 获取源文件名
    std::string sourceFile;
    while (true) {
        std::cout << "\n请输入源文件名(带.cpp后缀): ";
        std::getline(std::cin, sourceFile);
        
        if (sourceFile.size() > 4 && 
            toLower(sourceFile.substr(sourceFile.size() - 4)) == ".cpp") {
            if (fs::exists(sourceDir / sourceFile)) {
                break;
            }
            std::cout << "文件不存在，请重新输入\n";
        } else {
            std::cout << "文件名必须以.cpp结尾\n";
        }
    }

    // 4. 获取输出文件名
    std::string outputFile;
    while (true) {
        std::cout << "\n请输入输出文件名(带.exe后缀): ";
        std::getline(std::cin, outputFile);
        
        if (outputFile.size() > 4 && 
            toLower(outputFile.substr(outputFile.size() - 4)) == ".exe") {
            break;
        }
        std::cout << "文件名必须以.exe结尾\n";
    }

    // 5. 头文件路径配置
    std::vector<std::string> includePaths;
    std::cout << "\n需要添加额外的头文件搜索路径吗？";
    if (getYesNo("")) {
        std::cout << "请输入要添加的头文件路径（直接按回车结束输入，输入0或N跳过）:\n";
        std::string path;
        while (true) {
            std::cout << "头文件路径 " << includePaths.size() + 1 << ": ";
            std::getline(std::cin, path);
            
            if (path.empty() || toLower(path) == "0" || toLower(path) == "n") {
                break;
            }
            includePaths.push_back(path);
        }
    }

    // 6. 库文件配置
    std::vector<std::string> libraryPaths;
    std::vector<std::string> libraries;
    std::cout << "\n需要链接额外的库吗？";
    if (getYesNo("")) {
        // 库名称输入
        std::cout << "请输入要链接的库名（不带lib前缀和.a/.lib后缀，直接按回车结束输入）:\n";
        std::string lib;
        while (true) {
            std::cout << "库名 " << libraries.size() + 1 << ": ";
            std::getline(std::cin, lib);
            
            if (lib.empty()) {
                break;
            }
            libraries.push_back(lib);
        }

        // 库路径输入
        std::cout << "\n需要指定库文件搜索路径吗？";
        if (getYesNo("")) {
            std::cout << "请输入库文件路径（直接按回车结束输入）:\n";
            std::string libPath;
            while (true) {
                std::cout << "库路径 " << libraryPaths.size() + 1 << ": ";
                std::getline(std::cin, libPath);
                
                if (libPath.empty()) {
                    break;
                }
                libraryPaths.push_back(libPath);
            }
        }
    }

    // 7. 编译选项
    bool enableWarnings = getYesNo("\n启用所有警告(-Wall)?");
    bool enableO2 = getYesNo("启用O2优化?");
    bool hideConsole = getYesNo("隐藏控制台窗口(-mwindows)?");

    // 构建编译命令
    std::string command = "g++ " + stdOption + " ";
    
    if (enableWarnings) command += "-Wall ";
    if (enableO2) command += "-O2 ";
    if (hideConsole) command += "-mwindows ";
    
    // 添加头文件路径
    for (const auto& path : includePaths) {
        command += "-I\"" + path + "\" ";
    }
    
    // 添加库文件路径
    for (const auto& path : libraryPaths) {
        command += "-L\"" + path + "\" ";
    }
    
    // 添加源文件
    command += "\"" + sourceFile + "\" ";
    
    // 添加输出文件
    command += "-o \"" + outputFile + "\" ";
    
    // 添加库
    for (const auto& lib : libraries) {
        command += "-l" + lib + " ";
    }

    // 执行编译
    std::cout << "\n=== 生成的编译命令 ===\n" << command << "\n\n";
    if (getYesNo("确认执行编译?")) {
        executeCompilation(command, sourceDir);
    } else {
        std::cout << "编译已取消\n";
    }

    return 0;
}
