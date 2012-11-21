!-------------------------------------------------------------------------------
!  infglk.h - an Inform library to allow easy access to glk functions
!    under glulx
!  Dynamically created by glk2inf.pl on 11/19/2011 at 16:32:20.
!  Send comments or suggestions to: katre@plover.net
!-------------------------------------------------------------------------------
#Ifdef TARGET_GLULX;
#Ifndef infglk_h;
Constant infglk_h;
Message "[Including <infglk>]";
System_file;

#Ifdef infglk_h;  ! remove "Constant declared but not used" warnings
#Endif;

Constant GLK_NULL 0;

! Constant definitions from glk.h
Constant gestalt_Version 0;
Constant gestalt_CharInput 1;
Constant gestalt_LineInput 2;
Constant gestalt_CharOutput 3;
Constant gestalt_CharOutput_CannotPrint 0;
Constant gestalt_CharOutput_ApproxPrint 1;
Constant gestalt_CharOutput_ExactPrint 2;
Constant gestalt_MouseInput 4;
Constant gestalt_Timer 5;
Constant gestalt_Graphics 6;
Constant gestalt_DrawImage 7;
Constant gestalt_Sound 8;
Constant gestalt_SoundVolume 9;
Constant gestalt_SoundNotify 10;
Constant gestalt_Hyperlinks 11;
Constant gestalt_HyperlinkInput 12;
Constant gestalt_SoundMusic 13;
Constant gestalt_GraphicsTransparency 14;
Constant gestalt_Unicode 15;
Constant gestalt_UnicodeNorm 16;
Constant gestalt_LineInputEcho 17;
Constant gestalt_LineTerminators 18;
Constant gestalt_LineTerminatorKey 19;
Constant gestalt_DateTime 20;
Constant gestalt_Sound2 21;
Constant gestalt_AlphaAventuras $ffff;
Constant evtype_None 0;
Constant evtype_Timer 1;
Constant evtype_CharInput 2;
Constant evtype_LineInput 3;
Constant evtype_MouseInput 4;
Constant evtype_Arrange 5;
Constant evtype_Redraw 6;
Constant evtype_SoundNotify 7;
Constant evtype_Hyperlink 8;
Constant evtype_VolumeNotify 9;
Constant keycode_Unknown $ffffffff;
Constant keycode_Left $fffffffe;
Constant keycode_Right $fffffffd;
Constant keycode_Up $fffffffc;
Constant keycode_Down $fffffffb;
Constant keycode_Return $fffffffa;
Constant keycode_Delete $fffffff9;
Constant keycode_Escape $fffffff8;
Constant keycode_Tab $fffffff7;
Constant keycode_PageUp $fffffff6;
Constant keycode_PageDown $fffffff5;
Constant keycode_Home $fffffff4;
Constant keycode_End $fffffff3;
Constant keycode_Func1 $ffffffef;
Constant keycode_Func2 $ffffffee;
Constant keycode_Func3 $ffffffed;
Constant keycode_Func4 $ffffffec;
Constant keycode_Func5 $ffffffeb;
Constant keycode_Func6 $ffffffea;
Constant keycode_Func7 $ffffffe9;
Constant keycode_Func8 $ffffffe8;
Constant keycode_Func9 $ffffffe7;
Constant keycode_Func10 $ffffffe6;
Constant keycode_Func11 $ffffffe5;
Constant keycode_Func12 $ffffffe4;
Constant keycode_MAXVAL 28;
Constant style_Normal 0;
Constant style_Emphasized 1;
Constant style_Preformatted 2;
Constant style_Header 3;
Constant style_Subheader 4;
Constant style_Alert 5;
Constant style_Note 6;
Constant style_BlockQuote 7;
Constant style_Input 8;
Constant style_User1 9;
Constant style_User2 10;
Constant style_NUMSTYLES 11;
Constant wintype_AllTypes 0;
Constant wintype_Pair 1;
Constant wintype_Blank 2;
Constant wintype_TextBuffer 3;
Constant wintype_TextGrid 4;
Constant wintype_Graphics 5;
Constant winmethod_Left $00;
Constant winmethod_Right $01;
Constant winmethod_Above $02;
Constant winmethod_Below $03;
Constant winmethod_DirMask $0f;
Constant winmethod_Fixed $10;
Constant winmethod_Proportional $20;
Constant winmethod_DivisionMask $f0;
Constant winmethod_Border $000;
Constant winmethod_NoBorder $100;
Constant winmethod_BorderMask $100;
Constant fileusage_Data $00;
Constant fileusage_SavedGame $01;
Constant fileusage_Transcript $02;
Constant fileusage_InputRecord $03;
Constant fileusage_TypeMask $0f;
Constant fileusage_TextMode $100;
Constant fileusage_BinaryMode $000;
Constant filemode_Write $01;
Constant filemode_Read $02;
Constant filemode_ReadWrite $03;
Constant filemode_WriteAppend $05;
Constant seekmode_Start 0;
Constant seekmode_Current 1;
Constant seekmode_End 2;
Constant stylehint_Indentation 0;
Constant stylehint_ParaIndentation 1;
Constant stylehint_Justification 2;
Constant stylehint_Size 3;
Constant stylehint_Weight 4;
Constant stylehint_Oblique 5;
Constant stylehint_Proportional 6;
Constant stylehint_TextColor 7;
Constant stylehint_BackColor 8;
Constant stylehint_ReverseColor 9;
Constant stylehint_NUMHINTS 10;
Constant stylehint_just_LeftFlush 0;
Constant stylehint_just_LeftRight 1;
Constant stylehint_just_Centered 2;
Constant stylehint_just_RightFlush 3;
Constant imagealign_InlineUp $01;
Constant imagealign_InlineDown $02;
Constant imagealign_InlineCenter $03;
Constant imagealign_MarginLeft $04;
Constant imagealign_MarginRight $05;

