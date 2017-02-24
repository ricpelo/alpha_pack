!
! Contenedor.h
!
! Un contenedor que, cuando se examina, describe su contenido. Un objeto de esta
! clase debe usar la propiedad 'descripcion_real' en lugar de 'descripcion'.
!


System_file;


class Contenedor
  with
    descripcion_real "",
    description [;
      PrintOrRun(self, descripcion_real, 1);
      print " ";
      if (self has open) {
        if (children(self) == 0) {
          "Est� abiert", (o) self, ", pero vac�", (o) self, ".";
        } else {
          print "Est� abiert", (o) self, ", y en su interior puedes ver ";
          WriteListFrom(child(self), ENGLISH_BIT);
          print ".^";
        }
      } else {
        if (self has transparent) {
          print "Est� cerrad", (o) self, ", pero al ser transparente puedes ver ";
          if (children(self) == 0) {
            "que est� vac�o.";
          } else {
            print "que en su interior hay ";
            WriteListFrom(child(self), ENGLISH_BIT);
            print ".^";
          }
        } else {
          "Est� cerrad", (o) self, ".";
        }
      }
    ],
  has
    container;

