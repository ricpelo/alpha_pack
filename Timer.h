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


Message " _________________________________________________________________";
Message "|                                                                 |";
Message "|                  * TIMER:  I M P O R T A N T E *                |";
Message "|                  ===============================                |";
Message "| 1. Pon 'Replace KeyDelay;' justo antes de 'Include ~Parser.h~;' |";
Message "| 2. En tu rutina HandleGlkEvent() (creala si no la tienes ya),   |";
Message "|    llama a ControlTimer.CT_HandleGlkEvent(ev, context, buffer)  |";
Message "|_________________________________________________________________|";


!! Nuestra particular versi�n de HandleGlkEvent:
![ HandleGlkEvent ev context buffer;
!  return ControlTimer.CT_HandleGlkEvent(ev, context, buffer);
!];


! Nuestra particular versi�n de WaitDelay:
#ifndef WaitDelay;
[ WaitDelay delay;
  return ControlTimer.CT_WaitDelay(delay);
];
#endif;


! Nuestra particular versi�n de KeyDelay:
[ KeyDelay delay;
  return ControlTimer.CT_KeyDelay(delay);
];


Class GestorTimer
  with
    condicion true,             ! Si es false, no se ejecutar� el evento
    activado true,              ! Si es false, el gestor est� desactivado
    duracion 0,                 ! N�mero de ticks necesarios para ejecutarse
    compensacion 0,             ! Ticks que se suman en el primer ciclo
    evento 0,                   ! El evento a ejecutar
    ActivarGestor [;            ! Activa el gestor
      self.activado = true;
    ],
    DesactivarGestor [;         ! Desactiva el gestor
      self.activado = false;
    ],
    AgregarGestor [;            ! Agrega este gestor en un hueco de la lista
      return ControlTimer.AgregarGestor(self);
    ],
    AgregarGestorAlFinal [;
      return ControlTimer.AgregarGestorAlFinal(self);
    ],
    InsertarGestor [ pos;       ! Inserta este gestor empujando los dem�s
      return ControlTimer.InsertarGestor(self, pos);
    ],
    AsignarGestor [ pos;        ! Coloca este gestor en una posici�n de la lista
      return ControlTimer.AsignarGestor(self, pos);
    ],
    EliminarGestor [;           ! Elimina este gestor de la lista de gestores
      return ControlTimer.EliminarGestor(self, 0);
    ],
    ActivarMutex [;             ! Activa el mutex sobre este gestor
      ControlTimer.ActivarMutex(self);
    ],
    PosicionDelGestor [;        ! En qu� posici�n est� dentro del array
      return ControlTimer.BuscarPosicion(self);
    ],
    SustituirGestor [ nuevo;    ! Sustituye este gestor por el nuevo
      return ControlTimer.SustituirGestor(self, nuevo);
    ],
    IntercambiarConGestor [ g;  ! Intercambia este gestor con otro
      return ControlTimer.IntercambiarGestores(self, g);
    ];


