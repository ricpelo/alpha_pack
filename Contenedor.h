!
! Contenedor.h
!
! Un contenedor que, cuando se examina, describe su contenido. Un objeto de esta
! clase debe usar la propiedad 'descripcion_real' en lugar de 'descripcion'.
!


System_file;


Class Contenedor
  with
    descripcion_real "",
    descripcion [;
      print (string) self.descripcion_real, " ";
      if (self has abierto) {
        if (children(self) == 0) {
          "Est� abiert", (o) self, ", pero vac�", (o) self, ".";
        } else {
          print "Est� abiert", (o) self, ", y en su interior puedes ver ";
          EscribirListaDesde(child(self), ESPANOL_BIT);
          print ".^";
        }
      } else {
        if (self has transparente) {
          print "Est� cerrad", (o) self, ", pero al ser transparente puedes ver ";
          if (children(self) == 0) {
            "que est� vac�o.";
          } else {
            print "que en su interior hay ";
            EscribirListaDesde(child(self), ESPANOL_BIT);
            print ".^";
          }
        } else {
          "Est� cerrad", (o) self, ".";
        }
      }
    ],
  has
    recipiente;

