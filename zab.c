#include <stdlib.h> 
#include <stdio.h>
#include <string.h>

int p = 0;
int lab = 0;
int i = 0;
int labr = 0;
int leader = 0;

typedef struct _tag {
    int p;
    int lab;
    int i;
    int labr;
} tag;

typedef struct _msg_propose {
    int p;
    int i;
} msg_propose;

msg_propose init_propose () {
    msg_propose m;
    m.p = rand();
    m.i = rand();
}

int filter_propose (msg_propose m, int p, int i) {
    if (m.p == p && m.i == i)
        return 1;
    return 0;
}

typedef struct _msg_ack_p {
    int p; 
    int lab;
    int i;
    int labr;
    int val;
} msg_ack_p;

msg_ack_p init_ack_p () {
    msg_ack_p m;
    m.p = rand();
    m.lab = (rand() % 3) + 1;
    m.i = rand();
    m.labr = (rand() % 6) + 1;
}

int filter_ack_p (msg_ack_p m, int p, int lab, int i, int labr) {
    if (m.p == p && m.lab == lab && m.i == i && m.labr == labr)
        return 1;
    return 0;
}

typedef struct _msg_cmt {
    int p;
    int i;
    int labr;
} msg_cmt;

msg_cmt init_cmt () {
    msg_cmt m;
    m.p = rand();
    m.i = rand();
    m.labr = (rand() % 6) + 1;
}

int filter_cmt (msg_cmt m, int p, int i, int labr) {
    if (m.p == p && m.i == i && m.labr == labr)
        return 1;
    return 0;
}

typedef struct _msg_curr_e {
    int p;
    int lab;
} msg_curr_e;

msg_curr_e init_curr_e () {
    msg_curr_e m;
    m.p = rand();
    m.lab = (rand() % 6) + 1;
}

int filter_curr_e (msg_curr_e m, int p, int lab) {
    if (m.p == p && m.lab == lab)
        return 1;
    return 0;
}

typedef struct _msg_new_e {
    int p;
    int lab;
} msg_new_e;

msg_new_e init_new_e () {
    msg_new_e m;
    m.p = rand();
    m.lab = (rand() % 6) + 1;
}

int filter_new_e (msg_new_e m, int p, int lab) {
    if (m.p == p && m.lab == lab)
        return 1;
    return 0;
}

typedef struct _msg_ack_e {
    int p;
    int lab;
    // history not included (as it is not relevant to prove increasing tags)
} msg_ack_e;

msg_ack_e init_ack_e () {
    msg_ack_e m;
    m.p = rand();
    m.lab = (rand() % 6) + 1;
}

int filter_ack_e (msg_ack_e m, int p, int lab) {
    if (m.p == p && m.lab == lab)
        return 1;
    return 0;
}

typedef struct _msg_new_l {
    int p;
    int lab;
    // a and history not included (as it is not relevant to prove increasing tags)
} msg_new_l;

msg_new_l init_new_l () {
    msg_new_l m;
    m.p = rand();
    m.lab = (rand() % 6) + 1;
}

int filter_new_l (msg_new_l m, int p, int lab) {
    if (m.p == p && m.lab == lab)
        return 1;
    return 0;
}

typedef struct _msg_ack_l {
    int p;
    int lab;
    // a and history not included (as it is not relevant to prove increasing tags)
} msg_ack_l;

msg_ack_l init_ack_l () {
    msg_ack_l m;
    m.p = rand();
    m.lab = (rand() % 6) + 1;
}

int filter_ack_l (msg_ack_l m, int p, int lab) {
    if (m.p == p && m.lab == lab)
        return 1;
    return 0;
}

typedef struct _msg_com {
    int p;
    int lab;
    // a and history not included (as it is not relevant to prove increasing tags)
} msg_com;

msg_com init_com () {
    msg_com m;
    m.p = rand();
    m.lab = (rand() % 6) + 1;
}

int filter_com (msg_com m, int p, int lab) {
    if (m.p == p && m.lab == lab)
        return 1;
    return 0;
}

int sendingThread (int p, int lab, int i, int labr){

    i = 1;

    labr = 1;
    
    while (1){
        // Send v-in
        i++;

        // assert p > old_p || (p == old_p) ==> lab > old_lab || (p == old_p && lab == old_lab) ==> i > old_i || (p == old_p && lab == old_lab && i == old_i) ==> labr >= old_labr
        // old_p = p;
        // old_lab = lab;
        // old_i = i;
        // old_labr = labr;
    }
}

