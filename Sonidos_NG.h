!--------------------------------------------------------------------------
!
! SONIDOS: Una librería de manejo 'realista' de las fuentes de sonido
!
! Para usar con InformaTE! de Zak.
!
! Esta librería intenta 'ocultar' todo el tratamiento realista
! de fuentes de sonido, permitiendo al jugador 'escuchar' sonidos
! lejanos, amortiguados o incluso deformados por los obstáculos
! que se encuentren en su camino.
!
! Desarrollado por Mel Hython en el Febrero del 2000
!
!---------------------------------------------------------------------------

!System_file;


!===============================================================================
! CONSTANTES Y DEFINICIONES GENERALES
!===============================================================================


Message "Estás incluyendo el módulo de Sonidos de Mel Hython
    ¡Atente a las consecuencias!";

#ifndef WORDSIZE;
Constant WORDSIZE = 2;
#endif;

#ifndef Lugar;
Class Lugar with number;
#endif;

#ifndef en_ruta;
Attribute en_ruta;
#endif;

Property cercano_a 0;

#Stub RutinaPreLocal 0;

Default SONIDOS_MAX_NIVEL_ANIDAMIENTO = 10;
Default SONIDOS_MAX_NUM_OYENTES       = 100;
Default SONIDOS_MAX_NUM_LUGARES       = 100;
Default SONIDOS_MAX_NUM_RUIDOS        = 100;

Default NIVEL_AUDIBLE  = 2;
Default NIVEL_SUSURRO  = 4;
Default NIVEL_MURMULLO = 6;
Default NIVEL_NORMAL   = 8;
Default NIVEL_ALTO     = 10;
Default NIVEL_MUYALTO  = 12;


!===============================================================================
! VARIABLES GLOBALES
!===============================================================================


Array cont_oyente table cO1 cO2 cO3 cO4 cO5 cO6 cO7 cO8 cO9 cO10;

! Reducciones de intensidad sonora:
Global coste_por_obstaculo          = 4;  ! Por pared o puerta cerrada
Global coste_por_habitacion         = 2;  ! Por puerta abierta
Global coste_por_contenedor         = 1;  ! Por contenedor abierto
Global coste_por_contenedor_cerrado = 2;  ! Por contenedor cerrado

! Fifo de llamadas a oyentes. Como mucho 10 anidamientos
Global indice_anidamiento = 0;

! Lugares:
Array  tabla_lugares table SONIDOS_MAX_NUM_LUGARES;
Global indice_lugares = 0;
Global lugares_apuntados = false;

! Oyentes:
Array  tabla_oyentes table SONIDOS_MAX_NUM_OYENTES;
Global indice_oyentes = 0;
Global oyentes_apuntados = false;

! Ruidos:
Array  tabla_ruidos table SONIDOS_MAX_NUM_RUIDOS;
Global indice_ruidos = 0;
Global ruidos_apuntados = false;

Global sonido_tras_un_obstaculo;


!===============================================================================
! CLASES
!===============================================================================


! Clase para pasar contenidos al método 'escuchado':
Class ContenidoSonoro
  with
    clase 0,             ! --> TipoDeSonido usado para emitirlo
    origen 0,            ! --> fuente del sonido
    modo 0,              ! --> modo de emisión del sonido
    intensidad 0,        ! --> potencia percibida
    se_puede_ver false,  ! --> el jugador ve la fuente
    se_escucha_en 0,     ! --> de donde parece provenir
    texto "";            ! --> lo que dice el sonido si es inteligible


