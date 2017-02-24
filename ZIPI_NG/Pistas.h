Include "Gipi.h";
ZIPI_Menu ZIPI_Menu0
with
  ZIPI_titulo "Sistema de pistas interactivas",
  ZIPI_item
    ZIPI_Menu1
    ZIPI_Separador
;
ZIPI_Menu ZIPI_Menu1
with
  ZIPI_titulo "Informaci�n general - Lee esto primero",
  ZIPI_informacion "Instrucciones b�sicas de inter�s si juegas a una aventura por primera vez",
  ZIPI_item
    ZIPI_Otro0
    ZIPI_Otro1
    ZIPI_Otro2
    ZIPI_Otro3
    ZIPI_Otro4
    ZIPI_Otro5
    ZIPI_Otro6
    ZIPI_Menu2
    ZIPI_Otro14
;
ZIPI_Menu ZIPI_Menu2
with
  ZIPI_titulo "Problemas y preguntas frecuentes",
  ZIPI_item
    ZIPI_Otro7
    ZIPI_Otro8
    ZIPI_Otro9
    ZIPI_Otro10
    ZIPI_Otro11
    ZIPI_Otro12
    ZIPI_Otro13
;
ZIPI_Otro ZIPI_Otro0
with
  ZIPI_titulo "�C�mo funciona este sistema de pistas?",
  ZIPI_ejecutar [;
       print "^- Te puedes desplazar por los men�s usando las flechas del cursor arriba y abajo. ";
       print "^- Para seleccionar una opci�n del men�, puedes pulsar las teclas ~Entrar~ o ~P~. ";
       print "^- Para volver al men� anterior, o salir del sistema de pistas, pulsa ~Q~. ";
       print "^- Cuando est�s leyendo pistas, pulsa ~P~ para ver la pista siguiente (si es que ";
       print "^  hay m�s), o pulsa ~Q~ para retroceder a la pantalla anterior. ";
  ];
ZIPI_Otro ZIPI_Otro1
with
  ZIPI_titulo "Acciones y �rdenes",
  ZIPI_ejecutar [;
       print "^El C�rculo es una aventura conversacional que te sumerge en un mundo de terror y ";
       print "^suspense donde t� eres el protagonista y con el que podr�s interactuar ";
       print "^introduciendo �rdenes con el teclado. ";
       print "^^";
       print "Dichas �rdenes tienen una sintaxis t�pica de �VERBO� o �VERBO NOMBRE�, aunque ";
       print "^pueden construirse �rdenes mucho m�s complejas si fuese necesario. ";
       print "^^";
       print "Algunos ejemplos: ";
       print "^^";
       print "> COGE LA CAJA DE HERRAMIENTAS ";
       print "^> ABRE EL ARMARIO ";
       print "^> COGE EL TRAJE ROJO Y PONTELO ";
       print "^> SACA TODO DE DENTRO DEL ARMARIO ";
       print "^^";
       print "Los verbos se pueden escribir en imperativo (�COGE�) o infinitivo (�COGER�), si bien ";
       print "^se recomienda la primera opci�n, m�s que nada porque resulta m�s corta. Y los ";
       print "^art�culos (�EL�, �LA�) se pueden poner, aunque no son necesarios. ";
  ];
