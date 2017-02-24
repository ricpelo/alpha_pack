!
! ExaminarFalso.h
!
! Copyright (c) 2012 Ricardo Pérez López (Alpha Aventuras)
!

! This program is free software: you can redistribute it and/or modify
! it under the terms of the GNU General Public License as published by
! the Free Software Foundation, either version 3 of the License, or
! (at your option) any later version.
!
! This program is distributed in the hope that it will be useful,
! but WITHOUT ANY WARRANTY; without even the implied warranty of
! MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
! GNU General Public License for more details.
!
! You should have received a copy of the GNU General Public License
! along with this program.  If not, see <http://www.gnu.org/licenses/>.

!
! Permite usar simplemente el nombre de un objeto como sinónimo de
! 'EXAMINAR OBJETO'. Copiado de
! http://rec.arts.int-fiction.narkive.com/Cz9j184N/inform-6-parser-question
!

System_file;

#ifndef UnknownVerb;
[ UnknownVerb x;
  verb_wordnum = 0;
  return 'no.verb';
];
#endif; ! UnknownVerb

#ifndef PrintVerb;
[ PrintVerb v;
  if (v == 'no.verb') "examinar";
  rfalse;
];
#endif; ! PrintVerb

Verb 'no.verb' * noun -> Examine;

