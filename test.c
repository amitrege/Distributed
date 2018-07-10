int main() {
    int r;

    /*
    while (r) {
        // Do something
    }
*/
    //_memcad("assume(r==0)");
    // r should be 0
    assert(r == 0);
    
    _memcad("assume(r>=0)");

    // r should be >= 0
    assert(r >= 0);
}