!
! Clase general para todos los sonidos
!
Class TipoDeSonido
  with
    !
    ! La descripcion mínima del sonido irá en 'short_name'
    ! o en las comillas de definición del objeto
    !
    reconocible     false,   ! El jugador reconoce el origen incluso a distancia
    verbo_oir       "Oyes",  ! Descripcion del medio de recepcion
    verbo_decir     "dice",  ! Verbo base usado para decir cosas
    verbo_emitir    "emite", ! Verbo base usado para emitir sonidos
    adjetivo_de_voz 0,       ! Adjetivo referente solo a voz
    usar_distancia  false,   ! Por defecto no se habla de la distancia
    usar_potencia   true,    ! por defecto se describe la potencia
    usar_direccion  true,    ! por defecto se dice de donde proviene si se sabe
    !
    ! Textos para dar flexibilidad:
    !
    adjetivo_lejana     "lejana",         ! Distancia lejana-femenina
    adjetivo_muy_lejana "muy lejana",     ! Distancia muy lejana-femenina
    adjetivo_lejano     "lejano",         ! Distancia lejano-masculino
    adjetivo_muy_lejano "muy lejano",     ! Distancia muy lejano-masculino
    no_voz_susurro      ", que apenas se oye",
    no_voz_murmullo     " sonando muy bajito",
    no_voz_normal       0,
    no_voz_alto         " sonando con fuerza",
    no_voz_muy_alto     ", con una intensidad que casi te deja sordo",
    voz_susurro         "pero apenas se entiende",
    voz_murmullo        "pero suena como un susurro",
    voz_normal          0,
    voz_alto            "con un torrente de voz",
    voz_muy_alto        "con tal fuerza que casi te deja sordo",
    !
    ! Rutinas:
    !
    ! Cuando el sonido proviene de un objeto que el jugador puede ver:
    suena_aqui [ contenido;
      print (The) contenido.origen, " ", (string) VR(self.verbo_emitir),
            " ", (the) self, ".^";
    ],
    ! Rutina que devuelve un texto indicando nada, 'lejana' o 'muy lejana' según
    ! si la fuente se ve y su intensidad sonora:
    di_distancia [ intensidad se_puede_ver femenino;
      if (se_puede_ver) return 0;
      if (femenino) {
        if (intensidad <= NIVEL_SUSURRO)       return self.adjetivo_muy_lejana;
        else if (intensidad <= NIVEL_MURMULLO) return self.adjetivo_lejana;
        else                                   return 0;
      } else {
        if (intensidad <= NIVEL_SUSURRO)       return self.adjetivo_muy_lejano;
        else if (intensidad <= NIVEL_MURMULLO) return self.adjetivo_lejano;
        else                                   return 0;
      }
    ],
    ! Rutina que devuelve una descripcion de potencia auditiva escuchada:
    di_potencia [ intensidad modo esVoz;
      ! intensidad --> potencia auditiva
      ! modo       --> forma en la que fue emitido
      ! esVoz      --> contiene true si el contenido tenía texto
      modo = modo;
      if (~~esVoz) {
        if (intensidad <= NIVEL_SUSURRO)       return self.no_voz_susurro;
        else if (intensidad <= NIVEL_MURMULLO) return self.no_voz_murmullo;
        else if (intensidad <= NIVEL_NORMAL)   return self.no_voz_normal;
        else if (intensidad <= NIVEL_ALTO)     return self.no_voz_alto;
        else                                   return self.no_voz_muy_alto;
      } else {
        if (intensidad <= NIVEL_SUSURRO)       return self.voz_susurro;
        else if (intensidad <= NIVEL_MURMULLO) return self.voz_murmullo;
        else if (intensidad <= NIVEL_NORMAL)   return self.voz_normal;
        else if (intensidad <= NIVEL_ALTO)     return self.voz_alto;
        else                                   return self.voz_muy_alto;
      }
    ],
    ! Rutina que imprime el mensaje final de jugador. Esta es la rutina que
    ! puede redefinir el programador para crear sus propios sonidos:
    escuchado [ contenido no_decir_nada
      texto_potencia texto_distancia _esVoz;
      ! El objeto contenido tiene los atributos:
      !
      ! origen         --> fuente del sonido
      ! modo           --> forma en la que fue emitido el sonido
      ! intensidad     --> potencia percibida del sonido de 0 a 10
      ! se_puede_ver   --> true si el jugador lo ve directamente
      ! se_escucha_en  --> origen aparente (contenedor, dirección...)
      ! texto          --> lo que dice la voz

      if (no_decir_nada) rfalse;

      new_line;

      ! No se ha pasado un texto es un sonido ininteligible, una voz 
      if (contenido.texto == 0) {
        _esVoz = false;      ! En principio no es voz

        if (VR(self.usar_distancia)) {
          texto_distancia = self.di_distancia(contenido.intensidad,
                                              contenido.se_puede_ver,
                                              self has female);
        } else {
          texto_distancia = 0;
        }

        ! En esta clase base si el umbral de audicion está en 3
        if (contenido.intensidad > NIVEL_AUDIBLE) {
          ! Es escuchado por el jugador
          if ((contenido.se_puede_ver) && (VR(self.usar_direccion))) {
            ! Se ve, indicar origen
            if (VR(self.verbo_emitir) == 0) {
              print (The) contenido.origen, " ", (string) VR(self.verbo_decir);
              ! Justo para este caso, al usar el verbo de decir, se cambia a la
              ! coletilla que se usa con texto
              _esVoz = true;
            } else {
              if (self.suena_aqui ~= 0) self.suena_aqui(contenido);
              else print (The) contenido.origen, " ",
                         (string) VR(self.verbo_emitir),
                         " ", (the) self, ".^";
              rfalse;
            }
          } else {
            ! No se ve directamente
            if ((VR(self.usar_direccion)) && (contenido.se_escucha_en ~= 0)) {
              ! pero se sabe de donde
              print (string) VR(self.verbo_oir), " ", (the) self;
              if (texto_distancia ~= 0) print " ", (string) VR(texto_distancia);
              print " proveniente ", (DireccionSonido) contenido.se_escucha_en;
            } else {
              ! y no se sabe de donde
              print (string) VR(self.verbo_oir), " ", (the) self;
              if (texto_distancia ~= 0) print " ", (string) VR(texto_distancia);
              print " proveniente de no se sabe dónde";
            }
          }

          ! Marcar intensidad vista
          if (VR(self.usar_potencia)) {
            texto_potencia = self.di_potencia(contenido.intensidad,
                                              contenido.modo, _esVoz);
            if (texto_potencia ~= 0) print (string) VR(texto_potencia), ".^";
            else                     print ".^";
          } else {
            print ".^";
          }
        }
      } else {
        if (self.usar_distancia) {
          texto_distancia = self.di_distancia(contenido.intensidad,
                                              contenido.se_puede_ver, _esVoz);
        } else {
          texto_distancia = 0;
        }

        ! Tiene texto, es una voz
        if (contenido.intensidad > NIVEL_AUDIBLE) {
          ! Es escuchado por el jugador
          if ((contenido.se_puede_ver) && (VR(self.usar_direccion))) {
            ! Se ve, indicar origen
            if (contenido.intensidad > NIVEL_SUSURRO) {
              ! Se entiende lo que dice
              print (The) contenido.origen, " ", (string) VR(self.verbo_decir);

              if (self.adjetivo_de_voz ~= 0) {
                print " con voz ", (string) VR(self.adjetivo_de_voz);
              }
              if (texto_distancia ~= 0) print " ", (string) VR(texto_distancia);
              
              print ": @<<", (string) contenido.texto, "@>>";
            } else {
              ! No se entiende lo que dice
              print (The) contenido.origen, " ", (string) VR(self.verbo_decir),
                    " algo";

              if (self.adjetivo_de_voz ~= 0) {
                print " con voz ", (string) VR(self.adjetivo_de_voz);
              }
              if (texto_distancia ~= 0) print " ", (string) VR(texto_distancia);
            }
          } else {
            ! No se ve directamente
            if (VR(self.reconocible)) {
              if ((VR(self.usar_direccion)) && (contenido.se_escucha_en ~= 0)) {
                ! Se sabe de donde viene
                print (string) VR(self.verbo_oir), " la voz ";

                if (self.adjetivo_de_voz ~= 0) {
                  print (string) VR(self.adjetivo_de_voz), " ";
                }
                if (texto_distancia ~= 0) {
                  print (string) VR(texto_distancia), " ";
                }

                print (del) contenido.origen, "proveniente ",
                      (DireccionSonido) contenido.se_escucha_en, " que ",
                      (string) VR(self.verbo_decir);
              } else {
                ! No se sabe de donde viene
                print (string) VR(self.verbo_oir), " la voz ";

                if (self.adjetivo_de_voz ~= 0) {
                  print (string) VR(self.adjetivo_de_voz), " ";
                }
                if (texto_distancia ~= 0) {
                  print (string) VR(texto_distancia), " ";
                }

                print (del) contenido.origen,
                      "proveniente de no se sabe dónde que ",
                      (string) VR(self.verbo_decir);
              }
            } else {
              if ((VR(self.usar_direccion)) && (contenido.se_escucha_en ~= 0)) {
                ! Se sabe de donde viene
                print (string) VR(self.verbo_oir), " una voz ";

                if (self.adjetivo_de_voz ~= 0) {
                  print (string) VR(self.adjetivo_de_voz), " ";
                }
                if (texto_distancia ~= 0) {
                  print (string) VR(texto_distancia), " ";
                }

                print "proveniente ", (DireccionSonido) contenido.se_escucha_en, " que ",
                      (string) VR(self.verbo_decir);
              } else {
                ! No se sabe de donde viene
                print (string) self.verbo_oir, " una voz ";

                if (self.adjetivo_de_voz ~= 0) {
                  print (string) VR(self.adjetivo_de_voz), " ";
                }
                if (texto_distancia ~= 0) {
                  print (string) VR(texto_distancia), " ";
                }

                print "proveniente de no se sabe dónde que ",
                      (string) VR(self.verbo_decir);
              }
            }

            if (contenido.intensidad > NIVEL_SUSURRO) {
              print ": @<<", (string) contenido.texto, "@>>";
            } else {
              print " algo";
            }
          }

          ! Marcar intensidad vista
          if (VR(self.usar_potencia)) {
            texto_potencia = self.di_potencia(contenido.intensidad,
                                              contenido.modo, true);
            if (texto_potencia ~= 0) print (string)VR(texto_potencia), ".^";
            else                     print ".^";
          } else {
            print ".^";
          }
        }
      }
    ];

