!
! Mapeador.h - Una librería para generar el mapa de la aventura automáticamente
!
! Copyright (c) 2012 Ricardo Pérez López (Alpha Aventuras)
!

! This program is free software: you can redistribute it and/or modify
! it under the terms of the GNU General Public License as published by
! the Free Software Foundation, either version 3 of the License, or
! (at your option) any later version.
!
! This program is distributed in the hope that it will be useful,
! but WITHOUT ANY WARRANTY; without even the implied warranty of
! MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
! GNU General Public License for more details.
!
! You should have received a copy of the GNU General Public License
! along with this program.  If not, see <http://www.gnu.org/licenses/>.


System_file;

Include "infglk";

Message " __________________________________________________________________";
Message "|                                                                  |";
Message "|                 * MAPEADOR:  I M P O R T A N T E *               |";
Message "|                 ==================================               |";
Message "| 1. Pon 'Include ~Mapeador.h~;' despues de 'Include ~SGW+DMX.h~;' |";
Message "| 2. Define la rutina EncenderGraficos()                           |";
Message "| 3. Crea una rutina HandleGlkEvent() y llama desde ella a:        |";
Message "|    Mapa_HandleGlkEvent(ev, context, buffer)                      |";
Message "| 4. Crea una rutina IdentifyGlkObject() y llama desde ella a:     |";
Message "|    Mapa_IdentifyGlkObject(fase, tipo, ref, rock)                 |";
Message "|__________________________________________________________________|";

Constant GG_MAPAWIN_ROCK = 212;

Global gg_mapawin = 0;
Global ladoCuadrado = 61;
Global g_sitio = 0;              ! El sitio actual (se usa en Mapa_HandleGlkEvent)
Global lugaresDibujados = 0;

Default MAPA_COLOR_LOCAL          = $ffffff;
Default MAPA_COLOR_CURSOR         = $00aaaa;
Default MAPA_COLOR_ACTUAL         = $aaaaaa;
Default MAPA_COLOR_PUERTA_ABIERTA = $00ff00;
Default MAPA_COLOR_PUERTA_CERRADA = $ff0000;
Default MAPA_COLOR_ARRIBA_ABAJO   = $0000ff;
Default MAPA_COLOR_DENTRO_FUERA   = $0000ff;
Default MAPA_COLOR_BORDE          = $ffffff;
Default MAPA_COLOR_CONEXION       = $ffffff;

Default MAPA_MAX_LUGARES = 30;

Array lugaresMapaPosX  --> MAPA_MAX_LUGARES;
Array lugaresMapaPosY  --> MAPA_MAX_LUGARES;
Array lugaresMapaLugar --> MAPA_MAX_LUGARES;

Verb meta 'mapa'
  *                 -> Mapa;

[ Mapa_IdentifyGlkObject fase tipo ref rock
  cenx ceny;
   if (fase == 0) { ! Poner cero en todos nuestros objetos glk
      gg_mapawin = 0;
      return;
   }       
   if (fase == 1) { ! Reiniciar correctamente las variables glk
      switch (tipo) {
         0: ! es una ventana
            switch (rock) {
               GG_MAPAWIN_ROCK: gg_mapawin = ref;
            }
         1: ! es un flujo
            ! pero no hay flujos en este ejemplo
         2: ! es una referencia a fichero
            ! pero no hay ficheros en este ejemplo
      }
      return;
   }       
   if (fase == 2) { ! Actualizar nuestras ventanas
     if (gg_mapawin ~= 0) {
       glk_window_get_size(gg_mapawin, gg_arguments, gg_arguments + WORDSIZE);
       cenx = (gg_arguments-->0) / 2; ! ancho / 2
       ceny = (gg_arguments-->1) / 2; ! alto / 2
       RefrescarMapa(g_sitio, cenx, ceny);
     }
   }
];

[ Mapa_HandleGlkEvent ev context buffer
  cenx ceny;
  context = context;
  buffer = buffer;
  switch (ev-->0) {
    evtype_Redraw, evtype_Arrange:
      if (gg_mapawin ~= 0 && g_sitio ~= 0) {
!        AbrirVentanaMapa();
        glk_window_get_size(gg_mapawin, gg_arguments, gg_arguments + WORDSIZE);
        cenx = (gg_arguments-->0) / 2; ! ancho / 2
        ceny = (gg_arguments-->1) / 2; ! alto / 2
        RefrescarMapa(g_sitio, cenx, ceny);
      }
  }
];

