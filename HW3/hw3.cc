#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <pwd.h>
#include <grp.h>

using namespace std;

static string PROGNAME;
 //corn fritters



vector<string> parseFormatString(string str, int len) {
	vector<string> tokens;
	size_t pos = 0;
	while ((pos = str.find('%')) != string::npos) {
		tokens.push_back(str.substr(pos+1, len));
		str.erase(pos, 1);
	}
	return tokens;
}



// Start of HW3, TODO: Section out homeworks into seperate classes.
void printContents(Path path) {
	if(findMediaType(string magicNum, vector< 
}


int main(int argc, char* argv[]) {
    PROGNAME = argv[0];
	if(argv[1] == nullptr) {
		cerr << PROGNAME << " usage: ./hw3 [FILE] [DIRECTORY]\n";
		return 1;
	}

	// Read the flags
	//vector<string> tokens = parseFormatString(argv[1], 1);

	// Create an array containing each directory/file
	vector<string> directories;
	for(int i = 2; i < argc; i++) {
		string elem(argv[i]);
		directories.push_back(elem);
	}
	
	// Determine the media types of files
	const string csDir = getpwnam("cs253") -> pw_dir; // reads the directory of cs253 user
        string dir = csDir + "/pub/media-types";
        vector< pair<string, string> > mediaTypes = readMediaTypeFile(dir);
	
	
	struct stat statbuf;
	for(auto path : directory) {
        int openFile = lstat(path.c_str(), &statbuf);
		if (openFile != 0) {
			cerr << PROGNAME << ": cannot access '" << path << "': No such file or directory\n";
			continue;
		}
		// https://bit.ly/2SdYo4G
		string tokens = string(argv[1]);
		for(string::size_type i = 0; i < tokens.size(); ++i) {
			if( tokens[i] != '%') {
                		cout << tokens[i];
            		}
                	else {
                	++i;
                if(tokens[i] == 'n') {
                    cout << path;
                }
                else if(tokens[i] == 'p') {
                    string output;
                    permissions(statbuf, output);
                    cout << output;
                }
                else if(tokens[i] == 'u') {
                    cout << user_UID(statbuf);
                }
                else if(tokens[i] == 'U') {
                    cout << user_NAME(user_UID(statbuf));
                }
                else if(tokens[i] == 'g') {
                    cout << group_UID(statbuf);
                }
                else if(tokens[i] == 'G') {
                    cout << group_NAME(group_UID(statbuf));
                }
                else if(tokens[i] == 's') {
                    cout << size(statbuf);
                }
                else if(tokens[i] == 'a') {
                    cout << time(statbuf, 1, 0, 0);
                }
                else if(tokens[i] == 'm') {
                    cout << time(statbuf, 0, 1, 0);
                }
                else if(tokens[i] == 'c') {
                    cout << time(statbuf, 0, 0, 1);
                }
                else if(tokens[i] == 'M') {
                    cout << findMediaType(readMagicNumber(path), mediaTypes, statbuf);
                }
                }
            }
            cout << endl;
    }
    return 0;
}