!
! Clase de objetos que hablan usando estos sonidos
!
Class ObjetoHablante
  with
    ! Simplemente contiene un método para llamar a la función de provocar 
    ! un sonido de clase Voz usando su propia voz (que sera de tipo plano si
    ! no se cambia):
    voz tipo_plano,
    habla [ _texto _intensidad _modo;
      ! print "~",(name)self, " usando ", (name)self.voz, "~^";
      ! Rellenamos el contenido de forma apropiada
      cont_general.origen        = self;            ! El que sea
      cont_general.modo          = _modo;           ! El que sea
      cont_general.se_puede_ver  = TestScope(self); ! Fuera de la vista
      cont_general.se_escucha_en = 0;               ! Da igual
      cont_general.texto         = _texto;          ! Texto pedido
      cont_general.clase         = VR(self.voz);    ! El objeto TipoDeSonido
      ! Intensidad proporcionada o intensidad media
      if (_intensidad ~= 0) cont_general.intensidad = _intensidad;
      else                  cont_general.intensidad = NIVEL_NORMAL;
      ! 'Tocar' el sonido
      TocaSonido(cont_general);
    ];

!
! Clase de objetos que 'escuchan' los sonidos que se producen
!
Class ObjetoOyente
  with
    ! Simplemente reciben en el método 'oido' dos parámetros correspondientes al
    ! tipo de voz usada y al contenido acústico
    int_pend 0,
    se_esc_pend 0,
    se_pue_pend 0,
    apunta [ _sonido;
      if (self hasnt en_ruta) {
        self.int_pend    = _sonido.intensidad;
        self.se_esc_pend = _sonido.se_escucha_en;
        self.se_pue_pend = _sonido.se_puede_ver;
        give self en_ruta;
      }
    ],
    limpia [;
      give self ~en_ruta;
      self.int_pend = 0;
    ],
    oido [ _sonido;
      ! El objeto base hace una tontería; tiene que ser cada objeto el que 
      ! haga algo particular para reaccionar ante los sonidos que oiga
      if ((TestScope(self)) && (_sonido.intensidad > NIVEL_AUDIBLE) &&
          (_sonido.origen ~= self)) {
        print "^", (The) self, " parece haber oído algo.^";
        ! print (name)_sonido.se_escucha_en, _sonido.se_escucha_en;
      }
    ];


