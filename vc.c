#include <stdlib.h> 
#include <stdio.h>
#include <string.h>

typedef struct _msg_startVC {
    int v;
    int pid;
} msg_startVC;

msg_startVC init_startVC(int num) {
    msg_startVC m;
    m.v = rand();
    m.pid = rand() % num;
}

int filter_startVC (msg_startVC m, int v) {
    if (m.v > v) 
        return 1;
    return 0; 
}

typedef struct _msg_doVC {
    int v;
    int pid;
    int log_k;
    int log_top;
} msg_doVC;

msg_doVC init_doVC(int num) {
    msg_doVC m;
    m.v = rand();
    m.pid = rand() % num;
    m.log_k = rand();
    m.log_top = rand();
}

int filter_doVC (msg_doVC m, int v) {
    if (m.v == v) 
        return 1;
    return 0; 
}

typedef struct _msg_startView {
    int v;
    int pid;
    int log_k;
    int log_top;
} msg_startView;

msg_startView init_startView(int num) {
    msg_startView m;
    m.v = rand();
    m.pid = rand() % num;
    m.log_k = rand();
    m.log_top = rand();
}

int filter_startView (msg_startView m, int v) {
    if (m.v > v) 
        return 1;
    return 0; 
}

typedef struct _msg_prep {
    int v;
    int n;
    int k;
} msg_prep;

msg_prep init_prep() {
    msg_prep m;
    m.v = rand();
    m.n = rand();
    m.k = rand();
}

int filter_prep (msg_prep m, int n, int v) {
    if (m.v == v && m.n == n+1) 
        return 1;
    return 0; 
}

typedef struct _msg_prepOK {
    int v;
    int n;
    int k;
} msg_prepOK;

msg_prepOK init_prepOK() {
    msg_prepOK m;
    m.v = rand();
    m.n = rand();
}

int filter_prepOK (msg_prepOK m, int v, int k) {
    if (m.n == k+1 && m.v == v) 
        return 1;
    return 0; 
}

/*
int vc_msg (msg m, int v) {
    if ((m.lab_vc == 1 || m.lab_vc == 2) && m.v > v)  
        return 1;
    return 0; 
}
*/

int NormalOp(int pid, int leader, int v, int n, int k, int num, int *log)
{
    int lab;
    int retry;
    int timeout;
    int mbox;
    int cmd; // Declaration for incoming command
    int vc_msg;
    int failure = 0;

    msg_prep mbox_prep[2*num];
    int num_mbox_prep = 0;
    msg_prep m_prep;

    msg_prepOK mbox_prepOK[2*num];
    int num_mbox_prepOK = 0;
    msg_prepOK m_prepOK;

    while (1) {
        if (pid == leader) {
            while (retry) {
                // cmd = in()
                cmd = rand();

                // Command is empty
                if (cmd == 0)
                {
                    lab = 1; // Prepare

                    // assert
                    // failure = 0;

                    // send empty prepare with commit number
                }
                else
                {
                    n++;
                    //log[n] = cmd;

                    lab = 1; // Prepare
                    // assert
                    // failure = 0;

                    // send prep
                }
            }


            lab = 2;

            retry = rand() % 2;
            while (retry) {
            
                timeout = rand() % 2;
                if (timeout)
                    return 0;
   

                // receive Transaction
                m_prepOK = init_prepOK();
                if(filter_prepOK(m_prepOK, v, k)) {
                    mbox_prepOK[num_mbox_prepOK] = m_prepOK;
                    num_mbox_prepOK++;
                } 

                if (num_mbox_prepOK >= num/2) { 
                    // out()
                    k++;
                        
                    // Empty mbox
                    memset(mbox_prepOK,0,sizeof(mbox_prepOK));
                    num_mbox_prepOK = 0;
                }
                else {
                    failure = 1;
                }
            

                vc_msg = rand() % 10;
                if (vc_msg)              // if msg is startVC or doVC
                    return 0; // to VC

                retry = rand() % 2;
            }
        }
        else {
            while (retry) {
                // Empty mbox
                memset(mbox_prep,0,sizeof(mbox_prep));
                num_mbox_prep = 0;   
    
                // receive Transaction
                retry = rand() % 2;
                while(retry){
                    m_prep = init_prep();
                    if(filter_prep(m_prep, v, n)) {
                        mbox_prep[num_mbox_prep] = m_prep;
                        num_mbox_prep++;
                    } 
                    if(num_mbox_prep >= 1) 
                        break; 
                    retry = rand() % 2;
    
                    vc_msg = rand() % 10;
                    if (vc_msg)              // if msg is startVC or doVC
                        return 0; // to VC
                }
    
                timeout = rand() % 2;
                if (timeout) 
                    return 0;
                
                if (num_mbox_prep >= 1) {
                    if (pid != leader) {
                        n++;
                        //log[n] = m.req;
                        if (mbox_prep[0].k > k) {
                            if (mbox_prep[0].k > n)
                            {
                                // State transfer
                            }
                            k = mbox_prep[0].k;
                            // Commit all requests < k
                        }
                    }
                    
                    lab = 2;

                    // send Ack
                                    
                }
            }
        }
    }    
}

