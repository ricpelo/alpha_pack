!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
!
!   PNJPUERTAS  Una libreria para InformATE complementaria a PNJMovil
!   para la codificación sencilla de puertas
!   utilizables por los PNJs.
!   Escrita por Jævius. <jaevius@teleline.es>
!
!   Basada en una traducción y adaptación de la
!   libreria DOORS de L. Ross Raszewski.<rraszews@skipjack.bluecrab.org>
!   
! La documentación sobre el uso de la librería se incluyen en el
! archivo adjunto PNJPuertas.txt, dado su extensión.


#ifndef tras_abrir;
Property tras_abrir;
#endif;

#ifndef pnj_abrir;
Property pnj_abrir;
!Property additive pnj_abrir;       ! ¿Por qué additive? (c) Alpha
#endif;

Constant DEJA_ALEATORIO   = 0;
Constant DEJA_ABIERTO     = 1;
Constant DEJA_CERRADO     = 2;
Constant DEJA_CERRADO_CON = 3;
Constant NO_PUEDE_ABRIR   = 4;

Class PnjPuerta
  with
    name_f 'puerta',
    name_fp 'puertas',
    interior 0,                     ! (c) Alpha
    exterior 0,                     ! (c) Alpha
    door_dir [;
      if (parent(self) == self.ladoa) return self.dira;
      else                            return self.dirb;
    ],
    door_to [;
      if (parent(self) == self.ladoa) return self.ladob;
      else                            return self.ladoa;
    ],
    ladoa 0,
    dira 0,
    ladob 0,
    dirb 0,
    found_in 0 0,
    lado_cierre 0,
    bloqueada false,
    entra_o_sale [;
      if (self.interior == 0 or parent(self)) return "entra";
      else                                    return "sale";
    ],
    entrando_o_saliendo [;
      if (self.interior == 0 or parent(self)) return "entrando";
      else                                    return "saliendo";
    ],
    el_o_ella [
      gna;
      gna = GetGNAOfObject(self);
      switch (gna) {
        0, 6:  print "él";
        1, 7:  print "ella";
        3, 9:  print "ellos";
        4, 10: print "ellas";
      }
    ],
    pnj_abrir [ quien res x;
      if (self has open) {
        res = 0;
      } else {
        if (quien.tras_abrir == NO_PUEDE_ABRIR) {
          if (TestScope(quien)) {
            print "^", (Link_) quien, " quiere abrir ", (link) self,
                  ", pero no puede.^";
          } else {
            if (TestScope(self)) {
              print "^Se oyen ruidos al otro lado ", (link_del) self,
                    ", como si algo intentara abrirla.^";
            }
          }
          rfalse;
        } else {
          if (self hasnt locked) {
            res = 7;
          } else {
            if (self provides with_key) { 
              if (self.with_key in quien) { 
                if (self.lado_cierre == 0 || quien in self.lado_cierre) {
                  res = 1;
                } else {
                  res = 2;
                }
              } else {
                if (self.lado_cierre == 0 || quien in self.lado_cierre) {
                  res = 3;
                } else {
                  res = 4;
                }
              }
            } else {
              if (self.lado_cierre == 0 || quien in self.lado_cierre) {
                res = 5;
              } else {
                res = 6;
              }
            }
          }
        }
      }
      
      if (self.bloqueada) {
        switch (res) {
          1: res = 8;
          5: res = 9;
          7: res = 10;
        }
      }

      !****** YA TENEMOS EN RES CODIFICADO EL RESULTADO DEL MOVIMIENTO DEL PNJ
      ! EN RELACION CON LA PUERTA. Los códigos son:
      ! 0.- Puerta abierta.- Pnj pasa sin problema
      ! 1.- Puerta cerrada con llave y PNJ tienen la llave.- pasa sin problema
      ! 2.-  "  " " " " " ,pero no hay cerradura a este lado.- no pasa
      ! 3.-  "  " " "  no tiene la llave. No pasa.
      ! 4.-  "  " " "   "  "  "   , además no hay cerradura aquí. No pasa.
      ! 5.-  Puerta cerrada con pestillo y está al alcance del PNJ.- Pasa.
      ! 6.-  "  " " Y NO está al alcance pnj. No pasa.
      ! 7.-  Puerta cerrada pero sin llave ni pestillo. Pasa.
      ! 8, 9 y 10 corresponden a 1, 5 y 7 (únicos casos en los que el
      ! pnj atravesaría la puerta abriendola), cuando ésta se ha bloqueado. No
      ! pasan.
      !************* AHORA DESCRIBIMOS LA ACCION -SI SE VE EL PNJ O LA PUERTA...

      if (TestScope(quien)) {
        new_line;
        switch (res) {
          0: print (Link_el_) quien, " ", (string) self.entra_o_sale(), " por ",
                   (link_el) self, ".^";
          1: print (Link_el_) quien, " abre con ", (a) self.with_key, " ",
                   (link_el) self, " y ", (string) self.entra_o_sale(), " por ",
                   (string) self.el_o_ella(), ".^";
          2, 3, 4, 6: print (Link_el_) quien, " intenta abrir ", (link_el) self,
                            " pero parece cerrad", (o) self, ".^";
          5: print (Link_el_) quien, " abre ", (link_el) self,
                   " accionando el pestillo, y ", (string) self.entra_o_sale(),
                   " por ", (string) self.el_o_ella(), ".^";
          7: print (Link_el_) quien, " abre ", (link_el) self, " y ",
                   (string) self.entra_o_sale(), " por ",
                   (string) self.el_o_ella(), ".^";
          8: print (Link_el_) quien, " intenta abrir con ", (a) self.with_key,
                   " ", (link_el) self, ", pero parece que algo l", (o) self,
                   " atranca.^";
          9: print (Link_el_) quien, " acciona el pestillo ", (link_del) self,
                   " pero algo impide que l", (o) self, " abra.^";
         10: print (Link_el_) quien, " intenta abrir ", (link_el) self,
                   " pero parece atascad", (o) self, " con algo.^";
        }
      } else {
        if (TestScope(self)) {
          new_line;
          switch (res) {
            0: print (Link_el_) quien, " ", (string) self.entra_o_sale(),
                     " por ", (link_el) self, ".^";
            1: print "Oyes abrirse una cerradura, y  ", (link_el) self,
                      " se abre ",
                     (string) self.entrando_o_saliendo(), " ", (link_el) quien,
                     ".^";
            2, 3, 4, 6, 8, 9, 10: print "Alguien intenta abrir ",
                                        (link_el) self,
                                        " sin conseguirlo.^";
            5: print "Oyes descorrer un pestillo, y ", (link_el) self,
                     " se abre ", (string) self.entrando_o_saliendo(), " ",
                     (link_el) quien, ".^";
            7: print (Link_el_) self, " se abre y ",
                     (string) self.entra_o_sale(), " ", (link_el) quien,
                     " por ", (string) self.el_o_ella(), ".^";
          }
        }
      }
  
      !******************* ESTADO DE LA PUERTA TRAS ABRIRLA **********************
      ! En función de la propiedad tras_abrir del PNJMovil... 
  
      if (res == 0 && self hasnt openable) rtrue;
   
      ! Si la puerta estaba abierta y no es abrible - no es necesario ejecutar
      ! codigo de cierre.
      ! Esto permite crear entradas como arcos, dinteles, etc, sin puerta
      ! propiamente dicha.
  
      x = 0;
      if (quien provides tras_abrir) x = quien.tras_abrir;
      if (x == DEJA_ALEATORIO) x = random(3);
  
      ! PRIMERO IMPRIMIMOS EL MENSAJE SI LA ACCION ES VISIBLE POR EL JUGADOR
  
      if (TestScope(self) || TestScope(quien)) {
        if (x == DEJA_CERRADO && res == 0 or 1 or 5 or 7) {
          new_line;
          print (Link_el_) quien, " cierra ", (link_el) self, ".^";
        }
        if (x == DEJA_CERRADO_CON && res == 0 or 1 or 5 or 7) {
          new_line;
          print (Link_el_) quien, " cierra ", (link_el) self;
          if (self.lado_cierre == parent(quien)) {
            print ".^";
          } else {
            switch (res) {
              0, 7: if (self provides with_key) {
                      if (self.with_key in quien) print " con llave.^";
                      else                        print".^";
                    } else {
                      if (self has lockable) print " echándole el pestillo.^";
                      else                   print ".^";
                    }
                 1: print " con llave.^";
                 5: print " echándole el pestillo.^";
            } ! fin de switch
          }
        } ! fin de if (x == DEJA_CERRADO_CON...
      } ! fin de if (PruebaDeAlcance....
  
      ! AHORA EJECUTAMOS EL RESULTADO DE TODO EL PROCESO *************************
  
      switch (res) {
        0: if (x == DEJA_CERRADO or DEJA_CERRADO_CON) give self ~open;
           if (x == DEJA_CERRADO_CON &&
               self.lado_cierre ~= parent(quien)) {
             if (self provides with_key) {
               if (self.with_key in quien) give self locked;
               else                        rtrue;
             } else {
               if (self has lockable) give self locked;
             }
           }
           rtrue;
        1, 5: if (x == DEJA_ABIERTO or DEJA_CERRADO) give self ~locked;
              if (x == DEJA_ABIERTO)                 give self open;
              if (x == DEJA_CERRADO_CON &&
                  self.lado_cierre ~= 0)             give self ~locked;
              rtrue;
        7: if (x == DEJA_ABIERTO) give self open;
           if (x == DEJA_CERRADO_CON && self.lado_cierre ~= parent(quien)) {
             if (self provides with_key) {
               if (self.with_key in quien) give self locked;
               else                        rtrue;
             } else {
               if (self has lockable) give self locked;
             }
           }
           rtrue;
        default:
          rfalse;
      }
    ],
    before [;
      Open, Close: 
        if (self.bloqueada) {
          print "^Algo bloquea ", (link_el) self, " impidiendo que l", (o) self;
          if (action == ##Open) " abras.";
          else                  " cierres.";
        }
  
      Open:
        if (self provides with_key) {
          rfalse;
        } else {
          if (self has lockable && self has locked) {
            "^Está cerrad", (o) self, " con un pestillo.^";
          }
        }
      
      ! Corrige un defecto de la libreria.- en puertas cerradas con pestillo,
      ! al intentar abrirlas te informaba que estaban cerradas con llave.
      ! (supongo que un problema de traduccion (...lock)
  
      Lock, Unlock:
        if (self.lado_cierre == 0 or location) {
          rfalse;
        } else {
          if (self provides with_key) {
            "No hay cerradura a este lado ", (link_del) self, ".";
          }
          "No existe pestillo a este lado ", (link_del) self, ".";
        }
  
      ! El lado_cierre afecta tanto a los Pnjs como al jugador
    
      Examine:
        if (self provides description) {
          PrintOrRun(self, description, 1);
          print " ";
        }
!        else                           new_line;
  
        if (self.lado_cierre == 0 or location) {
          if (self provides with_key) {
            print (Link_el_) self,
                  " puede abrirse y cerrarse desde aquí con una llave.^";
          } else {
            if (self has lockable) {
              print (Link_el_) self,
                    " puede abrirse y cerrarse desde aquí con un pestillo.^";
            }
          }
        }
  
        if (self has openable) {
          if (self has open) "Ahora está abiert", (o) self, ".";
          else               "Está cerrad", (o) self, "."; ! ^";
        }
        rtrue;    
    ],
    each_turn [
      o;
      Open, Close:
        if (noun ofclass PnjPuerta) {
          LimpiarPropagacion();
          objectloop (o ofclass Ruido) {
            if (o.jugador_oye()) {
              if (o notin location && o hasnt absent) move o to location;
              TocaDesde(o, true);
            }
          }
        }
    ],
  has
    door static;


[ IniciarPuertas o i j;
  objectloop (j ofclass PnjPuerta) { 
    objectloop (o in compass) {
      i = o.door_dir;
      if (j provides i) {
        j.ladoa = j.i;
        j.dirb = i;
      }
    }
    objectloop (o in compass) {
      i = o.door_dir;
      if ((j provides i) && (j.dirb ~= i)) {
        j.ladob = j.i;
        j.dira = i;
      }
    }
    j.&found_in-->0 = j.ladoa;
    j.&found_in-->1 = j.ladob;
  }
];