!
! Objeto que produce sonido ambiente
!
Class Ruido
  class TipoDeSonido
  with
    name 'sonido',
    frase 0,          ! Texto que se dice permanentemente
    intensidad 0,     ! Nivel con el que se habla
    origen 0,         ! Objeto que lo produce
    emision 0,        ! El sonido a emitir
    sonando false,    ! De base está sonando
    verbo_emitir "está emitiendo", ! Verbo base usado para emitir sonidos
    tocar_sonido [ volumen;
      #ifdef DEBUG;
      if (self.emision == 0) {
        "*** ERROR: Se intenta tocar un sonido sin definirlo antes ***"; 
      }
      #endif;
      if (self.emision ~= 0) {
        if (Damusix.SonandoDeFondo(self.emision)) {
          Damusix.VolumenCanal(self.canal, volumen);
        } else {
          Damusix.AsignarCanal(self.emision, self.canal, volumen, 
                               SONIDO_REPETIR);
          Damusix.TocarCanal(self.canal);
        }
      }
    ],
    parar_sonido [;
      Damusix.PararCanal(self.canal);
    ],
    sonar [ vol;
      self.tocar_sonido(vol);
    ],
    quitar [;
      self.parar();
      remove self;
      give self absent;
    ],
    found_in [;
      return self.jugador_oye();
    ],
    jugador_oye [
      enc_orig inicial se_oye _intensidad;
      ! Se ve si el sonido alcanza 'location'
      if ((~~self.origen) || (~~self.sonando)) return false;
      if (self.intensidad ~= 0) _intensidad = VR(self.intensidad);
      else                      _intensidad = NIVEL_NORMAL;

      inicial = parent(VR(self.origen));

      ! Intentamos 'sacar' el sonido a un 'Lugar'
      enc_orig = false;

      while (inicial) {
        if (inicial ofclass Lugar) {
          ! Lo hemos logrado, propagar sonido desde ahí
          PropagaSonido(inicial, _intensidad);
          enc_orig = true;
          break;
        } else {
          ! Cada 'escalada' implica perder potencia
          if (inicial ~= selfobj) {
            if (inicial has open) {
              _intensidad = _intensidad - coste_por_contenedor;
            } else {
              _intensidad = _intensidad - coste_por_contenedor_cerrado;
            }
          }

          ! Seguimos escalando en los padres
          inicial = parent(inicial);
          
          ! Caso especial: nos saltamos al jugador como origen válido de sonidos
          if (inicial == selfobj) inicial = location;
        }
      }

      if (enc_orig == false) return false;

      se_oye = ((location has en_ruta) && (location.number > NIVEL_AUDIBLE));

      ! Limpiar la propagación
      LimpiarPropagacion();

      return se_oye;
    ],
    describe [;
      TocaDesde(self);
      rtrue;
    ],
    mensaje_iniciar [;
      print "Empiezas a oir ", (the) self, ".^";
    ],
    iniciar [ no_decir_nada;
      if (~~self.sonando) {
        self.sonando = true;
        if ((self hasnt concealed) && (self.jugador_oye())) {
          if (~~no_decir_nada) self.mensaje_iniciar();
          move self to location;
          TocaDesde(self, true);
        }
      }
    ],
    mensaje_detener [;
      print "Dejas de oir ", (the) self, ".^";
    ],
    detener [ no_decir_nada;
      if (self.sonando) {
        if ((self hasnt concealed) && (self.jugador_oye())) {
          if (~~no_decir_nada) self.mensaje_detener();
        }
        self.parar_sonido();
        self.sonando = false;
        move self to cont_general;
      }
    ],
    before [;
      Examine, Listen:
        ! Escuchar un sonido es como examinar una cosa 
        ! muestra su descripción si es que existe
        if (self.jugador_oye()) {
          if (self provides description) {
            PrintOrRun(self, description);
            rtrue;
          } else {
            rfalse;
          }
        } else {
          "No puedes oir ", (the) self, " por aquí cerca.";
        }

      default:
        if (TestScope(self.origen)) {
          noun = self.origen;
          ! Esto es porque al hacer el truco de la línea anterior (de cambiar
          ! el noun por la cara) luego no se ejecuta la rutina before:
          return RunRoutines(noun, before);
        }
        "No puedes hacer eso con un sonido.";
    ],
  has static;


!===============================================================================
! OBJETOS
!===============================================================================


ContenidoSonoro cO1;
ContenidoSonoro cO2;
ContenidoSonoro cO3;
ContenidoSonoro cO4;
ContenidoSonoro cO5;
ContenidoSonoro cO6;
ContenidoSonoro cO7;
ContenidoSonoro cO8;
ContenidoSonoro cO9;
ContenidoSonoro cO10;