ZIPI_Otro ZIPI_Otro2
with
  ZIPI_titulo "Algunas acciones frecuentes",
  ZIPI_ejecutar [;
       print "^Las siguientes son algunas de las acciones que se utilizan de manera m�s frecuente en ";
       print "^una aventura conversacional: ";
       print "^^";
       print "COGE tal . . . .: Coger el objeto 'tal'. ";
       print "^DEJA tal . . . .: Dejar el objeto 'tal'. ";
       print "^ABRE tal . . . .: Abrir 'tal' (normalmente una puerta o alg�n recipiente cerrado). ";
       print "^CIERRA tal . . .: Cerrar 'tal' (normalmente una puerta o alg�n recipiente abierto). ";
       print "^METE tal EN cual: Meter el objeto 'tal' dentro de 'cual' (por ejemplo, una caja). ";
       print "^SACA tal DE cual: Sacar el objeto 'tal' de dentro de 'cual' (por ejemplo, una caja). ";
       print "^PONTE tal . . . : Ponerse el objeto 'tal' (normalmente una prenda de ropa). ";
       print "^QUITATE tal . . : Quitarse el objeto 'tal' (normalmente una prenda de ropa). ";
       print "^ENTRA . . . . . : Entrar dentro de alg�n sitio, posiblemente a trav�s de una puerta. ";
       print "^SAL . . . . . . : Salir de alg�n sitio, posiblemente a trav�s de una puerta. ";
       print "^EXAMINA tal (o ";
       print "^EX tal o X tal) : Describir el objeto 'tal' con detalle. ";
       print "^INVENTARIO (o I): Mostrar un listado con los objetos que posees actualmente. ";
  ];
ZIPI_Otro ZIPI_Otro3
with
  ZIPI_titulo "Moverse por el mundo",
  ZIPI_ejecutar [;
       print "^El mundo de El C�rculo est� compuesto por multitud de recintos interconectados entre ";
       print "^s� llamados localidades. Las salidas desde tu localidad actual se muestran siempre ";
       print "^a continuaci�n de la descripci�n de dicha localidad. Para moverte de una localidad ";
       print "^a otra, simplemente teclea la salida correspondiente. ";
       print "^^";
       print "Por ejemplo: supongamos que tu localidad actual tiene como salidas �norte� y ";
       print "^�afuera�. En ese caso, para moverte hacia la localidad situada al norte de la ";
       print "^actual, puedes teclear las �rdenes NORTE, VETE AL NORTE, IR AL NORTE o simplemente ";
       print "^N. Y para salir fuera, puedes teclear SAL, SALIR o FUERA. Los puntos cardinales se ";
       print "^pueden abreviar a N, S, E, O, NE, SE, NO y SO. ";
  ];
ZIPI_Otro ZIPI_Otro4
with
  ZIPI_titulo "Otros personajes del juego",
  ZIPI_ejecutar [;
       print "^A veces, ser� necesario enviar �rdenes a alg�n personaje (por ejemplo, al Mayor ";
       print "^Jason). En tal caso, deber�s indicar el nombre del personaje seguido de una coma, ";
       print "^y a continuaci�n la orden que deseas enviarle: ";
       print "^^";
       print "> JASON, COGE TU TRAJE ";
       print "^> JASON, SIGUEME ";
       print "^^";
       print "Para hablar con un personaje, o bien preguntarle algo, se usa la misma sintaxis: ";
       print "^^";
       print "> JASON, HABLAME DE TI ";
       print "^> JASON, QUE OPINAS SOBRE EL PLANETA ";
       print "^^";
       print "Saber trabajar conjuntamente con los personajes es crucial para alcanzar el �xito ";
       print "^en esta aventura. ";
  ];
