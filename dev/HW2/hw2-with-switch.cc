#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <pwd.h>
#include <grp.h>

using namespace std;

static string PROGNAME;
 //corn fritters

string filename(string files[], int index) {
	return files[index];
}

int user_UID(string user) {
	// https://ibm.co/2GwdIIR
	// Option to access via stat
	struct passwd *pwd;
	if (( pwd = getpwnam(user.c_str()) ) == NULL) {
		cerr << PROGNAME << ": no matching password record found\n";
		return 1;
	}
	else {
        return pwd->pw_uid;
	}
}
int user_UID(struct stat &statbuf) {
	// https://ibm.co/2GwdIIR
	// Option to access via stat
	return static_cast<int>(statbuf.st_uid);
}

string user_NAME(int uid) {
	struct passwd *pwd;
	if((pwd = getpwuid(uid)) != NULL)
		return pwd->pw_name;
	else
		cerr << PROGNAME << ": " << uid << " not found in user database\n";
	return "uid not found";
}

int group_UID(struct stat &statbuf) {
    return statbuf.st_gid;
}
string group_NAME(int gid) {
    struct group *grp;
    if((grp = getgrgid(gid)) != NULL)
        return grp->gr_name;
    else
        cerr << PROGNAME << ": " << gid << " not found in group database\n";
    return "gid not found";
}


int permissions(struct stat &statbuf, string &output) {
	ostringstream os;
	if(S_ISDIR(statbuf.st_mode)) os <<  "d";
	else if(S_ISLNK(statbuf.st_mode)) os << "l";
	else if(S_ISREG(statbuf.st_mode)) os << "-";
	else {
		cerr << PROGNAME << ": " <<  "replace with file" << " is an undefined file type, wtf?!\n";
		return 1;
	}
	os << (statbuf.st_mode & S_IRUSR ? 'r' : '-');
	os << (statbuf.st_mode & S_IWUSR ? 'w' : '-');
	os << (statbuf.st_mode & S_IXUSR ? 'x' : '-');
	os << (statbuf.st_mode & S_IRGRP ? 'r' : '-');
	os << (statbuf.st_mode & S_IWGRP ? 'w' : '-');
	os << (statbuf.st_mode & S_IXGRP ? 'x' : '-');
	os << (statbuf.st_mode & S_IROTH ? 'r' : '-');
	os << (statbuf.st_mode & S_IWOTH ? 'w' : '-');
	os << (statbuf.st_mode & S_IXOTH ? 'x' : '-');
	output = os.str();
	return 0;
}
int size(struct stat &statbuf) {
	return statbuf.st_size;
}
string time(struct stat &statbuf, bool access = 0, bool mod = 1, bool status = 0) {
    time_t fileTime;
    if(mod) {
        fileTime = statbuf.st_mtime;
    }
    else if(access) {
        fileTime = statbuf.st_atime;
    }
    else if(status) {
        fileTime = statbuf.st_ctime;
    }
	auto timeval = localtime(&fileTime);
	char buf[32];
	strftime(buf, sizeof(buf), "%Y-%m-%dT%T", timeval);
	string timeOutput(buf);
	return timeOutput;
}

vector<string> parseFormatString(string str, int len) {
	vector<string> tokens;
	size_t pos = 0;
	while ((pos = str.find('%')) != string::npos) {
		tokens.push_back(str.substr(pos+1, len));
		str.erase(pos, 1);
	}
	return tokens;
}
/*
map<string, string> readMediaTypeFile(string dir) {
	map<string, string> mediaTypes;
	ifstream inFile;
	inFile.open(dir);
	string token;
	pair<string, string> mediaEntry;
	while(inFile >> token) {
		mediaEntry.first = token;
		inFile >> token;
		mediaEntry.second = token;
		mediaTypes.insert(mediaEntry);
	}
	return mediaTypes;
} */

vector< pair<string, string> > readMediaTypeFile(string dir) {
	vector< pair <string, string> > mediaTypes;
	ifstream inFile;
	inFile.open(dir);
	string token;
	pair<string, string> mediaEntry;
	while(inFile >> token) {
		mediaEntry.first = token;
		inFile >> token;
		mediaEntry.second = token;
		mediaTypes.push_back(mediaEntry);
	}
	return mediaTypes;
}