int Broadcast (int num, int pid, int leader, int *p, int *lab, int *i, int *labr, int *old_p, int *old_lab, int *old_i, int *old_labr) {
    msg_propose mbox_propose[2*num];
    int num_mbox_propose = 0;
    msg_propose m_propose;

    msg_ack_p mbox_ack_p[2*num];
    int num_mbox_ack_p = 0;
    msg_ack_p m_ack_p;

    msg_cmt mbox_cmt[2*num];
    int num_mbox_cmt = 0;
    msg_cmt m_cmt;

    int retry;

    // To clear the first assert on loop entry
    *old_i = 0;
    *old_labr = 0;

    while(1) {
        *i = 1;
        *labr = 1; // Prp

        // assert *p > *old_p || (*p == *old_p) ==> *lab > *old_lab || (*p == *old_p && *lab == *old_lab) ==> *i > *old_i || (*p == *old_p && *lab == *old_lab && *i == *old_i) ==> *labr >= *old_labr
        // *old_p = *p;
        // *old_lab = *lab;
        // *old_i = *i;
        // *old_labr = *labr;

        if (pid != leader) {
            // Empty mbox
            memset(mbox_propose,0,sizeof(mbox_propose));
            num_mbox_propose = 0;
    
            // receive propose
            retry = rand() % 2;
            while(retry) {
                m_propose = init_propose();
    
                if(filter_propose(m_propose, *p, *i)) {
                    mbox_propose[num_mbox_propose] = m_propose;
                    num_mbox_propose++;
                }
    
                if (num_mbox_propose >= 1)
                    break;
    
                retry = rand() % 2;
            }

            if (num_mbox_propose >= 1) {
                // Update mbox
                // Update history
            
                *labr = 2; // ack_p

                // assert *p > *old_p || (*p == *old_p) ==> *lab > *old_lab || (*p == *old_p && *lab == *old_lab) ==> *i > *old_i || (*p == *old_p && *lab == *old_lab && *i == *old_i) ==> *labr >= *old_labr
                // *old_p = *p;
                // *old_lab = *lab;
                // *old_i = *i;
                // *old_labr = *labr;

                // send(p, lab, i, labr, val) to leader

                *labr = 3; // cmt

                // assert *p > *old_p || (*p == *old_p) ==> *lab > *old_lab || (*p == *old_p && *lab == *old_lab) ==> *i > *old_i || (*p == *old_p && *lab == *old_lab && *i == *old_i) ==> *labr >= *old_labr
                // *old_p = *p;
                // *old_lab = *lab;
                // *old_i = *i;
                // *old_labr = *labr;
                
                // Empty mbox
                memset(mbox_cmt,0,sizeof(mbox_cmt));
                num_mbox_cmt = 0;

                // receive commit from leader
                retry = rand() % 2;
                while(retry) {
                    m_cmt = init_cmt();
        
                    if(filter_cmt(m_cmt, *p, *i, *labr)) {
                        mbox_cmt[num_mbox_cmt] = m_cmt;
                        num_mbox_cmt++;
                    }
        
                    if (num_mbox_cmt >= 1)
                        break;
        
                    retry = rand() % 2;
                }

                if (num_mbox_cmt >= 1) {
                    // Commit
                    // out()
                    (*i)++;
                }
                else {
                    (*p)++;
                    break;
                }               
            }
            else
                continue;
        }
        else {
            // send propose

            *labr = 2; // ack_p
            
            // assert *p > *old_p || (*p == *old_p) ==> *lab > *old_lab || (*p == *old_p && *lab == *old_lab) ==> *i > *old_i || (*p == *old_p && *lab == *old_lab && *i == *old_i) ==> *labr >= *old_labr
            // *old_p = *p;
            // *old_lab = *lab;
            // *old_i = *i;
            // *old_labr = *labr;
            
            // Empty mbox
            memset(mbox_ack_p,0,sizeof(mbox_ack_p));
            num_mbox_ack_p = 0;

            // receive ack_p from followers
            retry = rand() % 2;
            while(retry) {
                m_ack_p = init_ack_p();
    
                if(filter_ack_p(m_ack_p, *p, *lab, *i, *labr)) {
                    mbox_ack_p[num_mbox_ack_p] = m_ack_p;
                    num_mbox_ack_p++;
                }
    
                if (num_mbox_ack_p >= num/2)
                    break;
    
                retry = rand() % 2;
            }

            if (num_mbox_ack_p >= num/2) {
                *labr = 3; // Commit

                // assert *p > *old_p || (*p == *old_p) ==> *lab > *old_lab || (*p == *old_p && *lab == *old_lab) ==> *i > *old_i || (*p == *old_p && *lab == *old_lab && *i == *old_i) ==> *labr >= *old_labr
                // *old_p = *p;
                // *old_lab = *lab;
                // *old_i = *i;
                // *old_labr = *labr;

                // send commit (labr, a, i) to all

                (*i)++;
            }
            else {
                (*p)++;
                break;
            }
        }
    }
}

