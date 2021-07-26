        .MODEL SMALL
        .STACK
        .DATA  
        
        
matrix   DW 'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P'
matrix1  DW 16 DUP (?)
N        EQU 3 ;number of rot
M        EQU 4 ;MATRIX SIZE   


        .CODE          
        
rotate_1 PROC ;procedure which rotates fro n=1 (for n >1 iit is taken care by loop in the main)


        PUSH CX ; push register also used in startup
        MOV SI, M*M*2    ;MOD
        MOV BX, M*(M-1)*2 ; MOD store in BX indx of element of last row of matrix that has to be decreased by M*(M-1) for their rotate matrix1 pos


iterate:DEC SI 
        DEC SI;;; 
        CMP SI, BX
        JAE last_row ;if SI is higher or equal than BX I jump to last_row
        MOV DI, SI
        ADD DI, M*2  ;MOD
        MOV AX, matrix[SI] ; I have a probelm with reposndus in closing parenthesis sinc ax is 16 bits it is ok to copy from matrix to ax
        MOV matrix1[DI], AX ;moving element in old offset (SI) to new offset (DI) in matrix1
        CMP SI, 0
        JNE iterate ; when SI will be equal to 0 we will have updated all offsets in matrix1
        JMP copyback ;if SI =0 I have finished I just need to copy back from matrix1 to matrix

last_row:
        MOV DI,SI
        SUB DI, M*(M-1)*2 ; in this case it is the last row so we have to sub 12  MOD
        MOV AX, matrix[SI] ;same as prviously done updating oset
        MOV matrix1[DI], AX
        JMP iterate ; no need to do check on SI here since the first jump condition will be true only for first 4 elements

copyback:
        MOV SI, M*M*2   ;MOD

iter:   DEC SI ; just copying from matrix1 to matrix
        DEC SI ; 
        MOV AX, matrix1[SI] ;
        MOV matrix[SI], AX;
        cmp SI, 0
        JNE iter
        POP CX
        
        ret
        ENDP

        .STARTUP    
        
        MOV CX, N

ciclo:
        dec CX
        CALL rotate_1
        cmp CX, 0
        JNE ciclo    
        
        
        .EXIT
        END