! Usado como base
Object cont_general
  class ContenidoSonoro
  with
    clase tipo_plano;

!
! Objetos que se usan en muchas funciones de la librería
!
Object tipo_plano
  class TipoDeSonido;


!===============================================================================
! FUNCIONES
!===============================================================================


! Tocador de sonidos desde objetos 'no-hablantes':
[ TocaDesde _modo no_decir_nada;
  cont_general.origen = _modo.origen;           ! El que sea
  cont_general.modo   = _modo;                  ! El que sea
  cont_general.texto  = _modo.frase;            ! Texto pedido
  cont_general.clase  = _modo;                  ! Clase dada
  if (_modo.intensidad == 0) {                  ! Si no han dicho nada
    cont_general.intensidad = NIVEL_NORMAL;     !   --> Intensidad media
  } else {                                      ! si sí:
    cont_general.intensidad = _modo.intensidad; !   --> la que hayan dicho
  }
  cont_general.se_puede_ver = TestScope(_modo.origen);

  return TocaSonido(cont_general, no_decir_nada);
];


! Transmisión recursiva de un sonido desde un objeto EN una habitación:
[ TocaSonido contenido no_decir_nada
  i inicial temp_i k enc_orig origen_aparente j hab_esc tipo _oido ind_oye
  retor;

  ! La primera vez crear el array
  RegistraTodo();

  retor = false;

  ! por comodidad de manejo
  tipo = contenido.clase;

  ! contenido.origen es el objeto que produce el sonido
  origen_aparente = contenido.origen;
  if (contenido.origen ofclass Lugar) inicial = contenido.origen;
  else                                inicial = parent(contenido.origen);

  ! Intentamos 'sacar' el sonido a un 'Lugar'
  enc_orig = false;

  while (inicial) {
    if (inicial ofclass Lugar) {
      ! Lo hemos logrado, propagar sonido desde ahí
      PropagaSonido(inicial, contenido.intensidad);
      contenido.se_escucha_en = origen_aparente;
      enc_orig = true;
      break;
    } else {
      ! Sólo cambiamos el origen aparente si el padre no es un soporte, no es
      ! transparente, ni un contenedor abierto
      if ((inicial hasnt supporter) && (inicial hasnt transparent) &&
          (inicial hasnt open)) {
        origen_aparente = inicial;
      }

      ! Seguimos escalando en los padres
      inicial = parent(inicial);

      ! Cada 'escalada' implica perder potencia
      if (origen_aparente has open) {
        contenido.intensidad = contenido.intensidad - coste_por_contenedor;
      } else {
        contenido.intensidad = contenido.intensidad -
                               coste_por_contenedor_cerrado;
      }
      
      ! Caso especial, nos saltamos al jugador como origen válido de los sonidos
      if (inicial == selfobj) inicial = location;
    }
  }

  if (enc_orig == false) {
    ! print_ret (The) contenido.origen, " no está en ningún sitio.^";
    return false;
  }

  ! Vemos si el jugador oye el sonido
  if ((location ofclass Lugar) && (location has en_ruta) &&
      (location.number > NIVEL_AUDIBLE)) {
    ! Buscamos un origen 'aparente', simplemente un Lugar cercano con 
    ! intensidad mayor
    enc_orig = false;
    objectloop (i in Compass) {
      k = ConduceSonidoA(i, location);
      ! print (name)i, "-", (name)k, "-", k.cantidad, "-", location.cantidad, " ";
      if (k && (k has en_ruta) && (k.number > location.number)) {
        ! Esta misma vale
        enc_orig = true;
        break;
      }
    }
    ! print "^";

    temp_i = contenido.intensidad;
    contenido.intensidad = location.number;
    ! print "De: ", (name)contenido.origen, "^";
    contenido.se_puede_ver = TestScope(contenido.origen);
    ! Si proviene de un 'Lugar' diferente ponemos el aparente
    ! si no, dejamos la fuente original aparente
    ! print "¿?: ", (name)inicial, " ", (name)i, "^";
    if (location ~= inicial) contenido.se_escucha_en = i;

    ! El jugador "escucha" el sonido:
    tipo.escuchado(contenido, no_decir_nada);
    
    ! print "*La intensidad es ", contenido.intensidad, "*";
    
    if (tipo ofclass Ruido) {
      tipo.tocar_sonido(VolumenSonido(contenido.intensidad));
    } else {
      Damusix.TocarV(tipo.emision,
                     VolumenSonido(contenido.intensidad));
    }

    retor = true;
    contenido.intensidad = temp_i;
  }

  ! Vemos si algún oyente oye el sonido
  for (ind_oye = 1 : ind_oye <= indice_oyentes : ind_oye++) {
    i = tabla_oyentes-->ind_oye;
    ! Si el objeto no está en un Lugar, escuchar desde 'fuera' del contenedor
    hab_esc = parent(i);
    
    while (hab_esc) {
      if (hab_esc ofclass Lugar) {
        break;
      } else {
        ! Cada 'escalada' implica perder potencia
        if (hab_esc has open) {
          contenido.intensidad = contenido.intensidad - coste_por_contenedor;
        }
        else {
          contenido.intensidad = contenido.intensidad -
                                 coste_por_contenedor_cerrado;
        }

        ! Seguimos escalando en los padres
        hab_esc = parent(hab_esc);
      }
    }

    if (hab_esc has en_ruta) {
      ! Sí que lo oye. Buscamos un origen 'aparente': simplemente un Lugar
      ! cercano con intensidad mayor
      if (hab_esc == parent(i)) {
        enc_orig = false;
        objectloop (j in Compass) {
          k = ConduceSonidoA(j, parent(i));
          if (k && (k has en_ruta) && (k.number > parent(i).number)) {
            ! Esta misma vale
            enc_orig = true;
            break;
          }
        }
      } else {
        ! Lamentablemente, si el oyente está en algo que no sea un Lugar,
        ! el origen será desconocido... simplemente 'fuera'
        enc_orig = false;
      }

      if (~~enc_orig) contenido.se_escucha_en = 0;
    
      temp_i = contenido.intensidad;
      contenido.intensidad = hab_esc.number;
      contenido.se_puede_ver = TestScope(contenido.origen, i);
      ! Si proviene de un 'Lugar' diferente ponemos el aparente
      ! si no, dejamos la fuente original aparente
      if (parent(i) ~= inicial) contenido.se_escucha_en = j;
      ! print (name)i, " -> ", contenido.intensidad, "^";
      i.apunta(contenido);
      contenido.intensidad = temp_i;
    }
  }

  ! Limpiar la propagación:
  LimpiarPropagacion();

  ! Ahora hacer que oigan los oyentes:
  for (ind_oye = 1 : ind_oye <= indice_oyentes : ind_oye++) {
    i = tabla_oyentes-->ind_oye;
    if (i has en_ruta) {
      _oido = ClonaContenido(contenido);
      _oido.intensidad = i.int_pend;
      _oido.se_puede_ver = i.se_pue_pend;
      _oido.se_escucha_en = i.se_esc_pend;
      !  print (name)i, " --> ", _oido.intensidad, " ", i.int_pend, "^";
      i.limpia();
      i.oido(_oido);
      DestruyeContenido();
    }
  }

  return retor;
];


