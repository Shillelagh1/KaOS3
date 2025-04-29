#include "int_errh.h"

// Contains the general error handlers for now.
ISR_STUB_NO_ERR(0); // #DE
ISR_STUB_NO_ERR(1); // #DB
ISR_STUB_NO_ERR(2); // NMI
ISR_STUB_NO_ERR(3); // #BP
ISR_STUB_NO_ERR(4); // #OF
ISR_STUB_NO_ERR(5); // #BR
ISR_STUB_NO_ERR(6); // #UD
ISR_STUB_NO_ERR(7); // #NM
ISR_STUB_ERR(8);    // #DF
ISR_STUB_NO_ERR(9); 
ISR_STUB_ERR(10);   // #TS
ISR_STUB_ERR(11);   // #NP
ISR_STUB_ERR(12);   // #SS
ISR_STUB_ERR(13);   // #GP
ISR_STUB_ERR(14);   // #PF
ISR_STUB_NO_ERR(15);
ISR_STUB_NO_ERR(16);// #MF
ISR_STUB_ERR(17);   // #AC
ISR_STUB_NO_ERR(18);// #MC
ISR_STUB_NO_ERR(19);// #XM
ISR_STUB_NO_ERR(20);// #VE
ISR_STUB_ERR(21);   // #CP