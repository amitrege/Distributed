typedef struct _msg {
    int i;
} msg;

int filter (msg m) {
    if (m.i == 0)
    	return 1;
    return 0;
}

int f (int i) {
    if (i == 0)
	return 1;
    return 0;
}

int main() {
    msg m1;
    m1.i = 0;

    assert(0 == 0);

    if(f(m1.i)) {
    	assert (m1.i == 0);
    }

/*
    if (filter(m1)) {
    	//assert(m.i == 0);
	assert (0 == 0);
    }
*/

    int j;
    _memcad("assume(j>0)");
    if (j>1){
    	assert(j > 1);
    }

    int retry;
    int a[100];
    int num = 0;

    while(retry && num < 10) {
    	if (f(m1.i)) {
	    a[num] = m1.i;
	    num++;
	}
    }

    assert(num >= 0);
    
    int i = 0;
    int old_i = 0;
    m1.i = 0;
    while(1) {
    	if (m1.i == 1)
    	    i++;
	assert(i > old_i);
	old_i = i;
    }
    return 0;
}
