!
! EscenarioAbrible.h
!
! Crea objetos que son parte del escenario pero se pueden abrir y
! cerrar y s�lo se describen cuando est�n abiertos.
!


System_file;


Class EscenarioAbrible
  with
    si_abierto [; print (_El) self, " est� abiert", (o) self, ".^"; rtrue; ],
    si_cerrado [; print (_El) self, " est� cerrad", (o) self, ".^"; rtrue; ],
    ! Cuando est� abierto, pasa de ser escenario a ser est�tico. Esto se hace para que
    ! se muestre cuando est� abierto, usando la propiedad si_abierto. En realidad, la
    ! propiedad si_cerrado no se usa, porque cuando est� cerrado es un escenario, y por
    ! tanto no se describe.
    abrir [;
      give self abierto;
      give self ~escenario;
      give self estatico;
      self.gancho_abrir();
    ],
    cerrar [;
      give self ~abierto;
      give self escenario;
      give self ~estatico;
      self.gancho_cerrar();
    ],
    gancho_abrir [;
      self.gancho();
    ],
    gancho_cerrar [;
      self.gancho();
    ],
    gancho [; rfalse; ],
    despues [;
      Abrir:
        self.abrir();
      Cerrar:
        self.cerrar();
    ],
  has
    escenario abrible;

