
! prand.h
! release 1
! Copyright 2001 Anson Turner.
!
! To use:
! Prior to including the file, you may declare constant PS_TAB_SIZE,
! which determines the number of strings the system can remember.
! (If unspecified, it defaults to 1000.) Of course, it also consumes
! that many words of dynamic memory.
!
! Call prand() with up to 7 strings as parameters, e.g.:
!
!     prand("blue ","red ","green ","speckled onyx ");
!
! The routine will print one of the strings at random, except that
! it will not print the same string a second time until all of the
! strings have been printed at least once. Thus, if the above call
! was repeated 4 times, the results might be:
!
!     green blue speckled onyx red 
!
! But *not*:
!
!     blue green red green 
!
! Because "green" would have been printed twice, while "speckled onyx"
! would not have been printed at all.
!
! A note about strings in Inform is in order. Each double-quoted
! string in the source is compiled into a static string, which is
! accessed via its address in memory. Two strings which may contain
! the exact same set of characters are nevertheless completely
! different strings as far as the system is concerned. Thus:
!
!     prand ("blue ","red ");  prand ("blue ","red ");
!
! could very well result in:
!
!     red red 
!
! After all the strings in the call have been printed once, the output
! becomes completely random. (Not in the sense that you will get
! _Hamlet_ by running it enough times, but rather that it will
! randomly choose one of the strings without concern for repetition.)
! 
! Note that prand() returns the "index" of the string chosen, meaning,
! for example, if it prints the second string, it returns 2.
!
! strand() can be used if you want to choose from more than 7 strings,
! by embedding calls to it within a call to prand(). E.g.:
!
! prand(strand("one","two","three","four"),
!       strand("five","six","seven","eight"))
!
! This method will allow, somewhat inefficiently, up to 49 strings. If
! you want more, you could always embed strand() calls within strand()
! calls, but don't, because it's insane.
!
! All of the above describes what happens when the global prand_options
! is 0. If it is set to something, most likely by calling one of the
! wrapper functions, prand() may behave differently.
!
! prand_options takes a bitmap with the following possible components:
!
! PRAND_SEQUENTIAL_FIRST    Prints the strings in the order they are
!                           given, then becomes random.
!
! PRAND_HOLD_ON_LAST        After all the strings have been printed
!                           once, keeps repeating the last one printed.
!
! PRAND_RETURN_STRING       Instead of returning the index of the
!                           string printed, returns the string itself
!                           (that is, its address).
!
! PRAND_DONT_PRINT          Does not print anything, merely returning
!                           either the index or the string. It also
!                           does not record the string as having been
!                           printed.
!
! PRAND_NEW_ONLY            If all the strings have already been
!                           printed once, does nothing.
!
! PRAND_USE_PRINTX          (This option only applies if using my
!                           alternative library, Platypus.)
!                           Filters the printed string through
!                           PrintX().
!
! The wrapper functions, and the options they use, are:
!
! prandonce()           PRAND_NEW_ONLY and PRAND_SEQUENTIAL_FIRST
! prandlast()           PRAND_HOLD_ON_LAST and PRAND_SEQUENTIAL_FIRST
! prandseq()            PRAND_SEQUENTIAL_FIRST
! strand()              PRAND_RETURN_STRING and PRAND_DONT_PRINT
! 
! You can, of course, also set prand_options manually before calling
! prand(). (Note that prand_options is reset to 0 after calling any
! wrapper function.) You can also define your own wrappers. But you
! knew that already.
!
! Modificaciones (c) 2012 Ricardo Pérez (Sothoth) - Alpha Aventuras

System_file;

Default PS_TAB_SIZE  = 1000; ! Número máx. de cadenas DIFERENTES a imprimir
Default PS_TEMP_SIZE = 50;   ! Número máx. de cadenas en cada llamada a prand

Array printed_strings --> PS_TAB_SIZE;   ! Cadenas que se han impreso
Array ps_temp          -> PS_TEMP_SIZE;  ! Posiciones de cadenas en llamadas  
Array str_temp        --> PS_TEMP_SIZE;  ! Cadenas en una llamada a prand

