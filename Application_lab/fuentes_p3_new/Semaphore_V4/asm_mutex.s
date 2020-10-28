// Ruben Gran
.arch armv8-a
.text
.global armv8_fetch_and_add
.type armv8_fetch_and_add, %function

armv8_fetch_and_add:
    ldaxr w2, [x0]
    add w4, w1, w2
    stlxr w3, w4, [x0]
    cbnz w3, armv8_fetch_and_add
    mov w0, w2
    ret


.global armv8_mutex_lock
.align 2
.type armv8_mutex_unlock,%function

armv8_mutex_lock:
    .cfi_startproc
    mov w3, 1
armv8_mutex_lock_loop:
    ldaxr w2, [x0]
    cbnz w2, armv8_mutex_lock_loop
    mov w4, #1
    stlxr w3, w4, [x0]
    cbnz w3, armv8_mutex_lock_loop
    ;dmb sy
    dsb sy
    ret 
    .cfi_endproc

.global armv8_mutex_unlock
.align 2
.type armv8_mutex_unlock,%function
armv8_mutex_unlock:
    .cfi_startproc
    ;dmb sy
    mov w1, #0
    armv8_mutex_unlock_loop:
    str w1, [x0]
    dsb sy
    ret
    .cfi_endproc


    .global armv8_mutex_sleep_lock
    .align 2
    .type armv8_mutex_sleep_lock,%function

armv8_mutex_sleep_lock:
    .cfi_startproc
    sevl
armv8_mutex_sleep_lock_loop:
    wfe
    ldaxr w1, [x0]
    cbnz w1, armv8_mutex_sleep_lock_loop
    mov w1, #1
    stlxr w2, w1, [x0]
    cbnz w2, armv8_mutex_sleep_lock_loop

    dsb sy
    ret 
    .cfi_endproc

    .global armv8_mutex_sleep_unlock
    .align 2
    .type armv8_mutex_sleep_unlock,%function
armv8_mutex_sleep_unlock:
    .cfi_startproc
    mov w1, #0
    str w1, [x0]
    dsb sy
    sevl
    ret
    .cfi_endproc
