// Assumption - network delay is bounded

typedef struct _msg {
    int lab;
    int count;
    int req;
} msg;

int filter_msg_0(msg* m, int count) {
    if (m->req == 0 && m->count >= count) {
        return 1;
    }
    return 0;
}

int filter_msg_1(msg* m, int count) {
    if (m->count >= count && m->req == 1) {
        return 1;
    }
    return 0;
}

int filter_ack_0 (msg* m, int count) {
    if (m->count >= count && m->req == 0) {
        return 1;
    }
    return 0;
}

int filter_ack_1 (msg* m, int count) {
    if (m->count >= count && m->req == 1) {
        return 1;
    }
    return 0;
}

int AlternatingBit(int id) {
    int lab = 0;
    int count = 0;

    int old_lab = 0;
    int old_count = count - 1;  // needed for first assertion
    
    msg* mbox_msg[2];
    msg m;
    int num_mbox_msg;
    int num_mbox_ack;

    volatile int random;

    int retry;

    while (count < 10000) {
        lab = 1;

        assert((count > old_count) || ((count == old_count) && (lab > old_lab)));
	    old_count = count;
        old_lab = lab;

        if (id == 1) { // Process is A

            retry = random;
            while (retry) {
                // send 0 to B

                retry = random;
            }

            lab = 2;

            assert((count > old_count) || ((count == old_count) && (lab > old_lab)));
	        old_count = count;
            old_lab = lab;

            // Recieve ack for 0
            // Empty mbox
            num_mbox_ack = 0;

            retry = random;

            while (1) {
                // m = receive()
                if (m.lab == 2) {  // Ack for 0
                    if (filter_ack_0(&m, count)) {
                        // mbox_ack[num_mbox_ack] = &m_ack;
                        num_mbox_ack = num_mbox_ack + 1;
                    }
    
                    if (num_mbox_ack >= 1) {
                        break;
                    }
                }

                retry = random;
            }

            lab = 3;
            
            assert((count > old_count) || ((count == old_count) && (lab > old_lab)));
            old_count = count;
            old_lab = lab;

            retry = random;
            while (retry) {
                // send 1 to B

                retry = random;
            }

            lab = 4;

            assert((count > old_count) || ((count == old_count) && (lab > old_lab)));
            old_count = count;
            old_lab = lab;

            // Recieve ack for 0
            // Empty mbox
            num_mbox_ack = 0;

            retry = random;

            while (1) {
                if(m.lab == 4) {
                    if (filter_ack_1(&m, count)) {
                        // mbox_ack[num_mbox_ack] = &m_ack;
                        num_mbox_ack = num_mbox_ack + 1;
                    }
    
                    if (num_mbox_ack >= 1) {
                        break;
                    }
                }

                retry = random;
            }

            count = count + 1;
        }
        else {  // Process is B
            // Recieve ack for 0
            // Empty mbox
            num_mbox_msg = 0;

            retry = random;

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

                retry = random;
            }

            lab = 2;
            
            assert((count > old_count) || ((count == old_count) && (lab > old_lab)));
            old_count = count;
            old_lab = lab;

            retry = random;
            
            while (retry) {
                // send ack to A
                
                retry = random;
            }

            lab = 3;

            assert((count > old_count) || ((count == old_count) && (lab > old_lab)));
            old_count = count;
            old_lab = lab;

            // Recieve ack for 0
            // Empty mbox
            num_mbox_msg = 0;

            retry = random;
            
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

                retry = random;
            }
            lab = 4;
            
            assert((count > old_count) || ((count == old_count) && (lab > old_lab)));
            old_count = count;
            old_lab = lab;

            retry = random;
            while (retry) {
                // send ack to A

                retry = random;
            }
            
            count = count + 1;
        }
    }
}

int main() {
    AlternatingBit(0);
    return 0;
} 

/*
Code for Interproc

proc AlternatingBit(id:int) returns (count : int)
var lab:int, retry:int, m:int; 
begin
    lab = 0;
    retry = 1;
    count = 0;

    while true and count < 1000 
    do
        lab = 1;
        if id == 1 then
            while retry == 1
            do
                retry = random;
                assume retry>=0 and retry <= 1;
            done;
            
            lab = 2;
            m = random;

            if m == 2 then 
                lab = 3;
                
                while retry == 1
                do
                    retry = random;
                    assume retry>=0 and retry <= 1;
                done;

                lab = 4;

                m = random;
                if m == 4 then
                    lab = 1;
                endif;
            endif;
        else
            m = random;

            if m == 1 then
                lab = 2; 
                while retry == 1
                do
                    retry = random;
                    assume retry>=0 and retry <= 1;
                done;
                lab = 3;

                m = random;
                if m==3 then
                    lab = 4;
                    while retry == 1
                    do
                        retry = random;
                        assume retry>=0 and retry <= 1;
                    done;
                endif;
            endif;
        endif;

        count = count + 1;
    done;
end

var pid:int;
begin
  pid = 0;
  pid = AlternatingBit(pid);
end
*/
