int main() {    
    char *x = new char[10];
    x[9] = 'a'; // invalid index
    delete[] x;
    return 0;
}