[ EncenderGraficos;
  rfalse;
];

[ DibujarPuertaMapa cenx ceny ck
  color;
  if (ck == 2) color = MAPA_COLOR_PUERTA_ABIERTA;
  else         color = MAPA_COLOR_PUERTA_CERRADA;
  glk_window_fill_rect(gg_mapawin, color,
                       cenx - ladoCuadrado / 8, ceny - ladoCuadrado / 8,
                       ladoCuadrado / 4 + 1, ladoCuadrado / 4 + 1);
];

#ifndef LugarReal;
[ LugarReal;
  if (location == thedark) return real_location;
  else                     return location;
];
#endif;

[ DibujarMapa sitio posx posy central
  color sep mitad x y ck;
  if (~~(sitio.dibujado)) {
    sitio.dibujado = true;
    if (sitio == LugarReal()) color = MAPA_COLOR_ACTUAL;
    else if (central == 1)    color = MAPA_COLOR_CURSOR;
    else                      color = MAPA_COLOR_LOCAL;
    mitad = ladoCuadrado / 2;
    glk_window_fill_rect(gg_mapawin, MAPA_COLOR_CONEXION, posx - mitad,
                         posy - mitad, ladoCuadrado, ladoCuadrado);
    glk_window_fill_rect(gg_mapawin, color, posx - mitad + 3, posy - mitad + 3,
                         ladoCuadrado - 6, ladoCuadrado - 6);
    lugaresMapaPosX-->lugaresDibujados = posx - mitad;
    lugaresMapaPosY-->lugaresDibujados = posy - mitad;
    lugaresMapaLugar-->lugaresDibujados = sitio;
    lugaresDibujados++;
    sep = ladoCuadrado + mitad;
    ck = ComprobarSalidaMapa(sitio, e_to); 
    if (ck) {
      if (ck == 2 or 3) DibujarPuertaMapa(posx + mitad, posy, ck);
      glk_window_fill_rect(gg_mapawin, MAPA_COLOR_CONEXION, posx + mitad,
                           posy - 1,
                           sep - ladoCuadrado + 1, 3);
      DibujarMapa(DestinoSalidaMapa(sitio, e_to), posx + sep, posy, 0);
    }
    ck = ComprobarSalidaMapa(sitio, w_to); 
    if (ck) {
      if (ck == 2 or 3) DibujarPuertaMapa(posx - mitad, posy, ck);
      glk_window_fill_rect(gg_mapawin, MAPA_COLOR_CONEXION,
                           posx - sep + mitad, posy - 1,
                           sep - ladoCuadrado + 1, 3);
      DibujarMapa(DestinoSalidaMapa(sitio, w_to), posx - sep, posy, 0);
    }
    ck = ComprobarSalidaMapa(sitio, n_to); 
    if (ck) {
      if (ck == 2 or 3) DibujarPuertaMapa(posx, posy - mitad, ck);
      glk_window_fill_rect(gg_mapawin, MAPA_COLOR_CONEXION,
                           posx - 1, posy - sep + mitad, 3,
                           sep - ladoCuadrado + 1);
      DibujarMapa(sitio.n_to, posx, posy - sep, 0);
    }
    ck = ComprobarSalidaMapa(sitio, s_to);
    if (ck) {
      if (ck == 2 or 3) DibujarPuertaMapa(posx, posy + mitad, ck);
      glk_window_fill_rect(gg_mapawin, MAPA_COLOR_CONEXION, posx - 1,
                           posy + mitad, 3,
                           sep - ladoCuadrado + 1);
      DibujarMapa(sitio.s_to, posx, posy + sep, 0);
    }
    ck = ComprobarSalidaMapa(sitio, nw_to);
    if (ck) {
      if (ck == 2 or 3) DibujarPuertaMapa(posx - mitad, posy - mitad, ck);
      for (x = posx - sep + mitad, y = posy - sep + mitad : x <= posx - mitad : x++, y++) {
        glk_window_fill_rect(gg_mapawin, MAPA_COLOR_CONEXION, x - 1, y + 1, 1, 1);
        glk_window_fill_rect(gg_mapawin, MAPA_COLOR_CONEXION, x, y, 1, 1);
        glk_window_fill_rect(gg_mapawin, MAPA_COLOR_CONEXION, x + 1, y - 1, 1, 1);
      }
      DibujarMapa(sitio.nw_to, posx - sep, posy - sep, 0);
    }
    ck = ComprobarSalidaMapa(sitio, ne_to);
    if (ck) {
      if (ck == 2 or 3) DibujarPuertaMapa(posx + mitad, posy - mitad, ck);
      for (x = posx + mitad, y = posy - mitad : x <= posx + sep - mitad : x++, y--) {
        glk_window_fill_rect(gg_mapawin, MAPA_COLOR_CONEXION, x - 1, y - 1, 1, 1);
        glk_window_fill_rect(gg_mapawin, MAPA_COLOR_CONEXION, x, y, 1, 1);
        glk_window_fill_rect(gg_mapawin, MAPA_COLOR_CONEXION, x + 1, y + 1, 1, 1);
      }
      DibujarMapa(sitio.ne_to, posx + sep, posy - sep, 0);
    }
    ck = ComprobarSalidaMapa(sitio, sw_to);
    if (ck) {
      if (ck == 2 or 3) DibujarPuertaMapa(posx - mitad, posy + mitad, ck);
      for (x = posx - sep + mitad, y = posy + sep - mitad : x <= posx - mitad : x++, y--) {
        glk_window_fill_rect(gg_mapawin, MAPA_COLOR_CONEXION, x - 1, y - 1, 1, 1);
        glk_window_fill_rect(gg_mapawin, MAPA_COLOR_CONEXION, x, y, 1, 1);
        glk_window_fill_rect(gg_mapawin, MAPA_COLOR_CONEXION, x + 1, y + 1, 1, 1);
      }
      DibujarMapa(sitio.sw_to, posx - sep, posy + sep, 0);
    }
    ck = ComprobarSalidaMapa(sitio, se_to);
    if (ck) {
      if (ck == 2 or 3) DibujarPuertaMapa(posx + mitad, posy + mitad, ck);
      for (x = posx + mitad, y = posy + mitad : x <= posx + sep - mitad : x++, y++) {
        glk_window_fill_rect(gg_mapawin, MAPA_COLOR_CONEXION, x - 1, y + 1, 1, 1);
        glk_window_fill_rect(gg_mapawin, MAPA_COLOR_CONEXION, x, y, 1, 1);
        glk_window_fill_rect(gg_mapawin, MAPA_COLOR_CONEXION, x + 1, y - 1, 1, 1);
      }
      DibujarMapa(sitio.se_to, posx + sep, posy + sep, 0);
    }
    ck = ComprobarSalidaMapa(sitio, u_to);
    if (ck) {
      if (ck == 2 or 3) DibujarPuertaMapa(posx + mitad / 2, posy - mitad / 2 + mitad / 4, ck);
      glk_window_fill_rect(gg_mapawin, MAPA_COLOR_ARRIBA_ABAJO, posx + mitad / 2,
                           posy - mitad / 2, 1, mitad);
      for (x = 1 : x <= mitad / 4 : x++) {
        glk_window_fill_rect(gg_mapawin, MAPA_COLOR_ARRIBA_ABAJO, posx + mitad / 2 + x,
                             posy - mitad / 2 + x, 1, 1);
        glk_window_fill_rect(gg_mapawin, MAPA_COLOR_ARRIBA_ABAJO, posx + mitad / 2 - x,
                             posy - mitad / 2 + x, 1, 1);
      }
    }
    ck = ComprobarSalidaMapa(sitio, d_to);
    if (ck) {
      if (ck == 2 or 3) DibujarPuertaMapa(posx + mitad / 2, posy + mitad / 2 - mitad / 4, ck);
      glk_window_fill_rect(gg_mapawin, MAPA_COLOR_ARRIBA_ABAJO, posx + mitad / 2,
                           posy - mitad / 2, 1, mitad);
      for (x = 1 : x <= mitad / 4 : x++) {
        glk_window_fill_rect(gg_mapawin, MAPA_COLOR_ARRIBA_ABAJO, posx + mitad / 2 + x,
                             posy + mitad / 2 - x, 1, 1);
        glk_window_fill_rect(gg_mapawin, MAPA_COLOR_ARRIBA_ABAJO, posx + mitad / 2 - x,
                             posy + mitad / 2 - x, 1, 1);
      }
    }
    ck = ComprobarSalidaMapa(sitio, in_to);
    posx = posx - mitad / 3;
    if (ck) {
      if (ck == 2 or 3) DibujarPuertaMapa(posx + mitad / 2 - mitad / 4, posy, ck);
      glk_window_fill_rect(gg_mapawin, MAPA_COLOR_DENTRO_FUERA, posx - mitad / 2,
                           posy, mitad, 1);
      for (x = 1 : x <= mitad / 4 : x++) {
        glk_window_fill_rect(gg_mapawin, MAPA_COLOR_DENTRO_FUERA, posx + mitad / 2 - x,
                             posy - x, 1, 1);
        glk_window_fill_rect(gg_mapawin, MAPA_COLOR_DENTRO_FUERA, posx + mitad / 2 - x,
                             posy + x, 1, 1);
      }
    }
    ck = ComprobarSalidaMapa(sitio, out_to);
    if (ck) {
      if (ck == 2 or 3) DibujarPuertaMapa(posx - mitad / 2 + mitad / 4, posy, ck);
      glk_window_fill_rect(gg_mapawin, MAPA_COLOR_DENTRO_FUERA, posx - mitad / 2,
                           posy, mitad, 1);
      for (x = 1 : x <= mitad / 4 : x++) {
        glk_window_fill_rect(gg_mapawin, MAPA_COLOR_DENTRO_FUERA, posx - mitad / 2 + x,
                             posy + x, 1, 1);
        glk_window_fill_rect(gg_mapawin, MAPA_COLOR_DENTRO_FUERA, posx - mitad / 2 + x,
                             posy - x, 1, 1);
      }
    }
  }
];