! The actual glk functions.
[ glk_exit _vararg_count ret;
! glk_exit ()
  ! And now the @glk call
  @glk 1 _vararg_count ret;
  return ret;
];

[ glk_set_interrupt_handler _vararg_count ret;
! glk_set_interrupt_handler (func)
  ! And now the @glk call
  @glk 2 _vararg_count ret;
  return ret;
];

[ glk_tick _vararg_count ret;
! glk_tick ()
  ! And now the @glk call
  @glk 3 _vararg_count ret;
  return ret;
];

[ glk_gestalt _vararg_count ret;
! glk_gestalt (sel val)
  ! And now the @glk call
  @glk 4 _vararg_count ret;
  return ret;
];

[ glk_gestalt_ext _vararg_count ret;
! glk_gestalt_ext (sel val arr arrlen)
  ! And now the @glk call
  @glk 5 _vararg_count ret;
  return ret;
];

[ glk_char_to_lower _vararg_count ret;
! glk_char_to_lower (ch)
  ! And now the @glk call
  @glk 160 _vararg_count ret;
  return ret;
];

[ glk_char_to_upper _vararg_count ret;
! glk_char_to_upper (ch)
  ! And now the @glk call
  @glk 161 _vararg_count ret;
  return ret;
];

[ glk_window_get_root _vararg_count ret;
! glk_window_get_root ()
  ! And now the @glk call
  @glk 34 _vararg_count ret;
  return ret;
];

[ glk_window_open _vararg_count ret;
! glk_window_open (split method size wintype rock)
  ! And now the @glk call
  @glk 35 _vararg_count ret;
  return ret;
];

[ glk_window_close _vararg_count ret;
! glk_window_close (win result)
  ! And now the @glk call
  @glk 36 _vararg_count ret;
  return ret;
];

[ glk_window_get_size _vararg_count ret;
! glk_window_get_size (win widthptr heightptr)
  ! And now the @glk call
  @glk 37 _vararg_count ret;
  return ret;
];

[ glk_window_set_arrangement _vararg_count ret;
! glk_window_set_arrangement (win method size keywin)
  ! And now the @glk call
  @glk 38 _vararg_count ret;
  return ret;
];

[ glk_window_get_arrangement _vararg_count ret;
! glk_window_get_arrangement (win methodptr sizeptr keywinptr)
  ! And now the @glk call
  @glk 39 _vararg_count ret;
  return ret;
];

[ glk_window_iterate _vararg_count ret;
! glk_window_iterate (win rockptr)
  ! And now the @glk call
  @glk 32 _vararg_count ret;
  return ret;
];

