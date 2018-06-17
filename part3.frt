: IMMEDIATE push_lw @ cfa 1 - dup c@ 1 or swap c! ;

: if ' branch0 , here 0 , ; IMMEDIATE

: else ' branch , here 0 , swap here swap ! ; IMMEDIATE

: then here swap ! ; IMMEDIATE

: repeat here ; IMMEDIATE

: until ' branch0 , , ; IMMEDIATE

: over >r dup r> swap ;

: 2dup over over ;

: for
    ' swap ,
    ' >r ,
    ' >r ,
    here
    ' r> ,
    ' r> ,
    ' 2dup ,
    ' >r ,
    ' >r ,
    ' < ,
    ' branch0 ,
    here 0 ,
    swap
; IMMEDIATE

: endfor
       ' r> ,
       ' lit , 1 ,
       ' + ,
       ' >r ,
       ' branch , ,
       here swap !
       ' r> ,
       ' drop ,
       ' r> ,
       ' drop ,
;  IMMEDIATE



