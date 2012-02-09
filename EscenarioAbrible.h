!
! EscenarioAbrible.h
!
! Crea objetos que son parte del escenario pero se pueden abrir y
! cerrar y sólo se describen cuando están abiertos.
!


System_file;


class EscenarioAbrible
  with
    when_open [; print (The) self, " está abiert", (o) self, ".^"; rtrue; ],
    when_closed [; print (The) self, " está cerrad", (o) self, ".^"; rtrue; ],
    ! Cuando está abierto, pasa de ser escenario a ser estático. Esto se hace para que
    ! se muestre cuando está abierto, usando la propiedad si_abierto. En realidad, la
    ! propiedad si_cerrado no se usa, porque cuando está cerrado es un escenario, y por
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