ZIPI_Otro ZIPI_Otro5
with
  ZIPI_titulo "Algunos comandos �tiles",
  ZIPI_ejecutar [;
       print "^SONIDO SI/NO . . . . : Activar o desactivar todos los efectos sonoros. ";
       print "^SONIDO FONDO SI/NO . : Activar o desactivar el sonido de fondo. ";
       print "^SONIDO DETECTOR SI/NO: Activar o desactivar el sonido del detector de movimiento. ";
       print "^GRAFICOS SI/NO . . . : Activar o desactivar la visualizaci�n de gr�ficos. ";
       print "^GRAFICOS PEQUE�OS/ ";
       print "^  MEDIANOS/GRANDES . : Cambia el tama�o de la ventana de gr�ficos (por omisi�n ";
       print "^                       se mostrar�n en tama�o MEDIANO). ";
       print "^CONFIGURACION . . . .: Muestra un men� de opciones de gr�ficos y sonido. ";
       print "^SALIDAS (o X) . . . .: Mostrar las salidas visibles en la localidad actual. ";
       print "^SALIDAS SI/NO . . . .: Mostrar (o no) autom�ticamente las salidas visibles en ";
       print "^                       cada localidad. ";
       print "^SAVE (o GUARDAR) . . : Almacenar la situaci�n actual en un archivo externo, para ";
       print "^                       poder volver a �l en cualquier momento. ";
       print "^LOAD (o RECUPERAR) . : Recuperar una situaci�n previamente guardada con SAVE o ";
       print "^                       GUARDAR. ";
       print "^AYUDA . . . . . . . .: Por si alguna vez te encuentras atascado. ";
       print "^REINICIAR . . . . . .: Volver a empezar desde el principio. ";
       print "^ACABAR (o FIN) . . . : Salir de la aventura (cuidado: no guarda la situaci�n actual). ";
  ];
ZIPI_Otro ZIPI_Otro6
with
  ZIPI_titulo "Consejos y ayudas",
  ZIPI_ejecutar [;
       print "^- Examina todo lo que encuentres. Varias veces si hace falta. ";
       print "^- Recoge todos los objetos que puedas; podr�an serte de utilidad. ";
       print "^- Habla con los dem�s personajes de la aventura. A veces te dar�n pistas �tiles. ";
       print "^- El azar es parte de esta aventura. Tenlo en cuenta a la hora de llevar a cabo ";
       print "^  determinadas acciones. ";
       print "^- Graba frecuentemente tu situaci�n con la orden GUARDAR. Nunca se sabe lo que ";
       print "^  puedes encontrarte tras esa esquina... ";
       print "^- Si te encuentras con alg�n fallo o no sabes c�mo avanzar, puedes plantear tus ";
       print "^  dudas en el foro del CAAD o en alpha.aventuras@@64gmail.com. ";
  ];
ZIPI_Otro ZIPI_Otro7
with
  ZIPI_titulo "Al jugar la aventura, las letras se ven feas o con colores extra�os",
  ZIPI_ejecutar [;
       print "^a) [Si usas otro sistema que no sea Windows] Aseg�rate de tener instalada ";
       print "^   la �ltima versi�n de Gargoyle (al menos, la 2011.1). Los usuarios de ";
       print "^   Windows que instalen la aventura con el programa de instalaci�n no ";
       print "^   necesitan comprobar esto, ya que Gargoyle se instala con la aventura. ";
       print "^ ";
       print "^b) Aseg�rate de que exista el archivo 'garglk.ini' en la misma carpeta que ";
       print "^   la aventura. Para ello: ";
       print "^ ";
       print "^    i. Si usas Linux, y has instalado la aventura usando el paquete .deb, ";
       print "^       comprueba si existe el archivo en esta ruta: ";
       print "^ ";
       print "^         /usr/share/games/alphaaventuras-elcirculo/garglk.ini ";
       print "^ ";
       print "^   ii. Si estas usando la versi�n multiplataforma, comprueba simplemente ";
       print "^       que el archivo 'garglk.ini' se encuentra en la misma carpeta que la ";
       print "^       aventura 'elcirculo.blb', junto al resto de los archivos incluidos. ";
       print "^ ";
       print "^Si todo lo anterior no funciona, instala de nuevo la aventura. ";
  ];