[ glk_window_get_rock _vararg_count ret;
! glk_window_get_rock (win)
  ! And now the @glk call
  @glk 33 _vararg_count ret;
  return ret;
];

[ glk_window_get_type _vararg_count ret;
! glk_window_get_type (win)
  ! And now the @glk call
  @glk 40 _vararg_count ret;
  return ret;
];

[ glk_window_get_parent _vararg_count ret;
! glk_window_get_parent (win)
  ! And now the @glk call
  @glk 41 _vararg_count ret;
  return ret;
];

[ glk_window_get_sibling _vararg_count ret;
! glk_window_get_sibling (win)
  ! And now the @glk call
  @glk 48 _vararg_count ret;
  return ret;
];

[ glk_window_clear _vararg_count ret;
! glk_window_clear (win)
  ! And now the @glk call
  @glk 42 _vararg_count ret;
  return ret;
];

[ glk_window_move_cursor _vararg_count ret;
! glk_window_move_cursor (win xpos ypos)
  ! And now the @glk call
  @glk 43 _vararg_count ret;
  return ret;
];

[ glk_window_get_stream _vararg_count ret;
! glk_window_get_stream (win)
  ! And now the @glk call
  @glk 44 _vararg_count ret;
  return ret;
];

[ glk_window_set_echo_stream _vararg_count ret;
! glk_window_set_echo_stream (win str)
  ! And now the @glk call
  @glk 45 _vararg_count ret;
  return ret;
];

[ glk_window_get_echo_stream _vararg_count ret;
! glk_window_get_echo_stream (win)
  ! And now the @glk call
  @glk 46 _vararg_count ret;
  return ret;
];

[ glk_set_window _vararg_count ret;
! glk_set_window (win)
  ! And now the @glk call
  @glk 47 _vararg_count ret;
  return ret;
];

[ glk_stream_open_file _vararg_count ret;
! glk_stream_open_file (fileref fmode rock)
  ! And now the @glk call
  @glk 66 _vararg_count ret;
  return ret;
];

[ glk_stream_open_memory _vararg_count ret;
! glk_stream_open_memory (buf buflen fmode rock)
  ! And now the @glk call
  @glk 67 _vararg_count ret;
  return ret;
];

[ glk_stream_close _vararg_count ret;
! glk_stream_close (str result)
  ! And now the @glk call
  @glk 68 _vararg_count ret;
  return ret;
];

[ glk_stream_iterate _vararg_count ret;
! glk_stream_iterate (str rockptr)
  ! And now the @glk call
  @glk 64 _vararg_count ret;
  return ret;
];

[ glk_stream_get_rock _vararg_count ret;
! glk_stream_get_rock (str)
  ! And now the @glk call
  @glk 65 _vararg_count ret;
  return ret;
];

[ glk_stream_set_position _vararg_count ret;
! glk_stream_set_position (str pos seekmode)
  ! And now the @glk call
  @glk 69 _vararg_count ret;
  return ret;
];

[ glk_stream_get_position _vararg_count ret;
! glk_stream_get_position (str)
  ! And now the @glk call
  @glk 70 _vararg_count ret;
  return ret;
];

[ glk_stream_set_current _vararg_count ret;
! glk_stream_set_current (str)
  ! And now the @glk call
  @glk 71 _vararg_count ret;
  return ret;
];

[ glk_stream_get_current _vararg_count ret;
! glk_stream_get_current ()
  ! And now the @glk call
  @glk 72 _vararg_count ret;
  return ret;
];

[ glk_put_char _vararg_count ret;
! glk_put_char (ch)
  ! And now the @glk call
  @glk 128 _vararg_count ret;
  return ret;
];

[ glk_put_char_stream _vararg_count ret;
! glk_put_char_stream (str ch)
  ! And now the @glk call
  @glk 129 _vararg_count ret;
  return ret;
];

[ glk_put_string _vararg_count ret;
! glk_put_string (s)
  ! And now the @glk call
  @glk 130 _vararg_count ret;
  return ret;
];

[ glk_put_string_stream _vararg_count ret;
! glk_put_string_stream (str s)
  ! And now the @glk call
  @glk 131 _vararg_count ret;
  return ret;
];

[ glk_put_buffer _vararg_count ret;
! glk_put_buffer (buf len)
  ! And now the @glk call
  @glk 132 _vararg_count ret;
  return ret;
];

