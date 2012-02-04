!
! Timer.h - Una librer�a para la gesti�n de eventos temporizados
!
! Copyright (c) 2012 Ricardo P�rez L�pez (Alpha Aventuras)
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


#ifdef HandleGlkEvent;
  Message "[Timer: Usando rutina HandleGlkEvent() proporcionada por el juego]";
  Message "[Timer: IMPORTANTE: NO OLVIDES LLAMAR EN ESA RUTINA A...]";
  Message "[Timer: ControlTimer.HandleGlk(ev, context, buffer) ]";
#endif; ! HandleGlkEvent


! Nuestra particular versi�n de HandleGlkEvent:
[ HandleGlkEvent ev context buffer;
  ControlTimer.HandleGlk(ev, context, buffer);
];


! Nuestra particular versi�n de EsperarTecla:
[ EsperarTecla s delay;
  return ControlTimer.CTEsperarTecla(s, delay);
];


! Nuestra particular versi�n de EsperarTiempo:
[ EsperarTiempo espera;
  return ControlTimer.CTEsperarTiempo(espera);
];


#ifndef VR;
  [ VR valor;
    if (ZRegion(valor) == 2)
      return valor();
    else
      return valor;
  ];
#endif;


Class GestorTimer
  with
    condicion true,          ! Si es false, no se ejecutar� el evento
    duracion  0,             ! N�mero de ticks necesarios para ejecutarse
    evento 0,                ! El evento a ejecutar
    AsignarGestor [ pos;     ! A�ade este gestor a la lista de gestores
      return ControlTimer.AsignarGestor(self, pos);
    ],
    EliminarGestor [;        ! Elimina este gestor de la lista de gestores
      return ControlTimer.EliminarGestor(self, 0);
    ],
    ActivarMutex [;          ! Activa el mutex sobre este gestor
      ControlTimer.ActivarMutex(self);
    ],
    PosicionDelGestor [;     ! En qu� posici�n est� dentro del array
      return ControlTimer.BuscarPosicion(self);
    ],
    SustituirGestor [ nuevo;
      return ControlTimer.SustituirGestor(self.PosicionDelGestor(), nuevo);
    ];


