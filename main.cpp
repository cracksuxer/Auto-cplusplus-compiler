#include <iostream>
#include <string>
#include <filesystem>
#include <vector>
#include <chrono>
#include <fstream>
#include <cstdlib>



#define ALLFILES 0
#define SPECFILES 1

struct data {
  int option_;
  std::vector<std::string> files_;
  bool rest_ = false;
};

void ClearScreen(){
  std::cout << std::string( 100, '\n' );
}

int Compile(data input_data) {
    std::filesystem::path cwd = std::filesystem::current_path();
    auto dirIter = std::filesystem::recursive_directory_iterator(cwd);
    std::string command = "g++ ";
    switch (input_data.option_) {
      case ALLFILES: {
        for (const auto &entry : dirIter) {
          std::string extension = entry.path().extension().string();
          if(extension == ".cc" || extension == ".h")
            command += entry.path().filename().string() + " ";
        }
        break;
      }
      case SPECFILES:
        for(auto vec_itr = input_data.files_.begin() + 1; vec_itr != input_data.files_.end(); vec_itr++)
          command += *vec_itr + " ";
        break;
      default:
        break;
    }

    command += "-std=c++17 -Wall -o output";
    std::cout << "\033[1;31m- Compiling [\033[0m" + command + "\033[1;31m]\033[0m ";

    return !system((const char*)command.c_str());
}

int Execute(const std::string &executable) {
  std::cout << "\033[1;31m\n- Executing [\033[0m" + executable + "\033[1;31m]\033[0m\n\n";
  return !system((const char*)executable.c_str());
}


std::string AddArgs(std::string &executable) {
    std::cout << "\033[0;37m- Add command line arguments? Y/N: ";
    std::string first_option, out_exec;
    {
      std::string line;
      std::getline(std::cin, line);
      std::istringstream ss(line);
      ss >> first_option;
    }
    if(first_option == "Y" || first_option == "y") {
      std::cout << "-> ";
      std::string cl_args;
      std::getline(std::cin, cl_args);
      out_exec = "output.exe " + cl_args;
      executable.clear();
      executable = out_exec;
    }
    return executable;
}

int Exit() {
  std::cout << "\n\nExit? Y/N: ";
  std::string exit_opt;
  {
    std::string line;
    std::getline(std::cin, line);
    std::istringstream ss(line);
    ss >> exit_opt;
  }
  return (exit_opt == "Y" || exit_opt == "y");
}

data Input(int argc, char *argv[]) {
  data input_data;
  if(argc == 1) {
    std::cout << "Not enough arguments. Try --help for more information." << std::endl;
    exit(1);  
  }
  if(argc == 2 && std::string(argv[1]) == "--help") {
    std::cout << "This program compiles ur files and runs the outputed file automatically.\n"
                  "Set first argument as '0' for compiling everything inside the working directory\n"
                  "Otherwise, set it as 0 and then, write the files that u want to compile.\n\n"
                  "Examples:\n"
                  "./compile 0\n"
                  "./compile 1 file.cpp file.h main.cpp" << std::endl;
    exit(1);
  }
  if(std::string(argv[1]) == ".rest" || std::string(argv[1]) == ".\\.rest" || std::string(argv[1]) == ".\rest") {
    input_data.rest_ = true;
    return input_data;
  }
  input_data.option_ = std::stoi(argv[1]);
  for(int i = 1; argv[i] != NULL; i++)
    input_data.files_.emplace_back(argv[i]);
  return input_data;
}


void SaveExec(const std::string &executable) {
  std::ofstream save(".rest");
  save << executable;
}


int main (int argc, char *argv[]) {
  std::cout << "\033[1;34m\n< - - Auto C++ compiler and runner - - >\n\n\033[0m";
  bool exit = false;
  data input_data = Input(argc, argv);
  std::string executable = "output.exe";
  if(input_data.rest_)
    std::getline(std::ifstream(".rest"), executable);
  else {
    if(!Compile(input_data)) return 1; 
    std::cout << "\033[1;32msuccess!\n\033[0m\n";
  }
  
  while(true) {
    AddArgs(executable);
    auto time1 = std::chrono::high_resolution_clock::now();
    if(!Execute(executable)) return 1; 
    auto time2 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> ms_double = time2 - time1;

    SaveExec(executable);

    std::cout << "\033[1;33m\n\n<<<\033[0m Program executed in: "
              << ms_double.count() << "ms \033[1;33m>>>\033[0m";
    
    if(Exit()) break;
    system("CLS");
  }

  std::cout << "\033[1;34m\n\n< - - - - - - - - - - - - - - - - - - - >\n\n\033[0m";
  return 0;
}




/*     int fileCount = std::count_if(
      begin(dirIter),
      end(dirIter),
      [](auto& entry) { return entry.is_regular_file(); }
  );
*/