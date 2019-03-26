int main() {
    double *p = new double[1000];
    delete[] p; // have to delete as an array
    return 0;
}