[ RefrescarMapa sitio cenx ceny
  o;
!  clearMainWindow();
  if (sitio provides sgw_img) drawImageSGW(gg_bigwin, sitio.sgw_img, POS_CENTRADO,
                                           BORDEWIN, BORDEWIN);
  glk_window_get_size(gg_mapawin, gg_arguments, gg_arguments + WORDSIZE);
  glk_window_fill_rect(gg_mapawin, MAPA_COLOR_BORDE, 0, 0, gg_arguments-->0, gg_arguments-->1);
  glk_window_fill_rect(gg_mapawin, $000000, 2, 2, gg_arguments-->0 - 4, gg_arguments-->1 - 4);
  lugaresDibujados = 0;
  DibujarMapa(sitio, cenx, ceny, 1);
  objectloop (o ofclass Lugar) o.dibujado = false;
  ImprimirBarraEstadoMapa(sitio);
];

[ ImprimirBarraEstadoMapa sitio;
  glk($002F, gg_statuswin); ! select
  glk($002A, gg_statuswin); ! clear
  ImprimirNombreSitioMapa(sitio);
  glk($002F, gg_objwin); ! select
  AyudaMapa(sitio);
  glk($002F, gg_mainwin);   ! select
  #ifdef IMPRIMIR_DESCRIPCION_MAPA;
  PrintOrRun(sitio, description);
  #endif;
];

