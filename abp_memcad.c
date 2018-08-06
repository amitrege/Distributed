// Assumption - network delay is bounded

typedef struct _msg {
    int lab;     // Label of the round
    int count;   // Round Number
    int req;     // Takes values 0 or 1 (alternating bits) 
} msg;

// Message 0 sent by Process A to B
int filter_msg_0(msg* m, int count) {
    if (m->req == 0 && m->count == count) {
        return 1;
    }
    return 0;
}

// Message 1 sent by Process A to B
int filter_msg_1(msg* m, int count) {
    if (m->count == count && m->req == 1) {
        return 1;
    }
    return 0;
}

// Ack for 0 sent by Process B to A
int filter_ack_0 (msg* m, int count) {
    if (m->count == count && m->req == 0) {
        return 1;
    }
    return 0;
}

// Ack for 0 sent by Process B to A
int filter_ack_1 (msg* m, int count) {
    if (m->count == count && m->req == 1) {
        return 1;
    }
    return 0;
}

// Initialize a message
void init_msg(msg* m, int lab, int count, int req) {
    m->lab = lab;
    m->count = count;
    m->req = req;
}

int AlternatingBit(int id) {
    // Tags
    int lab = 0;
    int count = 0;

    // Old Tags
    int old_lab = 0;
    int old_count = count - 1;  // needed for first assertion in the first iteration
    
    // msg* mbox_msg[2];
    msg m;
    int num_mbox_msg;

    // Acts as random number generators
    volatile int random;
    volatile int random1;

    int retry;

    while (1) {
        lab = 1;

        assert(count == (old_count + 1));
        assert((count > old_count) || ((count == old_count) && (lab > old_lab)));
	    old_count = count;
        old_lab = lab;

        if (id == 1) { // Branch for Process A

            retry = random;
            while (retry) {
                msg m_0;
                m_0.lab = lab;
                m_0.count = count;
                m_0.req = 0;
                // send m_0 to B
                assert((m_0.lab == lab) && (m_0.count == count));

                retry = random;
            }

            lab = lab + 1; // lab = 2

            assert((count == old_count) && (lab == (old_lab + 1)));
            assert((count > old_count) || ((count == old_count) && (lab > old_lab)));
	        old_count = count;
            old_lab = lab;

            // Recieve ack for 0
            // Empty mbox
            num_mbox_msg = 0;

            while (1) {
                // m = receive()
                if (m.lab == 2) {  // Ack for 0
                    if (filter_ack_0(&m, count)) {
                        // mbox_ack[num_mbox_ack] = &m_ack;
                        num_mbox_msg = num_mbox_msg + 1;
                    }
    
                    if (num_mbox_msg >= 1) {
                        break;
                    }
                }
            }

            lab = lab + 1; // lab = 3
            
            assert((count == old_count) && (lab == (old_lab + 1)));           
            assert((count > old_count) || ((count == old_count) && (lab > old_lab)));
            old_count = count;
            old_lab = lab;

            retry = random;
            while (retry) {
                msg m_1;
                m_1.lab = lab;
                m_1.count = count;
                m_1.req = 1;
                // send m_1 to B
                assert((m_1.lab == lab) && (m_1.count == count));

                retry = random;
            }

            lab = lab + 1; // lab = 4
            
            assert((count == old_count) && (lab == (old_lab + 1)));            
            assert((count > old_count) || ((count == old_count) && (lab > old_lab)));
            old_count = count;
            old_lab = lab;

            // Recieve ack for 0
            // Empty mbox
            num_mbox_msg = 0;

            while (1) {
                if(m.lab == 4) {
                    if (filter_ack_1(&m, count)) {
                        // mbox_ack[num_mbox_ack] = &m_ack;
                        num_mbox_msg = num_mbox_msg + 1;
                    }
    
                    if (num_mbox_msg >= 1) {
                        break;
                    }
                }
            }

            count = count + 1;
        }
        else {  // Process is B
            // Recieve ack for 0
            // Empty mbox
            num_mbox_msg = 0;

            while (1) {
                if(m.lab == 1) {
                    if (filter_msg_0(&m, count)) {
                        // mbox_msg[num_mbox_msg] = &m1;
                        num_mbox_msg = num_mbox_msg + 1;
                    }
    
                    if (num_mbox_msg >= 1) {
                        break;
                    }
                }
            }

            lab = lab + 1; // lab = 2
            
            assert((count == old_count) && (lab == (old_lab + 1)));            
            assert((count > old_count) || ((count == old_count) && (lab > old_lab)));
            old_count = count;
            old_lab = lab;

            retry = random;
            while (retry) {
                msg a_0;
                a_0.lab = lab;
                a_0.count = count;
                a_0.req = 0;
                // send a_0 to B
                assert((a_0.lab == lab) && (a_0.count == count));
                
                retry = random;
            }

            lab = lab + 1; // lab = 3
            
            assert((count == old_count) && (lab == (old_lab + 1)));            
            assert((count > old_count) || ((count == old_count) && (lab > old_lab)));
            old_count = count;
            old_lab = lab;

            // Recieve ack for 0
            // Empty mbox
            num_mbox_msg = 0;
            
            while (1) {
                if(m.lab == 3) {  // Message 1
                    if (filter_msg_1(&m, count)) {
                        //  mbox_msg[num_mbox_msg] = &m2;
                         num_mbox_msg = num_mbox_msg + 1;
                     }
     
                     if (num_mbox_msg >= 1) {
                         break;
                     }
                }
            }
            lab = lab + 1; // lab = 4
            
            assert((count == old_count) && (lab == (old_lab + 1)));            
            assert((count > old_count) || ((count == old_count) && (lab > old_lab)));
            old_count = count;
            old_lab = lab;

            retry = random;
            while (retry) {
                msg a_1;
                a_1.lab = lab;
                a_1.count = count;
                a_1.req = 1;
                // send a_1 to B
                assert((a_1.lab == lab) && (a_1.count == count));
                
                retry = random;
            }

            count = count + 1;
        }
    }
}

int main() {
    AlternatingBit(1);
    return 0;
} 