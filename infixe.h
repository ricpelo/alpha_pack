! ==============================================================================
!   INFIX:  Support for the optional library debugger extension "Infix".
!
!   Supplied for use with Inform 6 -- Release 6/11 -- Serial number 040227
!
!   Copyright Graham Nelson 1993-2004 but freely usable (see manuals)
!
!   Cambios y mejoras de Saimazoom (Alien soft) y Sothoth (Alpha Aventuras)
!
!   This file is automatically Included in your game file by "Grammar" if you
!   supply the -X compiler switch.
! ==============================================================================

System_file;

! ------------------------------------------------------------------------------

#Ifdef DEBUG;

Constant INFIXTT_NUMBER          0;
Constant INFIXTT_ARRAY           1;
Constant INFIXTT_ROUTINE         2;
Constant INFIXTT_CONSTANT        3;
Constant INFIXTT_DWORD           4;
Constant INFIXTT_ACTION          5;
Constant INFIXTT_ATTRIBUTE       6;
Constant INFIXTT_PROPERTY        7;
Constant INFIXTT_GLOBAL          8;
Constant INFIXTT_NAMEDOBJECT     9;
Constant INFIXTT_SYSFUN         10;
Constant INFIXTT_STATICSTRING   11;
Constant INFIXTT_LOGICAL        12;

Global infix_term_type;
Global infix_data1;
Global infix_data2;
Global infix_lvalue;
Global infix_parsed_lvalue;
Array  infix_tolowercase -> 256;
#Ifdef VN_1630;
Array  infix_text buffer 126;
#Ifnot;
Array  infix_text -> 128;
#Endif; ! VN_

#ifndef PrintToBuffer;
[ PrintToBuffer buf len a b;
  if (b) {
    if (metaclass(a) == Object && a.#b == WORDSIZE
        && metaclass(a.b) == String)
      buf-->0 = PrintAnyToArray(buf+WORDSIZE, len, a.b);
    else
      buf-->0 = PrintAnyToArray(buf+WORDSIZE, len, a, b);
  } else
    buf-->0 = PrintAnyToArray(buf+WORDSIZE, len, a);
  if (buf-->0 > len) buf-->0 = len;
  return buf-->0;
];
#endif; ! PrintToBuffer

