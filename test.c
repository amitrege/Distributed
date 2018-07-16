void test () { 
    int retry;
    volatile int random;
    volatile int random2;
    int cmd;
    
    int currentTerm = 0;
    int lab_election = 0;
    int commitIndex = 0;
    int lab_normal = 0;
    int lastIndex = 0;

    int old_term = 0;
    int old_lab_election = 0;
    int old_commit = 0;
    int old_lab_normal = 0;
    int old_LLI = 0;

    retry = random;
    
    while (retry) {
        lab_normal = 1;
        
        assert(old_lab_normal == 0 || old_lab_normal == 1);

        old_lab_normal = lab_normal;
        
        retry = random;
    }
}

int main() {
    test();
    return 0;
}