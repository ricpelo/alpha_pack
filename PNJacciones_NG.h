! Implementaci�n de las "acciones" para la librer�a PNJactor
! Versi�n 1.0

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
    aux1 = accion; aux2 = uno;
    accion = ##Coger; actor = quien; uno = que;
    r = RutinasAntesPNJ();
      
    if (r == false) {
      move que to quien;
      if (RutinasDespuesPNJ() == false)
        ! Mensaje de �xito
        if (SeVen(quien, jugador))
          print_ret (_El) actor, " recoge ", (el) uno, ".";

      accion = aux1;
      uno = aux2;
      rtrue;
    }
      
    accion = aux1;
    uno = aux2;
  }
  rfalse;
];

!
! Acci�n DEJAR
!
[ PNJDejar quien que
  aux1 aux2 r;

  if (PuedeTocar(quien, que)) {
    if (AntepasadoComun(quien, que) == quien) {
      aux1 = accion; aux2 = uno; uno = que;
      accion = ##Dejar; actor = quien;
      r = RutinasAntesPNJ();
          
      if (r == false) {
        move que to parent(quien);
          if (RutinasDespuesPNJ() == false)
            ! Mensaje de �xito
            if (SeVen(quien, jugador))
              print_ret (_El) actor, " deja ", (el) uno, ".";

        accion = aux1;
        uno = aux2;
        rtrue;
      }
          
      accion = aux1;
      uno = aux2;
    }
  }
  rfalse;
];

!
! Acci�n INVENTARIO
!
[ PNJInvAncho quien;
  estilo_inventario = INFOTOTAL_BIT + ESPANOL_BIT + RECURSIVO_BIT;
  PNJInv(quien);
];

[ PNJInvAlto quien;
  estilo_inventario = INFOTOTAL_BIT + INDENTAR_BIT + NUEVALINEA_BIT + RECURSIVO_BIT;
  PNJInv(quien);
];

[ PNJInv quien x
	aux1 aux2 r;

  aux1 = accion; aux2 = uno;               ! Guardar para restaurar al final
  accion = ##Inv;
  actor = quien;
  r = RutinasAntesPNJ();                   ! Ejecutar las rutinas "antesPNJ", etc...
  
  if (r == false) {                        ! Solo si retorna false proseguimos
    r = RutinasDespuesPNJ();
    
    if (r == false)  {                     ! Solo si retorna false emitimos nuestro
      if (SeVen(quien, jugador) == false)  ! mensaje por defecto
        rtrue;
        
      if (child(quien) == 0) {
        print (El_) quien, " te muestra sus manos vac�as.^";
        rtrue;
      }
    
  	  if (estilo_inventario == 0)
	      return InvAltoSub();

  	  print (El_) quien, "te dice -Llevo";

      if (estilo_inventario & NUEVALINEA_BIT ~= 0)
        print ":^";
      else
        print " ";

      EscribirListaDesde(child(quien), estilo_inventario, 1);
	  
      if (estilo_inventario & ESPANOL_BIT ~= 0) print ".^";
	  
      x = 0; ! To prevent a "not used" error

      ! Finalizada la acci�n, restauramos las variables y retornamos
      ! true, para indicar que se ha realizado con �xito
      accion = aux1; uno = aux2;
      rtrue;
    }
  }
  
  ! Si la acci�n no pudo efectuarse, restaurar variables y retornar false
  accion = aux1; uno = aux2;
  rfalse;
];

!
! Acci�n SOPLAR
!
[ PNJSoplar quien que ! Los par�metros de esta acci�n
   aux1 aux2 r;       ! Otras variables locales
 
  aux1 = accion; aux2 = uno;    ! Guardar para restaurar al final

  ! 1 y 2) Comprobar que est� al alcance y que se puede tocar
  if (PuedeTocar(quien, que)) {
    ! 3) Restricciones adicionales para esta acci�n
    !    Por ejemplo, a lo mejor queremos permitir s�lo que se
    !    soplen objetos "soplables". O podr�amos obligar a que s�lo
    !    se puedan soplar los objetos de inventario...
    !    Si no se cumple la restricci�n podemos cargar un codigo de
    !    error apropiado en RazonErrorPNJ o bien emitir un mensaje
    !    (previa comprobacion de que SeVen(quien, jugador).
    !    Y seguidamente retornar false como indicador de que
    !    la acci�n ha fallado.

    ! En este caso, en la acci�n Soplar no vamos a imponer ninguna
    ! condici�n especial. Se puede Soplar cualquier objeto que se
    ! pueda tocar.
    ! 4) Preparar variables para ejecutar la acci�n ...

    accion = ##Soplar;        ! Soplar debe ser una accion
                              ! definida en la gram�tica, si no la
                              ! constante ##Soplar no estar� definida
    actor = quien; uno = que;

    ! 4) ... y comprobar si alg�n objeto captura esta acci�n para
    !    impedirla

    r = RutinasAntesPNJ();    ! Ejecutar las rutinas "antesPNJ", etc...

    if (r == false) {         ! Solo si retorna false proseguimos
      ! 5) Llevar a cabo la acci�n "Soplar", cambiando el estado
      !    de los objetos que lo requieran. Por ejemplo, podr�amos
      !    cambiar alg�n atributo o propiedad del objeto soplado.
      !
      !    En este ejemplo no haremos nada especial, pero otras
      !    acciones s� requieren c�digo aqu�. Por ejemplo Coger debe
      !    mover objetos al actor.

      ! 6) Una vez efectuada la acci�n, emitir un mensaje apropiado. Primero
      ! damos la oportunidad a los objetos de que lo emitan ellos mismos

      r = RutinasDespuesPNJ();
      
      if (r==false) {          ! Solo si retorna false emitimos nuestro mensaje por defecto
        ! 7) Emitir el mensaje de exito de esta acci�n
        if (SeVen(jugador, actor))  ! Si se ven, se imprime el mensaje
          print_ret (El_) actor, " sopla", (n) actor, " ", (el) uno, ".";
      }
      
      ! Finalizada la acci�n, restauramos las variables y retornamos
      ! true, para indicar que se ha realizado con �xito
      accion = aux1; uno = aux2;
      rtrue;
    }
  }

  ! Si la acci�n no pudo efectuarse, restaurar variables y retornar false
  accion = aux1; uno = aux2;
  rfalse;
];