[ glk_put_buffer_stream _vararg_count ret;
! glk_put_buffer_stream (str buf len)
  ! And now the @glk call
  @glk 133 _vararg_count ret;
  return ret;
];

[ glk_set_style _vararg_count ret;
! glk_set_style (styl)
  ! And now the @glk call
  @glk 134 _vararg_count ret;
  return ret;
];

[ glk_set_style_stream _vararg_count ret;
! glk_set_style_stream (str styl)
  ! And now the @glk call
  @glk 135 _vararg_count ret;
  return ret;
];

[ glk_get_char_stream _vararg_count ret;
! glk_get_char_stream (str)
  ! And now the @glk call
  @glk 144 _vararg_count ret;
  return ret;
];

[ glk_get_line_stream _vararg_count ret;
! glk_get_line_stream (str buf len)
  ! And now the @glk call
  @glk 145 _vararg_count ret;
  return ret;
];

[ glk_get_buffer_stream _vararg_count ret;
! glk_get_buffer_stream (str buf len)
  ! And now the @glk call
  @glk 146 _vararg_count ret;
  return ret;
];

[ glk_stylehint_set _vararg_count ret;
! glk_stylehint_set (wintype styl hint val)
  ! And now the @glk call
  @glk 176 _vararg_count ret;
  return ret;
];

[ glk_stylehint_clear _vararg_count ret;
! glk_stylehint_clear (wintype styl hint)
  ! And now the @glk call
  @glk 177 _vararg_count ret;
  return ret;
];

[ glk_style_distinguish _vararg_count ret;
! glk_style_distinguish (win styl1 styl2)
  ! And now the @glk call
  @glk 178 _vararg_count ret;
  return ret;
];

[ glk_style_measure _vararg_count ret;
! glk_style_measure (win styl hint result)
  ! And now the @glk call
  @glk 179 _vararg_count ret;
  return ret;
];

[ glk_fileref_create_temp _vararg_count ret;
! glk_fileref_create_temp (usage rock)
  ! And now the @glk call
  @glk 96 _vararg_count ret;
  return ret;
];

[ glk_fileref_create_by_name _vararg_count ret;
! glk_fileref_create_by_name (usage name rock)
  ! And now the @glk call
  @glk 97 _vararg_count ret;
  return ret;
];

[ glk_fileref_create_by_prompt _vararg_count ret;
! glk_fileref_create_by_prompt (usage fmode rock)
  ! And now the @glk call
  @glk 98 _vararg_count ret;
  return ret;
];

[ glk_fileref_create_from_fileref _vararg_count ret;
! glk_fileref_create_from_fileref (usage fref rock)
  ! And now the @glk call
  @glk 104 _vararg_count ret;
  return ret;
];

[ glk_fileref_destroy _vararg_count ret;
! glk_fileref_destroy (fref)
  ! And now the @glk call
  @glk 99 _vararg_count ret;
  return ret;
];

[ glk_fileref_iterate _vararg_count ret;
! glk_fileref_iterate (fref rockptr)
  ! And now the @glk call
  @glk 100 _vararg_count ret;
  return ret;
];

[ glk_fileref_get_rock _vararg_count ret;
! glk_fileref_get_rock (fref)
  ! And now the @glk call
  @glk 101 _vararg_count ret;
  return ret;
];

[ glk_fileref_delete_file _vararg_count ret;
! glk_fileref_delete_file (fref)
  ! And now the @glk call
  @glk 102 _vararg_count ret;
  return ret;
];

[ glk_fileref_does_file_exist _vararg_count ret;
! glk_fileref_does_file_exist (fref)
  ! And now the @glk call
  @glk 103 _vararg_count ret;
  return ret;
];

[ glk_select _vararg_count ret;
! glk_select (event)
  ! And now the @glk call
  @glk 192 _vararg_count ret;
  return ret;
];

[ glk_select_poll _vararg_count ret;
! glk_select_poll (event)
  ! And now the @glk call
  @glk 193 _vararg_count ret;
  return ret;
];

[ glk_request_timer_events _vararg_count ret;
! glk_request_timer_events (millisecs)
  ! And now the @glk call
  @glk 214 _vararg_count ret;
  return ret;
];