[ InfixPrintAttribute x; print (string) #attribute_names_array-->x; ];

[ InfixPrintProperty x;  print (property) x; ];

[ InfixPrintGlobal x;    print (string) #global_names_array-->x; ];

[ InfixPrintAction x;    print (string) #action_names_array-->(x-#lowest_action_number); ];

[ InfixPrintRoutine x;   print (string) #routine_names_array-->(x-#lowest_routine_number); ];

[ InfixPrintConstant x;  print (string) #constant_names_array-->(x-#lowest_constant_number); ];

[ InfixPrintArray x;     print (string) #array_names_array-->(x-#lowest_array_number); ];

[ InfixPrintFakeAction x;
                        print (string) #fake_action_names_array-->(x-#lowest_fake_action_number); ];

[ InfixPrintPA x n;
    for (n=#lowest_routine_number : n<=#highest_routine_number : n++) {
        if (x == Symb__Tab(INFIXTT_ROUTINE, n)) {
            print (InfixPrintRoutine) n; return;
        }
    }
    print "Routine(", x, ")";
];

[ InfixMatchPrule PrintingRule range1 range2 wa wl t i i2 it2 itlc j k plus;
    itlc = infix_tolowercase;
    if (itlc->255 == 0) {
        for (j=0 : j<256 : j++) itlc->j = j;
        itlc->'A' = 'a';    itlc->'B' = 'b';
        itlc->'C' = 'c';    itlc->'D' = 'd';
        itlc->'E' = 'e';    itlc->'F' = 'f';
        itlc->'G' = 'g';    itlc->'H' = 'h';
        itlc->'I' = 'i';    itlc->'J' = 'j';
        itlc->'K' = 'k';    itlc->'L' = 'l';
        itlc->'M' = 'm';    itlc->'N' = 'n';
        itlc->'O' = 'o';    itlc->'P' = 'p';
        itlc->'Q' = 'q';    itlc->'R' = 'r';
        itlc->'S' = 's';    itlc->'T' = 't';
        itlc->'U' = 'u';    itlc->'V' = 'v';
        itlc->'W' = 'w';    itlc->'X' = 'x';
        itlc->'Y' = 'y';    itlc->'Z' = 'z';
    }
    switch(PrintingRule) {
      InfixPrintAttribute:
        if (wa->0 == '~') { wl--; wa++; plus = 100; } ! A tilde
        t = #attribute_names_array;
      InfixPrintProperty:   t = #property_names_array;
      InfixPrintAction:     t = #action_names_array;
      InfixPrintFakeAction: t = #fake_action_names_array;
      InfixPrintGlobal:     t = #global_names_array;
      InfixPrintRoutine:    t = #routine_names_array;
      InfixPrintAction:     t = #constant_names_array;
      InfixPrintArray:      t = #array_names_array;
    }
    
    !print "Looking at table ",t," from ",range2, " to ", range1," ";    
    i2 = range2-range1; it2 = infix_text+WORDSIZE;
    for (i=0 : i<=i2 : i++) {
        !print i," ";
        #ifdef TARGET_ZCODE;
        infix_text-->0 = 62; @output_stream 3 infix_text;
        if (t) print (string) t-->i; else PrintingRule(i+range1);
        @output_stream -3;
        #ifnot;
        
        if (t) PrintToBuffer(infix_text, 62, t-->i);
            else PrintToBuffer(infix_text, 62, PrintingRule, i+range1);

        !     
        #endif;

        k = infix_text-->0;
      
        if (k ~= wl) jump XL;
        if (itlc->(it2->0) ~= wa->0) jump XL;
        
        for (j=1 : j<k : j++) ! Compare the strings...
            if (itlc->(it2->j) ~= wa->j) jump XL;       
        numero_interpretado = i + range1 + plus;
        rtrue;
      .XL;
    }
    rfalse;
];

[ InfixActionToken;
    if (InfixMatchPrule(InfixPrintAction, #lowest_action_number,
        #highest_action_number, DireccionDePalabra(np), LongitudDePalabra(np))) {
        np++; infix_lvalue = numero_interpretado; return 0;
    }
    if (InfixMatchPrule(InfixPrintFakeAction, #lowest_fake_action_number,
        #highest_fake_action_number, DireccionDePalabra(np), LongitudDePalabra(np))) {
        np++; infix_lvalue = numero_interpretado; return 0;
    }
    return -1;
];

[ InfixRvalueTerm n w i initial_np wa wl sign base digit dcount;

  
    initial_np = np;

    infix_parsed_lvalue = -1;
    infix_term_type = INFIXTT_NUMBER;
    
    w = SiguientePalabraParar();
    !print "W:",w;
    
    if (w == -1) return -1;

    wa = DireccionDePalabra(np-1);
    wl = LongitudDePalabra(np-1);
    
    
    if (wa->0 == '-' or '$' or '0' or '1' or '2' or '3'
              or '4' or '5' or '6' or '7' or '8' or '9') {
        ! Parse decimal, hex or binary number

        sign = 1; base = 10; dcount = 0;
        if (wa->0 == '-') { sign = -1; wl--; wa++; }
        else {
            if (wa->0 == '$') { base = 16; wl--; wa++; }
            if (wa->0 == '$') { base = 2; wl--; wa++; }
        }
        if (wl == 0) return -1;
        n = 0;
        while (wl > 0) {
            if (wa->0 >= 'a') digit = wa->0 - 'a' + 10;
            else digit = wa->0 - '0';
            dcount++;
            switch (base) {
              2:
                if (dcount == 17) return -1;
              10:
                if (dcount == 6) return -1;
                if (dcount == 5) {
                    if (n > 3276) return -1;
                    if (n == 3276) {
                        if (sign == 1 && digit > 7) return -1;
                        if (sign == -1 && digit > 8) return -1;
                    }
                }
              16:
                if (dcount == 5) return -1;
            }
            if (digit >= 0 && digit < base) n = base*n + digit;
            else return -1;
            wl--; wa++;
        }
        numero_interpretado = n*sign; return 1;
    }

    ! Parse character constant 'a'

    if (wl == 3 && wa->0 == ''' && wa->2 == ''') {
        numero_interpretado = wa->1; return 1;
    }

    ! ##Action, 'dword'

    switch (w) {
      '##':
        infix_term_type = INFIXTT_ACTION;
        w = SiguientePalabraParar(); if (w == -1) return -1;
        np--;
        if (InfixActionToken() == 0) return 1;
        return -1;
      '^^':
        infix_term_type = INFIXTT_DWORD;
        w = SiguientePalabraParar(); if (w == -1) return -1;
        numero_interpretado = w; return 1;
    }

    ! Test for attribute, property, class name, variable name, array name, routine
    ! name, constant name
    ! print "Testing...";
    np--;
    if ((wa->0 >= 'a' && wa->0 <= 'z') ||
        (wa->0 >= 'A' && wa->0 <= 'Z') ||
         wa->0 == '_') {
        
       ! print wl,"chars^";
       ! print "Attribute?....";
        infix_term_type = INFIXTT_ATTRIBUTE;
        if (InfixMatchPrule(InfixPrintAttribute, #lowest_attribute_number,
            #highest_attribute_number, wa, wl)) {
            !print "yes^";
            np++; return 1; }
        
        infix_term_type = INFIXTT_PROPERTY;
      !  print "no ^Propertie? (from ",#lowest_property_number," to ",#highest_property_number,")";
!      [ InfixMatchPrule PrintingRule range1 range2 wa wl t i i2 it2 itlc j k plus;
        if (InfixMatchPrule(InfixPrintProperty, #lowest_property_number,
            #highest_property_number, wa, wl)) {
       !     print "yes^";
            #ifdef TARGET_GLULX;
                ! The numero_interpretado will be found in the name_table but in glulx the number will be different
                ! thant the real property number. We perform this operation to find its position
                ! in the properties table.
            if (numero_interpretado > (#identifiers_table-->1) - 1)
              numero_interpretado = (numero_interpretado -#identifiers_table-->1)+INDIV_PROP_START;
!              numero_interpretado = numero_interpretado - (INDIV_PROP_START - (#identifiers_table-->1));
            #endif;
            np++; return 1;
            }
        !print "no ^Global?...";
        infix_term_type = INFIXTT_GLOBAL;
        if (InfixMatchPrule(InfixPrintGlobal, #lowest_global_number,
            #highest_global_number, wa, wl)) {
            infix_parsed_lvalue = numero_interpretado-16;
            numero_interpretado = #globals_array-->infix_parsed_lvalue;
            !print "yes^";
            np++; return 1;
        }
        !print "no ^Array?...";
        infix_term_type = INFIXTT_ARRAY;
        if (InfixMatchPrule(InfixPrintArray, #lowest_array_number,
            #highest_array_number, wa, wl)) {
            infix_parsed_lvalue = numero_interpretado;
            numero_interpretado = Symb__Tab(INFIXTT_ARRAY,numero_interpretado);
            infix_data1 = temp__global3;
            infix_data2 = temp__global2;
            !print "yes^";
            np++; return 1;
        }
        !print "no ^ROUTINE?...";
        infix_term_type = INFIXTT_ROUTINE;
        !print "(from ",#lowest_routine_number," to ",#highest_routine_number,")";
        if (InfixMatchPrule(InfixPrintRoutine, #lowest_routine_number,
            #highest_routine_number, wa, wl)) {
            infix_parsed_lvalue = numero_interpretado;
            numero_interpretado = Symb__Tab(INFIXTT_ROUTINE,numero_interpretado);
            infix_data1 = temp__global3;
            infix_data2 = temp__global2;
            !print "yes^";
            np++; return 1;
        }
        !print "no ^CONSTANT?...";
        infix_term_type = INFIXTT_CONSTANT;
        if (InfixMatchPrule(InfixPrintConstant, #lowest_constant_number,
            #highest_constant_number, wa, wl)) {
            infix_parsed_lvalue = numero_interpretado;
            numero_interpretado = Symb__Tab(INFIXTT_CONSTANT,numero_interpretado);
            infix_data1 = temp__global3;
            infix_data2 = temp__global2;
            !print "yes^";
            np++; return 1;
        }
        
        !print "no^";
        
        switch (w) {
          'parent', 'child', 'children',
          'random', 'metaclass', 'sibling':
            numero_interpretado = w;
            infix_parsed_lvalue = INFIXTT_SYSFUN;
            np++; return 1;
        }
    }

    infix_term_type = INFIXTT_NAMEDOBJECT;

    np = initial_np; i = InterpretarToken(TT_ALCANCE, InfixBigScope);

    if (i == RPG_REPARSE) return i;
    if (i > RPG_MULTIPLE) {
        print "(", (name) i, " (", i, "))^";
        numero_interpretado = i; return 1;
    }
    return -1;
]; ! end of InfixRvalueTerm

[ InfixBigScope x;
    if (estadio_alcance == 1) return false;  ! No multiples here
    if (estadio_alcance == 2) {
        objectloop (x ofclass Object) PonerAlAlcance(x);
        return true; ! That's the whole scope
    }
    print "; I'm unable to make any sense of that term.^";
];

[ InfixCheckLineSpaced wa wl i force altered;
    for (i=1 : i<=parse->1 : i++) {
        wa = DireccionDePalabra(i);
        wl = LongitudDePalabra(i);
        if (wl > 3 && wa->0 == ''' && wa->(wl-1) == ''') {
            wa->(wl-1) = ' ';
            if (wa->(wl-2) == '/' && wa->(wl-3) == '/') {
                wa->(wl-2) = ' ';
                wa->(wl-3) = ' ';
            }
            LTI_Insert(wa-buffer, ''');
            LTI_Insert(wa-buffer + 2, ' ');
            altered = true; break;
        }
    }
    for (i=WORDSIZE : i<buffer->1 + WORDSIZE : i++) {
        force = false;
        if (buffer->i == '-' && buffer->(i+1) == '-' && buffer->(i+2) == '>')
            force = true;
        if (force) {
            if (i>WORDSIZE && buffer->(i-1) ~= ' ') {
                LTI_Insert(i++, ' '); altered = true;
            }
            if (buffer->(i+3) ~= ' ') {
                LTI_Insert(i+3, ' '); i++; altered = true;
            }
            i = i + 2; continue;
        }

        if (buffer->i == ':' && buffer->(i+1) == ':') force = true;
        if (buffer->i == '-' && buffer->(i+1) == '>') force = true;
        if (buffer->i == '.' && buffer->(i+1) == '&') {
            buffer->i = ']'; force = true;
        }
        if (buffer->i == '.' && buffer->(i+1) == '#') {
            buffer->i = ']'; force = true;
        }
        if (buffer->i == ']' && buffer->(i+1) == '&') force = true;
        if (buffer->i == ']' && buffer->(i+1) == '#') force = true;
        if (buffer->i == '+' && buffer->(i+1) == '+') force = true;
        if (buffer->i == '-' && buffer->(i+1) == '-') force = true;
        if (buffer->i == '&' && buffer->(i+1) == '&') force = true;
        if (buffer->i == '|' && buffer->(i+1) == '|') force = true;
        if (buffer->i == '~' && buffer->(i+1) == '~') force = true;

        if (buffer->i == '=' && buffer->(i+1) == '=') force = true;
        if (buffer->i == '~' && buffer->(i+1) == '=') force = true;
        if (buffer->i == '>' && buffer->(i+1) == '=') force = true;
        if (buffer->i == '<' && buffer->(i+1) == '=') force = true;
        if (buffer->i == '#' && buffer->(i+1) == '#') force = true;

        if (force) {
            if (i>WORDSIZE && buffer->(i-1) ~= ' ') {                    
                LTI_Insert(i++, ' '); altered = true;
            }
            if (buffer->(i+2) ~= ' ') {
                LTI_Insert(i+2, ' '); i++; altered = true;
            }
            i = i + 1; continue;
        }

        if (buffer->i == '+') force = true;
        if (buffer->i == '-') force = true;
        if (buffer->i == '*') force = true;
        if (buffer->i == '/') force = true;
        if (buffer->i == '%') force = true;
        if (buffer->i == '(') force = true;
        if (buffer->i == ')') force = true;
        if (buffer->i == '<' && buffer->(i-1) ~= ';') force = true;
        if (buffer->i == '>') force = true;
        if (buffer->i == ',') force = true;
        if (buffer->i == '.') force = true;
        if (buffer->i == '&') force = true;
        if (buffer->i == '|') force = true;
        if (buffer->i == '~') force = true;
        if (buffer->i == '=') force = true;
        if (force) {
            if (i > WORDSIZE && buffer->(i-1) ~= ' ') {
                LTI_Insert(i++, ' '); altered = true;
            }
            if (buffer->(i+1) ~= ' ') {
                LTI_Insert(i+1, ' '); i++; altered = true;
            }
        }
    }
    for (i=WORDSIZE : i<buffer->1 + WORDSIZE : i++)
        if (buffer->i == '~') { buffer->i = '['; altered = true; }
    return altered;
]; ! end of InfixCheckLineSpaced

Array InfixRV_rvals --> 32;
Array InfixRV_lvals --> 32;
Array InfixRV_op --> 32;
Array InfixRV_lop --> 32;
Array InfixRV_rop --> 32;
Array InfixRV_types --> 32;
Array InfixRV_commas --> 32;

[ InfixInBounds addr index n;
    if (addr < #array__start || addr > #array__end)
        rtrue;
    for (n=#lowest_array_number : n<=#highest_array_number : n++) {
        if (addr == Symb__Tab(INFIXTT_ARRAY, n)) {
            if (temp__global3 == 1 or 3)
                temp__global2=temp__global2*WORDSIZE+WORDSIZE-1;
            if (index > temp__global2) {
                print "Array index out of range";
                rfalse;
            }
        }
    }
    rtrue;
];

[ InfixRvalue acc w i n flag base expecting_term max maxi lop rop lvalside
              a b sysfun_f;

    if (InfixCheckLineSpaced()) return RPG_REPARSE;

   !w = np; for (i=0 : i<10 : i++) { np = w; InfixRvalueTerm(); print i, "^"; }
   !np = w;
   ! print "InfixRvalue...";
    expecting_term = true; base = 0;
    do {
        w = SiguientePalabraParar(); 
        if (expecting_term) {  
    
            switch (w) {
              '-//':
                InfixRV_rvals-->n = 'unary-'; InfixRV_types-->n = base + 8;
              '[//':
                InfixRV_rvals-->n = w; InfixRV_types-->n = base + 6;
              '[[':
                InfixRV_rvals-->n = w; InfixRV_types-->n = base + 2;
              '++':
              
                InfixRV_rvals-->n = 'pre++'; InfixRV_types-->n = base + 9;
              '--':
                InfixRV_rvals-->n = 'pre--'; InfixRV_types-->n = base + 9;
              '(//':

                InfixRV_rvals-->n = w; InfixRV_types-->n = -3; base=base+100;
              ')//':

                InfixRV_rvals-->n = w; InfixRV_types-->n = -3; base=base-100;
                if (base < 0) { np--; flag = true; }
              -1:
                flag = true;
              default:
                np--;
                if (InfixRValueTerm() == 1) {
                    InfixRV_rvals-->n = numero_interpretado; ! parsed number is a global variable
                    InfixRV_lvals-->n = infix_parsed_lvalue;
                    InfixRV_types-->n = -1;
                    expecting_term = false;
                 
                }
                else flag = true;
            }
        }
        else {
            expecting_term = true;
          
            switch (w) {
              palabra_coma:
                InfixRV_rvals-->n = w; InfixRV_types-->n = base;
              '=//':
              
                InfixRV_rvals-->n = w; InfixRV_types-->n = base + 1;
              '&&', '||':
                InfixRV_rvals-->n = w; InfixRV_types-->n = base + 2;
              '==', '[=', '>//', '>=', '<//', '<=', 'has', 'hasnt',
              'in', 'notin', 'ofclass', 'provides':
                InfixRV_rvals-->n = w; InfixRV_types-->n = base + 3;
              'or':
                InfixRV_rvals-->n = w; InfixRV_types-->n = base + 4;
              '+//', '-//':
                InfixRV_rvals-->n = w; InfixRV_types-->n = base + 5;
              '*//', '@{2f}//', '%//', '&//', '|//':
                InfixRV_rvals-->n = w; InfixRV_types-->n = base + 6;
              '->', '-->':
                InfixRV_rvals-->n = w; InfixRV_types-->n = base + 7;
              ']&', ']#':
                InfixRV_rvals-->n = w; InfixRV_types-->n = base + 10;
               DESPUES1__WD :              
                InfixRV_rvals-->n = w; InfixRV_types-->n = base + 12;
              '::':
                InfixRV_rvals-->n = w; InfixRV_types-->n = base + 13;
              '(//':
                !print "it will call a function...";
                InfixRV_rvals-->n = '(rcall';
                InfixRV_types-->n = base + 11; base = base + 100;
              ')//':
                InfixRV_rvals-->n = w; InfixRV_types-->n = -3;
                base = base - 100;
                if (base < 0) { np--; flag = true; }
                expecting_term = false;
              '++':
                InfixRV_rvals-->n = 'post++'; InfixRV_types-->n = base + 9;
                expecting_term = false;
              '--':
                InfixRV_rvals-->n = 'post--'; InfixRV_types-->n = base + 9;
                expecting_term = false;
              default:              
                flag = true;
            }
        }
        n++;
    } until (flag || n == 32);
    if (base > 0) return -1;
    n--; if (n == 0) return -1;
    np--;

    for (i=0 : i<n : i++) {
        acc = 0; if (InfixRV_types-->i ~= -3) acc = InfixRV_rvals-->i;
        InfixRV_op-->i = acc;
        
    }

    for (::) {
!
! for debugging...
!       for (i=0 : i<n : i++) {
!           if (InfixRV_types-->i == -1) print InfixRV_rvals-->i, " ";
!           else if (InfixRV_types-->i == -3) print " # ";
!           else if (InfixRV_types-->i == -2) print " ## ";
!           else print (address) InfixRV_rvals-->i, "_", InfixRV_types-->i, " ";
!       }
       new_line;
! End of debugging....

        max = -2;
        for (i=0 : i<n : i++) if (InfixRV_types-->i > max) {
                            max = InfixRV_types-->i; maxi = i; }
        if (max == -1) { numero_interpretado = InfixRV_rvals-->maxi; return 1; }

        lop = maxi-1; rop = maxi+1;
        while (lop >= 0 && InfixRV_types-->lop < -1) lop--;
        while (rop < n && InfixRV_types-->rop < -1) rop++;
        if (lop >= 0) InfixRV_lop-->maxi = InfixRV_rvals-->lop;
        if (rop < n) InfixRV_rop-->maxi = InfixRV_rvals-->rop;
        flag = false;
        infix_term_type = INFIXTT_NUMBER;
        
        
        
        switch (InfixRV_rvals-->maxi) 
        {
          palabra_coma:
            acc = (InfixRV_rvals-->rop);
          '=//', 'pre++', 'post++', 'pre--', 'post--':
            lvalside = lop;
            switch (InfixRV_rvals-->maxi) {
              '=//':        acc = (InfixRV_rvals-->rop);
              'pre++':      acc = (InfixRV_rvals-->rop) + 1; lvalside = rop;
              'pre--':      acc = (InfixRV_rvals-->rop) - 1; lvalside = rop;
              'post++':     acc = (InfixRV_rvals-->lop) + 1;
              'post--':     acc = (InfixRV_rvals-->lop) - 1;
            }
            switch (InfixRV_op-->lvalside) {
               DESPUES1__WD :
                (InfixRV_lop-->lvalside).(InfixRV_rop-->lvalside) = acc;
              '->':
                if (InfixInBounds(InfixRV_lop-->lvalside, InfixRV_rop-->lvalside))
                    (InfixRV_lop-->lvalside)->(InfixRV_rop-->lvalside) = acc;
              '-->':
                if (InfixInBounds(InfixRV_lop-->lvalside, WORDSIZE * InfixRV_rop-->lvalside))
                    (InfixRV_lop-->lvalside)-->(InfixRV_rop-->lvalside) = acc;
              default:
                w = InfixRV_lvals-->lvalside; if (w == -1) return -1;
                #ifdef TARGET_ZCODE;
                @storew #globals_array w acc;
                #ifnot;
                @astore #globals_array w acc;
                #endif;
            }
            switch(InfixRV_rvals-->maxi) {
              'post++': acc--;
              'post--': acc++;
            }
          '(rcall':
            
            switch (InfixRV_op-->lop) 
            {
               DESPUES1__WD :
                a = InfixRV_lop-->lop; b = InfixRV_rop-->lop;
              default:
                a = InfixRV_rvals-->lop; b = call;
                if (InfixRV_lvals-->lop == INFIXTT_SYSFUN)
                    sysfun_f = true;
            }
           
            w = 0;
            i = maxi + 1; base = 100;
            if (InfixRV_types-->i == -1 && InfixRV_rvals-->i == ')//') {

                if (sysfun_f) return -1;
                acc = a.b();
            }
            else {
                while (base > 0) {
                    if (InfixRV_types-->i == -3 && InfixRV_rvals-->i == ')//') base = base - 100;
                    if (InfixRV_types-->i == -3 && InfixRV_rvals-->i == '(//') base = base + 100;
                    if (InfixRV_op-->i == '(rcall') base = base + 100;
                    if (base == 100 && InfixRV_op-->i == palabra_coma) {
                        InfixRV_commas-->(w++) = i;
!                       print "Comma found at ", i, "^";
                    }
                    i++;
                }
!               print "Num args = ", w + 1, "^";
!               for (i=0 : i<w : i++)
!                   print "arg: ", InfixRV_lop-->(InfixRV_commas-->i), "^";
!               print "arg: ", InfixRV_rvals-->rop, "^";
                switch (w+1) {
                  1:
                    if (sysfun_f) {
                        b = InfixRV_rvals-->rop;
                        infix_term_type = INFIXTT_NAMEDOBJECT;
                        switch(a) {
                          'metaclass':
                            acc = metaclass(b);
                          'parent':
                            acc = parent(b);
                          'child':
                            acc = child(b);
                          'children':
                            acc = children(b);
                            infix_term_type = INFIXTT_NUMBER;
                          'random':
                            acc = random(b);
                            infix_term_type = INFIXTT_NUMBER;
                          'sibling':
                            acc = sibling(b);
                        }
                    }
                    else
                    {
                           
                        acc = a.b(InfixRV_rvals-->rop);                   
                    }
                     
                  2:
                          
                    if (sysfun_f) return -1;
                    acc = a.b(InfixRV_lop-->(InfixRV_commas-->0),
                              InfixRV_rvals-->rop);
                  3:
                    if (sysfun_f) return -1;
                    acc = a.b(InfixRV_lop-->(InfixRV_commas-->0),
                              InfixRV_lop-->(InfixRV_commas-->1),
                              InfixRV_rvals-->rop);
                  4:
                    if (sysfun_f) return -1;
                    acc = a.b(InfixRV_lop-->(InfixRV_commas-->0),
                              InfixRV_lop-->(InfixRV_commas-->1),
                              InfixRV_lop-->(InfixRV_commas-->2),
                              InfixRV_rvals-->rop);
                  5:
                    if (sysfun_f) return -1;
                    acc = a.b(InfixRV_lop-->(InfixRV_commas-->0),
                              InfixRV_lop-->(InfixRV_commas-->1),
                              InfixRV_lop-->(InfixRV_commas-->2),
                              InfixRV_lop-->(InfixRV_commas-->3),
                              InfixRV_rvals-->rop);
                  default:
                  
                    return -1;
                }
            }
          '+//':        acc = (InfixRV_rvals-->lop) + (InfixRV_rvals-->rop);
          '-//':        acc = (InfixRV_rvals-->lop) - (InfixRV_rvals-->rop);
          '*//':        acc = (InfixRV_rvals-->lop) * (InfixRV_rvals-->rop);
          '@{2f}//':    acc = (InfixRV_rvals-->lop) / (InfixRV_rvals-->rop);
          '%//':        acc = (InfixRV_rvals-->lop) % (InfixRV_rvals-->rop);
           DESPUES1__WD :    acc = (InfixRV_rvals-->lop) . (InfixRV_rvals-->rop);
          '->':         acc = (InfixRV_rvals-->lop) -> (InfixRV_rvals-->rop);
          '-->':        acc = (InfixRV_rvals-->lop) --> (InfixRV_rvals-->rop);
          ']&':         acc = (InfixRV_rvals-->lop) .& (InfixRV_rvals-->rop);
          ']#':         acc = (InfixRV_rvals-->lop) .# (InfixRV_rvals-->rop);
          '::':         acc = (InfixRV_rvals-->lop) :: (InfixRV_rvals-->rop);
          '&//':        acc = (InfixRV_rvals-->lop) & (InfixRV_rvals-->rop);
          '|//':        acc = (InfixRV_rvals-->lop) | (InfixRV_rvals-->rop);
          '&&':         acc = (InfixRV_rvals-->lop) && (InfixRV_rvals-->rop);
                        infix_term_type = INFIXTT_LOGICAL;
          '||':         acc = (InfixRV_rvals-->lop) || (InfixRV_rvals-->rop);
                        infix_term_type = INFIXTT_LOGICAL;
          '<//':        acc = (InfixRV_rvals-->lop) < (InfixRV_rvals-->rop);
                        infix_term_type = INFIXTT_LOGICAL;
          '<=':         acc = (InfixRV_rvals-->lop) <= (InfixRV_rvals-->rop);
                        infix_term_type = INFIXTT_LOGICAL;
          '>//':        acc = (InfixRV_rvals-->lop) > (InfixRV_rvals-->rop);
                        infix_term_type = INFIXTT_LOGICAL;
          '>=':         acc = (InfixRV_rvals-->lop) >= (InfixRV_rvals-->rop);
                        infix_term_type = INFIXTT_LOGICAL;
          '==':         acc = (InfixRV_rvals-->lop) == (InfixRV_rvals-->rop);
                        infix_term_type = INFIXTT_LOGICAL;
          '[=':         acc = (InfixRV_rvals-->lop) ~= (InfixRV_rvals-->rop);
                        infix_term_type = INFIXTT_LOGICAL;
          'has':        acc = (InfixRV_rvals-->lop) has (InfixRV_rvals-->rop);
                        infix_term_type = INFIXTT_LOGICAL;
          'hasnt':      acc = (InfixRV_rvals-->lop) hasnt (InfixRV_rvals-->rop);
                        infix_term_type = INFIXTT_LOGICAL;
          'in':         acc = (InfixRV_rvals-->lop) in (InfixRV_rvals-->rop);
                        infix_term_type = INFIXTT_LOGICAL;
          'notin':      acc = (InfixRV_rvals-->lop) notin (InfixRV_rvals-->rop);
                        infix_term_type = INFIXTT_LOGICAL;
          'provides':   acc = (InfixRV_rvals-->lop) provides (InfixRV_rvals-->rop);
                        infix_term_type = INFIXTT_LOGICAL;
          'ofclass':    acc = (InfixRV_rvals-->lop) ofclass (InfixRV_rvals-->rop);
                        infix_term_type = INFIXTT_LOGICAL;
          '[[':         acc = ~~ (InfixRV_rvals-->rop); flag = true;
          '[//':        acc = ~ (InfixRV_rvals-->rop); flag = true;
          'unary-':     acc = - (InfixRV_rvals-->rop); flag = true;
        } ! end of switch(InfixRV_rvals-->maxi)
        
                
        InfixRV_rvals-->maxi = acc;
        InfixRV_types-->maxi = -1;
        if (rop < n) InfixRV_types-->rop = -2;
        if (flag == false && lop >= 0) InfixRV_types-->lop = -2;
    } ! end of for (::)

]; ! end of InfixRvalue

! ------------------------------------------------------------------------

[ InfixWelcomeSub;
    print "; Welcome to the ~Infix~ debugger (1/990428), which makes the
        following verbs available:^^
        ~; <expression>~: evaluates this Inform expression: e.g.
        ~; location~ will print the value of the variable ~location~,
        ~; 3*5+1~ will print 16, ~; children(old cloth bag)~ will tell you
        how many items are in it. (You can name objects either by their
        names inside the source code, such as ~n_obj~, or by typing the
        names by which the game's parser would normally know them, such
        as ~north wall~: the effect is the same.)^
        Any expression is allowed except that you can't use double-quoted
        strings of text: but you can send messages, call routines or
        assign values to variables, properties and array entries.
        ^   ~; score++~ is one way to get on in the world.
        ^   ~; deadflag = true~ is one way to get out of it.
        ^   ~; StopDaemon(nasty little dwarf)~ will keep you from being pestered.^
        Conditions like ~; score>20~ are also allowed and print
        either 1 (if true) or 0 (if false).^^";
    print "~;examine <something>~ or ~;x <something>~ gives full details
        of whatever it is. For instance, ~;x ##Take~ describes the Take
        action; ~;x Class~ the class Class; ~;x 'drop'~ the dictionary
        word ~drop~ and so on for numbers, routines, arrays and of course
        objects.  ~;xo~ examines something as an object, so for instance
        ~;x location~ tells you about the variable ~location~, but ~;xo
        location~ tells you what object it refers to.^^";
    print "~;give~, ~;remove~ and ~;move~ work like the corresponding
        Inform statements.^^";
    print "~;<~ causes an action: for instance, ~;< Eat cheese~.^^";
    print "~;watch~ or ~;w~ can set a watch on various activities:
        type just ~;w~ for details.^^";
    print "~;inventory~ or ~;i~ describes the contents of this story file.^";
];

[ InfixEvalSub; InfixExamineP(true); ];

[ InfixActionSub;
    print "; <", (InfixPrintAction) infix_lvalue;
    if (uno) print " (", (the) uno, ")";
    if (otro) print " (", (the) otro, ")";
    print ">^";
    if (otro) <<(infix_lvalue) uno otro>>;
    if (uno) <<(infix_lvalue) uno>>;
    <<(infix_lvalue)>>;
];

[ InfixGiveSub f t;
    t=0; ! Just to skip the warning
    print "; give (", (the) uno, ") ";
    if (otro < 0) { otro = ~otro; f=true; }
    if (otro < 0 || otro >= 48) "<No such attribute>";
    if (f) print "@@126";
    print (DebugAttribute) otro;
    
#ifdef TARGET_ZCODE;
     if (f) @clear_attr uno otro;
     else @set_attr uno otro;
#ifnot;
    t = otro+8;
    if (f) @astorebit uno t 0; ! give uno ~otro; 
    else @astorebit uno t 1;   ! give uno otro;
#endif;
];

[ InfixGiveNotSub;
  otro = ~otro;
  InfixGiveSub();
];

[ InfixMoveSub;
    print "; move (", (the) uno, ") to (", (the) otro, ")";
    move uno to otro;
];

[ InfixRemoveSub;
    print "; remove (", (the) uno, ")";
    remove uno;
];

[ InfixHex x y;
    y = (x & $7f00) / $100;
    if (x < 0) y = y + $80;
    x = x & $ff;
    print (Infixhexdigit) y/$10, (Infixhexdigit) y, (Infixhexdigit) x/$10, (Infixhexdigit) x;
];

[ Infixhexdigit x; x = x % $10; if (x < 10) print x; else print (char) 'a'+x-10; ];

[ InfixExamineOSub;
    infix_data1 = metaclass(uno);
    infix_term_type = INFIXTT_NAMEDOBJECT;  ! Antes era INFIXTT_CONSTANT
    InfixExamineP(false);
];

[ InfixExamineSSub;
    infix_term_type = INFIXTT_STATICSTRING;
    InfixExamineP(false);
];

[ InfixExamineSub;     InfixExamineP(false); ];

[ InfixExamineP brief x a b w flag lines;
  
    switch (infix_term_type) {
      INFIXTT_NUMBER:
        if (brief) "; == ", uno;
        print "; The number ", uno, " == $", (InfixHex) uno;
        if (uno >= 32 && uno < 127) print " == '", (char) uno, "'";
        new_line;
      INFIXTT_NAMEDOBJECT:
        print "~", (name) uno, "~ (", uno, ")^"; if (brief) return;
        <<MostrarObjeto uno>>;
      INFIXTT_CONSTANT:
        if (brief) "; == ", uno;
        switch (infix_data1 & 15) {
            nothing:
                print "; Constant ", (InfixPrintConstant) infix_parsed_lvalue,
                " == ", uno, "^";
            2: <<MostrarObjeto uno>>;      ! Object
            1:                             ! Class
                print "Class ", (name) uno, "^";
                objectloop (a ofclass uno) {
                    if (flag) print ", "; else print "Contains: ";
                    print (name) a, " (", a, ")"; flag=true;
                }
                if (flag == false) "No object is of this class";
        }
        new_line;
      INFIXTT_ATTRIBUTE:
        if (brief) "; == ", uno;
        if (uno >= 48 || uno < 0) "; No such attribute";
        print "; Attribute ", (InfixPrintAttribute) uno,
            " (numbered ", uno, ")^";
        objectloop (x has uno) {
            if (flag) print ", ";
            else print "Each of these ~has ", (InfixPrintAttribute) uno, "~: ";
            print (name) x, " (", x, ")"; flag = true;
        }
        if (flag == false) "No object ~has ", (InfixPrintAttribute) uno, "~";
        new_line;
      INFIXTT_PROPERTY:

        if (brief) "; == ", uno;
        print "; Property ", (property) uno, " (numbered ", uno, ")^";
        objectloop (x provides uno) {
            if (flag) print ", "; else print "Provided by: ";
            print (name) x, " (", x, ")"; flag = true;
        }
        if (flag == false) "Which is not provided by any object";
        new_line;
      INFIXTT_DWORD:
        if (brief) "; == ", uno;
        if (uno == 0) "; This word is not in the dictionary";
        a = uno->#dict_par1;
        print "; Dictionary word '", (address) uno;
        if (a & 4) print "//p";
        print "' (address ", uno, ")";
        if (a) {
            print ": ";
            if (a & 2)   print "meta ";
            if (a & 1)   print "verb   ";
            if (a & 8)   print "preposition   ";
            if (a & 4)   print "pluralising ";
            if (a & 128) print "uno ";
        }
        new_line;
        if (a & 1) <<MostrarVerbo uno>>;
      INFIXTT_ROUTINE:
        if (brief) "; == ", uno;
        print "; Routine ", (InfixPrintRoutine) infix_parsed_lvalue, " (number ",
          infix_parsed_lvalue, ", packed address ", uno, ")^";
      INFIXTT_GLOBAL:
        if (brief) "; == ", uno;
        print "; Global ", (InfixPrintGlobal) infix_parsed_lvalue, " == ", uno, "^";
      INFIXTT_ARRAY:
        if (brief) "; == ", uno;
        print "; Array ", (InfixPrintArray) infix_parsed_lvalue, " ";
        infix_data1 = infix_data1 % 16;
        switch (infix_data1) {
          0:    print "->"; a=0;
          1:    print "-->"; a=0;
          2:    print "string"; a=1;
          3:    print "table"; a=1;
          4:    print "buffer"; a=WORDSIZE;
        }
        print " ", infix_data2 + 1 - a, "^; == "; b = infix_data2;
        for (w=b : w>=a : w--)
            if (infix_data1 == 0 or 2 or 4) { if (uno->w) break; }
            else { if (uno-->w) break; }
        if (b-w < 5) w=b;
        for (: x<=w : x++) {
            if (infix_data1 == 0 or 2 or 4) print uno->x, " ";
            else print uno-->x, " ";
            if (x+1 == a) print ": ";
            }
        if (w < b) print "(then ", b-w, " zero entries)";
        new_line;
      INFIXTT_ACTION:
        if (brief) "; == ", uno;
        if (uno >= #lowest_fake_action_number && uno <= #highest_fake_action_number)
           "; Fake action ", (InfixPrintFakeAction) uno,
            " (numbered ", uno, ")^Is not generated by any grammar";
        print "; Action ", (InfixPrintAction) uno,
            " (numbered ", uno, ")^";

        #ifdef TARGET_ZCODE;
        w = HDR_DICTIONARY-->0;
        for (b=0 : b<(HDR_DICTIONARY-->0 + 5)-->0 : b++) {
            w = HDR_DICTIONARY-->0 + 7 + b*9;
            if ((w->#dict_par1) & 1) {
                a = (HDR_STATICMEMORY-->0)-->($ff-(w->#dict_par2));
                lines = a->0; a++;
                for (: lines>0 : lines--) {
                    a = DesempaquetarLineaGramatica(a);
                    if (accion_que_seria == uno) {
                        print "'", (address) w, "' "; DepurarLineaGramatica();
                        new_line;
                        flag = true;
                    }
                }
            }
        }   
        #Ifnot;
        for (b=0 : b < #dictionary_table-->0 : b++) {
            w = #dictionary_table + WORDSIZE + b*(DICT_WORD_SIZE + 7);
            if ((w->#dict_par1) & 1) {
               a = (#grammar_table)-->($100-(w->#dict_par2));
                lines = a->0; a++;
                for (: lines>0 : lines--) {
                    a = DesempaquetarLineaGramatica(a);
                    if (accion_que_seria == uno) {
                       print "'", (address) w, "' "; DepurarLineaGramatica();
                        new_line;
                        flag = true;
                    }
                }
            }
        }
        #Endif;
        if (flag == 0) "Is not generated by any grammar";
      INFIXTT_SYSFUN:
        if (brief) "; == ", uno;
        "; System function ~", (address) infix_parsed_lvalue, "~ has
        not been overridden by any routine and so has its standard definition.";
      INFIXTT_STATICSTRING:
        if (brief) "; == ", uno;
        if (metaclass(uno) ~= String) "; ", uno, " is not a string.";
        print "~", (string) uno, "~^";
      INFIXTT_LOGICAL:
        if (uno == true) "; true"; if (uno == false) "; false";
        "; ", uno;
    }
   
]; ! end of InfixExamineP

[ InfixDescribeWatchSub x y z s flag aflag;
    print "; The Infix ~;watch~ verb allows you to set a watch on any named
        routine(s) or objects: for instance ~;watch ScoreSub~ or
        ~;watch silver bars~. You can also:
        ^    ~;watch objects~: changes to attribute or property settings";
    if (debug_flag & 8) print " (on)"; else print " (off)";

    print ";^    ~;watch timers~: the running of timers and daemons each turn";
    if (debug_flag & 4) print " (on)"; else print " (off)";

    print ";^    ~;watch messages~: all messages sent";
    if (debug_flag & 1) print " (on)"; else print " (off)";

    print ";^    ~;watch actions~: all actions generated";
    if (debug_flag & 2) print " (on)"; else print " (off)";

    print ".^~~;watch~ can be abbreviated to ~;w~ and use ~off~ to stop
        watching: for instance ~;w location off~.^";
    aflag = debug_flag;
    objectloop (x has infix__watching) flag = true; aflag = aflag || flag;
    if (flag) print "The following objects are currently being watched: ";
    flag = false;
    objectloop (x has infix__watching) {
        if (flag) print ", "; flag = true;
        print (name) x, " (", x, ")";
    }
    if (flag) new_line;
    s = (#highest_routine_number - #lowest_routine_number);
    if (s%8 == 0) s=s/8; else s=s/8+1;
    for (flag=false,x=0 : x<s : x++) if (#routine_flags_array->x) flag = true;
    aflag = aflag || flag;
    if (flag) print "The following routines are currently being watched: ";
    for (x=0,flag=false : x<s : x++) {
        for (y=1,z=0 : y<256 : z++,y=y*2) {
            if ((#routine_flags_array->x) & y) {
                if (flag) print ", "; flag = true;
                print (InfixPrintRoutine)
                    #lowest_routine_number + x*8 + z;
            }
        }
    }
    if (flag) new_line;
    if (aflag == false) "At present, nothing is being watched.";
];

[ InfixWatchOnSub i j k l;
    if (uno == 0) return InfixDescribeWatchSub();
    if (infix_term_type == INFIXTT_ROUTINE) {
        i = infix_parsed_lvalue/8;
        for (j=0,k=1 : j<infix_parsed_lvalue%8 : j++) k=k*2;
        l = #routine_flags_array->i;
        l = l | k;

        
        #ifdef TARGET_ZCODE;
         @storeb #routine_flags_array i l;
        #ifnot;
        @astoreb #routine_flags_array i l;
        #endif;

       "; Watching routine ", (InfixPrintRoutine) infix_parsed_lvalue, ".";
    }
    if (metaclass(uno) == Object) {
        give uno infix__watching;
       "; Watching object ~", (name) uno, "~ (", uno, ").";
    }
    InfixDescribeWatchSub();
  ];

[ InfixWatchOffSub i j k l t;
    t=0; ! Skip the warning
    if (uno == 0) return InfixDescribeWatchSub();
    if (infix_term_type == INFIXTT_ROUTINE) {
        i = infix_parsed_lvalue/8;
        for (j=0,k=1 : j<infix_parsed_lvalue%8 : j++) k=k*2;
        l = #routine_flags_array->i;
        l = l & (~k);
        
        #ifdef TARGET_ZCODE;
         @storeb #routine_flags_array i l;
        #ifnot;
        @astoreb #routine_flags_array i l;
        #endif;
       "; Not watching ", (InfixPrintRoutine) infix_parsed_lvalue, ".";
    }
    if (metaclass(uno) == Object) {
    #ifdef TARGET_ZCODE;
        @clear_attr uno infix__watching;
    #ifnot;
        t = infix__watching+8;
        @astorebit uno t 0; ! give uno ~otro; 
    #endif;
       "; Not watching object ~", (name) uno, "~ (", uno, ").";
    }
    InfixDescribeWatchSub();
];





[ InfixList from to tab filter i flag;
   
    for (i=from : i<=to : i++)
    
        if (tab-->(i-from)) {
            flag = true;
            if (tab == #array_names_array) {
                Symb__Tab(INFIXTT_ARRAY, i);
                flag = ~~(temp__global3 & 16);
            }
            if (tab == #routine_names_array) {
                Symb__Tab(INFIXTT_ROUTINE,i);
                flag = ~~(temp__global3 & 16);
            }
            if (tab == #constant_names_array) {
                Symb__Tab(INFIXTT_CONSTANT,i);
                flag = (~~(temp__global3 & 16)) && (temp__global3 % 16 == filter);
            }
            if (flag) print (string) tab-->(i-from), " ";
        }
    new_line;
];

![ vInfixList from to tab filter i flag;
!    print "Called with from ",from," to ",to," table ",tab;
!    for (i=from : i<=to : i++)
!    {      
!        print "(",i,"=",tab-->(i-from),")";
!        if (tab-->(i-from)) {
!            flag = true;
!            if (tab == #array_names_array) {
!                Symb__Tab(INFIXTT_ARRAY, i);
!                flag = ~~(temp__global3 & 16);
!            }
!            if (tab == #routine_names_array) {
!                Symb__Tab(INFIXTT_ROUTINE,i);
!                flag = ~~(temp__global3 & 16);
!            }
!            if (tab == #constant_names_array) {
!                Symb__Tab(INFIXTT_CONSTANT,i);
!                flag = (~~(temp__global3 & 16)) && (temp__global3 % 16 == filter);
!            }
!            if (flag) print (string) tab-->(i-from), " ";
!        }
!     }   
!    new_line;
!];


[ InfixInvSub i;

    print (string) Historia, (string) Titular;
    style bold;
    print "^- ", (number) #highest_object_number - #lowest_object_number + 1,
        " objetos;^";

    print "- Constantes nombres de objeto no-librer�a: ";
    style roman;
    InfixList(#lowest_constant_number, #highest_constant_number, #constant_names_array, 2);

    style bold;
    print "- ", (number) #highest_class_number - #lowest_class_number + 1, " clases: ";
    style roman;
    for (i=#lowest_class_number : i<=#highest_class_number : i++)
        print (name) #class_objects_array-->i, " ";
    new_line;

    style bold;
    print "- Arrays no-librer�a: ";
    style roman;
    InfixList(#lowest_array_number, #highest_array_number, #array_names_array);

    style bold;
    print "- Rutinas no-librer�a: ";
    style roman;
    InfixList(#lowest_routine_number, #highest_routine_number, #routine_names_array);

    style bold;
    print "- Constantes no-librer�a: ";
    style roman;
    InfixList(#lowest_constant_number, #highest_constant_number, #constant_names_array, 0);

    style bold;
    print "- Propiedades comunes: ";
    style roman;
    #ifdef TARGET_ZCODE;
        InfixList(#lowest_property_number, 63, #property_names_array);!, Z fixed size.
    #ifnot;
        InfixList(#lowest_property_number, (#identifiers_table-->1)-1, #property_names_array);!, Z fixed size.
    #endif;

    style bold;
    print "- Propiedades individuales: ";
    style roman;
    #ifdef TARGET_ZCODE;
        InfixList(64, #highest_property_number, #property_names_array + 63*2); ! , Z, 1element = 2bytes
    #ifnot;
    !    InfixList(#identifiers_table-->1, #highest_property_number, #property_names_array + (#identifiers_table-->1)*4); ! , GLULX, 1element = 4bytes
        InfixList((#identifiers_table-->1), (#identifiers_table-->3)+(#identifiers_table-->1)-1, 
                  #property_names_array + (#identifiers_table-->1-1)*4); ! , GLULX, 1element = 4bytes
                  
    !                 l = INDIV_PROP_START + #identifiers_table-->3;
!        print ":::",INDIV_PROP_START," ",#identifiers_table-->3," ",#identifiers_table-->1,"^";!    
!    256 start position of individual properties
!    22 Number of individual properties
!    51 Number of common properties
!    si modifico el c�digo para sumar: 277-72=+205
!    72-51=21 en la tabla de individuales, 256+21=277 

    #endif;

![ InfixList from to table filter i flag;

    style bold;
    print "- Atributos: ";
    style roman;
    InfixList(#lowest_attribute_number, #highest_attribute_number, #attribute_names_array);

    if (true) return;

    style bold;
    print "- Variables: ";
    style roman;
    InfixList(#lowest_global_number, #highest_global_number, #global_names_array);

    style bold;
    print "- Acciones: ";
    style roman;
    InfixList(#lowest_action_number, #highest_action_number, #action_names_array);

    style bold;
    print "- Acciones falsas: ";
    style roman;
    InfixList(#lowest_fake_action_number, #highest_fake_action_number, #fake_action_names_array);
];

Verb meta ";i" ";inv" ";inventario"
     * -> InfixInv;
Verb meta ";x" ";examina" ";ex"
     * InfixRvalue -> InfixExamine;
Verb meta ";xo" ";examinao" ";exo"
     * InfixRvalue -> InfixExamineO;
Verb meta ";xs" ";examinas" ";exs"
     * InfixRvalue -> InfixExamineS;
Verb meta ';<'
    * InfixActionToken                          -> InfixAction
    * InfixActionToken InfixRvalue              -> InfixAction
    * InfixActionToken InfixRvalue InfixRvalue  -> InfixAction;
Verb meta ';//'
    *                                           -> InfixWelcome
    * InfixRvalue                               -> InfixEval;
Verb meta ';give'
    * InfixRvalue InfixRvalue                   -> InfixGive
    * InfixRvalue 'not' InfixRvalue             -> InfixGiveNot;
Verb meta ';move'
    * InfixRvalue "to" InfixRvalue              -> InfixMove;
Verb meta ';remove'
    * InfixRvalue                               -> InfixRemove;
Verb meta ";watch" ";w" ";observa" ";o"
    *                     -> InfixWatchOn
    * "relojes"/"daemons"/"timers"     -> ActivarRelojes
    * "relojes"/"daemons"/"timers" "off" -> DesactivarRelojes
    * "actions"/"acciones"        -> ActivarAcciones
    * "actions"/"acciones" "off"    -> DesactivarAcciones
    * "messages"/"mensajes"        -> ActivarRutinas
    * "messages"/"mensajes" "off"    -> DesactivarRutinas
    * "objects"/"objetos"        -> CambiosOn
    * "objects"/"objetos" "off"        -> CambiosOff
    * InfixRvalueTerm             -> InfixWatchOn
    * InfixRvalueTerm "off"         -> InfixWatchOff;

#Endif; ! DEBUG

! ==============================================================================
