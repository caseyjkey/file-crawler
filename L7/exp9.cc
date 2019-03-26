int main() {
    long double *a = new long double [10];
    long double *b = a;        
    a+=3; // we are losing the first three indexs
    delete [] b;
    return 0;
}