[ ImprimirNombreSitioMapa sitio;
  glk($0086, style_SubHeader);
  glk($002B, gg_statuswin, 0, 0); ! locate
  print (name) sitio;
  glk($0086, style_Normal);
];

[ EsMapeable sitio;
  #ifdef DEBUG;
  return sitio ofclass Lugar;
  #ifnot;
  return sitio ofclass Lugar && sitio has visited;
  #endif;
];

[ ComprobarSalidaMapa sitio dir
  destino;
  if (sitio provides dir) {
    destino = sitio.dir;
    if (ZRegion(destino) == 3) return 4;
    if (ZRegion(destino) == 2) destino = destino();
    if (destino && destino has door) {
      if (destino has open) return 2;
      else                  return 3;
    }
    return EsMapeable(sitio.dir);
  }
  rfalse;
];

[ DestinoSalidaMapa sitio dir
  destino;
  if (sitio provides dir) {
    destino = sitio.dir;
    if (ZRegion(destino) == 2) destino = destino();
    if (destino has door) {
      if (sitio == destino.ladoa) destino = destino.ladob;
      else                        destino = destino.ladoa;
    }
    return destino;
  }
];

[ ValidarYRefrescarMapa sitio dir cenx ceny;
  if (ComprobarSalidaMapa(sitio, dir) == 1 or 2 or 3) {
    sitio = DestinoSalidaMapa(sitio, dir);
    RefrescarMapa(sitio, cenx, ceny);
  }
  return sitio;
];

