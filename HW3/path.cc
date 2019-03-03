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