[ glk_request_line_event _vararg_count ret;
! glk_request_line_event (win buf maxlen initlen)
  ! And now the @glk call
  @glk 208 _vararg_count ret;
  return ret;
];

[ glk_request_char_event _vararg_count ret;
! glk_request_char_event (win)
  ! And now the @glk call
  @glk 210 _vararg_count ret;
  return ret;
];

[ glk_request_mouse_event _vararg_count ret;
! glk_request_mouse_event (win)
  ! And now the @glk call
  @glk 212 _vararg_count ret;
  return ret;
];

[ glk_cancel_line_event _vararg_count ret;
! glk_cancel_line_event (win event)
  ! And now the @glk call
  @glk 209 _vararg_count ret;
  return ret;
];

[ glk_cancel_char_event _vararg_count ret;
! glk_cancel_char_event (win)
  ! And now the @glk call
  @glk 211 _vararg_count ret;
  return ret;
];

[ glk_cancel_mouse_event _vararg_count ret;
! glk_cancel_mouse_event (win)
  ! And now the @glk call
  @glk 213 _vararg_count ret;
  return ret;
];

[ glk_set_echo_line_event _vararg_count ret;
! glk_set_echo_line_event (win val)
  ! And now the @glk call
  @glk 336 _vararg_count ret;
  return ret;
];

[ glk_set_terminators_line_event _vararg_count ret;
! glk_set_terminators_line_event (win keycodes count)
  ! And now the @glk call
  @glk 337 _vararg_count ret;
  return ret;
];

[ glk_buffer_to_lower_case_uni _vararg_count ret;
! glk_buffer_to_lower_case_uni (buf len numchars)
  ! And now the @glk call
  @glk 288 _vararg_count ret;
  return ret;
];

[ glk_buffer_to_upper_case_uni _vararg_count ret;
! glk_buffer_to_upper_case_uni (buf len numchars)
  ! And now the @glk call
  @glk 289 _vararg_count ret;
  return ret;
];

[ glk_buffer_to_title_case_uni _vararg_count ret;
! glk_buffer_to_title_case_uni (buf len numchars lowerrest)
  ! And now the @glk call
  @glk 290 _vararg_count ret;
  return ret;
];

[ glk_put_char_uni _vararg_count ret;
! glk_put_char_uni (ch)
  ! And now the @glk call
  @glk 296 _vararg_count ret;
  return ret;
];

[ glk_put_string_uni _vararg_count ret;
! glk_put_string_uni (s)
  ! And now the @glk call
  @glk 297 _vararg_count ret;
  return ret;
];

[ glk_put_buffer_uni _vararg_count ret;
! glk_put_buffer_uni (buf len)
  ! And now the @glk call
  @glk 298 _vararg_count ret;
  return ret;
];

[ glk_put_char_stream_uni _vararg_count ret;
! glk_put_char_stream_uni (str ch)
  ! And now the @glk call
  @glk 299 _vararg_count ret;
  return ret;
];

[ glk_put_string_stream_uni _vararg_count ret;
! glk_put_string_stream_uni (str s)
  ! And now the @glk call
  @glk 300 _vararg_count ret;
  return ret;
];

[ glk_put_buffer_stream_uni _vararg_count ret;
! glk_put_buffer_stream_uni (str buf len)
  ! And now the @glk call
  @glk 301 _vararg_count ret;
  return ret;
];

[ glk_get_char_stream_uni _vararg_count ret;
! glk_get_char_stream_uni (str)
  ! And now the @glk call
  @glk 304 _vararg_count ret;
  return ret;
];

[ glk_get_buffer_stream_uni _vararg_count ret;
! glk_get_buffer_stream_uni (str buf len)
  ! And now the @glk call
  @glk 305 _vararg_count ret;
  return ret;
];

[ glk_get_line_stream_uni _vararg_count ret;
! glk_get_line_stream_uni (str buf len)
  ! And now the @glk call
  @glk 306 _vararg_count ret;
  return ret;
];

[ glk_stream_open_file_uni _vararg_count ret;
! glk_stream_open_file_uni (fileref fmode rock)
  ! And now the @glk call
  @glk 312 _vararg_count ret;
  return ret;
];

[ glk_stream_open_memory_uni _vararg_count ret;
! glk_stream_open_memory_uni (buf buflen fmode rock)
  ! And now the @glk call
  @glk 313 _vararg_count ret;
  return ret;
];

