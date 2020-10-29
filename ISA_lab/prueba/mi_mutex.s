//Ruben Gran

    .global mi_mutex_lock

mi_mutex_lock:
    .cfi_startproc
    mov w3, 1
mi_mutex_lock_loop:
    ldaxr w2, [x0]
    cbnz w2, mi_mutex_lock_loop
    mov w4, #1
    stlxr w3, w4, [x0]
    cbnz w3, mi_mutex_lock_loop
    ;dmb sy
    dsb sy
    ret 
    .cfi_endproc

    .global mi_mutex_unlock
    .align 2
    .type mi_mutex_unlock,%function
mi_mutex_unlock:
    .cfi_startproc
    ;dmb sy
    mov w1, #0
mi_mutex_unlock_loop:
    str w1, [x0]
    dsb sy
    ret
    .cfi_endproc
