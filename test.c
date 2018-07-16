void test_2 () { 
    int retry;
    volatile int random;
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
        // receive command from client
        cmd = random;
        if (cmd == 0) {   // Empty command (HeartBeat)
            lab_normal = 1;

            assert(currentTerm == old_term);
            assert(old_lab_election == lab_election);
            assert(commitIndex == old_commit);
            assert(old_lab_normal == 0 || old_lab_normal == 1);
            assert(lastIndex >= old_LLI);

            //assert ((currentTerm > old_term) || ((currentTerm == old_term) && (lab_election > old_lab_election)) || ((currentTerm == old_term) && (lab_election == old_lab_election) && (commitIndex > old_commit)) || ((currentTerm == old_term) && (lab_election == old_lab_election) && (commitIndex == old_commit) && (lab_normal > old_lab_normal)) || ((currentTerm == old_term) && (lab_election == old_lab_election) && (commitIndex == old_commit) && (lab_normal == old_lab_normal) && (lastIndex >= old_LLI)));            
            old_term = currentTerm;
            old_lab_election = lab_election;
            old_commit = commitIndex;
            old_lab_normal = lab_normal;
            old_LLI = lastIndex;

            // assert((currentTerm == old_term) && (lab_election == old_lab_election) && (commitIndex == old_commit) && (lab_normal == old_lab_normal) && (lastIndex == old_LLI));

            // send(term, leaderId, prevLogIndex, entries[], leaderCommit) with empty entries
        }
        else {
            lastIndex = lastIndex + 1;

            lab_normal = 1;

            assert(currentTerm == old_term);
            assert(old_lab_election == lab_election);
            assert(commitIndex == old_commit);
            assert(old_lab_normal == 0 || old_lab_normal == 1);
            assert(lastIndex >= old_LLI);

            //assert ((currentTerm > old_term) || ((currentTerm == old_term) && (lab_election > old_lab_election)) || ((currentTerm == old_term) && (lab_election == old_lab_election) && (commitIndex > old_commit)) || ((currentTerm == old_term) && (lab_election == old_lab_election) && (commitIndex == old_commit) && (lab_normal > old_lab_normal)) || ((currentTerm == old_term) && (lab_election == old_lab_election) && (commitIndex == old_commit) && (lab_normal == old_lab_normal) && (lastIndex >= old_LLI)));            
            old_term = currentTerm;
            old_lab_election = lab_election;
            old_commit = commitIndex;
            old_lab_normal = lab_normal;
            old_LLI = lastIndex;
                
            // send(term, leaderId, prevLogIndex, entries[], leaderCommit)
        }
        retry = random;
    }
}

int main() {
    test_2();
    return 0;
}