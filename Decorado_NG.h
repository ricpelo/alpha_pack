! Decorados.h (c) 2000 Zak McKracken
! V2.0 biplataforma
!
!
!  Se define la clase decorado.
!
!  Los objetos de esta clase son en realidad varios objetos del juego,
!  recogidos en un solo objeto del codigo.
!
!  Se entiende que son objetos minimalistas, con los que apenas se
!  puede interactuar, salvo examinarlos.
!
!  Un objeto de la clase decorado, no puede tener nombre_corto ni
!  descripcion, ya que en realidad representa a muchos objetos
!  diferentes. Lo que tiene en cambio es la propiedad describir, que
!  contiene una lista con la siguiente estructura:
!
!  with describir
!    'palabra' "descripcion" GENERO
!    'palabra2' "descripcion2" GENERO2
!    etc.
!    ,
!
! Ejemplo:
!
! Object decorado_celda Celda
! class decorado
! with  describir
!        'pared' "Paredes lugubres y frias" G_FEMENINO
!        'paredes' "Paredes lugubres y frias" G_FEMENINO+G_PLURAL
! ;
!
!
!  Observa que los elementos de la lista no llevan comas separando. Ni
!  siquiera al final de cada linea. Solo al final de todos los
!  elementos, habra que poner una coma (si vas a seguir definiendo
!  propiedades o atributos), o directamente punto y coma (si has
!  terminado la definicion del objeto).
!
! Como funciona:
!
!  Cuando el jugador intente cualquier accion usando como sustantivo
!  'pared', por ejemplo, y el objeto decorado_celda esta a su alcance
!  (el jugador esta dentro de la celda), este objeto se dara por
!  referenciado, y asi se lo notificara al parser, actualizando ademas
!   los siguientes campos del objeto decorado_celda:
!
!   palabra -> Toma el valor de la palabra usada por el jugador
!               ('pared')
!   descripcion -> Toma el valor de la cadena que sigue a 'pared', en
!              la propiedad describir es decir "Paredes lugubres y
!              frias"
!   genero -> Toma el genero asociado con esa entrada, es decir
!             G_FEMENINO
!
!  Despues deja que el parser continue. La libreria, poco despues,
!  llamara al objeto  decorado_celda, a su rutina antes, para
!  informarle de la accion que se pretende hacer con el. La rutina
!  proporcionada en esta clase, por defecto "deja pasar" la
!  accion Examinar, pero impide cualquier otra con el mensaje "Dejalo,
!  solo es decorado".
!
!  Si la accion es examinar, la libreria llevara a cabo su accion
!  estandar, y por tanto poco despues imprimira la descripcion de ese
!  objeto, que toma del campo descripcion y que ya contiene la cadena
!  correcta.
!
!  Puedes sobreescribir la rutina antes en los objetos que derive de
!  esta clase, y emitir mensajes genericos a tu gusto, del
!  estilo de:
!
!    "Deja ", (el) self, " en paz. No ", (es) self, " mas que decorado.";
!
!  Y el nombre que saldra sera en cada caso el de la palabra que haya
!  usado el jugador, por ejemplo:
!
!  > EMPUJA PAREDES
!  Deja las paredes en paz, no son mas que decorado.
!
!  Incluso, si quisieras particularizar para un objeto concreto,
!  podrias examinar la propiedad palabra, que te dira que palabra ha
!  usado el jugador. Por ejemplo:
!
!  Empujar: if (self.palabra=='techo') "No llegas.",
!           "Deja ", (el) self, " en paz.";
!
!
!  LIMITACIONES: si el nombre corto tiene mas de nueve letras, cuando
!  sea impreso sera truncado, por ejemplo:
!  > EMPUJA INSTALACION
!  Deja la instalaci en paz.
!
!  Puede evitarse redefiniendo habilmente la rutina nombre_corto en el
!  objeto derivado de la clase, y cambiandolo por algo como:
!
!     if (self.palabra=='instalacion') print "instalacion";
!     else print (address) self.palabra;
!     rtrue;
!
!  Pero evidentemente habria que particularizarlo para cada caso.
!

! Para compatibilidad con Inform 6.15 InformATE 6/7
#ifndef WORDSIZE;
Constant WORDSIZE=2;
Constant TARGET_ZCODE;
#endif;

System_file;

Message "Incluyendo Decorado_NG, por Zak y Sothoth";

!
! Rutina que ejecuta rutinas si las encuentra
!
#ifndef VR;
[ VR valor;
! print "[VR: ", valor, " --> ", ZRegion(valor), " ]^";
  if (ZRegion(valor) == 2) return valor();
  else                     return valor;
];
#endif;

