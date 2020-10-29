    .global mi_mutex_lock

mi_mutex_lock:
    .cfi_startproc
    sevl
mi_mutex_lock_loop:
    wfe
    ldaxr w1, [x0]
    cbnz w1, mi_mutex_lock_loop
    mov w1, #1
    stlxr w2, w1, [x0]
    cbnz w2, mi_mutex_lock_loop

    dsb sy
    ret 
    .cfi_endproc

    .global mi_mutex_unlock
    .align 2
    .type mi_mutex_unlock,%function
mi_mutex_unlock:
    .cfi_startproc
    mov w1, #0
    str w1, [x0]
    dsb sy
    sevl
    ret
    .cfi_endproc