[ glk_request_char_event_uni _vararg_count ret;
! glk_request_char_event_uni (win)
  ! And now the @glk call
  @glk 320 _vararg_count ret;
  return ret;
];

[ glk_request_line_event_uni _vararg_count ret;
! glk_request_line_event_uni (win buf maxlen initlen)
  ! And now the @glk call
  @glk 321 _vararg_count ret;
  return ret;
];

[ glk_buffer_canon_decompose_uni _vararg_count ret;
! glk_buffer_canon_decompose_uni (buf len numchars)
  ! And now the @glk call
  @glk 291 _vararg_count ret;
  return ret;
];

[ glk_buffer_canon_normalize_uni _vararg_count ret;
! glk_buffer_canon_normalize_uni (buf len numchars)
  ! And now the @glk call
  @glk 292 _vararg_count ret;
  return ret;
];

[ glk_image_draw _vararg_count ret;
! glk_image_draw (win image val1 val2)
  ! And now the @glk call
  @glk 225 _vararg_count ret;
  return ret;
];

[ glk_image_draw_scaled _vararg_count ret;
! glk_image_draw_scaled (win image val1 val2 width height)
  ! And now the @glk call
  @glk 226 _vararg_count ret;
  return ret;
];

[ glk_image_get_info _vararg_count ret;
! glk_image_get_info (image width height)
  ! And now the @glk call
  @glk 224 _vararg_count ret;
  return ret;
];

[ glk_window_flow_break _vararg_count ret;
! glk_window_flow_break (win)
  ! And now the @glk call
  @glk 232 _vararg_count ret;
  return ret;
];

[ glk_window_erase_rect _vararg_count ret;
! glk_window_erase_rect (win left top width height)
  ! And now the @glk call
  @glk 233 _vararg_count ret;
  return ret;
];

[ glk_window_fill_rect _vararg_count ret;
! glk_window_fill_rect (win color left top width height)
  ! And now the @glk call
  @glk 234 _vararg_count ret;
  return ret;
];

[ glk_window_set_background_color _vararg_count ret;
! glk_window_set_background_color (win color)
  ! And now the @glk call
  @glk 235 _vararg_count ret;
  return ret;
];

[ glk_schannel_create _vararg_count ret;
! glk_schannel_create (rock)
  ! And now the @glk call
  @glk 242 _vararg_count ret;
  return ret;
];

[ glk_schannel_destroy _vararg_count ret;
! glk_schannel_destroy (chan)
  ! And now the @glk call
  @glk 243 _vararg_count ret;
  return ret;
];

[ glk_schannel_iterate _vararg_count ret;
! glk_schannel_iterate (chan rockptr)
  ! And now the @glk call
  @glk 240 _vararg_count ret;
  return ret;
];

[ glk_schannel_get_rock _vararg_count ret;
! glk_schannel_get_rock (chan)
  ! And now the @glk call
  @glk 241 _vararg_count ret;
  return ret;
];

[ glk_schannel_play _vararg_count ret;
! glk_schannel_play (chan snd)
  ! And now the @glk call
  @glk 248 _vararg_count ret;
  return ret;
];

[ glk_schannel_play_ext _vararg_count ret;
! glk_schannel_play_ext (chan snd repeats notify)
  ! And now the @glk call
  @glk 249 _vararg_count ret;
  return ret;
];

[ glk_schannel_stop _vararg_count ret;
! glk_schannel_stop (chan)
  ! And now the @glk call
  @glk 250 _vararg_count ret;
  return ret;
];

[ glk_schannel_set_volume _vararg_count ret;
! glk_schannel_set_volume (chan vol)
  ! And now the @glk call
  @glk 251 _vararg_count ret;
  return ret;
];

[ glk_sound_load_hint _vararg_count ret;
! glk_sound_load_hint (snd flag)
  ! And now the @glk call
  @glk 252 _vararg_count ret;
  return ret;
];

[ glk_schannel_create_ext _vararg_count ret;
! glk_schannel_create_ext (rock volume)
  ! And now the @glk call
  @glk 244 _vararg_count ret;
  return ret;
];

[ glk_schannel_play_multi _vararg_count ret;
! glk_schannel_play_multi (chanarray chancount sndarray soundcount notify)
  ! And now the @glk call
  @glk 247 _vararg_count ret;
  return ret;
];