string stohex(int num) {
	// https://bit.ly/2InTEd9
    string d = "0123456789ABCDEF";
    string res;
    while(num > 0)
    {
        res = d[num % 16] + res;
        num /= 16;
    }
    if(res.length() == 1) res = "0" + res;
    return res;
}

string readMagicNumber(string dir) {
	ifstream file(dir);
	char chrctr;
	string magicNum;
	for ( int i = 0; i < 32; i++ ) {
		file.get(chrctr);
		if(chrctr < '!' || chrctr > '~') {
			magicNum += "%" +  stohex(chrctr);
		} else {
			magicNum += chrctr;
		}
	}
	return magicNum;
}

/*
string findMediaType(string magicNum, map<string, string> MediaTypes) {
	bool match;
	for(auto& elem : MediaTypes) {
		match = 0;
		for(unsigned int i = 0; i < elem.first.length(); i++) {
			if(magicNum[i] == elem.first[i]) {
				debug << "\n" << magicNum[i] << " and " << elem.first[i] << "\n";
				match = 1;
			}
		}
		debug << match;
		if(match) return elem.second;
	}
	return "application/octet-data";
} */


string findMediaType(string magicNum, vector< pair<string, string> > MediaTypes) {
	bool match;
	for(auto& elem : MediaTypes) {
		match = 1;
		for(unsigned int i = 0; i < elem.first.length(); i++) {
			if(magicNum[i] != elem.first[i]) {
                match = 0;
            }
		}
		if(match) return elem.second;
	}
	return "application/octet-data";
}

int main(int argc, char* argv[]) {

	if(argv[1] == nullptr) {
		cerr << PROGNAME << " usage: ./hw1 [FILE] [DIRECTORY]\n";
		return 1;
	}

	// Read the flags
	vector<string> tokens = parseFormatString(argv[1], 1);

	// Create an array containing each directory/file
	string directory[10] = {};
	for(int i = 2; i < argc; i++) {
		string elem(argv[i]);
		directory[i - 2] = elem;
	}
	
	// Determine the media types of files
    const string csDir = getpwnam("cs253") -> pw_dir;
    string dir = csDir + "/pub/media-types";
	//map<string, string> mediaTypes = readMediaTypeFile(dir);
    vector< pair<string, string> > mediaTypes = readMediaTypeFile(dir);
	
	
	struct stat statbuf;
	for(string file : directory) {
        int openFile = lstat(file.c_str(), &statbuf);
		if (openFile != 0) {
			cerr << PROGNAME << ": " << file << "failed to open\n";
			continue;
		}
		// https://bit.ly/2SdYo4G
        for(auto token : tokens) {
                cout << token;
                switch(token) {
                    case "n":
                        cout << file << " ";
                        break;
                    case "p":
                        string output;
                        permissions(&statbuf, &output);
                        cout << output << " ";
                        break;
                    case "u":
                        cout << user_UID(&statbuf) << " ";
                        break;
                    case "U":
                        cout << user_NAME(user_UID(&statbuf)) << " ";
                        break;
                    case "g":
                        cout << group_UID(&statbuf) << " ";
                        break;
                    case "G":
                        cout << group_NAME(group_UID(&statbuf)) << " ";
                        break;
                    case "s":
                        cout << size(&statbuf) << " ";
                        break;
                    case "a":
                        cout << time(&statbuf, access = 1, mod = 0) << " ";
                        break;
                    case "m":
                        cout << time(&statbuf) << " ";
                        break;
                    case "c":
                        cout << time(&statbuf, mod = 0, status = 1) << " ";
                        break;
                    case "M":
                        string magicNum = read
                        cout << findMediaType(readMagicNumber(file), mediaTypes) << " ";
                        break;
                }
        }
		
	}
	
	
    //for(auto& elem : mediaTypes) {
		//cout << elem.first << " | " << elem.second << "\n";
	//}
	string magicNum = readMagicNumber("hw2.cc");

	return 0;
}
