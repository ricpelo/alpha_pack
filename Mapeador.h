! Mapeador.h

System_file;

Global gg_mapa_win;
Global ladoCuadrado = 41;

Constant COLOR_LOCAL_MAP  = $ffffff;
Constant COLOR_CURSOR_MAP = $00ff00;
Constant COLOR_ACTUAL_MAP = $aaaaaa;
Constant COLOR_PUERTA_MAP = $ff0000;
Constant COLOR_UPDOWN_MAP = $0000ff;

Verb meta 'mapa'
  *                 -> Mapa;

[ DibujaPuerta cenx ceny;
  glk_window_fill_rect(gg_mapa_win, COLOR_PUERTA_MAP,
                       cenx - 5, ceny - 5, 10, 10);
];

[ DibujarMapa sitio posx posy central
  color sep mitad x y ck;
  if (~~sitio.dibujado) {
    sitio.dibujado = true;
    if (sitio == location) color = COLOR_ACTUAL_MAP;
    else if (central == 1) color = COLOR_CURSOR_MAP;
    else                   color = COLOR_LOCAL_MAP;
    mitad = ladoCuadrado / 2;
    glk_window_fill_rect(gg_mapa_win, color, posx - mitad, posy - mitad,
                                             ladoCuadrado, ladoCuadrado);
    sep = ladoCuadrado + mitad;
    ck = ComprobarSalida(sitio, e_to); 
    if (ck) {
      if (ck == 2) DibujaPuerta(posx + mitad, posy);
      else {
        glk_window_fill_rect(gg_mapa_win, $ffffff, posx + mitad, posy,
                                                   sep - ladoCuadrado + 1, 1);
        DibujarMapa(sitio.e_to, posx + sep, posy, 0);
      }
    }
    ck = ComprobarSalida(sitio, w_to); 
    if (ck) {
      if (ck == 2) DibujaPuerta(posx - mitad, posy);
      else {
        glk_window_fill_rect(gg_mapa_win, $ffffff, posx - sep + mitad, posy,
                                                 sep - ladoCuadrado + 1, 1);
        DibujarMapa(sitio.w_to, posx - sep, posy, 0);
      }
    }
    ck = ComprobarSalida(sitio, n_to); 
    if (ck) {
      if (ck == 2) DibujaPuerta(posx, posy - mitad);
      else {
        glk_window_fill_rect(gg_mapa_win, $ffffff, posx, posy - sep + mitad, 1,
                                                   sep - ladoCuadrado + 1);
        DibujarMapa(sitio.n_to, posx, posy - sep, 0);
      }
    }
    ck = ComprobarSalida(sitio, s_to);
    if (ck) {
      if (ck == 2) DibujaPuerta(posx, posy + mitad);
      else {
        glk_window_fill_rect(gg_mapa_win, $ffffff, posx, posy + mitad, 1,
                                                   sep - ladoCuadrado + 1);
        DibujarMapa(sitio.s_to, posx, posy + sep, 0);
      }
    }
    ck = ComprobarSalida(sitio, nw_to);
    if (ck) {
      if (ck == 2) DibujaPuerta(posx - mitad, posy - mitad);
      else {
        for (x = posx - sep + mitad, y = posy - sep + mitad : x <= posx - mitad : x++, y++) {
          glk_window_fill_rect(gg_mapa_win, $ffffff, x, y, 1, 1);
        }
        DibujarMapa(sitio.nw_to, posx - sep, posy - sep, 0);
      }
    }
    ck = ComprobarSalida(sitio, ne_to);
    if (ck) {
      if (ck == 2) DibujaPuerta(posx + mitad, posy - mitad);
      else {
        for (x = posx + mitad, y = posy - mitad : x <= posx + sep - mitad : x++, y--) {
          glk_window_fill_rect(gg_mapa_win, $ffffff, x, y, 1, 1);
        }
        DibujarMapa(sitio.ne_to, posx + sep, posy - sep, 0);
      }
    }
    ck = ComprobarSalida(sitio, sw_to);
    if (ck) {
      if (ck == 2) DibujaPuerta(posx - mitad, posy + mitad);
      else {
        for (x = posx - sep + mitad, y = posy + sep - mitad : x <= posx - mitad : x++, y--) {
          glk_window_fill_rect(gg_mapa_win, $ffffff, x, y, 1, 1);
        }
        DibujarMapa(sitio.sw_to, posx - sep, posy + sep, 0);
      }
    }
    ck = ComprobarSalida(sitio, se_to);
    if (ck) {
      if (ck == 2) DibujaPuerta(posx + mitad, posy + mitad);
      else {
        for (x = posx + mitad, y = posy + mitad : x <= posx + sep - mitad : x++, y++) {
          glk_window_fill_rect(gg_mapa_win, $ffffff, x, y, 1, 1);
        }
        DibujarMapa(sitio.se_to, posx + sep, posy + sep, 0);
      }
    }
    ck = ComprobarSalida(sitio, u_to);
    if (ck == 1) {
      glk_window_fill_rect(gg_mapa_win, COLOR_UPDOWN_MAP, posx + mitad / 2,
                                                          posy - mitad / 2,
                                                          1,
                                                          mitad);
      for (x = 1 : x <= mitad / 4 : x++) {
        glk_window_fill_rect(gg_mapa_win, COLOR_UPDOWN_MAP, posx + mitad / 2 + x,
                                                            posy - mitad / 2 + x,
                                                            1, 1);
        glk_window_fill_rect(gg_mapa_win, COLOR_UPDOWN_MAP, posx + mitad / 2 - x,
                                                            posy - mitad / 2 + x,
                                                            1, 1);
      }
    }
    ck = ComprobarSalida(sitio, d_to);
    if (ck == 1) {
      glk_window_fill_rect(gg_mapa_win, COLOR_UPDOWN_MAP, posx + mitad / 2,
                                                          posy - mitad / 2,
                                                          1, mitad);
      for (x = 1 : x <= mitad / 4 : x++) {
        glk_window_fill_rect(gg_mapa_win, COLOR_UPDOWN_MAP, posx + mitad / 2 + x,
                                                            posy + mitad / 2 - x,
                                                            1, 1);
        glk_window_fill_rect(gg_mapa_win, COLOR_UPDOWN_MAP, posx + mitad / 2 - x,
                                                            posy + mitad / 2 - x,
                                                            1, 1);
      }
    }
    ck = ComprobarSalida(sitio, in_to);
    posx = posx - mitad / 3;
    if (ck) {
      glk_window_fill_rect(gg_mapa_win, COLOR_UPDOWN_MAP, posx - mitad / 2,
                                                          posy,
                                                          mitad, 1);
      for (x = 1 : x <= mitad / 4 : x++) {
        glk_window_fill_rect(gg_mapa_win, COLOR_UPDOWN_MAP, posx + mitad / 2 - x,
                                                            posy - x,
                                                            1, 1);
        glk_window_fill_rect(gg_mapa_win, COLOR_UPDOWN_MAP, posx + mitad / 2 - x,
                                                            posy + x,
                                                            1, 1);
      }
    }
    ck = ComprobarSalida(sitio, out_to);
    if (ck) {
      glk_window_fill_rect(gg_mapa_win, COLOR_UPDOWN_MAP, posx - mitad / 2,
                                                          posy,
                                                          mitad, 1);
      for (x = 1 : x <= mitad / 4 : x++) {
        glk_window_fill_rect(gg_mapa_win, COLOR_UPDOWN_MAP, posx - mitad / 2 + x,
                                                            posy + x,
                                                            1, 1);
        glk_window_fill_rect(gg_mapa_win, COLOR_UPDOWN_MAP, posx - mitad / 2 + x,
                                                            posy - x,
                                                            1, 1);
      }
    }    
  }
];