ZIPI_Otro ZIPI_Otro8
with
  ZIPI_titulo "Las letras se ven demasiado grandes o demasiado peque�as",
  ZIPI_ejecutar [;
       print "^Puedes cambiar el tama�o de las letras editando el archivo 'garglk.ini': ";
       print "^ ";
       print "^a) En Windows estar� en la ruta: ";
       print "^   @@92Program Files@@92Juegos Conversacionales@@92El c�rculo@@92 ";
       print "^ ";
       print "^b) En Linux, si instalaste la aventura con el paquete .deb, estar� en: ";
       print "^   /usr/share/games/alphaaventuras-elcirculo/ ";
       print "^ ";
       print "^c) En la versi�n multiplataforma lo encontrar�s en la misma ruta que el resto ";
       print "^   de los archivos distribuidos con la aventura. ";
       print "^ ";
       print "^Cuando hayas abierto el archivo 'garglk.ini' con tu editor de textos favorito, ";
       print "^localiza las l�neas que empiecen por 'monosize' y 'propsize'. Para ajustar el ";
       print "^tama�o de las letras, simplemente cambia los n�meros que aparezcan al lado. ";
       print "^Poniendo n�meros mayores, las letras se ver�n m�s grandes, y viceversa. Aseg�rate ";
       print "^de que el 'propsize' sea siempre un poco mayor que el 'monosize'. ";
       print "^ ";
       print "^Guarda los cambios en el mismo archivo y ejecuta de nuevo la aventura. ";
  ];
ZIPI_Otro ZIPI_Otro9
with
  ZIPI_titulo "Los gr�ficos se ven demasiado grandes y es inc�modo jugar as�",
  ZIPI_ejecutar [;
       print "^a) Ampl�a el tama�o de la ventana del juego, maximiz�ndola hasta ocupar ";
       print "^   todo el escritorio. ";
       print "^ ";
       print "^b) Configura la aventura para que use gr�ficos m�s peque�os, usando el ";
       print "^   comando siguiente dentro del juego: ";
       print "^ ";
       print "^   > CONFIGURACI�N ";
       print "^    ";
       print "^   Una vez dentro, cambia el tama�o de los gr�ficos a uno peque�o (entre ";
       print "^   200 y 350 p�xeles puede resultar m�s adecuado para tu pantalla). ";
       print "^ ";
       print "^c) Si lo anterior no fuera suficiente, siempre puedes desactivar los gr�ficos ";
       print "^   completamente, aunque no te lo recomendamos, ya que con ello perder�as ";
       print "^   gran parte del atractivo de la aventura. Para ello, ejecuta el comando ";
       print "^   siguiente dentro del juego: ";
       print "^ ";
       print "^   > GR�FICOS NO ";
  ];
ZIPI_Otro ZIPI_Otro10
with
  ZIPI_titulo "Los gr�ficos se ven demasiado peque�os",
  ZIPI_ejecutar [;
       print "^Prueba lo siguiente: ";
       print "^ ";
       print "^ a) Ampl�a el tama�o de la ventana del juego, maximiz�ndola hasta ocupar ";
       print "^    todo el escritorio. Si la ventana es demasiado peque�a, los gr�ficos ";
       print "^    se reescalar�n autom�ticamente para que quepan completos dentro de la ";
       print "^    misma, pero se ver�n muy peque�os. ";
       print "^ ";
       print "^ b) Configura la aventura para que use gr�ficos m�s grandes, usando uno de ";
       print "^    los comandos siguientes dentro del juego: ";
       print "^ ";
       print "^    > GR�FICOS MEDIANOS ";
       print "^        (esto har� que la aventura use los gr�ficos a tama�o mediano) ";
       print "^ ";
       print "^    > GR�FICOS GRANDES ";
       print "^        (esto har� que la aventura use los gr�ficos al tama�o m�s grande) ";
       print "^ ";
       print "^    > CONFIGURACI�N ";
       print "^        (mostrar� un men� donde podr�s cambiar el tama�o con m�s detalle) ";
  ];