Global ps_tab_entries;         ! Número de cadenas impresas hasta ahora
Global ps_tab_index = -1;      ! Índice del array printed_strings
Global prand_options;          ! Opciones (ver constantes siguientes)

Constant PRAND_SEQUENTIAL_FIRST   = $1;
Constant PRAND_HOLD_ON_LAST       = $2;
Constant PRAND_RETURN_STRING      = $4;
Constant PRAND_DONT_PRINT         = $8;
Constant PRAND_NEW_ONLY           = $10;

[ prandonce _vararg_count
  a;
  prand_options = PRAND_NEW_ONLY + PRAND_SEQUENTIAL_FIRST;
  @call prand _vararg_count a;
  prand_options = false;
  return a;
];

[ prandlast _vararg_count
  a;
  prand_options = PRAND_HOLD_ON_LAST + PRAND_SEQUENTIAL_FIRST;
  @call prand _vararg_count a;
  prand_options = 0;
  return a;
];

[ prandseq _vararg_count
  a;
  prand_options = PRAND_SEQUENTIAL_FIRST;
  @call prand _vararg_count a;
  prand_options = 0;
  return a;
];

[ strand _vararg_count
  a;
  prand_options = PRAND_RETURN_STRING + PRAND_DONT_PRINT;
  @call prand _vararg_count a;
  prand_options = 0;
  return a;
];

[ strandonce _vararg_count
  a;
  prand_options = PRAND_RETURN_STRING + PRAND_DONT_PRINT +
                  PRAND_NEW_ONLY + PRAND_SEQUENTIAL_FIRST;
  @call prand _vararg_count a;
  prand_options = false;
  return a;
];

[ strandlast _vararg_count
  a;
  prand_options = PRAND_RETURN_STRING + PRAND_DONT_PRINT +
                  PRAND_HOLD_ON_LAST + PRAND_SEQUENTIAL_FIRST;
  @call prand _vararg_count a;
  prand_options = 0;
  return a;
];

[ strandseq _vararg_count
  a;
  prand_options = PRAND_RETURN_STRING + PRAND_DONT_PRINT +
                  PRAND_SEQUENTIAL_FIRST;
  @call prand _vararg_count a;
  prand_options = 0;
  return a;
];

[ prand _vararg_count
  i a str fl c;

  for ( : _vararg_count > 0: _vararg_count--) {
    @copy sp str;
    a++; i = FindByWord(str, printed_strings, ps_tab_entries);
    if (i == -1) { ps_temp->c = a; c++; }
    str_temp-->a = str;    
  }

  if (c) {
    if (prand_options & PRAND_SEQUENTIAL_FIRST) a = ps_temp->0;
    else                                        a = ps_temp->(random(c) - 1);
    jump prandlab01;
  }

  if (prand_options & PRAND_NEW_ONLY) rfalse;
  if (prand_options & PRAND_HOLD_ON_LAST == 0) a = random(a);
  jump prandlab03;

.prandlab01;
  fl = 1;
  if (ps_tab_index > -1) {
    i = ps_tab_index;
    ps_tab_index++;
    if (ps_tab_index > PS_TAB_SIZE) ps_tab_index = 0;
  } else {
    i = FindByWord(0, printed_strings, PS_TAB_SIZE);
    if (i == -1) {
      i = 0; ps_tab_index = 0;
      #ifdef DEBUG; print "***printed_strings table full!***^"; #endif;
    }
  }

.prandlab03;
  str = str_temp-->a;
  if (prand_options & PRAND_DONT_PRINT) jump prandlab04;
  if (fl) {
    ps_tab_entries++;
    printed_strings-->i = str;
  }
  print (string) str;

.prandlab04;
  if (prand_options & PRAND_RETURN_STRING) return str;
  return a;
];

! Esta rutina proviene originalmente de la librería Platypus:
[ FindByWord v arr words fln
  idx;
  if (fln == 0) fln = 1;
  else          words = words / fln;
  fln = fln * WORDSIZE;
  @linearsearch v WORDSIZE arr fln words 0 4 idx;
  return idx;
];