[ RefrescarMapa sitio cenx ceny
  o;
!  clearMainWindow();
  clearGraphicWindow();
  DibujarMapa(sitio, cenx, ceny, 1);
  objectloop (o ofclass Lugar) o.dibujado = false;
!  print (name) sitio;
];

[ EsMapeable sitio;
  return sitio ofclass Lugar && sitio has visited;
];

[ ComprobarSalida sitio dir
  destino;
  if (sitio provides dir) {
    destino = sitio.dir;
    if (ZRegion(destino) == 3) return 3;
    if (ZRegion(destino) == 2) destino = destino();
    if (destino && destino has door) return 2;
    return EsMapeable(sitio.dir);
  }
  rfalse;
];

[ DestinoSalida sitio dir
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

[ ValidarYRefrescar sitio dir cenx ceny;
  if (ComprobarSalida(sitio, dir) == 1 or 2) {
    sitio = DestinoSalida(sitio, dir);
!    sitio = sitio.dir;
    RefrescarMapa(sitio, cenx, ceny);
  }
  return sitio;
];

[ MapaSub
  ancho alto cenx ceny sitio tecla;
  #ifdef ControlTimer;
  ControlTimer.PausarTick();
  #endif;
  openGraphicWindow(700);
  gg_mapa_win = gg_bigwin;
  glk_window_get_size(gg_mapa_win, gg_arguments, gg_arguments + WORDSIZE);
  ancho = (gg_arguments-->0); ! -(px*2); ! ancho vent. (RESTA pixels_borde*2)
  alto  = (gg_arguments-->1); ! -(py*2); ! alto vent. (RESTA pixels_borde*2)
  cenx = ancho / 2;
  ceny = alto / 2;
  sitio = location;
  RefrescarMapa(sitio, cenx, ceny);
  while (true) {
    tecla = KeyDelay();
    switch (tecla) {
      'q', 'Q': jump Salir;
      'z', 'Z': ladoCuadrado = ladoCuadrado + 20;
                RefrescarMapa(sitio, cenx, ceny);
      'x', 'X': if (ladoCuadrado > 21) {
                  ladoCuadrado = ladoCuadrado - 20;
                  RefrescarMapa(sitio, cenx, ceny);
                }
      -5, '2': sitio = ValidarYRefrescar(sitio, s_to,  cenx, ceny);
      -4, '8': sitio = ValidarYRefrescar(sitio, n_to,  cenx, ceny);
      -2, '4': sitio = ValidarYRefrescar(sitio, w_to,  cenx, ceny);
      -3, '6': sitio = ValidarYRefrescar(sitio, e_to,  cenx, ceny);
      '7':     sitio = ValidarYRefrescar(sitio, nw_to, cenx, ceny);
      '9':     sitio = ValidarYRefrescar(sitio, ne_to, cenx, ceny);
      '1':     sitio = ValidarYRefrescar(sitio, sw_to, cenx, ceny);
      '3':     sitio = ValidarYRefrescar(sitio, se_to, cenx, ceny);
      '-':     sitio = ValidarYRefrescar(sitio, u_to, cenx, ceny);
      '+':     sitio = ValidarYRefrescar(sitio, d_to, cenx, ceny);
      '*':     sitio = ValidarYRefrescar(sitio, in_to, cenx, ceny);
      '/':     sitio = ValidarYRefrescar(sitio, out_to, cenx, ceny);
    }
  }
.Salir;
  clearMainWindow();
  EncenderGraficosSub(true);
  #ifdef ControlTimer;
  ControlTimer.ReanudarTick();
  #endif;
  <<Look>>;
];