ZIPI_Otro ZIPI_Otro11
with
  ZIPI_titulo "�No veo gr�ficos en ning�n momento!",
  ZIPI_ejecutar [;
       print "^Comprueba lo siguiente: ";
       print "^ ";
       print "^ a) Aseg�rate de tener activados los gr�ficos dentro del juego. Para ello, ";
       print "^    usa el comando siguiente dentro del juego: ";
       print "^ ";
       print "^    > GR�FICOS SI ";
       print "^ ";
       print "^    Ten en cuenta tambi�n que los gr�ficos se apagan autom�ticamente en ";
       print "^    ciertas partes de la aventura, para que el texto ocupe toda la ventana ";
       print "^    de juego. ";
       print "^ ";
       print "^ b) Aseg�rate de que no est�s en un lugar a oscuras y sin ninguna fuente de ";
       print "^    luz, como una linterna encendida. ";
  ];
ZIPI_Otro ZIPI_Otro12
with
  ZIPI_titulo "�No oigo nada!",
  ZIPI_ejecutar [;
       print "^Si no oyes m�sica ni sonidos durante el juego, comprueba lo siguiente: ";
       print "^ ";
       print "^ a) Aseg�rate de tener activados los sonidos dentro del juego. Para ello, ";
       print "^    usa el comando siguiente dentro del juego: ";
       print "^ ";
       print "^    > SONIDO SI ";
       print "^ ";
       print "^ b) Si no oyes el sonido de fondo, aseg�rate de tenerlo activado usando el ";
       print "^    comando siguiente: ";
       print "^ ";
       print "^    > SONIDO FONDO SI ";
  ];
ZIPI_Otro ZIPI_Otro13
with
  ZIPI_titulo "Estoy atascado y no s� c�mo continuar",
  ZIPI_ejecutar [;
       print "^Si andas perdido y no sabes c�mo resolver alg�n puzzle o situaci�n: ";
       print "^ ";
       print "^ a) Usa el sistema de pistas interactivas, tecleando: ";
       print "^ ";
       print "^      > PISTAS ";
       print "^ ";
       print "^    en cualquier momento del juego. ";
       print "^ ";
       print "^ b) Pregunta en el foro del CAAD, en la siguiente direcci�n: ";
       print "^ ";
       print "^      http://foro.caad.es ";
       print "^ ";
       print "^ c) Contacta con nosotros por alguna de las las estas v�as: ";
       print "^ ";
       print "^    - email: alpha.aventuras@@64gmail.com ";
       print "^    - web: http://www.alpha-aventuras.es ";
       print "^    - facebook: https://www.facebook.com/alphaaventuras ";
       print "^    - wikiCAAD: http://wiki.caad.es/Alpha_Aventuras ";
  ];
ZIPI_Otro ZIPI_Otro14
with
  ZIPI_titulo "Cr�ditos",
  ZIPI_ejecutar [;
       print "EL C�RCULO (c) 2012 Alpha Aventuras^ ";
       print "^- PROGRAMACI�N Y DISE�O: ";
       print "^     Ricardo P�rez L�pez (Sothoth) ";
       print "^- GR�FICOS: ";
       print "^     Manuel Mill�n Ruiz (Ubik) ";
       print "^- TEXTOS: ";
       print "^     Ricardo P�rez L�pez ";
       print "^- MAPEADO Y DOCUMENTACI�N: ";
       print "^     Ricardo P�rez L�pez ";
       print "^- PRUEBAS: ";
       print "^     Ricardo P�rez L�pez ";
       print "^     dddddd^ ";
       print "^  Para m�s informaci�n, usa el comando CR�DITOS durante el juego. ";
  ];


![ ZIPI_Intro;
!  print "Despl�zate con las flechas del cursor. Pulsa ", (s_input) "Enter", " para seleccionar, ", (s_input) "P", " para continuar, o ", (s_input) "Q", " para salir.";
!];


[ ZIPI_Empezar ;
!  ZIPI_InitGlk();
!  clearMainWindow();
!  ZIPI_Intro();
!  barra_estado.numero_lineas = 26;
!  barra_estado.dibujar();
  ZIPI_RunMenu(ZIPI_Menu0, true);
!  barra_estado.numero_lineas = 1;
!  barra_estado.dibujar();
];
