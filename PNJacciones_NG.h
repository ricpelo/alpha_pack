! Implementación de las "acciones" para la librería PNJactor
! Versión 1.0

System_file;

Constant PNJ_YA_MOVIDO = 10;
Constant PNJ_SIN_SALIDA = 11;
Constant PNJ_PUERTA_BLOQUEADA = 12;
Constant PNJ_PUERTA_CERRADA = 13;

! Accion COGER
!
[ PNJCoger quien que
  aux1 aux2 r;

  if (PuedeTocar(quien, que, 1)) {
    aux1 = action; aux2 = noun;
    action = ##Take; actor = quien; noun = que;
    r = RutinasAntesPNJ();
      
    if (r == false) {
      move que to quien;
      if (RutinasDespuesPNJ() == false)
        ! Mensaje de éxito
        if (SeVen(quien, player))
          print_ret (The) actor, " recoge ", (the) noun, ".";

      action = aux1;
      noun = aux2;
      rtrue;
    }
      
    action = aux1;
    noun = aux2;
  }
  rfalse;
];

!
! Acción DEJAR
!
[ PNJDejar quien que
  aux1 aux2 r;

  if (PuedeTocar(quien, que)) {
    if (CommonAncestor(quien, que) == quien) {
      aux1 = action; aux2 = noun; noun = que;
      action = ##Drop; actor = quien;
      r = RutinasAntesPNJ();
          
      if (r == false) {
        move que to parent(quien);
          if (RutinasDespuesPNJ() == false)
            ! Mensaje de éxito
            if (SeVen(quien, player))
              print_ret (The) actor, " deja ", (the) noun, ".";

        action = aux1;
        noun = aux2;
        rtrue;
      }
          
      action = aux1;
      noun = aux2;
    }
  }
  rfalse;
];

!
! Acción INVENTARIO
!
[ PNJInvAncho quien;
  inventory_style = FULLINV_BIT + ENGLISH_BIT + RECURSE_BIT;
  PNJInv(quien);
];

[ PNJInvAlto quien;
  inventory_style = FULLINV_BIT + INDENT_BIT + NEWLINE_BIT + RECURSE_BIT;
  PNJInv(quien);
];

[ PNJInv quien x
	aux1 aux2 r;

  aux1 = action; aux2 = noun;               ! Guardar para restaurar al final
  action = ##Inv;
  actor = quien;
  r = RutinasAntesPNJ();                   ! Ejecutar las rutinas "antesPNJ", etc...
  
  if (r == false) {                        ! Solo si retorna false proseguimos
    r = RutinasDespuesPNJ();
    
    if (r == false)  {                     ! Solo si retorna false emitimos nuestro
      if (SeVen(quien, player) == false)  ! mensaje por defecto
        rtrue;
        
      if (child(quien) == 0) {
        print (El_) quien, " te muestra sus manos vacías.^";
        rtrue;
      }
    
  	  if (inventory_style == 0)
	      return InvTallSub();

  	  print (El_) quien, "te dice -Llevo";

      if (inventory_style & NEWLINE_BIT ~= 0)
        print ":^";
      else
        print " ";

      WriteListFrom(child(quien), inventory_style, 1);
	  
      if (inventory_style & ENGLISH_BIT ~= 0) print ".^";
	  
      x = 0; ! To prevent a "not used" error

      ! Finalizada la acción, restauramos las variables y retornamos
      ! true, para indicar que se ha realizado con éxito
      action = aux1; noun = aux2;
      rtrue;
    }
  }
  
  ! Si la acción no pudo efectuarse, restaurar variables y retornar false
  action = aux1; noun = aux2;
  rfalse;
];