[ AbrirVentanaMapa
  altura;
  #ifdef ControlTimer;
  ControlTimer.PausarTick();
  #endif;
!  openGraphicWindow(200);
  if (gg_menuwin) {
    glk_window_close(gg_menuwin, 0);
    gg_menuwin = 0;
  }
  glk_window_get_size(gg_statuswin, gg_arguments, gg_arguments + WORDSIZE);
  altura = gg_arguments-->1;
!  glk_window_close(gg_statuswin, 0);
  if (gg_bigwin) glk_window_close(gg_bigwin, 0);
  gg_bigwin = glk_window_open(gg_mainwin,
                              winmethod_Left + winmethod_Proportional +
                              winmethod_NoBorder,
                              25, wintype_Graphics, GG_BIGWIN_ROCK);
  if (gg_objwin) glk_window_close(gg_objwin, 0);
  gg_objwin = glk_window_open(gg_mainwin,
                              winmethod_Left + winmethod_Fixed +
                              winmethod_NoBorder,
                              20, wintype_TextGrid, GG_STATUSWIN_ROCK);
!  glk_window_clear(gg_statuswin);
  if (gg_mapawin) glk_window_close(gg_mapawin, 0);
  gg_mapawin = glk_window_open(gg_mainwin,
                               winmethod_Above + winmethod_Proportional +
                               winmethod_NoBorder,
                               100, wintype_Graphics, GG_MAPAWIN_ROCK);
  if (gg_mapawin == 0) return;
  glk_request_mouse_event(gg_mapawin);
!  glk_request_hyperlink_event(gg_objwin);
  glk_window_set_background_color(gg_mapawin, SCBACK);
  glk_window_clear(gg_mapawin);
!  StatusLineHeight(7);
!  AyudaMapa();
  return altura;
];

[ CerrarVentanaMapa altura;
  StatusLineHeight(altura);
  glk_window_close(gg_mapawin, 0);
  gg_mapawin = 0;
  closeAllWindows();
  clearMainWindow();
  EncenderGraficos();
  #ifdef ControlTimer;
  ControlTimer.ReanudarTick();
  #endif;
  <<Look>>;
];

Global num_link = -100;

[ s_link str;
  glk(256, num_link);
  print (s_input) str;
  glk(256, 0);
  num_link--;
];

