// $Id: main.cpp,v 1.11 2018-01-25 14:19:29-08 - - $
//Kevin Wu kwu30@ucsc.edu, Daniel Nguyen dnguy182@ucsc.edu

#include <cstdlib>
#include <exception>
#include <iostream>
#include <string>
#include <unistd.h>
#include <cassert> //added
#include <regex> //added
#include <fstream> //added

using namespace std;

#include "listmap.h"
#include "xpair.h"
#include "util.h"

using str_str_map = listmap<string,string>;
using str_str_pair = str_str_map::value_type;

//returns pos of 1st non-option arg
void scan_options (int argc, char** argv) {
   opterr = 0;
   for (;;) {
      int option = getopt (argc, argv, "@:");
      if (option == EOF) break;
      switch (option) {
         case '@':
            debugflags::setflags (optarg);
            break;
         default:
            complain() << "-" << char (optopt) << ": invalid option"
                       << endl;
            break;
      }
   }
}

//helper func to parse input given to us in misc/matchlines.cpp
void parse(istream &in, str_str_map &m, string type){
   string line;
   int count=1;
   for (;;) {
      getline(in, line);

      if (in.eof()) break; //if in break
      cout << type << ": " << count << ": " << line << endl;

      regex comment_regex {R"(^\s*(#.*)?$)"};
      regex key_value_regex {R"(^\s*(.*?)\s*=\s*(.*?)\s*$)"};
      regex trimmed_regex {R"(^\s*([^=]+?)\s*$)"};

      smatch result;
      if (regex_search (line, result, comment_regex)) {
         //cout << "Comment or empty line." << endl;
         //continue;
      }else if (regex_search (line, result, key_value_regex)) {
         //cout << "key  : \"" << result[1] << "\"" << endl;
         //cout << "value: \"" << result[2] << "\"" << endl;
         if (result[1] != "" && result[2] == "") {
            //key = : delete key and value from map
            str_str_map::iterator temp = m.find(result[1]);
            m.erase(temp);
         }else if (result[1] != "" && result[2] != "") {
            //key = value : ITERATE thru list and check for lexicographical 
            //order, if u find a same key name, update the value, else create new key and value
            str_str_pair pair (result[1], result[2]);
            m.insert(pair);
            cout << result[1] << " = " << result[2] << endl;
         }else if (result[1] == "" && result[2] == "") {
            //= : iterate and print all key and value pairs in lexicographical order
            for (str_str_map::iterator itor = m.begin(); itor != m.end(); ++itor) {
               cout << (*itor).first << " = " << (*itor).second << endl;
            }
         }else if (result[1] == "" && result[2] != ""){
            // =value : iterate and print all key names with the give value lexicographically
            for (str_str_map::iterator itor = m.begin(); itor != m.end(); ++itor) {
               if (result[2] == (*itor).second)
                  cout << (*itor).first << " = " << (*itor).second << endl;
            }
         }
      }else if (regex_search (line, result, trimmed_regex)) {
         str_str_map::iterator itor = m.find(result[1]);
         string found = "key not found";
         string sign = ": ";
         if (itor != m.end()) {
            found = (*itor).second;
            sign = " = ";
         } 
         cout << line << sign << found << endl;
      }else {
         assert (false and "This can not happen.");
      }
      count ++;
   }
}

int main (int argc, char** argv) {
   sys_info::execname (argv[0]);
   scan_options (argc, argv);
   str_str_map m;
   //str_str_map m;
   if(argc==1){ //if argc is 1 calls helper func
      //that loops cin inputs & assigns them to key,val
      parse(cin, m, "-");
   }else{ //read files
      //goes thru all files if there are multiple
      for(int i = 1; i < argc; i++){
         string filename = argv[i];
         ifstream myfile(filename);
         if(filename == "-"){ //if "-" case
            parse(cin, m, "-");
         }else{ //else bring in the file
            parse(myfile, m, filename);
         }
         myfile.close();
      }
   }

   m.~listmap();


   return EXIT_SUCCESS;
}