[ VolumenSonido intensidad;
  if (intensidad < NIVEL_AUDIBLE)        return 0;
  else if (intensidad <= NIVEL_SUSURRO)  return 20;
  else if (intensidad <= NIVEL_MURMULLO) return 40;
  else if (intensidad <= NIVEL_NORMAL)   return 60;
  else if (intensidad <= NIVEL_ALTO)     return 80;
  else                                   return 100;
];


! Realiza el efecto de una voz lejana que se escucha siempre lejos
! reconocible o no:
[ VozLejana _texto _intensidad _tipo _origen _modo
  i ind_oye;
  ! Si el origen se deja vacío es que es desconocido

  ! Rellenamos el contenido de forma apropiada
  cont_general.origen        = _origen;       ! El que sea
  cont_general.modo          = _modo;         ! El que sea
  cont_general.se_puede_ver  = false;         ! Fuera de la vista
  cont_general.se_escucha_en = 0;             ! En un lugar desconocido
  cont_general.texto         = _texto;        ! Texto pedido
  if (_intensidad == 0) {                     ! Si no han dicho nada
    cont_general.intensidad = NIVEL_NORMAL;   !   --> Intensidad media
  } else {                                    ! sino
    cont_general.intensidad = _intensidad;    !   --> la que hayan dicho
  }
  if (_tipo == 0) cont_general.clase = tipo_plano;
  else            cont_general.clase = _tipo;

  ! Se toca con el sonido de clase base
  if (_tipo == 0) tipo_plano.escuchado(cont_general);
  else            _tipo.escuchado(cont_general);

  ! Avisar del sonido a todos los oyentes:
  for (ind_oye = 1 : ind_oye <= indice_oyentes : ind_oye++) {
    i = tabla_oyentes-->ind_oye;
    i.oido(ClonaContenido(cont_general));
    DestruyeContenido();
  }
];



!===============================================================================
! FUNCIONES AUXILIARES
!===============================================================================