[ AyudaMapa sitio
  fila sw;
  glk_set_window(gg_objwin);
  glk_window_clear(gg_objwin);
  num_link = -100;
  glk_window_get_cursor(gg_objwin, gg_arguments, gg_arguments + WORDSIZE);
  fila = gg_arguments-->1; ! Guardamos la fila actual
  fila = fila + 2;
  sw = false;
  if (ComprobarSalidaMapa(sitio, u_to)) {
    glk_window_move_cursor(gg_objwin, 1, fila++);
    print "- ", (s_link) "Arriba";
    sw = true;
  } else num_link--;
  if (ComprobarSalidaMapa(sitio, d_to)) {
    glk_window_move_cursor(gg_objwin, 1, fila++);
    print "- ", (s_link) "Abajo";
    sw = true;
  } else num_link--;
  if (ComprobarSalidaMapa(sitio, in_to)) {
    glk_window_move_cursor(gg_objwin, 1, fila++);
    print "- ", (s_link) "Dentro";
    sw = true;
  } else num_link--;
  if (ComprobarSalidaMapa(sitio, out_to)) {
    glk_window_move_cursor(gg_objwin, 1, fila++);
    print "- ", (s_link) "Fuera";
    sw = true;
  } else num_link--;

  if (sw) fila++;

  glk_window_move_cursor(gg_objwin, 1, fila++);
  print "- ", (s_link) "Zoom acercar";
  glk_window_move_cursor(gg_objwin, 1, fila++);
  print "- ", (s_link) "Zoom alejar";
  fila++;
  glk_window_move_cursor(gg_objwin, 1, fila++);
  print "- ", (s_link) "Cerrar mapa";

!  glk_window_get_size(gg_statuswin, gg_arguments, gg_arguments + WORDSIZE);
!  ancho = gg_arguments-->0;
!  ancho = (ancho / 6 + ancho / 5) / 2;
  
!  T Y U   7 8 9     ^      Arriba: 5 , Inicio   Dentro: * , Intro 
!  G   J   4   6   <   >     Abajo: 0 , Fin       Fuera: / , Retroceso
!  B N M   1 2 3     v     Acercar: Z , +        Volver: Q 
!                           Alejar: X , -
  
!  glk_window_move_cursor(gg_statuswin, ancho, 2); print (s_link) "T", " ", (s_link) "Y", " ", (s_link) "U", "   ", (s_link) "7", " ", (s_link) "8", " ", (s_link) "9", "     ", (s_link) "@@94", "     ", (s_link) "Arriba: 5 , Inicio", "   ", (s_link) "Dentro: * , Intro";
!  glk_window_move_cursor(gg_statuswin, ancho, 3); print (s_link) "G", "   ", (s_link) "J", "   ", (s_link) "4", "   ", (s_link) "6", "   ", (s_link) "<", "   ", (s_link) ">", "    ", (s_link) "Abajo: 0 , Fin", "       ", (s_link) "Fuera: / , Retroceso";
!  glk_window_move_cursor(gg_statuswin, ancho, 4); print (s_link) "B", " ", (s_link) "N", " ", (s_link) "M", "   ", (s_link) "1", " ", (s_link) "2", " ", (s_link) "3", "     ", (s_link) "v", "    ", (s_link) "Acercar: Z , +", "        ", (s_link) "Volver: Q";
!  glk_window_move_cursor(gg_statuswin, ancho, 5); print "                        ", (s_link) "Alejar: X , -";

!  print "^
!         ^ ", (s_input) "Cursor arriba", ", ", (s_input) "8", ", ", (s_input) "Y", ": ", (s_link) "Norte",
!           "       ", (s_input) "7", ", ", (s_input) "T", ": ", (s_link) "Noroeste",
!        "^ ", (s_input) "Cursor abajo", ", ", (s_input) "2", ", ", (s_input) "N", ": ", (s_link) "Sur",
!           "          ", (s_input) "9", ", ", (s_input) "U", ": ", (s_link) "Noreste",
!        "^ ", (s_input) "Cursor izquierda", ", ", (s_input) "4", ", ", (s_input) "G", ": ", (s_link) "Oeste",
!           "    ", (s_input) "1", ", ", (s_input) "B", ": ", (s_link) "Suroeste",
!        "^ ", (s_input) "Cursor derecha", ", ", (s_input) "6", ", ", (s_input) "J", ": ", (s_link) "Este",
!           "       ", (s_input) "3", ", ", (s_input) "M", ": ", (s_link) "Sureste",
!        "^ ", (s_input) "Inicio", ", ", (s_input) "5", ": ", (s_link) "Arriba",
!           "                ", (s_input) "Enter", ", ", (s_input) "*", ": ", (s_link) "Entrar",
!        "^ ", (s_input) "Fin", ", ", (s_input) "0", ": ", (s_link) "Abajo",
!           "                    ", (s_input) "Retroceso", ", ", (s_input) "/", ", ", (s_input) ".", ": ", (s_link) "Salir",
!        "^ ", (s_input) "Z", ", ", (s_input) "+", ": ", (s_link) "Acercar",
!           "                    ", (s_input) "Q", ": ", (s_link) "Volver al juego",
!        "^ ", (s_input) "X", ", ", (s_input) "-", ": ", (s_link) "Alejar";
];