[ glk_schannel_pause _vararg_count ret;
! glk_schannel_pause (chan)
  ! And now the @glk call
  @glk 254 _vararg_count ret;
  return ret;
];

[ glk_schannel_unpause _vararg_count ret;
! glk_schannel_unpause (chan)
  ! And now the @glk call
  @glk 255 _vararg_count ret;
  return ret;
];

[ glk_schannel_set_volume_ext _vararg_count ret;
! glk_schannel_set_volume_ext (chan vol duration notify)
  ! And now the @glk call
  @glk 253 _vararg_count ret;
  return ret;
];

[ glk_set_hyperlink _vararg_count ret;
! glk_set_hyperlink (linkval)
  ! And now the @glk call
  @glk 256 _vararg_count ret;
  return ret;
];

[ glk_set_hyperlink_stream _vararg_count ret;
! glk_set_hyperlink_stream (str linkval)
  ! And now the @glk call
  @glk 257 _vararg_count ret;
  return ret;
];

[ glk_request_hyperlink_event _vararg_count ret;
! glk_request_hyperlink_event (win)
  ! And now the @glk call
  @glk 258 _vararg_count ret;
  return ret;
];

[ glk_cancel_hyperlink_event _vararg_count ret;
! glk_cancel_hyperlink_event (win)
  ! And now the @glk call
  @glk 259 _vararg_count ret;
  return ret;
];

[ glk_current_time _vararg_count ret;
! glk_current_time (time)
  ! And now the @glk call
  @glk 352 _vararg_count ret;
  return ret;
];

[ glk_current_simple_time _vararg_count ret;
! glk_current_simple_time (factor)
  ! And now the @glk call
  @glk 353 _vararg_count ret;
  return ret;
];

[ glk_time_to_date_utc _vararg_count ret;
! glk_time_to_date_utc (time date)
  ! And now the @glk call
  @glk 360 _vararg_count ret;
  return ret;
];

[ glk_time_to_date_local _vararg_count ret;
! glk_time_to_date_local (time date)
  ! And now the @glk call
  @glk 361 _vararg_count ret;
  return ret;
];

[ glk_simple_time_to_date_utc _vararg_count ret;
! glk_simple_time_to_date_utc (time factor date)
  ! And now the @glk call
  @glk 362 _vararg_count ret;
  return ret;
];

[ glk_simple_time_to_date_local _vararg_count ret;
! glk_simple_time_to_date_local (time factor date)
  ! And now the @glk call
  @glk 363 _vararg_count ret;
  return ret;
];

[ glk_date_to_time_utc _vararg_count ret;
! glk_date_to_time_utc (date time)
  ! And now the @glk call
  @glk 364 _vararg_count ret;
  return ret;
];

[ glk_date_to_time_local _vararg_count ret;
! glk_date_to_time_local (date time)
  ! And now the @glk call
  @glk 365 _vararg_count ret;
  return ret;
];

[ glk_date_to_simple_time_utc _vararg_count ret;
! glk_date_to_simple_time_utc (date factor)
  ! And now the @glk call
  @glk 366 _vararg_count ret;
  return ret;
];

[ glk_date_to_simple_time_local _vararg_count ret;
! glk_date_to_simple_time_local (date factor)
  ! And now the @glk call
  @glk 367 _vararg_count ret;
  return ret;
];

! ====================================================================
! EXTENSIONES DE ALPHA AVENTURAS (c) Alpha
! ====================================================================

[ glk_incr_fontsize _vararg_count ret;
! glk_incr_fontsize ()
  ! And now the @glk call
  @glk $FFFE _vararg_count ret;
  return ret;
];

[ glk_decr_fontsize _vararg_count ret;
! glk_decr_fontsize ()
  ! And now the @glk call
  @glk $FFFD _vararg_count ret;
  return ret;
];

[ glk_window_stylehint_set _vararg_count ret;
! glk_window_stylehint_set (win styl hint val)
  ! And now the @glk call
  @glk $FFFC _vararg_count ret;
  return ret;
];

! ====================================================================
! FIN DE LAS EXTENSIONES DE ALPHA AVENTURAS
! ====================================================================

#Endif;
#Endif;
