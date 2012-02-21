! PNJactor.h
! (c) Zak, Junio 2002
!
! Librer�a que implementa "acciones para PSI". Ver la documentaci�n
! para m�s detalles de por qu� esto es necesario, y de c�mo se usa.
!

! ================================================================
! Nuevas propiedades para "capturar" en los objetos las acciones que
! intente realizar el PNJ
!
System_file;

Property additive antesPNJ NULL;
Property additive reaccionar_antesPNJ NULL;
Property additive despuesPNJ NULL;
Property additive reaccionar_despuesPNJ NULL;

#ifndef TAM_BUFFER_AUXILIAR;
Constant TAM_BUFFER_AUXILIAR 300;
#endif;

Global RazonErrorPNJ;
Array  PNJBufAux->TAM_BUFFER_AUXILIAR;
Global longitudCaptura;

! Constantes de error usadas por PNJ
Constant PNJ_IMPIDE_ANTES = 0;
Constant PNJ_OBJETO_NO_PRESENTE = 1;
Constant PNJ_EN_RECIPIENTE_CERRADO = 2;
Constant PNJ_INTENTA_COGER_CRIATURA = 3;
Constant PNJ_INTENTA_COGER_SUBOBJETO = 4;
Constant PNJ_OBJETO_INACCESIBLE = 5;
Constant PNJ_OBJETO_EN_RECIPIENTE_CERRADO = 6;
Constant PNJ_INTENTA_COGERSE_A_SI_MISMO = 7;
Constant PNJ_INTENTA_COGER_AL_JUGADOR = 8;
Constant PNJ_OBJETO_ESCENARIO_O_ESTATICO = 9;

[ RutinasAntesPNJ
  o r;
  
  sw__var = action;
  o = ScopeCeiling(actor);
  
  if (RecursivamenteEjecuta(o, reaccionar_antesPNJ)) {
    RazonErrorPNJ = PNJ_IMPIDE_ANTES;
    rtrue;
  }

  ! Esto es por si el propio actor tiene su rutina antesPNJ, por si quiere impedir la acci�n:
  ! (c) Alpha
  if (actor provides antesPNJ) {
    #ifdef CAPTURAR_SALIDA_PNJ;
      CapturarSalida();
    #endif;
    r = PrintOrRun(actor, antesPNJ);
    #ifdef CAPTURAR_SALIDA_PNJ;
      FinCapturarSalida();
      if (longitudCaptura > 0 && SeVen(actor, player))
        MostrarSalidaCapturada();
    #endif;

    if (r) {
      RazonErrorPNJ = PNJ_IMPIDE_ANTES;
      rtrue;
    }
  }

  if (noun provides antesPNJ) {
    #ifdef CAPTURAR_SALIDA_PNJ;
      CapturarSalida();
    #endif;
    r = PrintOrRun(noun, antesPNJ);
    #ifdef CAPTURAR_SALIDA_PNJ;
      FinCapturarSalida();
      if (longitudCaptura > 0 && SeVen(actor, player))
        MostrarSalidaCapturada();
    #endif;

    if (r) {
      RazonErrorPNJ = PNJ_IMPIDE_ANTES;
      rtrue;
    }
  }
  
  rfalse;
];

[ RutinasDespuesPNJ
  o r;

  sw__var = action;

  if (noun provides despuesPNJ) {
    #ifdef CAPTURAR_SALIDA_PNJ;
      CapturarSalida();
    #endif;
    r = PrintOrRun(noun, despuesPNJ);
    #ifdef CAPTURAR_SALIDA_PNJ;
      FinCapturarSalida();
      if (longitudCaptura > 0 && SeVen(actor, player))
        MostrarSalidaCapturada();
    #endif;

    if (r)
      rtrue;
  }
  
  o = ScopeCeiling(actor);

  ! Esto es por si el propio actor tiene su rutina despuesPNJ, para cambiar el mensaje por defecto:
  ! (c) Alpha
  if (actor provides despuesPNJ)
    if (PrintOrRun(actor, despuesPNJ))
      rtrue;
  
  if (RecursivamenteEjecuta(o, reaccionar_despuesPNJ))
    rtrue;
          
  rfalse;
];


[ RecursivamenteEjecuta obj rutina
  i r;

  if (obj provides rutina) {
    #ifdef CAPTURAR_SALIDA_PNJ;
      CapturarSalida();
    #endif;
    r = PrintOrRun(obj, rutina);
    #ifdef CAPTURAR_SALIDA_PNJ;
      FinCapturarSalida();
      if (longitudCaptura > 0 && SeVen(actor, player))
        MostrarSalidaCapturada();
    #endif;

    if (r)
      rtrue;
  }
  
  objectloop (i in obj)
    if (RecursivamenteEjecuta(i, rutina))
      rtrue;

  rfalse;
];

[ SeVen quien1 quien2;
  return ScopeCeiling(quien1) == ScopeCeiling(quien2);
];


