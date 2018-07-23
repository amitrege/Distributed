#include "assert.h"
#include<stdlib.h>

int filter_cr1 (int* m) {
    if ((*m) == 1) {
        return 1;
    }
    return 0;
}

int filter_cr2 (int* m) {
    if ((*m) == 2) {
        return 1;
    }
    return 0;
}

int filter_c1 (int* m) {
    if ((*m) == 3) {
        return 1;
    }
    return 0;
}

int filter_c2 (int* m) {
    if ((*m) == 4) {
        return 1;
    }
    return 0;
}

/*
int rand() {
    int r;
    if(r) {
        return 0;
    }
    else {
        return 1;
    }
} 
*/

void TwoPhaseCommit(int pid, int leader, int num) {
    int commit = 0;
    int count = 1;
    int num_mbox = 0;
    
    int retry;

    int lab = 1;

    int old_count = count - 1;
    int old_lab = 0;

    int m;
    int* mbox[200];  // memcad doesn't handle variable size

    while (count < 10) {
        // New Transaction

        lab = 1; // Commit Request 1
        
        assert((count > old_count) || ((count == old_count) && (lab > old_lab)));
        old_count = count;
        old_lab = lab;

        if (pid == leader) {
            // send Transaction to all

            lab = 2; // Commit Request 2

            assert((count > old_count) || ((count == old_count) && (lab > old_lab)));
            old_count = count;
            old_lab = lab;

            // receive msgs in mbox
            // Empty mbox
            num_mbox = 0;

            retry = rand();
            while(retry && num_mbox < num){
                if(filter_cr2(&m)) {
                    mbox[num_mbox] = &m;
                    num_mbox++;
                }
                
                if(num_mbox == num) {
                    break;
                }   
                
                retry = rand();         
            }

            if (num_mbox == num) {
                commit = 1;  // Move to Commit Phase
                lab = 3; // Commit Phase 1

                assert((count > old_count) || ((count == old_count) && (lab > old_lab)));
                old_count = count;
                old_lab = lab;

                // send commit message
            }
            else {
                commit = 0; // Perform rollback
                lab = 3;

                assert((count > old_count) || ((count == old_count) && (lab > old_lab)));
                old_count = count;
                old_lab = lab;
        
                // send rollback message
            }

            lab = 4;

            assert((count > old_count) || ((count == old_count) && (lab > old_lab)));
            old_count = count;
            old_lab = lab;

            // receive
            // Empty mbox
            num_mbox = 0;
            
            retry = rand();           

            while(retry && num_mbox < num){
                if(filter_c2(&m)) {
                    mbox[num_mbox] = &m;
                    num_mbox++;
                }  
                if(num_mbox == num) {
                    break;
                } 

                retry = rand();  
            } 
            
            if (num_mbox == num) {
                // Complete Transaction
                count = count + 1;
            }
            else {
                count = count + 1;
            }
        }
        else {
            // receive Transaction
            // Empty mbox
            num_mbox = 0;
            
            retry = rand(); 

            while(retry && num_mbox < 1) { 
                if(filter_cr1(&m)) {
                    mbox[num_mbox] = &m;
                    num_mbox++;
                } 

                if(num_mbox >= 1) {
                    break;
                } 
                
                retry = rand(); 
            }

            if (num_mbox >= 1) {
                lab = 2; // Commit Request 2
    
                assert((count > old_count) || ((count == old_count) && (lab > old_lab)));
                old_count = count;
                old_lab = lab;
            
                // send agreement/ abort to leader

                lab = 3;
                
                assert((count > old_count) || ((count == old_count) && (lab > old_lab)));
                old_count = count;
                old_lab = lab;

                // receive
                // Empty mbox
                num_mbox = 0;
            
                retry = rand();  

                while(retry && num_mbox < 1){
                    if(filter_c1(&m)) {
                        mbox[num_mbox] = &m;
                        num_mbox++;
                    }  

                    if(num_mbox >= 1) {
                        break;
                    }
                    
                    retry = rand();   
                }

                if (num_mbox >= 1) {
                    // Complete Transaction
                    lab = 4; // Commit Phase 2
                
                    assert((count > old_count) || ((count == old_count) && (lab > old_lab)));
                    old_count = count;
                    old_lab = lab;

                    // send ack

                    count = count + 1;
                }
                else {
                    count = count + 1;
                }
            }
            else {
                count = count + 1;
            }
        } 
    }
}

int main () {
    TwoPhaseCommit(0,0,3);
    return 0;
}