!
! CLASE DECORADO
!
! El algoritmo es el siguiente: se permiten varias palabras seguidas (o separadas por las
! part�culas el, la, los, las, de o del), siempre que dichas palabras sean sin�nimas. Se
! permite que una misma palabra sea sin�nima de dos o m�s palabras distintas, lo cual se
! usar�a sobre todo para crear adjetivos. Ejemplo:
!
! sinonimos
!   'grande' 'mesa'    -1
!   'grande' 'ventana' -1,
! describir
!   'mesa'    "Una mesa muy bonita."          G_FEMENINO
!   'ventana' "Una ventana abierta al mundo." G_FEMENINO
!
! Este ejemplo crea dos elementos en el decorado: la mesa y la ventana. Ambas tienen como
! sin�nimo la palabra 'grande' (que en realidad aqu� actuar�a como adjetivo de ambas).
! Se puede usar EX MESA, EX MESA GRANDE, EX VENTANA, EX VENTANA GRANDE... pero NO puede
! usarse EX MESA VENTANA o EX VENTANA MESA, ya que 'mesa' y 'ventana' no son sin�nimos
! (si A es sin�nimo de B y de C, eso no significa que B y C tambi�n lo sean). Aunque
! parece que no tiene demasiado sentido, s� es pr�ctico para el caso de que el sin�nimo
! sea un adjetivo (como el 'mesa' de este caso).
! Si el jugador escribe EX GRANDE, el algoritmo se decidir� por el primero que encuentre
! en la lista de sin�nimos (en este caso, la mesa).
!
class Decorado
  with
    description 0,   ! La descripci�n del objeto
    palabra_usada 0, ! La palabra exacta que ha usado el jugador
    gender 0,        ! El g�nero del objeto
    describe 0,      ! El array de descripciones
    sinonimos 0,     ! El array de sin�nimos
    palabra 0,       ! Si es un sin�nimo, la palabra correspondiente en 'describir'.
                     ! Si no lo es, vale lo mismo que 'palabra_usada'
    short_name [;
      print (address) self.palabra;
      rtrue;
    ],
    buscar_nombre [ x i j;   ! Se usa en ExaminarFalso
      for (i = 0: i < (self.#describe) / (3 * WORDSIZE): i++) {
        if ((self.&describe)-->(i * 3) == x) {
          self.description   = VR((self.&describe)-->(i * 3 + 1));
          self.actualizar_genero((self.&describe)-->(i * 3 + 2));
          self.palabra       = x;
          self.palabra_usada = x;
          PronounNotice(self);
          rtrue;
        }
      }
      if (self.sinonimos == 0) return false;
      for (j = 0: j < (self.#sinonimos) / (3 * WORDSIZE): j++) {
        if ((self.&sinonimos)-->(j * 3) == x) {
          for (i = 0: i < n: i++) {
            if ((self.&describe)-->(i * 3) == (self.&sinonimos)-->(j * 3 + 1)) {
              self.description   = VR((self.&describe)-->(i * 3 + 1));
              self.palabra       = (self.&describe)-->(i * 3);
              self.palabra_usada = (self.&sinonimos)-->(j * 3);
              self.gender = (self.&sinonimos)-->(j * 3 + 2);
              if (self.gender == -1) {
                self.gender = (self.&describe)-->(i * 3 + 2);
              }
              self.actualizar_genero(self.gender);
              PronounNotice(self);
              rtrue;
            }
          }
        }
      }
    ],
    parse_name [ i n w c r f j p;
      self.description = 0;
      n = (self.#describe) / (3 * WORDSIZE);

      if (w == 'el' or 'la' or 'los' or 'las')
        w = NextWord();

      c = r = 0;
      p = NULL;

      while (true) {
        w = NextWordStopped(); if (w == -1) return c;
        if (w == 'de' or 'del') {
          w = NextWordStopped(); if (w == -1) return c;
          r++;
        }
        if (w == 'el' or 'la' or 'los' or 'las') {
          w = NextWordStopped(); if (w == -1) return c;
          r++;
        }

        f = false;

        for (i = 0 : i < n : i++) {
          if ((self.&describe)-->(i * 3) == w) {
            if (p == NULL) {
              p = w;
            } else {
              if (p ~= w) {
                return c;
              }
            }
            f = true;
            if (self.description == 0) {
              self.description   = VR((self.&describe)-->(i * 3 + 1));
              self.palabra       = w;
              self.palabra_usada = w;
              self.actualizar_genero((self.&describe)-->(i * 3 + 2));
              PronounNotice(self);
            }
            c++;
            if (r > 0) {
              c = c + r;
              r = 0;
            }            
          }
        }

        if ((~~f) && self.sinonimos ~= 0) {
          for (j = 0: j < (self.#sinonimos) / (3 * WORDSIZE): j++) {
.synonymContinue;
            if ((self.&sinonimos)-->(j * 3) == w) {
              f = false;
              for (i = 0: i < n: i++) {
                if ((self.&describe)-->(i * 3) == (self.&sinonimos)-->(j * 3 + 1)) {
                  if (p == NULL) {
                    p = (self.&describe)-->(i * 3);
                  } else {
                    if (p ~= (self.&describe)-->(i * 3)) {
                      j++;
                      jump synonymContinue;
                    }
                  }
                  if (self.description == 0) {
                    self.description   = VR((self.&describe)-->(i * 3 + 1));
                    self.palabra       = (self.&describe)-->(i * 3);
                    self.palabra_usada = (self.&sinonimos)-->(j * 3);
                    self.gender        = (self.&sinonimos)-->(j * 3 + 2);
                    if (self.gender == -1) {
                      self.gender      = (self.&describe)-->(i * 3 + 2);
                    }
                    self.actualizar_genero(self.gender);
                  }
                  f = true;
                  break;
                }
              }
              if (f) {
                c++;
                if (r > 0) {
                  c = c + r;
                  r = 0;
                }
                break;
              } else {
                PronounNotice(self);
                return c;
              }
            }
          }
        }
      }
    ],
    actualizar_genero [ g;
      self.gender = g;
      if (self.gender == G_FEMENINO) give self female;
      if (self.gender == G_PLURAL)   give self pluralname;
    ],
    before [;
      Examine: rfalse;
      Take:    "No puedes hacerlo, ya que est�", (n) self, " fij", (o) self,
               " en su sitio.";
      Push:    print_ret (The) self, " no parece que pueda", (n) self,
               " ser empujad", (o) self, ".";
      Smell:   "No parece que huela", (n) self, " a nada especial.";
      Listen:  "No produce", (n) self, " ning�n sonido.";
      Search:  "No hay nada que buscar en eso.";
      Touch:   "No notas nada especial al tacto.";
      default: "No hay ninguna raz�n para hacer eso.";
    ],
  has
    scenery concealed;