!
! Acción SOPLAR
!
[ PNJSoplar quien que ! Los parámetros de esta acción
   aux1 aux2 r;       ! Otras variables locales
 
  aux1 = action; aux2 = noun;    ! Guardar para restaurar al final

  ! 1 y 2) Comprobar que está al alcance y que se puede tocar
  if (PuedeTocar(quien, que)) {
    ! 3) Restricciones adicionales para esta acción
    !    Por ejemplo, a lo mejor queremos permitir sólo que se
    !    soplen objetos "soplables". O podríamos obligar a que sólo
    !    se puedan soplar los objetos de inventario...
    !    Si no se cumple la restricción podemos cargar un codigo de
    !    error apropiado en RazonErrorPNJ o bien emitir un mensaje
    !    (previa comprobacion de que SeVen(quien, jugador).
    !    Y seguidamente retornar false como indicador de que
    !    la acción ha fallado.

    ! En este caso, en la acción Soplar no vamos a imponer ninguna
    ! condición especial. Se puede Soplar cualquier objeto que se
    ! pueda tocar.
    ! 4) Preparar variables para ejecutar la acción ...

    action = ##Blow;        ! Soplar debe ser una accion
                              ! definida en la gramática, si no la
                              ! constante ##Soplar no estará definida
    actor = quien; noun = que;

    ! 4) ... y comprobar si algún objeto captura esta acción para
    !    impedirla

    r = RutinasAntesPNJ();    ! Ejecutar las rutinas "antesPNJ", etc...

    if (r == false) {         ! Solo si retorna false proseguimos
      ! 5) Llevar a cabo la acción "Soplar", cambiando el estado
      !    de los objetos que lo requieran. Por ejemplo, podríamos
      !    cambiar algún atributo o propiedad del objeto soplado.
      !
      !    En este ejemplo no haremos nada especial, pero otras
      !    acciones sí requieren código aquí. Por ejemplo Coger debe
      !    mover objetos al actor.

      ! 6) Una vez efectuada la acción, emitir un mensaje apropiado. Primero
      ! damos la oportunidad a los objetos de que lo emitan ellos mismos

      r = RutinasDespuesPNJ();
      
      if (r==false) {          ! Solo si retorna false emitimos nuestro mensaje por defecto
        ! 7) Emitir el mensaje de exito de esta acción
        if (SeVen(player, actor))  ! Si se ven, se imprime el mensaje
          print_ret (El_) actor, " sopla", (n) actor, " ", (the) noun, ".";
      }
      
      ! Finalizada la acción, restauramos las variables y retornamos
      ! true, para indicar que se ha realizado con éxito
      action = aux1; noun = aux2;
      rtrue;
    }
  }

  ! Si la acción no pudo efectuarse, restaurar variables y retornar false
  action = aux1; noun = aux2;
  rfalse;
];

!
! Acción IR
!
[ PNJIr amover direccion
  origen destino mensaje aux1 aux2 aux3 p;

  mensaje = 2;
  origen = parent(amover);
  destino = ConduceA(direccion, origen, CAMINO_CUALQUIERA);

  aux1 = action; aux2 = noun; aux3 = second;
  action = ##Go; actor = amover; noun = origen; second = direccion;

  if (RutinasAntesPNJ()) jump Exit; ! Rutinas antesPNJ del actor y la localidad de origen

  ! Rutinas antesPNJ de la localidad de destino
  if (destino provides antesPNJ) {
    noun = destino;
    if (RutinasAntesPNJ()) jump Exit;
  }

  ! Si llegamos hasta aquí, es porque ni el actor, ni la localidad de origen
  ! ni la de destino se quejan.
  
  if (destino == 0) {
    RazonErrorPNJ = PNJ_SIN_SALIDA;
!    amover.pnj_bloqueado();
    #ifdef DEBUG;
      if (parser_trace > 1)
        print "[MoverPNJDir bloqueado: la dirección no lleva a ningún sitio]^";
    #endif;
    jump Exit;
  }

  p = origen.(direccion.door_dir);

  if (ZRegion(p) == 2) p = p();

  if (p && p has door)	{
    ! pnj_abrir retorna: 2 para atravesar la puerta normalmente
    !                    1 para atravesar la puerta pero impedir
    !                    que se imprima el texto de
    !                    "marchar/llegar"
    !                    0 para impedir al PNJ que use esa puerta
    if (p provides pnj_abrir) {
      CapturarSalida();
      mensaje = p.pnj_abrir(amover);
      FinCapturarSalida();
      longitudCaptura = 0;
      
      if (mensaje == false) {
        RazonErrorPNJ = PNJ_PUERTA_BLOQUEADA;
!        amover.pnj_bloqueado();
        #ifdef DEBUG;
          if (parser_trace > 1)
            print "[MoverPNJDir bloqueado: El pnj_abrir ", (del) p, " retornó falso]^";
        #endif;
        jump Exit;
      }
    } else if (p hasnt open) {
      RazonErrorPNJ = PNJ_PUERTA_CERRADA;
!      amover.pnj_bloqueado();
      #ifdef DEBUG;
        if (parser_trace > 1)
          print "[MoverPNJDir bloqueado: ", (the) p, " está cerrad", (o) p,
                " y no tiene pnj_abrir]^";
      #endif;
      jump Exit;
    }
  }

  ! Movemos al PNJ:
  MoverPNJ(amover, destino, ##Go, direccion);

  noun = destino;
  
  if (RutinasDespuesPNJ() == false) {
    ! Mensaje de éxito  
    if (origen == location && mensaje == 2) {
      if (ZRegion(self.marcha) == 3)  ! Imprimir el texto
        print "^", (The) self, " ", (string) self.marcha, " ", (DirDada) direccion, ".^";
      else
        self.marcha(direccion);
    }

    if (parent(self) == location && mensaje == 2) {
      direccion = NULL;

      objectloop (destino in compass)
        if (ConduceA(destino, location, CAMINO_CUALQUIERA) == origen)
          direccion = destino;

     	if (ZRegion(self.llega) == 3) {
        print "^", (The) self, " ", (string) self.llega;
        if (direccion ~= NULL)
          print " desde ", (the) direccion;
        print ".^";
      } else
        self.llega(direccion);
    }
    action = aux1; noun = aux2; second = aux3;
    rtrue;
  }

.Exit;
  action = aux1; noun = aux2; second = aux3;
  rfalse;
];

