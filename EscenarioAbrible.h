!
! EscenarioAbrible.h
!
! Crea objetos que son parte del escenario pero se pueden abrir y
! cerrar y s�lo se describen cuando est�n abiertos.
!


System_file;


class EscenarioAbrible
  with
    when_open [; print (The) self, " est� abiert", (o) self, ".^"; rtrue; ],
    when_closed [; print (The) self, " est� cerrad", (o) self, ".^"; rtrue; ],
    ! Cuando est� abierto, pasa de ser escenario a ser est�tico. Esto se hace para que
    ! se muestre cuando est� abierto, usando la propiedad si_abierto. En realidad, la
    ! propiedad si_cerrado no se usa, porque cuando est� cerrado es un escenario, y por
    ! tanto no se describe.
    abrir [;
      give self open;
      give self ~scenery;
      give self static;
      self.gancho_abrir();
    ],
    cerrar [;
      give self ~open;
      give self scenery;
      give self ~static;
      self.gancho_cerrar();
    ],
    gancho_abrir [;
      self.gancho();
    ],
    gancho_cerrar [;
      self.gancho();
    ],
    gancho [; rfalse; ],
    after [;
      Open:
        self.abrir();
      Close:
        self.cerrar();
    ],
  has
    scenery openable;