[ PuedeTocar quien item flag
  ancestro i;

  ! Determinar si el PNJ "quien" puede tocar el objeto "item"
  ! Si flag==true, aplicar restricciones relacionadas con Coger.
  !
  ! Esta rutina est� adaptada de la que viene en InformATE, llamada
  ! ObjetoEsIntocable, que estaba centrada en el jugador y emit�a
  ! mensajes cuando exist�a alguna barrera que imped�a tocar. Ahora,
  ! en lugar de mensajes, se retornan c�digos de error apropiados a
  ! trav�s de la variable RazonErrorPNJ

  ! El PNJ y el item que intenta coger pueden estar ambos metidos
  ! dentro de un objeto com�n. Hay que comprobar primero que el PNJ
  ! pueda "tocar" ese objeto com�n, y despu�s que desde ese objeto
  ! se pueda "tocar" al item.

  ! P.ej, el PNJ est� dentro de un barril abierto, dentro de un
  ! almac�n, y el item est� dentro de una caja cerrada, dentro del
  ! mismo almac�n. El antepasado com�n es el almac�n. El PNJ tiene
  ! "via libre" hasta el almac�n (pues el barril est� abierto), pero
  ! no hasta el item, ya que una caja cerrada se interpone entre el
  ! almac�n y el item.

  ! Hacemos varias comprobaciones al intentar coger:
  if (flag) {
  ! Empezamos comprobando si se quiere coger a s� mismo:
    if (quien == item) {
      RazonErrorPNJ = PNJ_INTENTA_COGERSE_A_SI_MISMO;
      rfalse;
    }
    ! Ahora si quiere coger al jugador:
    if (item == selfobj) {
      RazonErrorPNJ = PNJ_INTENTA_COGER_AL_JUGADOR;
      rfalse;
    }
    ! Comprobamos si el objeto es escenario o est�tico
    if (item has scenery || item has static) {
      RazonErrorPNJ = PNJ_OBJETO_ESCENARIO_O_ESTATICO;
      rfalse;
    }
  }
    
  ! De modo que empezamos por averiguar el antepasado com�n
  ancestro = CommonAncestor(quien, item);

  ! Si no hay antepasado comun, es que objeto y PNJ est�n en
  ! diferentes habitaciones
  if (ancestro == 0) {
    RazonErrorPNJ = PNJ_OBJETO_NO_PRESENTE;
    rfalse;
  }

  ! Si el antepasado com�n es el propio PNJ, es que el item est� en
  ! su inventario, tal vez dentro de otros objetos. Pero en todo
  ! caso no es necesario comprobar en ese caso si el PNJ puede
  ! "tocar" al ancestro com�n
  if (ancestro ~= quien) {
    ! En otro caso, es que el PNJ est� metido en alg�n sitio.
    ! Vamos a comprobar que para llegar desde el pnj hasta el
    ! ancestro com�n, s�lo atravesamos recipientes abiertos
    i = parent(quien); 
    while (i ~= ancestro) {
      if (i has container && i hasnt open)  {
        ! Si encontramos un recipiente cerrado, el PNJ no
        ! puede acceder al item, porque ni siquiera puede
        ! acceder al ancestro com�n
        RazonErrorPNJ = PNJ_EN_RECIPIENTE_CERRADO;
        rfalse;
      }
      i = parent(i);
    }
  }

  ! Si ha pasado el test anterior, el PNJ tiene una v�a para llegar
  ! con su mano desde dondequiera que est� encerrado el PNJ hasta el
  ! objeto que le contiene a �l y al item.

  ! Si el objeto que el PNJ intenta tocar es el propio ancestro, es
  ! decir, el propio recipiente que contiene al PNJ, ya ha pasado el
  ! test antes, no es necesario hacer m�s tests
  if (item ~= ancestro) {
    ! Si no, hay que comprobar ahora si entre el ancestro com�n y
    ! el item hay alguna barrera.
    i = parent(item);
    while (i ~= ancestro) {
      ! Si hay que aplicar las restricciones especiales de
      ! COGER, estas son: no se puede coger algo que pertenezca
      ! a un ser animado, ni algo que sea parte de otra cosa.
      if (flag && i hasnt container && i hasnt supporter) {
        if (i has animate) {
          RazonErrorPNJ = PNJ_INTENTA_COGER_CRIATURA;
          rfalse;
        }
        if (i has transparent) {
          RazonErrorPNJ = PNJ_INTENTA_COGER_SUBOBJETO;
          rfalse;
        }
        ! Y si no, es que el item est� dentro de otra cosa que
        ! no es un recipiente ni soporte, por tanto
        ! inaccesible.
        RazonErrorPNJ = PNJ_OBJETO_INACCESIBLE;
        rfalse;
      }

      ! Si no queremos plantear restricciones de Coger, sino
      ! simplemente comprobar si se puede tocar o no, basta
      ! verificar que la cadena de contenedores no tenga un
      ! recipiente cerrado.
      if (i has container && i hasnt open) {
        RazonErrorPNJ = PNJ_OBJETO_EN_RECIPIENTE_CERRADO;
      	rfalse;
      }
      i = parent(i);
    }
  }

  ! Si pasa todos los test, se puede tocar/coger
  rtrue;
];