[ MapaSub
  cenx ceny sitio tecla altura;
  altura = AbrirVentanaMapa();
  glk_window_get_size(gg_mapawin, gg_arguments, gg_arguments + WORDSIZE);
  cenx = (gg_arguments-->0) / 2; ! ancho / 2
  ceny = (gg_arguments-->1) / 2; ! alto / 2
  sitio = LugarReal();
  RefrescarMapa(sitio, cenx, ceny);
  while (true) {
    g_sitio = sitio;
!    tecla = KeyDelay();
    tecla = MapaTecla();
    if (g_sitio ~= sitio) {
      sitio = g_sitio;
      RefrescarMapa(sitio, cenx, ceny);
    }
    glk_window_get_size(gg_mapawin, gg_arguments, gg_arguments + WORDSIZE);
    cenx = (gg_arguments-->0) / 2; ! ancho / 2
    ceny = (gg_arguments-->1) / 2; ! alto / 2
    switch (tecla) {
      'q', 'Q', -106, -8:
        jump Salir;
      'z', 'Z', '+', -104:
        ladoCuadrado = ladoCuadrado + 20;
        RefrescarMapa(sitio, cenx, ceny);
      'x', 'X', '-', -105:
        if (ladoCuadrado > 21) {
          ladoCuadrado = ladoCuadrado - 20;
          RefrescarMapa(sitio, cenx, ceny);
        }
      -5, '2', 'n', 'N':
        sitio = ValidarYRefrescarMapa(sitio, s_to,   cenx, ceny);
      -4, '8', 'y', 'Y':
        sitio = ValidarYRefrescarMapa(sitio, n_to,   cenx, ceny);
      -2, '4', 'g', 'G':
        sitio = ValidarYRefrescarMapa(sitio, w_to,   cenx, ceny);
      -3, '6', 'j', 'J':
        sitio = ValidarYRefrescarMapa(sitio, e_to,   cenx, ceny);
      '7', 't', 'T':
        sitio = ValidarYRefrescarMapa(sitio, nw_to,  cenx, ceny);
      '9', 'u', 'U':
        sitio = ValidarYRefrescarMapa(sitio, ne_to,  cenx, ceny);
      '1', 'b', 'B':
        sitio = ValidarYRefrescarMapa(sitio, sw_to,  cenx, ceny);
      '3', 'm', 'M':
        sitio = ValidarYRefrescarMapa(sitio, se_to,  cenx, ceny);
      -12, 'a', 'A', '5', -100:
        sitio = ValidarYRefrescarMapa(sitio, u_to,   cenx, ceny); ! Inicio
      -13, 'z', 'Z', '0', -101:
        sitio = ValidarYRefrescarMapa(sitio, d_to,   cenx, ceny); ! Fin
      -6, '*', -102:
        sitio = ValidarYRefrescarMapa(sitio, in_to,  cenx, ceny); ! Enter
      -7, '.', '/', -103:
        sitio = ValidarYRefrescarMapa(sitio, out_to, cenx, ceny); ! Retroceso
!      'h', 'H':
!        AyudaMapa(sitio, cenx, ceny);
      #ifdef DEBUG;
      ' ': playerTo(sitio); jump Salir;
      #endif;
    }
  }
.Salir;
  CerrarVentanaMapa(altura);
  g_sitio = 0;
];

[ MapaTecla
  key done ix i;

  glk_request_hyperlink_event(gg_objwin);
  glk_request_mouse_event(gg_mapawin);
  glk_request_char_event(gg_mainwin);

  while (~~done) {
    glk_select(gg_event);
    ix = HandleGlkEvent(gg_event, 1, gg_arguments);
    if (ix == 2) {
      key = gg_arguments-->0;
      done = true;
    } else if (ix >= 0) {
      switch (gg_event-->0) {
        evtype_CharInput, evtype_Hyperlink:
          key = gg_event-->2;
          done = true;
        evtype_MouseInput:
          if (gg_event-->1 == gg_mapawin) {
            for (i = 0 : i < lugaresDibujados : i++) {
              if (gg_event-->2 >= lugaresMapaPosX-->i &&
                  gg_event-->2 <= lugaresMapaPosX-->i + ladoCuadrado &&
                  gg_event-->3 >= lugaresMapaPosY-->i &&
                  gg_event-->3 <= lugaresMapaPosY-->i + ladoCuadrado) {
                g_sitio = lugaresMapaLugar-->i;
                break;
              }
            }
          }
          key = 0; ! Devuelve la tecla 0, para no interferir
          done = true;
        evtype_Arrange, evtype_Redraw:
          DrawStatusLine();     ! Redibujamos la línea de estado
          glk_set_window(gg_statuswin);  ! Volvemos a la ventana anterior
          ! Cuando se redimensiona habiendo un menú de acciones, se
          ! devuelve la tecla 0, lo que hará que se redibuje la ventana:
          key = 0;
          done = true;
      }
    }
  }

  glk_cancel_char_event(gg_mainwin);
  glk_request_hyperlink_event(gg_objwin);
  glk_request_mouse_event(gg_mapawin);

  return key;
];

! Fin
