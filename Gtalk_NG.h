! --------------------------------------------------------------------
! GTALK.H : A Library File for Conversation Menus
! Version 3.00
! by Greg Boettcher, Krister Fundin, and Mark J. Tilford
! This library is freeware. You may use it for any purpose without 
! permission, but please credit us.
! --------------------------------------------------------------------
! 
! -----------
! Description
! -----------
! 
! This is an Inform library file designed to handle Photopia-style 
! conversation menus, sort of like Phtalkoo.h, but it has certain 
! features Phtalkoo.h lacks:
!   * Can do looping/repeating conversations, a la "Being Andrew 
!     Plotkin" or "The Cabal."
!   * Can do submenus, menu mazes, etc. Compile Gtalk.inf to see what 
!     I mean.
!   * Allows more readable code than Phtalkoo.h, in my humble opinion, 
!     since, unlike Phtalkoo.h, it allows quip/response pairs to be 
!     right next to each other in your code.
!   * Allows you to turn on/off the "0 to say nothing" option.
!   * Allows you to turn on/off the "What would you like to say?" 
!     message.
! 
! ---------------
! Version History
! ---------------
! 
! 3.00: 2006-07-29 Numerous changes, detailed below, the work of 
!                  Krister Fundin. E.g., lots of comments have been 
!                  added, and lots of code reformatted. Also, the 
!                  speed of performance has been increased, reportedly
!                  producing a measurable improvement on slower 
!                  computers.
! 2.04: 2006-05-06 It's now possible to modify a conversation menu 
!                  with a character before you've initiated a 
!                  conversation with that character, based on work by 
!                  Victor Gijsbers. In gtalk.inf, a new bug fix 
!                  prevents things like, "The weeds doesn't reply."
! 2.03: 2005-09-06 New features for non-English authors: modifications 
!                  by Victor Gijsbers.
! 2.02: 2005-02-08 Optimized a few things for speed and memory 
!                  efficiency. Fixed a couple of minor flaws in the 
!                  documentation. Rewrote Note #5.
! 2.01: 2005-02-02 Added documentation for the new changes.
! 2.00: 2005-01-29 Major improvements added, as specified below, 
!                  mostly the work of Mark J. Tilford. (Not a public 
!                  release.)
! 1.01: 2004-09-23 Original release by Greg Boettcher.
! 
! ---------------
! Acknowledgments
! ---------------
! 
! * This library owes quite a bit to Phtalkoo.h, which was written by 
!   David Glasser and was based on Adam Cadre's source code for 
!   Photopia. (I pulled the bitwise stuff from there and got my basic 
!   start from there, although most of the code here is original.
! * Special thanks to Roger Firth, Andrew Plotkin, and Cedric Knight 
!   for helping me with some complex array issues for a previous, 
!   unreleased version of this library.
! * And to J. Robinson Wheeler, for showing me his Phtalkoo.h
!   modifications.
! * And to Victor Gijsbers, who made international improvements for 
!   version 2.03, and also helped improve 2.04 so that it is now 
!   possible to modify a conversation menu with a character before 
!   you've initiated a conversation with that character.
! * Big thanks to Mark J. Tilford, who made the improvements resulting 
!   in version 2.00, making the syntax more flexible, adding Glulx 
!   support, and making other improvements.
! * And big thanks to Krister Fundin, who made the improvements
!   resulting in version 3.00, adding comments, optimizing things,
!   improving performance speed, etc. Krister also previously made 
!   some suggestions to improve memory efficiency. 
! 
! ---------------------------
! Before You Use This Library
! ---------------------------
! 
! First download Gtalk.inf if you don't already have it, and make sure 
! it's the same version as this file, Gtalk.h! Gtalk.inf is a demo 
! game using this library (Gtalk.h), and this library is meant to be 
! learned in conjunction with that demo. You should be able to get the 
! demo from the same place where you got this file.
! 
! -----------------------
! How to Use This Library
! -----------------------
! 
! 1. Insert the line:
!         Include "Gtalk";
!    somewhere early in your file. (I included it right after 
!    VerbLib.)
! 2. Each "talk-to-able" NPC in your game must be of class Character, 
!    and must contain a "quip" routine.
! 3. The "quip" routine consists of one big "switch" clause, like so:
!         Character NPC_ID "Name" someroom
!           with
!           ...
!           quip [a b;
!             switch (a) {
!               ...
!               ! Quip #20
!               201: "~Could I have that apple?~";
!               202: move apple to player;
!                    "~Sure, why not. Say, you're not from
!                      around here, are you?~";
!               203: qtype = [MainMenu or SubMenu];
!                    qqon  = true;
!                    killz = true;
!                    killq = true;
!                    qtransfer = [quip number];
!               204: ! Conversation options to follow
!                    return Qlist(b, 2,   ! 2 quips in
!                                         ! this "array"
!                                    21,  ! Quip #21
!                                    22); ! Quip #22
!               ...
!             }
!             rfalse;
!           ];
!    This sequence -- 201, 202, 203, and 204 -- collectively 
!    represents Quip #20, a sample conversation option. (Well, not 
!    really. This quip contains every possible element that a quip can 
!    have. No real quip would ever do that.)
! 4. Each conversation menu option -- each quip -- can contain any of 
!    the following elements:
!         ### = The quip's ID number. Mandatory. Note: Do *not* use a 
!               Quip #0! See Note #3 below.
!         ###1: Name: The quip's name, as shown among the list of 
!               conversation menu options.
!         ###2: Reply: The reply, including a textual response, and 
!               maybe also additional programming statements.
!         ###3: Additional options. Choose from among the following:
!           qtype = MainMenu; If the quip is a main menu.
!                 = SubMenu;  If the quip is a submenu.
!           qqon  = true;     Quip is turned on from the beginning of 
!                             the game. (See also Note #6 below.)
!                 = false;    Quip is turned off from the beginning of 
!                             the game.
!           killz = true;     To get rid of the "0 to say nothing" 
!                             option for any quip. (killz = "kill 
!                             zero")
!           killq = true;     To get rid of "What would you like to 
!                             say?" for any quip. (killq = "kill 
!                             question")
!           qtransfer =       To "go back to the main menu" or any 
!              [a quip ID];   other previously defined set of options. 
!                             Give the ID of the quip whose
!                             conversation menu options list (that is, 
!                             whose ###4 clause) you want to use.
!         ###4: A list of conversation menu options that can be chosen 
!               *after* this quip. See Step 5 below.
!    Note that none of these four elements is mandatory. For examples, 
!    consult Gtalk.inf.
! 5. With version 2.01 and up of Gtalk.h, the syntax for defining 
!    conversation options is considerably easier than it was before. 
!    However, it still requires some learning. For example, suppose 
!    you have a Quip 50, and you want it to provide five options -- 
!    namely, Quips 51, 52, 53, 54, and 55. You'd do something like 
!    this:
!         Character NPC_ID "Name" someroom
!           with
!           ...
!           quip [a b;
!             switch (a) {
!               ...
!               ! Quip #50
!               ...
!               504: return Qlist(b, 5,  ! 5 values in 
!                                        ! this "array"
!                                   51,  ! Quip #51
!                                   52,  ! Quip #52
!                                   53,  ! Quip #53
!                                   54,  ! Quip #54
!                                   55); ! Quip #55
!               ...
!             }
!             rfalse;
!           ];
!    As you can see, in ###4 you simply call the Qlist routine, where 
!    the first argument is b, the second argument is the total number 
!    of conversation options, and the remaining arguments are the ID 
!    numbers of the quips in your options list.
!       Note the format here; Quip #55 should be specified as 55, not 
!    as 551 or whatever.
!       Also note that your options list should include all the 
!    options that will ever be available at the given juncture. If you 
!    want a quip's options to change over time, then make sure its 
!    ###4 makes reference to all the quip options that it will ever
!    provide; then turn off any quips in the list that aren't valid 
!    choices at the beginning of the game.
!       But what if you have more than 5 options? This could be a 
!    problem, because conventional Z-machine Inform allows no more 
!    than 7 arguments when calling a routine.
!       Unless you're writing for Glulx, here's the best way to define 
!    more than 5 menu options:
!          504: return Qlist(b,   11, 51,52,53,54,55) |
!                      Qlist(b-6,     56,57,58,59,60,61);
!    (The | is a "bitwise or" operator and basically serves to make 
!    sure that the right value is returned from the right Qlist() 
!    call. Just trust me, it works. Or rather, trust Mark, who devised 
!    this clever bit of code.)
!       Here's the pattern for when you have more than 11 options:
!          504: return Qlist(b,    40, 51,52,53,54,55) |
!                      Qlist(b-6,      56,57,58,59,60,61) |
!                      Qlist(b-12,     62,63,64,65,66,67) |
!                      Qlist(b-18,     68,69,70,71,72,73) |
!                      Qlist(b-24,     74,75,76,77,78,79) |
!                      Qlist(b-30,     80,81,82,83,84,85) |
!                      Qlist(b-36,     86,87,88,89,90);
!    See also Gtalk.inf, Quip #70.
!       On the other hand, if you're writing for Glulx, the syntax is 
!    much simpler:
!          504: return Qlist(b, 40, 51,52,53,54,55,56,57,58,59,60,
!                                   61,62,63,64,65,66,67,68,69,70,
!                                   71,72,73,74,75,76,77,78,79,80,
!                                   81,82,83,84,85,86,87,88,89,90);
!    I have been informed that Glulx has the capacity for any number 
!    of arguments.
!       In case anybody's interested, there is also another option:
!          504: switch (b) {
!                  0: return 11;  ! 11 options
!                  1: return 51;  ! Quip #51
!                  2: return 52;  ! Quip #52
!                  3: return 53;  ! Quip #53
!                  4: return 54;  ! Quip #54
!                  5: return 55;  ! Quip #55
!                  6: return 56;  ! Quip #56
!                  7: return 57;  ! Quip #57
!                  8: return 58;  ! Quip #58
!                  9: return 59;  ! Quip #59
!                 10: return 60;  ! Quip #60
!                 11: return 61;  ! Quip #61
!               }
!    If you find this way easier, then feel free to use it, but it 
!    doesn't seem easier to me. This is the old, clumsy way, which is 
!    no longer necessary with Gtalk.h 2.x.
! 6. Each character must have (at least) one conversation menu option 
!    that represents the "main menu." It is this that's called into 
!    action first. (To find out how, read Step 8 below.) This main 
!    menu need not (should not) have a ###1 (option name) or a ###2
!    (reply), but it must have a ###4 menu options list, plus a ###3 
!    with "qtype = MainMenu;". (To find out why, read Note #2 below.)
! 7. If you have any submenus, your submenu quips should have a ###3 
!    with "qtype = SubMenu;". To find out why, read Note #2 below.
! 8. Each character has a "select" property routine. This is what you 
!    must call to set the conversation menu in motion. Call 
!    "self.select(X);" or "CharacterID.select(X);", where X is the 
!    Quip ID of your main menu.
! 9. It's unlikely that you'll want the choices in your conversation 
!    menus to always remain the same.
!      - To turn Quip #22 on:   self.qon(22);
!      - To turn Quip #22 off:  self.qoff(22);
!      - To turn on a series    self.qon(1,2,3,4,5);
!          of quips (up to 5):
!      - To turn off a series   self.qoff(1,2,3,4,5);
!          of quips (up to 5):
!      - To deal with a series  self.qset(1,0, 2,1);
!          of quips, turning              ! Quip #1 on
!          some on, others off.           ! Quip #2 off
!          (Only up to 2 quips
!          with Z-code Inform;
!          unlimited with Glulx)
!      - To test a quip:
!              self.qtest(22) is true  if Quip #22 is on
!                             is false if Quip #22 is off
!      - Or, if you're        ObjectID.qon(22);
!        calling from         ObjectID.qoff(22);
!        a different part     ObjectID.qon(1,2,3,4,5);
!        of your code:        ObjectID.qoff(1,2,3,4,5);
!                             flag = ObjectID.qtest(22);
!    And remember to use "###3: qqon = true;" for quips that are to be 
!    turned on from the beginning. Otherwise, all quips are initially 
!    turned off.
!10. Gtalk.h does not deal with any verb issues; you must do that on 
!    your own. Here are some tips.
!      Verb Tip 1:
!        Typically, games with conversation menus utilize the TALK 
!        verb rather than ASK and TELL. So you may wish to define the 
!        verb Talk. Then, for each of your NPCs, use the "before" 
!        routine to intercept this verb (or whatever verb(s) you
!        choose), and then call the NPC's "select" routine as 
!        described in Step 8.
!      Verb Tip 2:
!        You may also wish to disable the standard conversation verbs.
!          * You can do this the hard way, as Gtalk.inf does, by 
!            disabling the conversation verbs one by one -- namely:
!               Answer/Say/Shout/Speak
!               Ask
!               No
!               Sorry
!               Tell
!               Yes/Y
!            You may also wish to intercept "Order" and "Answer" in 
!            the "life" property of each of your NPCs, disabling that 
!            as well. ("Order" handles things like "HARRY, GO NORTH"; 
!            "Answer" handles things like "HARRY, HELLO" or "HARRY, 
!            ONIONS".) For more info, consult Gtalk.inf.
!          * Just before releasing this library I thought of an easier 
!            way to do this. Between Parser and VerbLib you could 
!            include:
!               Include "Parser";
!               Object LibraryMessages
!                 with before [;
!                   Answer, Ask, No, Orders,
!                   Sorry, Tell, Yes:  
!                     "To talk to someone, 
!                       please type TALK TO PERSON.";
!                 ];
!               Include "VerbLib";
!            However, I haven't tested this, so I leave this for you 
!            to experiment with.
!      Verb Tip 3:
!        I also recommend one more finishing touch: do an 
!        "Extend only 'speak' replace", making it so that it so that 
!        'speak' (but not 'answer', 'say', or or 'shout') becomes 
!        synonymous with 'talk'. Consult Gtalk.inf to see what I mean. 
!11. See also the following notes.
! 
! Note #1: Gtalk.h has an optional feature where it can routinely look 
! for any quip whose options are all turned off, and turn that quip 
! off for you. To turn on this feature, simply define the constant 
! AutoDeactivate just before you include Gtalk.h. See Gtalk.inf, which 
! does this. (This feature is not on by default because it might be 
! unwanted, and could theoretically slow games down, although this is 
! probably not a big factor.)
! 
! Note #2: What, really, is the difference between MainMenu quips, 
! SubMenu quips, and normal quips?
!   * MainMenu quips (unlike SubMenu quips and normal quips) will 
!     print "You can't think of anything to say" when all of their 
!     options are turned off.
!   * Having reached a MainMenu or SubMenu quip (as opposed to a 
!     normal quip) does not constitute actually having said anything. 
!     Gtalk.h takes this into account when deciding whether it makes 
!     sense to print the message "You decide not to say anything after 
!     all."
! 
! Note #3: Do NOT use a Quip #0. Just don't. Many of the routines in
! gtalk.h (qon, qoff, qset, and qtransfer) would fail if they 
! attempted to deal with a Quip #0, and beginning with version 3.x
! gtalk.h doesn't even attempt to process any quips numbered 0.
! 
! Note #4: Also, don't use negative numbers for Quip IDs. They won't 
! work as you'd expect. Just use Quip IDs from 1 to 65535, rather 
! than -32768 to 32767. It's a lot easier.
! 
! Note #5: The library is presently set up so that you can have quips 
! numbered 1 to 95 when writing for the Z-machine, or 1 to 191 quips 
! per NPC when writing for Glulx. This can be changed, and sometimes 
! should be changed, because it affects both your game's memory 
! consumption and the speed at which Gtalk.h runs. 
!    Let's see how this works by observing the following bit of code:
!       Class Character
!         has  animate,
!         with qflag 0 0 0 0 0 0,
!         !   For the    For
!         ! Z-machine: Glulx:
!         !         6      6 elements in the "qflag" word array
!         !     *   2  *   4 bytes per word
!         !     *   8  *   8 bits per byte
!         !     =  96  = 192 
!         !     -   1  -   1 because it's 0-95 or 0-191
!         !     =  95  = 191 highest quip value for each NPC
!         maxquip [;
!           return self.#qflag * 8 - 1;
!         ],
! As you can see, the highest quip value for each NPC, and thus the 
! maximum number of quips for each NPC, depends on the Character.qflag 
! array. There are normally six elements (six 0's) in the array. 
! Multiply by 16 as indicated above (or 32 if you're writing for 
! Glulx) and subtract 1, and you get a maximum of 95 quips per NPC 
! with the Z-machine (or 191 with Glulx).
!    You can, of course, override the Character class's qflag array. 
! For any of your NPCs, simply put a qflag array into the NPC's object 
! definition, and put in as many elements as you want to produce the
! desired maximum number of quips, as follows:
!       Number of  
!        elements  
!        in qflag   Highest quip value for each NPC:
!          array:   (Z-machine)  (Glulx)
!               1   *16-1=  15   *32-1=  31
!               2   *16-1=  31   *32-1=  63
!               3   *16-1=  47   *32-1=  95
!               4   *16-1=  63   *32-1= 127
!               5   *16-1=  79   *32-1= 159
!               6   *16-1=  95   *32-1= 191
!               7   *16-1= 111   *32-1= 223
!               8   *16-1= 127   *32-1= 255
!               9   *16-1= 143   *32-1= 287
!              10   *16-1= 159   *32-1= 319
!             etc.
!    The only restriction is, if you're writing for the Z-machine, you 
! can't use more than 6553 quips for any NPC. (Why 6553? Inform's 
! integer limits of -32768 to 32767 can also be represented as 0 to 
! 65535. Divide by ten, and you get 6553.) It's doubtful that anyone 
! would need more than 6553 quips for any one NPC.
! 
! Note #6: Gtalk.h now has an optional feature where you can make it 
! so all the quips are automatically turned on from the beginning of 
! the game unless their ###3 clause specifically says "qqon = false;". 
! To do this, simply define the constant QuipsOnByDefault, like so...
!      Constant QuipsOnByDefault;
! ...just before you include Gtalk.h. Gtalk.inf doesn't do this, but I 
! think you can figure out how to do this on your own.
! 
! Note #7: Thanks to Victor Gijsbers, Gtalk now makes things easier 
! for authors not writing in English. If your game uses a language 
! other than English, make sure to define the constants GT_SELECT, 
! GT_ZEROEXIT, GT_WOULDLIKE, GT_NOQUIP, and GT_NOSAY, setting them to 
! strings that are appropriate translations of the English expressions 
! of those constants (see below). If you are writing in English, all 
! you have to do is make sure that the constant EnglishNaturalLanguage 
! is defined, as English.h does automatically. Thanks again to Victor 
! Gijsbers for this modification.
! 
! -----------------------------------------------
! Differences Between Versions 1.x and 2.x and 3.x
! -----------------------------------------------
! 
! It is not necessary to read this section in order to use gtalk.h. It 
! is mostly here for my own reference, and also for anyone else who is 
! interested in knowing what modifications have been done to this 
! library. For a more concise version history, see above.
! 
! Changes new to version 2.x (mostly due to the work of Mark J. 
! Tilford):
!   * You no longer have to define your conversation menu options with 
!     a convoluted array-like routine, but can use a much easier 
!     syntax. This eliminates what was formerly Gtalk.h's biggest 
!     disadvantage, so I am grateful to Mark for helping with this.
!   * Glulx support has been added.
!   * Gtalk.h is now more efficient and faster, perhaps even faster 
!     enough for some people to notice the difference.
!   * You can now declare a constant QuipsOnByDefault, to make your 
!     conversation options on by default.
!   * The Character.qset() routine is now more flexible.
!   * Starting with version 2.03, Victor Gijsbers has made some 
!     modifications to make things easier for authors not writing 
!     games in English. See Note #7.
! 
! Changes new to version 3.x (mostly due to the work of 
! Krister Fundin):
! 
!   * Reformatted code, minor tweaks, some optimizations, lots of 
!     comments for the benefit of anyone who wants to mess around with 
!     the library and add more features.
!   * Added System_file and made the string constants into #Defaults, 
!     so that they can be individually replaced even in English games.
!   * Added two new strings (GT_OPTIONPREFIX and GT_OPTIONSUFFIX), 
!     which can be used to change the style of the option listing, 
!     e.g. into:
!        1. Say this
!              or
!        #2 Say that
!              or
!        Type 3 to say something else
!              etc.
!   * Some Character methods that were only used internally have been 
!     made into functions instead, since that means fewer properties 
!     and less dynamic memory used.
!   * Moved AutoDeactivate behaviour to a separate function in order 
!     to reduce clutter a bit
!   * Calculate maxquip only once, in qinitial (now called initquips).
!   * Made warnings for too high quip numbers a bit more elaborate, 
!     but they are only compiled when the DEBUG switch is set.
!   * Ditched warnings for odd number of arguments to qset, since that
!     doesn't seem like a mistake that should happen very easily.
!   * Changed the PowersOfTwo function into a table, for efficiency.
!   * Used qset directly as much as possible.
!   * Quip 0 is not allowed, so skip looping through it
!   * In Gtalk.inf, an "unweary" verb has been added, to test the 
!     consequences of  changing the on/off status of quips prior to 
!     initiating a conversation.


System_file;


! Define default versions of all strings that this extension uses, so 
! that they can be replaced, E.G. when developing a game in a language 
! other than English.
#default GT_SELECT       = "Select an option ";
#default GT_ZEROEXIT     = "or 0 to exit ";
#default GT_WOULDLIKE    = "What would you like to say?^";
#default GT_NOQUIP       = "You can't think of anything to say.^";
#default GT_NOSAY        = "You decide not to say anything after all.^";
#default GT_OPTIONPREFIX = "(";
#default GT_OPTIONSUFFIX = ") ";

#ifndef WORDSIZE;
Constant TARGET_ZCODE;
Constant WORDSIZE = 2;
#endif;

! This array of two-powers is used in order to save some time when
! processing quips.
Array GT_Powers
 -> $$000000001
    $$000000010
    $$000000100
    $$000001000
    $$000010000
    $$000100000
    $$001000000
    $$010000000;

! The five global variables that can be set in a quip's ###3 clause.
Global qtype;
Global qqon;
Global qtransfer;
Global killz;
Global killq;

! These values can be used for qtype in a quip's ###3 clause.
Constant MainMenu = 1;
Constant SubMenu  = 2;

! The Qlist function is used as a shorthand to defining options in a 
! quip's ###4 clause. We define separate Z-code and Glulx versions, 
! since the latter benefits from being able to accept any number of 
! arguments.
#ifdef TARGET_ZCODE;
[ Qlist num a1 a2 a3 a4 a5 a6;
   switch (num) {
     0: return a1;
     1: return a2;
     2: return a3;
     3: return a4;
     4: return a5;
     5: return a6;
   }
   return 0;
];
#ifnot;
[ Qlist _vararg_count n t;
  @copy sp n;
  _vararg_count--;
  if (n < 0 || n >= _vararg_count) return 0;
  while (n > 0) {
    n--;
    @copy sp t;
  }
  @copy sp t;
  return t;
];
#endif;

! The Character class. All NPCs with conversation menus should belong to
! this class.
Class Conversacion
  ! This is the array in which we store the on/off status of all our quips.
  ! The number of entries determines how many quips we can have at most,
  ! and this can be calculated like so:
  !
  !   For the    For
  ! Z-machine: Glulx:
  !         6      6 elements in the default "qflag" word array
  !     *   2  *   4 bytes per word
  !     *   8  *   8 bits per byte
  !     =  96  = 192
  !     -   1  -   1 because it's 0-95 or 0-191
  !     =  95  = 191 highest quip value for each NPC
  !
  ! If more quips are needed, override this property and add more zeroes
  ! to the array.
  with qflag 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0,

  ! the number of our highest quip will be stored here, so that we only
  ! have to calculate it once
  maxquip 0,

  ! Mostrar una opción del menú:
  mostrar_opcion [ curquip opc
    y oldy;

    while (true) {
      glk_window_get_cursor(gg_conversawin, gg_arguments,
                            gg_arguments + WORDSIZE);
      y = gg_arguments-->1; ! Guardamos la fila actual
      if (y >= self.altura()) self.cambiar_altura(y + 1);
      glk_window_move_cursor(gg_conversawin, 0, y);
      print "   ", (string) GT_OPTIONPREFIX;
      glk_set_style(style_Input);
      glk_set_hyperlink(opc + 48);
      print opc;
      glk_set_hyperlink(0);
      glk_set_style(style_Normal);
      print (string) GT_OPTIONSUFFIX;
      ! print the option text
      glk_set_hyperlink(opc + 48);
      if (opc ~= 0) self.quip(curquip * 10 + 1);
      else print (string) GT_ZEROEXIT;
      glk_set_hyperlink(0);
      oldy = y;    
      glk_window_get_cursor(gg_conversawin, gg_arguments, 
                            gg_arguments + WORDSIZE);
      y = gg_arguments-->1;
      new_line;
      if (y >= self.altura()) {
        self.cambiar_altura(y + 1);
        glk_window_move_cursor(gg_conversawin, 0, oldy);
      } else {
        break;
      }
    } 
  ],

  altura [;
    glk_window_get_arrangement(glk_window_get_parent(gg_conversawin),
                               gg_arguments, gg_arguments + WORDSIZE,
                               gg_arguments + 2 * WORDSIZE);
    return gg_arguments-->1;
  ],
  
  cambiar_altura [ altura;
    glk_window_get_arrangement(glk_window_get_parent(gg_conversawin),
                               gg_arguments, gg_arguments + WORDSIZE,
                               gg_arguments + 2 * WORDSIZE);
    glk_window_set_arrangement(glk_window_get_parent(gg_conversawin),
                               gg_arguments-->0,
                               altura,
                               gg_arguments-->2);
  ],
  
  ! This method carries out a conversation with our character. Call it with
  ! the first argument set to a quip which represents a main menu.
  select [ curquip no_decir_nada
    times quipnum numoptions onoptions o selected spoken y oldy ret;

    ! Check if we need to initialize. This process is, among other things,
    ! responsible for calculating our maxquip value, so we can see if it
    ! has been done by checking if maxquip is still set to zero.
    if (self.maxquip == 0) initquips(self);

    ! do a big loop, because this quip could be only the first of many in
    ! an extended conversation
    for (times = 1 : : times++) {
      ! turn off quips with no options on, if desired
      #ifdef AutoDeactivate;
      AutoDeactivateQuips(self);
      #endif;

      ! Reset global variables. Note that we don't care about qqon, since
      ! it's only used during the initquips() stage.
      qtype = 0;
      qtransfer = curquip;
      killz = 0;
      killq = 0;
    
      ! ask the current quip to set the five global variables (qtype,
      ! qqon, qtransfer, killz and killq) to their intended values
      self.quip(curquip * 10 + 3);

      ! if this quip is not a menu of some kind, note that that player
      ! has said something
      if (qtype ~= MainMenu && qtype ~= SubMenu) spoken = true;

      ! print the reply for this quip
      glk_set_window(gg_mainwin);
      self.quip(curquip * 10 + 2);
      glk_set_window(gg_conversawin);

      ! The next step is to see if we have any options to display. Assume
      ! that we don't.
      onoptions = 0;

      ! Get the quip argument for the option list.  Note that qtransfer
      ! will be set either to the current quip or to a different quip,
      ! but always to SOME quip
      quipnum = qtransfer * 10 + 4;

      ! get the total number of options, whether on or off
      numoptions = self.quip(quipnum, 0);

      ! if we have any options at all, see if any of them are on
      for (o = 1 : o <= numoptions : o++) {
        ! we don't care about how many options there are right now, so
        ! leave this loop as soon as we find at least one option that is on
        if (self.qtest(self.quip(quipnum, o))) onoptions++;
      }

      if (onoptions == 0) {
        ! There were no options available, hence nothing for the player
        ! to say, so this conversation is finished. If this was a main
        ! menu, report that the player can't think of anything to say,
        ! otherwise assume that something appropriate has already been
        ! printed in our reply.
        glk_set_window(gg_mainwin);
!       if (qtype == MainMenu)           (c) Alpha
          print (string) GT_NOQUIP;
        glk_set_window(gg_conversawin);
        return;
      }

      if (gg_conversawin == 0) {
!        glk_set_config(config_WPaddingY, 20);
        gg_conversawin = glk_window_open(gg_mainwin,
                                         winmethod_Below + winmethod_Fixed +
                                         winmethod_Border,
                                         onoptions + 1, wintype_TextGrid,
                                         GG_CONVERSAWIN_ROCK);
        glk_set_window(gg_conversawin);
        glk_request_hyperlink_event(gg_conversawin);
      } else {
        self.cambiar_altura(onoptions);
      }

      new_line;
      ! Go through the options again, now printing a list of those that
      ! are on. Note that we can reuse the variables quipnum and
      ! numoptions from the previous iteration.
      for (onoptions = 0, o = 1 : o <= numoptions : o++) {
        ! get this quip
        curquip = self.quip(quipnum, o);

        ! see if it's on
        if (self.qtest(curquip)) {
          ! print the number of this option (not the internal number,
          ! of course, but one that starts at 1 and increases for every
          ! option that we display)
          self.mostrar_opcion(curquip, ++onoptions);
        }
      }

      if (~~killz) self.mostrar_opcion(curquip, 0);

      y = 0;

      ! Now get the response from the player. Keep on asking until we get
      ! an acceptable answer.
      do {
        glk_window_move_cursor(gg_conversawin, 1, y + 1);
        print ">";
        selected = ZIPI_tecla(gg_conversawin);
        glk_window_move_cursor(gg_conversawin, 1, y + 1);
        print " ";

        switch (selected) {
          'q', 'Q', keycode_Escape:
            selected = '0';

          keycode_Up:
            if (y <= 0) continue;
            do {
              y--;
            } until (y == 0 ||
                     glk_window_get_char(gg_conversawin, 3, y + 1) == '[');
            continue;

          keycode_Down:
            if (y >= self.altura() - 2) continue;
            oldy = y;
            do {
              y++;
            } until (y == self.altura() - 1 ||
                     glk_window_get_char(gg_conversawin, 3, y + 1) == '[');
            if (glk_window_get_char(gg_conversawin, 3, y + 1) ~= '[') y = oldy;
            continue;

          keycode_Return:
            selected = glk_window_get_char(gg_conversawin, 4, y + 1);
            break;
        }
          
        selected = selected - 48;

        ! disallow 0, if desired
        if (killz && selected == 0) selected = -1;
      } until (selected >= 0 && selected <= onoptions);

      ! check for 0, which means that we're leaving this conversation
      if (selected == 0) {
        ! print a notice saying that the PC decided not to say anything
        ! after all, but only if nothing has actually been said so far
        glk_set_window(gg_mainwin);
!       if (~~spoken)                    (c) Alpha
          print (string) GT_NOSAY;
        glk_set_window(gg_conversawin);
        return;
      }

      glk_window_clear(gg_conversawin);

      ! go through the options once again, this time to retrieve the
      ! option that the player selected, and thus the quip that we want
      ! to process next
      for (onoptions = 0, o = 1 : o <= numoptions : o++) {
        ! get this quip
        curquip = self.quip(quipnum, o);

        ! see if it's on
        if (self.qtest(curquip)) {
          ! it's on, but is it the one the player wants?
          if (selected == ++onoptions) {
            if (no_decir_nada) {
              ret = self.quip(curquip * 10 + 2);
              if (ret) return ret;
              else     return selected;
            } else {
              ! It's the right one. Just break out of this loop, since
              ! we'll then jump back to the beginning with curquip set
              ! to the quip that the player selected.
              ! print the option text
              glk_set_window(gg_mainwin);
              player.dice_inicio();
              self.quip(curquip * 10 + 1);
              player.dice_fin();
              glk_set_window(gg_conversawin);
              break;
            }
          }
        }
      }
    }
  ],
  
  ! The following three methods are a bit limited in their use, since the
  ! Z-machine has a fixed limit on the number of arguments that can be
  ! given to a method. If we are compiling for Glulx, these will be
  ! replaced (at run-time) with better versions defined further on.
  
  ! turn at most two quips either on or off
  qset [ a b c d;
    qset_(self, a, b);
    qset_(self, c, d);
  ],
  
  ! Turn on at most five quips. The funky syntax here just means that if an
  ! argument is empty, then we don't bother checking those that follow
  qon [ a b c d e;
    if (a) { qset_(self, a, 1);
    if (b) { qset_(self, b, 1);
    if (c) { qset_(self, c, 1);
    if (d) { qset_(self, d, 1);
    if (e) { qset_(self, e, 1); }}}}}
  ],
  
  ! turn off at most five quips
  qoff [ a b c d e;
    if (a) { qset_(self, a, 0);
    if (b) { qset_(self, b, 0);
    if (c) { qset_(self, c, 0);
    if (d) { qset_(self, d, 0);
    if (e) { qset_(self, e, 0); }}}}}
  ],
  
  ! test a given quip to see if it is on or off
  qtest [ qp byte bits;
    ! check for too high quips, but only if we're compiling for debugging
    #ifdef DEBUG;
    if (qp > self.maxquip) {
      print "Gtalk: tried to test quip ", qp, " of ", (name) self,
            ", but the highest available quip is ", self.maxquip,
            ". Refer to the documentation on how to enlarge the qflag array.^";
      rfalse;
    }
    #endif;
  
    ! Break down the quip number into bytes and bits. We can't optimize
    ! away the division, since the Z-machine lacks a rotate operator, but
    ! we can at least substitute the modulo for a bit-wise AND.
    byte = qp / 8;
    bits = GT_Powers->(qp & $$111);
  
    ! return true if the quip is on
    return (self.&qflag->byte & bits == bits);
  ]
;


! Internal function: set a character's quip to a given on/off state.
[ qset_ char qp state byte bits;
  ! In most cases the quips will already be initialized
  ! at this point. But, just in case an author wants to
  ! turn quips on/off before the conversation menu is
  ! called up, initialize the quips here if necessary.
  if (char.maxquip == 0) initquips(char);

  ! check for too high quips, but only if we're compiling for debugging
  #ifdef DEBUG;
  if (qp > char.maxquip) {
     print "Gtalk: tried to set quip ", qp, " of ", (name) char,
           ", but the highest available quip is ", char.maxquip,
           ". Refer to the documentation on how to enlarge the qflag array.^";
     return;
  }
  #endif;
   
  ! break down the quip number into bytes and bits
  byte = qp / 8;
  bits = GT_Powers->(qp & $$111);

  ! turn the bit in question on or off
  if (state) char.&qflag->byte = char.&qflag->byte | bits;
  else       char.&qflag->byte = char.&qflag->byte & ~bits;
];


! Internal function: do some initialization for a given character.
[ initquips char qp;
  ! use glulx versions of some methods if we can
  #ifdef TARGET_GLULX;
  char.qon  = CharacterGlulx.qon;
  char.qoff = CharacterGlulx.qoff;
  char.qset = CharacterGlulx.qset;
  #endif;

  ! initialize the maxquip value
  char.maxquip = char.#qflag * 8 - 1;

  ! go through all the quips
  for (qp = 1 : qp <= char.maxquip : qp++) {
    ! see if we want quips to be on off by default
    #ifdef QuipsOnByDefault;
    qqon = 1;
    #ifnot;
    qqon = 0;
    #endif;

    ! Obtain the desired qqon value for this quip. We'll use the default
    ! unless the quip says otherwise.
    char.quip(qp * 10 + 3);
    if (qqon) char.qon(qp);
  }
];


#ifdef TARGET_GLULX;
! Glulx versions of a couple of methods. 
Object CharacterGlulx
  with
  qset [ _vararg_count qp state;
    while (_vararg_count > 0) {
      @copy sp qp;
      @copy sp state;
      qset_(self, qp, state);
      _vararg_count = _vararg_count - 2;
    }
  ],

  qon [ _vararg_count qp;
    while (_vararg_count > 0) {
      @copy sp qp;
      qset_(self, qp, 1);
      _vararg_count--;
    }
  ],

  qoff [ _vararg_count qp;
    while (_vararg_count > 0) {
      @copy sp qp;
      qset_(self, qp, 0);
      _vararg_count--;
    }
  ]
;
#endif;


#ifdef AutoDeactivate;
! Internal function: turn off all quips that have options that are all off.
! We only compile and make use of this if requested to.
[ AutoDeactivateQuips char keepgoing qp o quipnum numoptions;
  do {
    ! assume that we won't have to repeat this
    keepgoing = false;

    ! go through all quips
    for (qp = 1 : qp <= char.maxquip : qp++) {
      ! we don't need to check quips that are already off
      if (char.qtest(qp)) {
        ! get the option list and the number of options for this quip
        quipnum = qp * 10 + 4;
        numoptions = char.quip(quipnum, 0);

        ! see if we have any options at all
        if (numoptions) {
          ! go through the options
          for (o = 1 : o <= numoptions : o++) {
            ! If the quip for this option is on, then we can skip the
            ! current quip and move on to the next one. (Sorry for the
            ! spaghetti.)
            if (char.qtest(char.quip(quipnum, o))) jump nextquip;
          }

          ! We found no options that were on, so turn off this quip.
          ! Since this could affect other quips that have this quips as
          ! one of their options, also note that we have to go through
          ! this loop at least once more.
          qset_(char, qp, 0);
          keepgoing = true;
        }
      }
.nextquip;
    }
  } until (~~keepgoing);
];
#endif;

