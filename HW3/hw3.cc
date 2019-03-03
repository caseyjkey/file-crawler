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

string inttohex(int num) {
	// https://bit.ly/2InTEd9
    string d = "0123456789abcdef"; //
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
        //cout << "character: " << chrctr << "\n";
		if(chrctr < '!' || chrctr > '~') {
			magicNum += "%" +  inttohex(chrctr);
		} else {
			magicNum += chrctr;
		}
	}
	return magicNum;
}


string findMediaType(string magicNum, vector< pair<string, string> > MediaTypes, struct stat &statbuf) {
	if(S_ISDIR(statbuf.st_mode)) return "inode/directory";
	if(S_ISLNK(statbuf.st_mode)) return "inode/symlink";
    if(size(statbuf) == 0) return "inode/empty";
    
    bool match;
	for(auto& elem : MediaTypes) {
		match = 1;
		for(unsigned int i = 0; i < elem.first.length(); i++) {
            //cout << magicNum[i] << " compared to " << elem.first[i] << "\n";
			if(magicNum[i] != elem.first[i]) {
                match = 0;
            }
		}
		if(match) return elem.second;
	}
	return "application/octet-data";
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
	vector<string> directory;
	for(int i = 2; i < argc; i++) {
		string elem(argv[i]);
		directory.push_back(elem);
	}
	
	// Determine the media types of files
	const string csDir = getpwnam("cs253") -> pw_dir;
        string dir = csDir + "/pub/media-types";
        vector< pair<string, string> > mediaTypes = readMediaTypeFile(dir);
	
	
	struct stat statbuf;
	for(auto file : directory) {
        int openFile = lstat(file.c_str(), &statbuf);
		if (openFile != 0) {
			cerr << PROGNAME << ": cannot access '" << file << "': No such file or directory\n";
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
                    cout << file;
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
                    //cout << "Magic number: " << readMagicNumber(file) << "\n";
                    cout << findMediaType(readMagicNumber(file), mediaTypes, statbuf);
                }
                }
            }
            cout << endl;
    }
    return 0;
}