Object ControlTimer
  private
    gestores 0 0 0 0 0 0 0 0 0 0,         ! Array de gestores de eventos
    duracion_maxima 0,                    ! Duraci�n m�xima entre los gestores (en n� de ticks) 
    tick 0,                               ! Duraci�n del tick (en milisegundos)
    tick_pausado -1,                      ! Aqu� se guarda el tick cuando se pausa
    tick_pospuesto -1,                    ! Tick asignado durante un evento
    contador_ticks 1,                     ! El contador de ticks (va de 1 a duracion_maxima en ciclo)
    mutex 0,                              ! Sem�foro de exclusi�n mutua
    ha_imprimido_algo true,               ! Para saber si hay que restaurar la l�nea de �rdenes
    contexto_handle_glk false,            ! Estamos en un evento
    RestaurarLineaOrdenes [ buffer;       ! Restaura la l�nea de �rdenes
      if (banderafin == 1) {
        print "^^";
        M__L(##Miscelanea, 3);
        ActualizarEstatus();
        AfterGameOver();
      }
      M__L(##Prompt);
      buffer-->0 = gg_event-->2;
      glk($00D0, gg_mainwin, buffer + WORDSIZE,  ! glk_request_line_event
          INPUT_BUFFER_LEN - WORDSIZE, buffer-->0);
      return 1;
    ],
    RecalcularMaximo [ i max;             ! Vuelve a calcular la duraci�n m�xima
      max = -1;
      for (i = 0: i < self.#gestores / WORDSIZE: i++) {
        if (self.&gestores-->i > max) {
          max = self.&gestores-->i;
        }
      }
      self.duracion_maxima = max;
    ],
  with
    CTEsperarTiempo [ espera;
      glk($00D6, espera * 5);            ! request_timer_events
      while (1) {
        glk($00C0, gg_arguments);        ! glk_select(gg_arguments);
        if ((gg_arguments-->0) == 1) {
          glk($00D6, self.tick);
          break;
        }
      }
    ],
    CTEsperarTecla [ s delay;             ! Nuestra propia versi�n de EsperarTecla
      if (s) print (string) s;
      glk($00D6, delay * 5);              ! glk_request_timer_events
      glk($00D2, gg_mainwin);             ! glk_request_char_event(gg_mainwin);
      while (1) {
        glk($00C0, gg_arguments);         ! glk_select(gg_arguments);
        if ((gg_arguments-->0) == 2)      ! 2 = evType_CharInput
          break;
        if ((gg_arguments-->0) == 1) {
          glk($00D3, gg_mainwin);         ! glk_cancel_char_event
          glk($00D6, self.tick);          ! glk_request_timer_events
          return 0;
        }
      }
      glk($00D6, self.tick);
      return gg_arguments-->2;
    ],
    HandleGlk [ev context buffer i t;     ! Nuestra versi�n de HandleGlkEvent
      context = context;
      self.contexto_handle_glk = true;
      switch (ev-->0) {
        1: ! evtype_Timer == 1
          self.ReiniciarImpresion();
          for (i = 0: i < self.#gestores / WORDSIZE: i++) {
            t = self.&gestores-->i;
            ! Si hay gestor y su duraci�n es m�ltiplo del tick:
            if (t ~= 0 && self.contador_ticks % t.duracion == 0) {
              ! Si no hay mutex o lo tiene asignado el gestor t:
              if (self.mutex == 0 or t) {
                ! Si la condici�n se cumple:
                if (VR(t.condicion)) {
                  ! Si el evento retorna true:
                  if (t.evento ~= 0 && t.evento()) {
                    break;
                  }
                }
              }
            }
          }
          ! Contador c�clico entre 1 y duracion_maxima:
          if (self.contador_ticks >= self.duracion_maxima) {
            self.contador_ticks = 1;
          } else {
            self.contador_ticks++;
          }
          self.contexto_handle_glk = false;
          if (self.tick_pospuesto ~= -1) {
            self.ActivarTick(self.tick_pospuesto);
            self.tick_pospuesto = -1;
          }
          ! Si se ha imprimido algo en alg�n gestor, restauramos:
          if (self.ha_imprimido_algo) {
            return self.RestaurarLineaOrdenes(buffer);
          } else {
            return 1;
          }
      }
    ],
    BuscarPosicion [ g i pos;             ! Da la posici�n de un gestor en el array
      pos = -1;
      for (i = 0: i < self.#gestores / WORDSIZE: i++) {
        if (self.&gestores-->i == g) {
          pos = i;
          break;
        }
      }
      return pos;                         ! Devuelve -1 si no se encuentra
    ],
    SustituirGestor [ viejo nuevo pos;    ! Cambia un gestor por otro
      pos = self.BuscarPosicion(viejo);
      if (pos ~= -1) {
        self.AsignarGestor(nuevo, pos);
      }
    ],
    AsignarGestor [ g pos;                ! Asigna un gestor a una posici�n del array
      if (pos ~= 0) {
        if (pos < 0 || pos >= self.#gestores / WORDSIZE) {
          #ifdef DEBUG;
            print "ERROR: La posici�n para el gestor de timer sobrepasa los l�mites.^";
          #endif;
          return -1;
        }
      } else {
        ! Si no se indica posici�n, buscamos el siguiente hueco vac�o:
        pos = self.BuscarPosicion(0);
        if (pos < 0) {
          #ifdef DEBUG;
            print "ERROR: Superado n�mero m�ximo de gestores de timer.^";
          #endif;
          return -1;
        }
      }
      self.&gestores-->pos = g;
      if (g.duracion > self.duracion_maxima) {
        self.duracion_maxima = g.duracion;
      }
      return pos;
    ],
    EliminarGestor [ g pos d;             ! Elimina un gestor dados �l o su posici�n en el array
      if (g ~= 0) {
        pos = self.BuscarPosicion(g);
        if (pos == -1) {
          rfalse;
        }
      } else if (pos < 0 || pos >= self.#gestores / WORDSIZE) {
        rfalse;
      }
      d = (self.&gestores-->pos).duracion;
      self.&gestores-->pos = 0;
      if (d == self.duracion_maxima) {
        self.RecalcularMaximo();
      }
      return pos;
    ],
    PrepararImpresion [;                  ! Llamada por los gestores antes de imprimir algo
      if (~~(self.ha_imprimido_algo)) {
        self.ha_imprimido_algo = true;
        glk($00D1, gg_mainwin, gg_event); ! glk_cancel_line_event(gg_mainwin, gg_event);
      }
    ],
    ReiniciarImpresion [;                 ! Reinicia el indicador de 'imprimido algo'
      self.ha_imprimido_algo = false;
    ],
    AsignarTick [ t;                      ! Asigna la duraci�n del tick
      ! Durante la ejecuci�n de un evento, no se cambia el tick,
      ! sino que se retrasa el cambio hasta terminar con el ciclo actual:
      if (self.contexto_handle_glk) {
        self.tick_pospuesto = t;
      } else {
        self.tick = t;
      }
    ],
    ActivarTick [ t;                      ! Activa el timer (opcionalmente, asignando el tick antes)
      if (t ~= 0) {
        self.AsignarTick(t);
      } else {
        t = self.tick;
      }
      if (~~(self.contexto_handle_glk)) {
        glk($00D6, t);                    ! glk_request_timer_events
      }
    ],
    PausarTick [;                         ! Detiene el timer temporalmente
      self.tick_pausado = self.tick;
      self.DesactivarTick();
    ],
    ReanudarTick [;                       ! Reanuda el timer detenido
      self.ActivarTick(self.tick_pausado);
      self.tick_pausado = 0;
    ],
    DesactivarTick [;                     ! Desactiva el timer
      self.AsignarTick(0);
      self.ActivarTick();
    ],
    ReactivarTick [ t;                    ! Reactiva el timer (�til en algunos casos)
      t = self.tick;
      glk($00D6, t);                      ! glk_request_timer_events
    ],
    ActivarMutex [ g;                     ! Activa el mutex sobre un gestor
      self.mutex = g;
    ],
    DesactivarMutex [;                    ! Desactiva el mutex si lo hubiera
      self.mutex = 0;
    ],
    Reiniciar [ i;                        ! Pone todas las propiedades a sus valores por defecto
      self.DesactivarTick();
      self.tick = 0;
      self.tick_pausado = -1;
      self.tick_pospuesto = -1;
      self.duracion_maxima = 0;
      self.contador_ticks = 1;
      self.mutex = 0;
      self.ha_imprimido_algo = true;
      for (i = 0: i < self.#gestores / WORDSIZE: i++) {
        self.&gestores-->i = 0;
      }
    ];

