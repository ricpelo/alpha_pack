!
! EscenarioAbrible.h
!
! Crea objetos que son parte del escenario pero se pueden abrir y
! cerrar y sólo se describen cuando están abiertos.
!


System_file;


class EscenarioAbrible
  with
    when_open [;
      MostrarNombreLink(self, 1);
      print " está abiert", (o) self, ".^"; rtrue; ],
    when_closed [;
      MostrarNombreLink(self, 1);
      print " está cerrad", (o) self, ".^"; rtrue; ],
    ! Cuando está abierto, pasa de ser escenario a ser estático. Esto se hace
    ! para que se muestre cuando está abierto, usando la propiedad when_open.
    ! En realidad, la propiedad when_closed no se usa, porque cuando está
    ! cerrado es un escenario, y por tanto no se describe:
    describe [;
      new_line;
      if (self has open) PrintOrRun(self, when_open);
      else               PrintOrRun(self, when_closed);
      rtrue;
    ],
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

