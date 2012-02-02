# ZIPI
# ================================================================
# Z-InvisiPIstas
#
# (c) 1999 Zak McKraken
#
# Documentaci�n en Zipi.doc
#
BEGIN{
  menus=-1;
  hint=-1;
  textos=-1;
  print "Include \"Gipi.h\";";
}
{
  gsub("\"", "~");
  gsub("@", "@@64");
  gsub("\\\\","@@92");
}
/<INTRO>/{
  if (menus!=-1) {
    print "ERROR L"NR": <INTRO> Debe aparecer antes que los menus."\
      > "/dev/stderr";
    exit(-1);
  }
  intro=1;
  hay_intro=1;
  print "[ ZIPI_Intro ; ";
  next;
}
/<\/INTRO>/{
  intro=0;
  print "!  print \"[Pulsa 'T' para ejecutar en modo texto (las teclas no funcionar�n)]^\";\n"\
    "!  print \"[o cualquier otra tecla para ejecutar en modo menu manejado por teclas]^\";\n"\
    "!  ZIPI_modo=0;\n!  tecla=ZIPI_tecla();  if (tecla=='T' or 't') ZIPI_modo=1;\n"\
    "];\n";
  next;
}
/<TEXTO>/{
  textos++;
  split($0,a,"<TEXTO>");
  titulootro[textos]=a[2];
  lineas[textos]=0;
#  print "! Nivel=" nivel;
#  print "! Menu=" menunivel[nivel];
  m=menunivel[nivel];
#  print "! Opcion=" opciones[m];
  item[m, opciones[m]]="ZIPI_Otro" textos;
  opciones[m]++;
  otro=1;
  next;
}
/<\/TEXTO>/{
  if (center) {
    print "WARNING L"NR": <CENTRA> sin cerrar, lo cierro por defecto."\
      >     "/dev/stderr";
    center=0;
  }
  otro=0;
  next;
}
/<NADA>/{
  m=menunivel[nivel];
  item[m, opciones[m]]="ZIPI_Separador";
  opciones[m]++;
  nada=1;
  next;
}
/<CENTRA>/{
  if ((!intro)&&(!otro)) {
    print "ERROR L"NR": <CENTRA> Solo esta permitido dentro de <INTRO> o <TEXTO>"\
      > "/dev/stderr";
    exit(-1);
  }
  center=1;
  next;
}
/<\/CENTRA>/{
  if ((!intro)||(!center)) {
    print "ERROR L"NR": </CENTRA> fuera de lugar"\
      > "/dev/stderr";
    exit(-1);
  }
  center=0;
  next;
}
/<MENU>/{
  if ((otro)||(intro)){
    print "ERROR L"NR": <MENU> no puede usarse dentro de <TEXTO> o <INTRO>"\
      > "/dev/stderr";
    exit(-1);
  }
  menus++;
  split($0,a,"<MENU>");
  titulomenu[menus]=a[2];
  opciones[menus]=0;
  nivel++;
  menunivel[nivel]=menus;
  if (nivel!=0) {
    previo=menunivel[nivel-1];
#    print "! Opciones["previo"]="opciones[previo];
#    opcion[previo, opciones[previo]]=a[2];
    item[previo, opciones[previo]]="ZIPI_Menu"menus;
    opciones[previo]++;
  }
  next;
}
/<\/MENU>/{
  if (nivel==0) {
    print "ERROR L"NR": </MENU> fuera de lugar.">"/dev/stderr";
    exit(-1);
  }
  nivel--;
  next;
}
/<PREG>/{
  hint++;
  split($0,a,"<PREG>");
  titulohint[hint]=a[2];
  m=menunivel[nivel];
  if (opciones[m]==32) {
    print "ERROR L" NR ": M�s de 32 pistas en un menu.">"/dev/stderr";
    exit(-1);
  }
#  opcion[m,opciones[m]]=a[2];
  item[m, opciones[m]]="ZIPI_Pista"hint;
  opciones[m]++;
  pistas[hint]=0;
  next;
}
/<PISTA>/{
  split($0,a,"<PISTA>");
  pista[hint, pistas[hint]]=a[2];
  pistas[hint]++;
  next;
}
{
  if (center){
    print "  ZIPI_Center(\""$0"\", " length($0)");";
    next;
  }
  if (intro) {
    if ($0=="") 
      print "print \"^^\";";
    else
      print "  print \""$0" \";";
    next;
  }
  if (nada) {
    nada = 0;
    next;
  }
  if (otro) {
    t=    substr($0, match($0, "[^ \t]"));
    if (t~"<NOFONT>"){
      texto[textos, lineas[textos]++]="font off;";
      nofont=1;
      next;
    }
    else if (t~"</NOFONT>"){
      texto[textos, lineas[textos]++]="font on;";
      nofont=0;
      next;
    }
    else if (center)
      texto[textos, lineas[textos]]="ZIPI_Center(\""t"\", "length(t)");";
    else{
      if (($0~"^[ \t]*$")&&(!nofont)) t="^^\";";
      if (nofont) t=$0"^\";";
      else if (t!="^^\";") t=t" \";";
      texto[textos, lineas[textos]]="print \""t;
    }
    lineas[textos]++;
    next;
  }
  if (!($0~("^[ \t]*$"))) {
	print "WARNING: la l�nea "NR" ser� considerada continuaci�n de la l�nea anterior" \
	> "/dev/stderr";
      for (i=1;i<NF;i++)
        pista[hint, pistas[hint]-1]=pista[hint, pistas[hint]-1] " "$i;
  }

  # El texto libre fuera de <INTRO> o <TEXTO> se considera comentario
}
END{
  for (i=0; i<=menus;i++) {
    print "ZIPI_Menu ZIPI_Menu"i;
    print "with";
    print "  ZIPI_titulo \"" titulomenu[i] "\",";
#    print "  text";
#    for (j=0;j<opciones[i]; j++)
#      print "    \"" opcion[i, j] "\"";
    print "  ZIPI_item";
    for (j=0;j<opciones[i]; j++)
      print "    " item[i,j] ;
    print ";";
  }
  for (i=0; i<=hint; i++) {
    print "ZIPI_Pista ZIPI_Pista"i;
    print "with";
    print "  ZIPI_titulo \"" titulohint[i] "\",";
    print "  ZIPI_pistas";
    for (j=0; j<pistas[i]; j++)
      print "    \"" pista[i,j] "\"";
    print ";";
  }
  for (i=0; i<=textos; i++) {
    print "ZIPI_Otro ZIPI_Otro"i;
    print "with";
    print "  ZIPI_titulo \"" titulootro[i] "\",";
    print "  ZIPI_ejecutar [;";
    for (j=0; j<lineas[i]; j++)
      print "       "texto[i,j];
    print "  ];";
  }
  if (hay_intro==0) {
    print "\n\n[ ZIPI_Intro;";
    print "  print \"Despl�zate con las flechas del cursor. Pulsa \", (s_in) \"Enter\", \" para seleccionar, \", (s_in) \"P\", \" para continuar, o \", (s_in) \"Q\", \" para salir.\";";
#    print "  print \"[Pulsa 'T' para ejecutar en modo texto (sin menus)]^\";\n"\
#      "  print \"[o cualquier otra tecla para ejecutar en modo menu]^\";\n"\
#      "  ZIPI_modo=0;\n  tecla=ZIPI_Tecla();\n  if (tecla=='T' or 't') ZIPI_modo=1;\n"\
    print "];\n\n";
  }

  print "[ ZIPI_Empezar ;";
  print "!  ZIPI_InitGlk();";
  print "  clearMainWindow();";
  print "  ZIPI_Intro();";
  print "!  barra_estado.numero_lineas = 26;";
  print "!  barra_estado.dibujar();";
  print "  ZIPI_RunMenu(ZIPI_Menu0, true);";
  print "!  barra_estado.numero_lineas = 1;";
  print "!  barra_estado.dibujar();";
  print "];";
}