Object ControlTimer
  private
    gestores 0 0 0 0 0 0 0 0 0 0          ! Array de gestores de eventos
             0 0 0 0 0 0 0 0 0 0,
    duracion_maxima 0,                    ! Duraci�n m�xima entre los gestores (en n� de ticks) 
    tick 0,                               ! Duraci�n del tick (en milisegundos)
    tick_pausado -1,                      ! Aqu� se guarda el tick cuando se pausa
    tick_pospuesto -1,                    ! Tick asignado durante un evento
    condicion true,                       ! �Se deben ejecutar los timers?
    contador_ticks 1,                     ! El contador de ticks (va de 1 a duracion_maxima en ciclo)
    mutex 0,                              ! Sem�foro de exclusi�n mutua
    ha_imprimido_algo true,               ! Para saber si hay que restaurar la l�nea de �rdenes
    contexto_handle_glk false,            ! Estamos en un evento
    longitud 0,                           ! Longitud del buffer a restaurar
    MiGlkRequestTimerEvents [ t;          ! Establece timer y reinicia contador
      glk($00D6, t);                      ! glk_request_timer_events
      self.contador_ticks = 1;
    ],
    RestaurarLineaOrdenes [ buffer        ! Restaura la l�nea de �rdenes
      tick_anterior;
      if (deadflag ~= 0) {
        tick_anterior = self.tick;
        self.DesactivarTick();
        ! Todo el cuerpo del 'if' desde aqu� est� copiado de la librer�a:
        if (deadflag ~= 2) AfterLife();
        if (deadflag == 0) {
          self.ActivarTick(tick_anterior);
          return 1;
        }
        print "^^    ";
        #Ifdef TARGET_ZCODE;
        #IfV5; style bold; #Endif; ! V5
        #Ifnot; ! TARGET_GLULX
        glk($0086, 5); ! set alert style
        #Endif; ! TARGET_
        print "***";
        if (deadflag == 1) L__M(##Miscellany, 3);
        if (deadflag == 2) L__M(##Miscellany, 4);
        if (deadflag > 2)  {
          print " ";
          DeathMessage();
          print " ";
        }
        print "***";
        #Ifdef TARGET_ZCODE;
        #IfV5; style roman; #Endif; ! V5
        #Ifnot; ! TARGET_GLULX
        glk($0086, 0); ! set normal style
        #Endif; ! TARGET_
        #Ifdef NO_SCORE;
        print "^^";
        #Ifnot;
        print "^^^";
        #Endif; ! NO_SCORE
        ScoreSub();
        DisplayStatus();
        AfterGameOver();
      }
      L__M(##Prompt);
      buffer-->0 = self.longitud;
      self.longitud = 0;
      glk($00D0, gg_mainwin, buffer + WORDSIZE,  ! glk_request_line_event
          INPUT_BUFFER_LEN - WORDSIZE, buffer-->0);
      return 1;
    ],
    RecalcularMaximo [                    ! Vuelve a calcular la duraci�n m�xima
      i max d;
      max = -1;
      for (i = 0: i < self.#gestores / WORDSIZE: i++) {
        if (self.&gestores-->i ~= 0) {
          d = (self.&gestores-->i).duracion;
          if (d > max) max = d;
        }
      }
      self.duracion_maxima = max;
    ],
    AsignarTick [ t;                      ! Asigna la duraci�n del tick
      ! Durante la ejecuci�n de un evento, no se cambia el tick,
      ! sino que se retrasa el cambio hasta terminar con el ciclo actual:
      if (self.contexto_handle_glk) self.tick_pospuesto = t;
      else                          self.tick = t;
    ],
  with
    numero_ticks [;                       ! Devuelve el valor de contador_ticks
      return self.contador_ticks;
    ],
    DentroDeEvento [;                     ! Si se est� ejecutando un evento desde el timer
      return self.contexto_handle_glk;
    ],
    ! Nuestra propia versi�n de WaitDelay:
    CT_WaitDelay [ delay;
      glk($00D6, delay);                  ! request_timer_events
      while (1) {
        glk($00C0, gg_arguments);         ! glk_select
        if ((gg_arguments-->0) == 1) {    ! evtype_Timer
          glk($00D6, self.tick);
          break;
        }
      }
    ],
    ! Nuestra propia versi�n de KeyDelay:
    CT_KeyDelay [ delay
      key done ix;
      glk($00D2, gg_mainwin); ! request_char_event
      glk($00D6, delay);      ! request_timer_events
      while (~~done) {
        glk($00C0, gg_event); ! select
        ix = HandleGlkEvent(gg_event, 1, gg_arguments);
        if (ix == 2) {
          key = gg_arguments-->0;
          done = true;
        } else if (ix >= 0 && gg_event-->0 == 1 or 2) { ! Timer or CharInput
          key = gg_event-->2;
          done = true;
        }
      }
      glk($00D3, gg_mainwin); ! cancel_char_event
      glk($00D6, self.tick);  ! request_timer_events
      return key;
    ],
    ! Nuestra versi�n de HandleGlkEvent:
    CT_HandleGlkEvent [ ev context buffer
      i t re d;
      switch (ev-->0) {
        1: ! evtype_Timer == 1
           if (context == 1) return 1;  ! character input request
           self.contexto_handle_glk = true;
           if (self.condicion) {  ! Ver PausarTimers() y ReanudarTimers()
             self.ReiniciarImpresion();
             for (i = 0: i < self.#gestores / WORDSIZE: i++) {
               t = self.&gestores-->i;
               ! Si hay gestor:
               if (t ~= 0) {
                 d = t.duracion + t.compensacion;
                 if (t.compensacion ~= 0) re = true;
                 ! Si su duraci�n es m�ltiplo del tick:
                 if (self.contador_ticks % d == 0) {
                   ! Si el gestor est� activado:
                   if (ValueOrRun(t, activado)) {
                     ! Si no hay mutex o lo tiene asignado el gestor t:
                     if (self.mutex == 0 or t) {
                       ! Si la condici�n del gestor se cumple:
                       if (ValueOrRun(t, condicion)) {
                         ! Si el evento retorna true:
                         if (t.evento ~= 0 && t.evento()) {
                           t.compensacion = 0;
                           break;
                         }
                       }
                     }
                   }
                 }
               }
             }
           }
           ! Contador c�clico entre 1 y duracion_maxima:
           if (self.contador_ticks >= self.duracion_maxima) {
             self.contador_ticks = 1;
             if (re) self.RecalcularMaximo();
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
    BuscarPosicion [ g                    ! Da la posici�n de un gestor en el array
      i pos;
      pos = -1;
      for (i = 0: i < self.#gestores / WORDSIZE: i++) {
        if (self.&gestores-->i == g) {
          pos = i;
          break;
        }
      }
      return pos;                         ! Devuelve -1 si no se encuentra
    ],
    SustituirGestor [ viejo nuevo         ! Cambia un gestor por otro
      pos;
      pos = self.BuscarPosicion(viejo);
      if (pos ~= -1) return self.AsignarGestor(nuevo, pos);
      return -1;
    ],
    IntercambiarGestores [ g1 g2          ! Intercambia la posici�n de dos gestores
      pos1 pos2;
      pos1 = self.BuscarPosicion(g1);
      pos2 = self.BuscarPosicion(g2);
      if (pos1 == -1 || pos2 == -1) return -1;
      self.&gestores-->pos1 = g2;
      self.&gestores-->pos2 = g1;
      rtrue;
    ],
    AgregarGestor [ g                     ! Agrega un nuevo gestor en un hueco libre
      pos;
      pos = self.BuscarPosicion(0);
      if (pos < -1) {
        #ifdef DEBUG;
          print "ERROR: Superado n�mero m�ximo de gestores de timer.^";
        #endif;
        return -1;
      }
      return self.AsignarGestor(g, pos);
    ],
    AgregarGestorAlFinal [ g
      max i pos;
      max = self.#gestores / WORDSIZE - 1;
      for (i = max: i >= 0: i--) {
        if (self.&gestores-->i ~= 0) break; 
      }
      pos = i + 1;
      if (pos >= 0 && pos <= max && self.&gestores-->pos == 0) {
        self.AsignarGestor(g, pos);
      } else {
        #ifdef DEBUG;
          print "ERROR: Superado n�mero m�ximo de gestores de timer.^";
        #endif;
        return -1;
      }
    ],
    InsertarGestor [ g pos                ! Inserta un gestor en una posici�n, empujando
      i;
      if (pos < 0 || pos >= self.#gestores / WORDSIZE) {
        #ifdef DEBUG;
          print "ERROR: La posici�n para el gestor de timer sobrepasa los l�mites.^";
        #endif;
        return -1;
      }
      for (i = self.#gestores / WORDSIZE - 2: i >= pos: i--) {
        self.&gestores-->(i + 1) = self.&gestores-->i;
      }
      self.AsignarGestor(g, pos);
    ],
    AsignarGestor [ g pos                 ! Asigna un gestor a una posici�n del array
      d;
      if (pos < 0 || pos >= self.#gestores / WORDSIZE) {
        #ifdef DEBUG;
          print "ERROR: La posici�n para el gestor de timer sobrepasa los l�mites.^";
        #endif;
        return -1;
      }
      self.&gestores-->pos = g;
      g.compensacion = self.contador_ticks - 1;
      d = g.duracion + g.compensacion;
      if (d > self.duracion_maxima) {
        self.duracion_maxima = d;
      }
      return pos;
    ],
    EliminarGestor [ g pos                ! Elimina un gestor dados �l o su posici�n en el array
      d i;
      if (g ~= 0) {
        pos = self.BuscarPosicion(g);
        if (pos == -1) rfalse;
      } else if (pos < 0 || pos >= self.#gestores / WORDSIZE) {
        rfalse;
      }
      d = (self.&gestores-->pos).duracion;
      for (i = pos: i < self.#gestores / WORDSIZE - 1: i++) {
        self.&gestores-->i = self.&gestores-->(i + 1);
      }
      self.&gestores-->i = 0;
      if (d == self.duracion_maxima) self.RecalcularMaximo();
      return pos;
    ],
    PrepararImpresion [;                  ! Llamada por los gestores antes de imprimir algo
      if (~~(self.ha_imprimido_algo)) {
        self.ha_imprimido_algo = true;
        glk($00D1, gg_mainwin, gg_event); ! glk_cancel_line_event(gg_mainwin, gg_event);
        self.longitud = gg_event-->2;
        ! Esta l�nea s�lo debe ejecutarse cuando se use
        ! el int�rprete de Gargoyle modificado por m�:
        if (self.contexto_handle_glk) new_line;
      }
    ],
    ReiniciarImpresion [;                 ! Reinicia el indicador de 'imprimido algo'
      self.ha_imprimido_algo = false;
    ],
    ActivarTick [ t;                      ! Activa el tick (opcionalmente, asignando el tick antes)
      if (t ~= 0) self.AsignarTick(t);
      else        t = self.tick;
      if (~~(self.contexto_handle_glk)) self.MiGlkRequestTimerEvents(t);
    ],
    DesactivarTick [;                     ! Desactiva el tick
      self.tick = 0;
      self.MiGlkRequestTimerEvents(0);
    ],
    ReactivarTick [;                      ! Reactiva el tick (�til en algunos casos)
      self.MiGlkRequestTimerEvents(self.tick);
    ],
    PausarTick [;                         ! Detiene el tick temporalmente
      self.tick_pausado = self.tick;
      self.DesactivarTick();
    ],
    ReanudarTick [;                       ! Reanuda el tick detenido
      self.ActivarTick(self.tick_pausado);
      self.tick_pausado = 0;
    ],
    PausarGestores [;                     ! No ejecuta los eventos, pero el tiempo sigue corriendo
      self.condicion = false;
    ],
    ReanudarGestores [;                   ! Reanuda la ejecuci�n de los eventos
      self.condicion = true;
    ],
    ActivarMutex [ g;                     ! Activa el mutex sobre un gestor
      self.mutex = g;
    ],
    DesactivarMutex [;                    ! Desactiva el mutex si lo hubiera
      self.mutex = 0;
    ],
    Resetear [                            ! Pone todas las propiedades a sus valores por defecto
      i;
      self.DesactivarTick();
      self.tick = 0;
      self.tick_pausado = -1;
      self.tick_pospuesto = -1;
      self.duracion_maxima = 0;
      self.contador_ticks = 1;
      self.condicion = true;
      self.mutex = 0;
      self.ha_imprimido_algo = true;
      self.contexto_handle_glk = false;
      self.longitud = 0;
      for (i = 0: i < self.#gestores / WORDSIZE: i++) self.&gestores-->i = 0;
    ];