int main_thread(int pid, int num){
    int lab = 0;
    int p = 0;
    int i = 0;
    int labr = 0;

    int leader;

    msg_curr_e mbox_curr_e[2*num];
    int num_mbox_curr_e = 0;
    msg_curr_e m_curr_e;

    msg_new_e mbox_new_e[2*num];
    int num_mbox_new_e = 0;
    msg_new_e m_new_e;

    msg_ack_e mbox_ack_e[2*num];
    int num_mbox_ack_e = 0;
    msg_ack_e m_ack_e;

    msg_new_l mbox_new_l[2*num];
    int num_mbox_new_l = 0;
    msg_new_l m_new_l;

    msg_ack_l mbox_ack_l[2*num];
    int num_mbox_ack_l = 0;
    msg_ack_l m_ack_l;

    msg_com mbox_com[2*num];
    int num_mbox_com = 0;
    msg_com m_com;

    int retry;

    int old_p = 0;
    int old_lab = 0;
    int old_i = 0;
    int old_labr = 0;

    while (1) {
        leader = rand() % num;

        lab = 1; // Curr_E

        // assert p > old_p || (p == old_p) ==> lab > old_lab || (p == old_p && lab == old_lab) ==> i > old_i || (p == old_p && lab == old_lab && i == old_i) ==> labr >= old_labr
        // old_p = p;
        // old_lab = lab;
        // old_i = i;
        // old_labr = labr;
        
        // send (p,lab) to leader
    
        if (leader == pid){
            // Empty mbox
            memset(mbox_curr_e,0,sizeof(mbox_curr_e));
            num_mbox_curr_e = 0;

            // receive curr_e
            retry = rand() % 2;
            while(retry) {
                m_curr_e = init_curr_e();
    
                if(filter_curr_e(m_curr_e, p, lab)) {
                    mbox_curr_e[num_mbox_curr_e] = m_curr_e;
                    num_mbox_curr_e++;
                }
    
                if (num_mbox_curr_e >= num/2)
                    break;
    
                retry = rand() % 2;
            }
            
            if (num_mbox_curr_e >= num/2) {
                // Update set Q which is the set of pids in mbox
                // get max p in mbox and add 1
                p = p + (rand() % 3); // Actually, p is the max value of all p's received by the leader
                p = p++;

                lab = 2; // new_e

                // assert p > old_p || (p == old_p) ==> lab > old_lab || (p == old_p && lab == old_lab) ==> i > old_i || (p == old_p && lab == old_lab && i == old_i) ==> labr >= old_labr
                // old_p = p;
                // old_lab = lab;
                // old_i = i;
                // old_labr = labr;

                //send p to all (in set q)

                lab = 3; // ack_e

                // assert p > old_p || (p == old_p) ==> lab > old_lab || (p == old_p && lab == old_lab) ==> i > old_i || (p == old_p && lab == old_lab && i == old_i) ==> labr >= old_labr
                // old_p = p;
                // old_lab = lab;
                // old_i = i;
                // old_labr = labr;

                // Empty mbox
                memset(mbox_ack_e,0,sizeof(mbox_ack_e));
                num_mbox_ack_e = 0;

                // receive ack_e
                retry = rand() % 2;
                while(retry) {
                    m_ack_e = init_ack_e();
        
                    if(filter_ack_e(m_ack_e, p, lab)) {
                        mbox_ack_e[num_mbox_ack_e] = m_ack_e;
                        num_mbox_ack_e++;
                    }
        
                    if (num_mbox_ack_e >= num/2)
                        break;
        
                    retry = rand() % 2;
                }

                if(num_mbox_ack_e >= num/2) {
                    // Update history

                    lab = 4; // new_l

                    // assert p > old_p || (p == old_p) ==> lab > old_lab || (p == old_p && lab == old_lab) ==> i > old_i || (p == old_p && lab == old_lab && i == old_i) ==> labr >= old_labr
                    // old_p = p;
                    // old_lab = lab;
                    // old_i = i;
                    // old_labr = labr;

                    // send (p, lab, h) to Q

                    lab = 5; // ack_l

                    // assert p > old_p || (p == old_p) ==> lab > old_lab || (p == old_p && lab == old_lab) ==> i > old_i || (p == old_p && lab == old_lab && i == old_i) ==> labr >= old_labr
                    // old_p = p;
                    // old_lab = lab;
                    // old_i = i;
                    // old_labr = labr;

                    // Empty mbox
                    memset(mbox_ack_l,0,sizeof(mbox_ack_l));
                    num_mbox_ack_l = 0;

                    // receive ack_l
                    retry = rand() % 2;
                    while(retry) {
                        m_ack_l = init_ack_l();
            
                        if(filter_ack_l(m_ack_l, p, lab)) {
                            mbox_ack_l[num_mbox_ack_l] = m_ack_l;
                            num_mbox_ack_l++;
                        }
            
                        if (num_mbox_ack_l >= num/2)
                            break;
            
                        retry = rand() % 2;
                    }
                    
                    if (num_mbox_ack_l >= num/2) {
                        lab = 6; // cmt

                        // assert p > old_p || (p == old_p) ==> lab > old_lab || (p == old_p && lab == old_lab) ==> i > old_i || (p == old_p && lab == old_lab && i == old_i) ==> labr >= old_labr
                        // old_p = p;
                        // old_lab = lab;
                        // old_i = i;
                        // old_labr = labr;

                        // send (p, lab, h) to Q

                        // Start Broadcast
                        Broadcast(num, pid, leader, &p, &lab, &i, &labr, &old_p, &old_lab, &old_i, &old_labr);
                    }
                    else 
                        continue;
                }
                else 
                    continue;
            }
            else
                continue;
        }
        else {
            lab = 2; // New_E

            // assert p > old_p || (p == old_p) ==> lab > old_lab || (p == old_p && lab == old_lab) ==> i > old_i || (p == old_p && lab == old_lab && i == old_i) ==> labr >= old_labr
            // old_p = p;
            // old_lab = lab;
            // old_i = i;
            // old_labr = labr;

            // Empty mbox
            memset(mbox_new_e,0,sizeof(mbox_new_e));
            num_mbox_new_e = 0;

            // receive new_e
            retry = rand() % 2;
            while(retry) {
                m_new_e = init_new_e();
    
                if(filter_new_e(m_new_e, p, lab)) {
                    mbox_new_e[num_mbox_new_e] = m_new_e;
                    num_mbox_new_e++;
                }
    
                if (num_mbox_new_e >= 1)
                    break;
    
                retry = rand() % 2;
            }
            
            if (num_mbox_curr_e >= 1) {
                p = p + (rand() % 3); // Actually, p is the max value of all p's received by the leader
                p++;

                lab = 3; // ack_e

                // assert p > old_p || (p == old_p) ==> lab > old_lab || (p == old_p && lab == old_lab) ==> i > old_i || (p == old_p && lab == old_lab && i == old_i) ==> labr >= old_labr
                // old_p = p;
                // old_lab = lab;
                // old_i = i;
                // old_labr = labr;

                // send (p, lab, a, h) to leader

                lab = 4; // new_l

                // assert p > old_p || (p == old_p) ==> lab > old_lab || (p == old_p && lab == old_lab) ==> i > old_i || (p == old_p && lab == old_lab && i == old_i) ==> labr >= old_labr
                // old_p = p;
                // old_lab = lab;
                // old_i = i;
                // old_labr = labr;

                // Empty mbox
                memset(mbox_new_l,0,sizeof(mbox_new_l));
                num_mbox_new_l = 0;

                // receive new_l
                retry = rand() % 2;
                while(retry) {
                    m_new_l = init_new_l();
        
                    if(filter_new_l(m_new_l, p, lab)) {
                        mbox_new_l[num_mbox_new_l] = m_new_l;
                        num_mbox_new_l++;
                    }
        
                    if (num_mbox_new_l >= 1)
                        break;
        
                    retry = rand() % 2;
                }

                if(num_mbox_new_l >= 1) {
                    // Update a
                    // Update history

                    lab = 5; // ack_l

                    // assert p > old_p || (p == old_p) ==> lab > old_lab || (p == old_p && lab == old_lab) ==> i > old_i || (p == old_p && lab == old_lab && i == old_i) ==> labr >= old_labr
                    // old_p = p;
                    // old_lab = lab;
                    // old_i = i;
                    // old_labr = labr;

                    // send (lab, a, h) to leader

                    lab = 6; // cmt

                    // assert p > old_p || (p == old_p) ==> lab > old_lab || (p == old_p && lab == old_lab) ==> i > old_i || (p == old_p && lab == old_lab && i == old_i) ==> labr >= old_labr
                    // old_p = p;
                    // old_lab = lab;
                    // old_i = i;
                    // old_labr = labr;
                    
                    // Empty mbox
                    memset(mbox_com,0,sizeof(mbox_com));
                    num_mbox_com = 0;
                    
                    // receive commit
                    retry = rand() % 2;
                    while(retry) {
                        m_com = init_com();
            
                        if(filter_com(m_com, p, lab)) {
                            mbox_com[num_mbox_com] = m_com;
                            num_mbox_com++;
                        }
            
                        if (num_mbox_com >= 1)
                            break;
            
                        retry = rand() % 2;
                    }

                    if (num_mbox_com >= 1) {
                        // Update zvid

                        // Start Broadcast
                        Broadcast(num, pid, leader, &p, &lab, &i, &labr, &old_p, &old_lab, &old_i, &old_labr);
                    }
                    else
                        continue;
                }
                else
                    continue;
            }
            else {
                // Special Case (state transfer?) 
                p++;
                continue;
            }
        }
    }
}


