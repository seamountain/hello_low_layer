#include "./hello_malloc.h"
#include <assert.h>

void test1() {
    assert(); // 事前条件
    hello_malloc_init();
    assert(); // 事後条件
}

void test2() {
    assert(); // 事前条件
    void* adrs = hello_malloc(100);
    assert(adrs); // 事後条件
    hello_free(adrs);
    void* adrs2 = hello_malloc(100);
    assert(adrs == adrs2); // 事後条件
    hello_free(adrs2);
}

int main() {
    test1();
    test2();
}