[ PuedeVer quien item
  ancestro i;

  ! Determinar si el PNJ "quien" puede ver el objeto "item"
  ancestro = CommonAncestor(quien, item);

  ! Si no hay antepasado comun, es que objeto y PNJ est�n en
  ! diferentes habitaciones
  if (ancestro == 0) {
    RazonErrorPNJ = PNJ_OBJETO_NO_PRESENTE;
    rfalse;
  }

  ! Si el antepasado com�n es el propio PNJ, es que el item est� en
  ! su inventario, tal vez dentro de otros objetos. Pero en todo
  ! caso no es necesario comprobar en ese caso si el PNJ puede
  ! "ver" al ancestro com�n
  if (ancestro ~= quien) {
    ! En otro caso, es que el PNJ est� metido en alg�n sitio.
    ! Vamos a comprobar que para llegar desde el pnj hasta el
    ! ancestro com�n, no atravesamos ningun recipiente cerrado
    ! opaco.
    i = parent(quien); 
    while (i ~= ancestro) {
      if (i has container && i hasnt open && i hasnt transparent) {
        RazonErrorPNJ = PNJ_EN_RECIPIENTE_CERRADO;
        rfalse;
      }
      i = parent(i);
    }
  }

  ! Si ha pasado el test anterior, el PNJ tiene una v�a visual
  ! desde dondequiera que est� encerrado el PNJ hasta el
  ! objeto que le contiene a �l y al item.

  ! Si el objeto que el PNJ intenta ver es el propio ancestro, es
  ! decir, el propio recipiente que contiene al PNJ, ya ha pasado el
  ! test antes, no es necesario hacer m�s tests
  if (item ~= ancestro) {
    ! Si no, hay que comprobar ahora si entre el ancestro com�n y
    ! el item hay alguna barrera.
    i = parent(item);
    while (i ~= ancestro) {
      ! verificar que la cadena de contenedores no tenga un
      ! recipiente cerrado opaco.
      if (i has container && i hasnt open && i hasnt transparent) {
        RazonErrorPNJ = PNJ_OBJETO_EN_RECIPIENTE_CERRADO;
        rfalse;
      }
      i = parent(i);
    }
  }

  ! Si pasa todos los test, se puede ver
  rtrue;
];

! Rutinas de bajo nivel para capturar la salida en un array
!

#ifdef TARGET_GLULX;
Global streamanterior;
Global streambufferpnj;

[ CapturarSalida;
  streamanterior = glk($0048); ! stream_get_current
  streambufferpnj = glk($0043, PNJbufaux, TAM_BUFFER_AUXILIAR, 1, 0); ! stream_open_memory
  if (streambufferpnj == 0) return 0;
  glk($0047, streambufferpnj); ! stream_set_current
];

[ FinCapturarSalida;
  ! Restaurar el stream original
  glk($0047, streamanterior); ! stream_set_current

  ! Y cerrar el stream de memoria, obteniendo as� la longitud de lo
  ! que se ha escrito
  @copy $ffffffff sp;
  @copy streambufferpnj sp;
  @glk $0044 2 0; ! stream_close
  @copy sp longitudCaptura;
  @copy sp 0;

  if (longitudCaptura >= TAM_BUFFER_AUXILIAR)
    print "[** Error: se ha perdido texto en la captura **]^";

  return longitudcaptura;
];

[ MostrarSalidaCapturada i n;
  n = false;
  for (i = 0 : i < longitudCaptura : i++) {
    if (EsMayuscula(PNJBufAux->i) && i + 1 < longitudCaptura && EsMayuscula(PNJBufAux->(i + 1))) {
      n = true;
      style bold;
    }
    print (char) PNJBufAux->i;
    if (i + 1 < longitudCaptura && ~~EsMayuscula(PNJBufAux->(i + 1)) && n) {
      n = false;
      style roman;
    }
  }
  style roman;
];
#endif;

[ EsMayuscula c;
  return c == 'A' or 'B' or 'C' or 'D' or 'E' or 'F' or 'G' or 'H' or 'I' or
              'J' or 'K' or 'L' or 'M' or 'N' or '�' or 'O' or 'P' or 'Q' or
              'R' or 'S' or 'T' or 'U' or 'V' or 'W' or 'X' or 'Y' or 'Z';
];

#ifdef TARGET_ZCODE;
[ CapturarSalida;
  PNJBufAux-->0 = TAM_BUFFER_AUXILIAR - WORDSIZE;
  @output_stream 3 PNJBufAux;
];

[ FinCapturarSalida;
  @output_stream -3;
  longitudCaptura=PNJBufAux-->0;
  return longitudCaptura;
];

[ MostrarSalidaCapturada
  i;

  for (i = 0 : i < longitudCaptura : i++)
    print (char) PNJBufAux->(i + WORDSIZE);
];
#endif;