!
! Acci�n IR
!
[ PNJIr amover direccion
  i j p mensaje aux1 aux2 aux3 r fin;
    
  mensaje = 2;
  p = parent(amover);
  i = ConduceA(direccion, p, CAMINO_CUALQUIERA);
  fin = false;

  if (i == 0) {
    RazonErrorPNJ = PNJ_SIN_SALIDA;
!    amover.pnj_bloqueado();
    #ifdef DEBUG;
    if (parser_trace > 1)
      print "[MoverPNJDir bloqueado: la direcci�n no lleva a ning�n sitio]^";
    #endif;
    rfalse;
  }
    
  j = p.(direccion.direcc_puerta);

  if (ZRegion(j) == 2)
    j = j();

  if (j)
    if (j has puerta)	{
      ! pnj_abrir retorna: 2 para atravesar la puerta normalmente
      !                    1 para atravesar la puerta pero impedir
      !                    que se imprima el texto de
      !                    "marchar/llegar"
      !                    0 para impedir al PNJ que use esa puerta
      if (j provides pnj_abrir) {
        CapturarSalida();
        mensaje = j.pnj_abrir(amover);
        FinCapturarSalida();
        
        if (mensaje == false) {
          RazonErrorPNJ = PNJ_PUERTA_BLOQUEADA;
!          amover.pnj_bloqueado();
          #ifdef DEBUG;
          if (parser_trace > 1)
            print "[MoverPNJDir bloqueado: ", (el) j, "'s pnj_abrir retorn� falso]^";
          #endif;
          fin = true;
!          rfalse;
        }
      } else if (j hasnt abierto) {
        RazonErrorPNJ = PNJ_PUERTA_CERRADA;
!        amover.pnj_bloqueado();
        #ifdef DEBUG;
        if (parser_trace > 1)
          print "[MoverPNJDir bloqueado: ", (el) j, " est� cerrad", (o)j, " y no tiene pnj_abrir]^";
        #endif;
        fin = true;
!        rfalse;
      }
    }

  aux1 = accion; aux2 = uno; aux3 = otro;
  accion = ##Ir; actor = amover; uno = i; otro = direccion;
  r = RutinasAntesPNJ();                  ! Rutinas antesPNJ del actor y la localidad de destino

  if (r) {
    RazonErrorPNJ = PNJ_IMPIDE_ANTES;
    accion = aux1;
    uno = aux2;
    otro = aux3;
    rfalse;    
  }

  if (r == false) {
    ! Rutinas antesPNJ de la localidad de origen
    if (p provides antesPNJ) {
      CapturarSalida();
      r = ImprimirOEjecutar(p, antesPNJ);
      FinCapturarSalida();

      if (longitudcaptura > 0)
        if (SeVen(actor, jugador))
          MostrarSalidaCapturada();

      if (r) {
        RazonErrorPNJ = PNJ_IMPIDE_ANTES;
        accion = aux1;
        uno = aux2;
        otro = aux3;
        rfalse;
      }
    }
  }

  if (r == false) {
    if (fin) {
!      if (longitudcaptura > 0)
!        if (SeVen(actor, jugador))
!          MostrarSalidaCapturada();
      rfalse;
    }

    MoverPNJ(amover, i, ##Ir, direccion);
    uno = p;

    if (RutinasDespuesPNJ() == false) {
      ! Mensaje de �xito  
      if (p == localizacion && mensaje == 2) {
        if (ZRegion(self.marcha) == 3)  ! Imprimir el texto
          print "^", (The) self, " ", (string) self.marcha, " ", (DirDada) direccion, ".^";
        else
          self.marcha(direccion);
      }

      if (parent(self) == localizacion && mensaje == 2) {
        direccion = NULL;

        objectloop (i in brujula)
          if (ConduceA(i, localizacion, CAMINO_CUALQUIERA) == p)
            direccion = i;

       	if (ZRegion(self.llega) == 3) {
          print "^", (The) self, " ", (string) self.llega;
          if (direccion ~= NULL)
            print " desde ", (el) direccion;
          print ".^";
        } else
          self.llega(direccion);
      }
    }
      
    accion = aux1;
    uno = aux2;
    otro = aux3;
    rtrue;
  }
    
  accion = aux1;
  uno = aux2;
  otro = aux3;
  rfalse;
];