// Tag (v, lab_vc)
int VC(int pid, int num, int v)
{ 
    int lab_vc;
    int old_v; 
    int old_lab_vc;
    
    int n;
    int *log;
    int k;

    int retry;

    msg_startVC mbox_startVC[2*num];
    int num_mbox_startVC = 0;
    msg_startVC m_startVC;

    msg_doVC mbox_doVC[2*num];
    int num_mbox_doVC = 0;
    msg_doVC m_doVC;

    msg_startView mbox_startView[2*num];
    int num_mbox_startView = 0;
    msg_startView m_startView;    
    
    while (1) { 
    // loop invariant v > old_v
        
        int leader = v % num; // num = no of processes
        lab_vc = 1; // startVC
    
        // (v, lab_vc) > (old_v, old_lab_vc)
    
        old_v = v;
        old_lab_vc = lab_vc;
    
        // send <startVC v, i> to all
    
        // Empty mbox
        memset(mbox_startVC,0,sizeof(mbox_startVC));
        num_mbox_startVC = 0;   

        // receive Transaction
        retry = rand() % 2;
        while(retry){
            m_startVC = init_startVC(num);
            if(filter_startVC(m_startVC, v)) {
                mbox_startVC[num_mbox_startVC] = m_startVC;
                num_mbox_startVC++;
            } 

            if(num_mbox_startVC >= num/2) 
                break; 

            retry = rand() % 2;
        }

        if (num_mbox_startVC >= num/2) {             
            lab_vc = 2; // doVC
    
            // (v, lab_vc) > (old_v, old_lab_vc)
    
            old_v = v;
            old_lab_vc = lab_vc;
    
            if (pid == leader) {
                // empty mbox
                memset(mbox_doVC,0,sizeof(mbox_doVC));
                num_mbox_doVC = 0;

                // receive doVC messages
                retry = rand() % 2;
                while(retry){
                    m_doVC = init_doVC(num);
                    if(filter_doVC(m_doVC, v)) {
                        mbox_doVC[num_mbox_doVC] = m_doVC;
                        num_mbox_doVC++;
                    } 
                    if(num_mbox_doVC >= num/2) 
                        break; 
                    retry = rand() % 2;
                }

                if (num_mbox_doVC >= num/2) { // check m.msg = doVC, m.v = v and cardinality > num/2
                    //log = largest_log(mbox);
                    n = mbox_doVC[0].log_top;
                    k = mbox_doVC[0].log_k;

                    lab_vc++;  // startView

                    // (v, lab_vc) > (old_v, old_lab_vc)

                    old_v = v;
                    old_lab_vc = lab_vc;

                    // send <startView v, l, n, k> to all
                    NormalOp(pid,leader,v,n,k,num,log);
                    v++;                    
                }
                else { // if leader does not receive enough doVC msgs
                    v++;
                }
            }
            else {
                //send <doVC v,l,v',n,k,i> to leader

                lab_vc = 3;  // startView
                
                // (v, lab_vc) > (old_v, old_lab_vc)
                
                old_v = v;
                old_lab_vc = lab_vc;

                // Empty mbox
                memset(mbox_startView,0,sizeof(mbox_startView));
                num_mbox_startView = 0;   

                // receive Transaction
                retry = rand() % 2;
                while(retry) {
                    m_startView = init_startView(num);
                    if(filter_startView(m_startView, v)) {
                        mbox_startView[num_mbox_startView] = m_startView;
                        num_mbox_startView++;
                    } 

                    if(num_mbox_startView >= 1) 
                        break; 

                    retry = rand() % 2;
                }

                if (num_mbox_startView >= 1) { // m.msg = startView, m.v > v
                    // log = m.log;
                    n = mbox_startView[0].log_top;
                    k = mbox_startView[0].log_k;
                    v = mbox_startView[0].v;

                    NormalOp(pid,leader,v,n,k,num,log);
                }
                else {
                    v++
                }    
            }
        }
        else { // if not enough startVC msgs
            v++;
        }
    }
}

int main(){
    VC(0,5,0);
}
