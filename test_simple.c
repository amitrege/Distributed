#include "assert.h"

typedef struct _msg {
    int count;
    int req;
} msg;

int filter_msg_0(msg* m, int count) {
    if (m->req == 0 && m->count == count) {
        return 1;
    }
    return 0;
}

int filter_msg_1(msg* m, int count) {
    if (m->count == count && m->req == 1) {
        return 1;
    }
    return 0;
}

typedef struct _ack {
    int count;
    int req;
} ack;

int filter_ack_0 (ack* m, int count) {
    if (m->count == count && m->req == 0) {
        return 1;
    }
    return 0;
}

int filter_ack_1 (ack* m, int count) {
    if (m->count == count && m->req == 1) {
        return 1;
    }
    return 0;
}

int rand() {
    int r;
    if(r) {
        return 0;
    }
    else {
        return 1;
    }
} 

int main() {
    int lab = 0;
    int count = 0;

    int old_lab = 0;
    int old_count = count - 1;  // needed for first assertion
    
    msg* mbox_msg[2];
    msg m1;
    msg m2;
    int num_mbox_msg;

    ack* mbox_ack[2];
    ack m_ack;
    int num_mbox_ack;

    int retry;

    while (count < 10) {
        lab = 1;

        assert(count > old_count || ((count == old_count) && (lab > old_lab)));
        old_count = count;
        old_lab = lab;

        retry = rand();
        while(retry) {
            assert(0 == 0);
            retry = rand();
        }

        count = count + 1;
    }
    return 0;
}