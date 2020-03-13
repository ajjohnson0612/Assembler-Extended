// make sure your assembler skips comments
// also make sure it skips  over blank lines


// leading and trailing white space
A=D-M
    A=D-M
    A=D-M			//space


(label_test)
// make sure you can run any possible instruction










// dest assembler directives
//.EQU cool 12345
//@cool
//.EQU oh_cool 7645
//@oh_cool
//.EQU bin_num 0b101
//@bin_num


// properly use hex and binary
@1234
@0x646A
@0X10
@0b1
@0B101000000000000

x0x1b
x0b0000000

// creates variables (because there are no corresponding labels)
@test
@thing

// LEGAL labels
($MONEY$)
(___)
(_.$:)
(Xx_$6969$_xX)

//**********************************
// ERROR CHECKING
//**********************************

// can't redefine directives/labels
.EQU cool 11111
(label_test)

// not a redefinition
@okay
@okay

//Destination
B=D-M
B=D-M;JMP

// jump
D-M;JPM
D-M;POOP
A=D-M;JPM

// comp
D-D
D-D;JMP
A=D-D
A=M-A;JEQ

// all together
B=Y&X;JPM
B=2;JPM
B=2
3;PPD

// A-type illegal value integer is to large
@0b1000000000000000
@32768 // max is 32767
@0x8000

// illegal labels
(%okay%)
())
()
(okay
(123dk)
(YEET*&)
(//okay)
(
(//test
)

@
@123obs

// illegal EQU
.EQ yes 1
.EQU 12yes 408
.EQU yes90
.EQU 409
.EQU
.
.EQUsdfhjk
.EQU18

