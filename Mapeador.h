! Mapeador.h

Global gg_mapa_win;
Global ladoCuadrado = 41;

Constant COLOR_LOCAL_MAP  = $ffffff;
Constant COLOR_CURSOR_MAP = $00ff00;
Constant COLOR_ACTUAL_MAP = $aaaaaa;

Verb meta 'mapea'
  *                 -> Mapa;

[ DibujarMapa sitio posx posy central color sep mitad x y;
  if (~~sitio.dibujado) {
    sitio.dibujado = true;
    if (sitio == location) color = COLOR_ACTUAL_MAP;
    else if (central == 1) color = COLOR_CURSOR_MAP;
    else                   color = COLOR_LOCAL_MAP;
    mitad = ladoCuadrado / 2;
    glk_window_fill_rect(gg_mapa_win, color, posx - mitad, posy - mitad,
                                             ladoCuadrado, ladoCuadrado);
    sep = ladoCuadrado + mitad;
    if (sitio provides e_to && EsMapeable(sitio.e_to)) {
      glk_window_fill_rect(gg_mapa_win, $ffffff, posx + mitad, posy,
                                                 sep - ladoCuadrado + 1, 1);
      DibujarMapa(sitio.e_to, posx + sep, posy, 0);
    }
    if (sitio provides w_to && EsMapeable(sitio.w_to)) {
      glk_window_fill_rect(gg_mapa_win, $ffffff, posx - sep + mitad, posy,
                                                 sep - ladoCuadrado + 1, 1);
      DibujarMapa(sitio.w_to, posx - sep, posy, 0);
    }
    if (sitio provides n_to && EsMapeable(sitio.n_to)) {
      glk_window_fill_rect(gg_mapa_win, $ffffff, posx, posy - sep + mitad, 1,
                                                 sep - ladoCuadrado + 1);
      DibujarMapa(sitio.n_to, posx, posy - sep, 0);
    }
    if (sitio provides s_to && EsMapeable(sitio.s_to)) {
      glk_window_fill_rect(gg_mapa_win, $ffffff, posx, posy + mitad, 1,
                                                 sep - ladoCuadrado + 1);
      DibujarMapa(sitio.s_to, posx, posy + sep, 0);
    }
    if (sitio provides nw_to && EsMapeable(sitio.nw_to)) {
      for (x = posx - sep + mitad, y = posy - sep + mitad : x <= posx - mitad : x++, y++) {
        glk_window_fill_rect(gg_mapa_win, $ffffff, x, y, 1, 1);
      }
      DibujarMapa(sitio.nw_to, posx - sep, posy - sep, 0);
    }
    if (sitio provides ne_to && EsMapeable(sitio.ne_to)) {
      for (x = posx + mitad, y = posy - mitad : x <= posx + sep - mitad : x++, y--) {
        glk_window_fill_rect(gg_mapa_win, $ffffff, x, y, 1, 1);
      }
      DibujarMapa(sitio.ne_to, posx + sep, posy - sep, 0);
    }
    if (sitio provides sw_to && EsMapeable(sitio.sw_to)) {
      for (x = posx - sep + mitad, y = posy + sep - mitad : x <= posx - mitad : x++, y--) {
        glk_window_fill_rect(gg_mapa_win, $ffffff, x, y, 1, 1);
      }
      DibujarMapa(sitio.sw_to, posx - sep, posy + sep, 0);
    }
    if (sitio provides se_to && EsMapeable(sitio.se_to)) {
      for (x = posx + mitad, y = posy + mitad : x <= posx + sep - mitad : x++, y++) {
        glk_window_fill_rect(gg_mapa_win, $ffffff, x, y, 1, 1);
      }
      DibujarMapa(sitio.se_to, posx + sep, posy + sep, 0);
    }
  }
];

[ RefrescarMapa sitio cenx ceny o;
  clearMainWindow();
  DibujarMapa(sitio, cenx, ceny, 1);
  objectloop (o ofclass Lugar) o.dibujado = false;
  print (name) sitio;
];

[ EsMapeable sitio;
  return sitio ofclass Lugar && sitio has visited;
];

[ MapaSub ancho alto cenx ceny sitio tecla;
!  ControlTimer.PausarTimer();
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
      -5, '2': if (sitio provides s_to && EsMapeable(sitio.s_to)) {
            sitio = sitio.s_to;
            RefrescarMapa(sitio, cenx, ceny);
          }
      -4, '8': if (sitio provides n_to && EsMapeable(sitio.n_to)) {
           sitio = sitio.n_to;
           RefrescarMapa(sitio, cenx, ceny);
          }
      -2, '4': if (sitio provides w_to && EsMapeable(sitio.w_to)) {
            sitio = sitio.w_to;
            RefrescarMapa(sitio, cenx, ceny);
          }
      -3, '6': if (sitio provides e_to && EsMapeable(sitio.e_to)) {
            sitio = sitio.e_to;
            RefrescarMapa(sitio, cenx, ceny);
          }
    }
  }
.Salir;
  clearMainWindow();
  EncenderGraficosSub(true);
!  ControlTimer.ReanudarTimer();
  <<Look>>;
];