! Proporciona la localidad en esa dirección a través de paredes que dejan pasar
! sonido:
[ DameLugarCercano este_lugar direccion lon
  i;
  if (~~(este_lugar provides cercano_a)) {
    ! No hay lugares cercanos
    return 0;
  }

  ! ver número de entradas
  lon = (este_lugar.#cercano_a) / WORDSIZE;
  ! print lon, "^";

  ! Recorrerlas en pares
  i = 0;
  while (lon > 0) {
    if (VR(este_lugar.&cercano_a-->i) == direccion) {
      if (lon > 1) return (VR(este_lugar.&cercano_a-->(i + 1)));
      else         return 0;
    }

    lon = lon - 2;
    i = i + 2;
  }

  return 0;
];


[ ConduceSonidoA direccion este_lugar
  k; ! tmp tmp2;
  sonido_tras_un_obstaculo = false;

  ! print "-ConduceSonidoA: ", (name) direccion, ":";
  if (este_lugar provides direccion.door_dir) { ! Conexion normal
    k = este_lugar.(direccion.door_dir);
  } else {                                      ! Pared que deja pasar el sonido
    if (este_lugar provides cercano_a)
      k = DameLugarCercano(este_lugar, direccion);
    else
      k = 0;

    if (~~k) return 0;
    sonido_tras_un_obstaculo = true;
  }

  if (ZRegion(k) == 2) { 
    ! print "Rutina que devuelve ";
    k = k();
  }
  if (ZRegion(k) ~= 1) { 
    ! print " no se puede pasar^";
    return 0;
  }

  if (k has door) {
    sonido_tras_un_obstaculo = (k hasnt open);
    k = k.(direccion.door_dir);
    ! print " que lleva a ";
  }
    
  ! print (name) k;
  if (~~(k ofclass Lugar)) {
    ! print " que no es un lugar válido.^";
    return 0;
  }

  ! print ".^";
  return k;
];


[ ClonaContenido _origen;
  ! print "[",indiceAnidamiento+1,"] ", (name)_origen.clase, " desde ", (name)_origen.origen, "^";
  if (indice_anidamiento < SONIDOS_MAX_NIVEL_ANIDAMIENTO) {
    indice_anidamiento++;
    (cont_oyente-->indice_anidamiento).clase         = _origen.clase;
    (cont_oyente-->indice_anidamiento).origen        = _origen.origen;
    (cont_oyente-->indice_anidamiento).modo          = _origen.modo;
    (cont_oyente-->indice_anidamiento).intensidad    = _origen.intensidad;
    (cont_oyente-->indice_anidamiento).se_puede_ver  = _origen.se_puede_ver;
    (cont_oyente-->indice_anidamiento).se_escucha_en = _origen.se_escucha_en;
    (cont_oyente-->indice_anidamiento).texto         = _origen.texto;
  }

  return (cont_oyente-->indice_anidamiento);
];


[ DestruyeContenido;
  if (indice_anidamiento > 0) indice_anidamiento--;
  ! print "[", indiceAnidamiento, "]^";
];


[ DireccionSonido d
  l dest;
  l = LugarReal();
  if (d ofclass CompassDirection) {
    dest = l.(d.door_dir);
    if (dest has door && dest has open) dest = dest.(d.door_dir);
  } else {
    dest = d;
  }
  DelMostrarNombre(dest);
!  if (dest provides del_nombre_direccion) dest.del_nombre_direccion();
!  else                                    print (del) dest;
];


[ LimpiarPropagacion
  ind_lug i;
  for (ind_lug = 1 : ind_lug <= indice_lugares : ind_lug++) {
    i = tabla_lugares-->ind_lug;
    if (i has en_ruta) {
      give i ~en_ruta;
      i.number = 0;
    }
  }
];


! Nueva función de Escuchar que 'oye' todos los ruidos:
[ ListenEspecial
  i contador ind_rud;
  contador = 0;
  if (noun == 0) {
    for (ind_rud = 1 : ind_rud <= indice_ruidos : ind_rud++) {
      ! print "<", ind_rud, ",", indice_ruidos, ">^";
      i = tabla_ruidos-->ind_rud;
      if (i.sonando && TocaDesde(i)) {
        ! Puede ser escuchado por el jugador
        contador++;
      }
    }
  } else {
    for (ind_rud = 1 : ind_rud <= indice_ruidos : ind_rud++) {
      i = tabla_ruidos-->ind_rud;
      if ((i.sonando) && (i.origen == noun)) {
        "Parece que ", (the) noun, " ", VR(i.verbo_emitir), " ", (the) i, ".";
      }
    }

    if (noun in Compass) {
      "Escuchas un poco por si ", (del) noun, " proviene algún sonido.";
    } else if (noun == selfobj) {
      "Te escuchas un rato, pero no oyes nada raro.";
    } else if (noun has animate) {
      "Escuchas un poco por si ", (el) noun, " dice algo.";
    } else if (noun ofclass Ruido && noun.sonando) {
      TocaDesde(noun);
      rtrue;
    } else {
      "Escuchas un poco por si ", (el) noun, " emite algún sonido.";
    }
  }

  ! print "[Escuchar con ", (name) noun, " y total ", contador, "]^";

  if (contador == 0) {
    ! Si no se escucha nada mensaje estandar de la librería
    L__M(##Listen, 1, noun);
  }
];


[ ListenNadaSub;
  ! print "[EscucharKK con ", (name) noun, "]^";
  ! L__M(##Listen, 1, noun);
  "No veo eso por aquí.";
];


[ ListenSub;
  ListenEspecial();
];


[ Locale obj;
  LocaleEspecial(obj);
];


! Nuevo Locale simplemente para enganchar un nuevo hook general de usuario justo
! antes de los mensajes:
[ LocaleEspecial descin text_without_ALSO text_with_ALSO
  o p num_objs must_print_ALSO;

  ! Nuevo hook
  TrataRuidos();

  objectloop (o in descin) give o ~workflag;
  num_objs = 0;
  objectloop (o in descin)
    if (o hasnt concealed && NotSupportingThePlayer(o)) {
      #Ifndef MANUAL_PRONOUNS;
      PronounNotice(o);
      #Endif;
      if (o has scenery) {
        if (o has supporter && child(o)) SayWhatsOn(o);
      }
      else {
        give o workflag; num_objs++;
        p = initial;
        if ((o has door or container) && o has open && o provides when_open) {
          p = when_open; jump Prop_Chosen;
        }
        if ((o has door or container) && o hasnt open && o provides when_closed) {
          p = when_closed; jump Prop_Chosen;
        }
        if (o has switchable && o has on && o provides when_on) {
          p = when_on; jump Prop_Chosen;
        }
        if (o has switchable && o hasnt on && o provides when_off) {
          p = when_off;
        }

      .Prop_Chosen;

        if (o.&describe && RunRoutines(o, describe)) {
          must_print_ALSO = true;
          give o ~workflag; num_objs--;
          continue;
        }
        if (o.p && (o hasnt moved || p ~= initial)) {
          new_line;
          PrintOrRun(o, p);
          must_print_ALSO = true;
          give o ~workflag; num_objs--;
          if (o has supporter && child(o)) SayWhatsOn(o);
        }
      }
    }

  if (num_objs == 0) return 0;

  if (actor ~= player) give actor concealed;
  if (text_without_ALSO) {
    new_line;
    if (must_print_ALSO) print (string) text_with_ALSO, " ";
    else                 print (string) text_without_ALSO, " ";
    WriteListFrom(child(descin),
      ENGLISH_BIT+RECURSE_BIT+PARTINV_BIT+TERSE_BIT+CONCEAL_BIT+WORKFLAG_BIT);
  }
  else {
    if (must_print_ALSO) L__M(##Look, 5, descin);
    else                 L__M(##Look, 6, descin);
  }
  if (actor ~= player) give actor ~concealed;
  return num_objs;
];


! Función para determinar si está al alcance del oído
[ Oible
  i ind_rud;
  ! print "[ Oible con ", scope_stage, "]^";
  switch (scope_stage) {
    1: rfalse;
    2: for (ind_rud = 1 : ind_rud <= indice_ruidos : ind_rud++) {
         i = tabla_ruidos-->ind_rud;
         PlaceInScope(i);
       }
  }
];


[ PropagaSonido habitacion _intensidad
  i k _nueva;
  ! print (name)habitacion, " con intensidad ", _intensidad, "^";

  if (habitacion ~= 0) {
    give habitacion en_ruta;
    habitacion.number = _intensidad;
    ! Propagamos a todos los Lugares
    objectloop (i in Compass) {
      k = ConduceSonidoA(i, habitacion);
      if (k) {
        if (sonido_tras_un_obstaculo) _nueva = _intensidad -
                                               coste_por_obstaculo;
        else                          _nueva = _intensidad -
                                               coste_por_habitacion;
        if ((k.number < _nueva) && (k ofclass Lugar)) PropagaSonido(k, _nueva);
      }
    }
  }
];


! Función que apunta todos los lugares:
[ RegistraLugares
  i;

  if (lugares_apuntados) return;

  objectloop (i ofclass Lugar) {
    if (indice_lugares < SONIDOS_MAX_NUM_LUGARES) {
      indice_lugares++;
      tabla_lugares-->indice_lugares = i;
    } else {
      print "ERROR: superado el límite de lugares; ", (the) i,
            " no podrá ser usada como lugar de sonidos.^";
    }
  }

  lugares_apuntados = true;
];


! Función necesaria para registrar oyentes
[ RegistraOyente _oyente;
  if (_oyente ofclass ObjetoOyente) {
    if (indice_oyentes < SONIDOS_MAX_NUM_OYENTES) {
      indice_oyentes++;
      tabla_oyentes-->indice_oyentes = _oyente;
      return true;
    }
  }
  return false;
];


[ RegistraOyentes
  i;
  if (oyentes_apuntados) return;
  objectloop (i ofclass ObjetoOyente) RegistraOyente(i);
  oyentes_apuntados = true;
];


! Función necesaria para registrar Ruidos
[ RegistraRuido _ruido;
  if (_ruido ofclass Ruido) {
    if (indice_ruidos < SONIDOS_MAX_NUM_RUIDOS) {
      indice_ruidos++;
      tabla_ruidos-->indice_ruidos = _ruido;
      return true;
    }
!    return false;
  }

  return false;
];


[ RegistraRuidos
  i;
  if (ruidos_apuntados) return;
  objectloop (i ofclass Ruido) RegistraRuido(i);
  ruidos_apuntados = true;
];


! Funcion de registro general
[ RegistraTodo;
  if (~~lugares_apuntados) RegistraLugares();
  if (~~ruidos_apuntados)  RegistraRuidos();
  if (~~oyentes_apuntados) RegistraOyentes();
];


!
! Tratamiento de ruidos colgando de Local
!
[ TrataRuidos;
  ! Nuevo hook
  RutinaPreLocal();

  ! Tratamiento de ruidos
  ! QQQ: de momento vacío no se me ocurre realmente nada que
  ! hacer con ella y que cumpla todos los requisitos
];


! Rutina que ejecuta rutinas si las encuentra
#ifndef VR;
[ VR valor;
  ! print "[VR: ", valor, " --> ", ZRegion(valor), " ]^";
  if (ZRegion(valor) == 2) return valor();
  else                     return valor;
];
#endif;


!
! Cambiar el Scope de 'Escuchar'
!
Extend 'escucha' replace
  *                   -> Listen
  * 'a//' noun        -> Listen
  * noun              -> Listen
  * 'a//' scope=Oible -> Listen
  * scope=Oible       -> Listen
  * topic             -> ListenNada;

