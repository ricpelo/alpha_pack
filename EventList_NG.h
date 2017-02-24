! Version 1.00 of EventList.h, by Jim Aikin.
!
! Please report any bugs or make feature suggestions to midiguru23@sbcglobal.net.
!
! Versión NG de Alpha Aventuras:
! - Ahora la rutina run_script() no imprime las cadenas, sino que las devuelve
! - Se añade la rutina at_end() en StopEventList para saber si se ha llegado
!   al final de la lista.
!
! The main purpose of EventList.h is to allow more realistic NPC conversations. For instance,
! when an NPC doesn't have a response to a given topic when asked or told about it, you may
! want to write several messages along the lines of "Betty is gazing at her shoes, and doesn't
! seem to have heard you." The weakness of Inform's random number generator is that, even if you
! give it a number of options of this sort, it's quite capable of printing the same option
! four or five times before it chooses a different one.
!
! A ShuffledEventList doesn't have this weakness. It will always go through all of its available
! outputs, then reshuffle them in a new order and start over. Occasionally the last item in one
! run-through will be repeated as the first item in the next run-through, but other than that
! you should see no repetition.
!
! A ShuffledEventList is also useful for adding randomized atmospheric text events to a room,
! as the list could be invoked by a daemon.
!
! The EventList classes are also able to run property methods and global Routines,
! so they're capable of far more functionality than simply printing out text.
!
! Objects of the EventList classes can have up to 16 events in a property called event_list.
! The events can be double-quoted strings (which will be printed), names of global Routines
! (which will be run), or names of local property methods within the EventList object itself
! (which will be run). The items in the event_list are NOT separated by commas when the
! object is coded, because they're stored in a property array.
!
! For example:
!
!  ShuffledEventList my_list
!    with
!    event_list
!      "Oh, what a beautiful mornin'!^"
!      "Oh, what a beautiful day!^"
!      "I got a wonderful feelin',^"
!      "Everything's goin' my way.^"
!      setVariable
!    ,
!    setVariable [;
!      some_obj.foo = 3;
!      "There's a bright golden haze on the meadow...^";
!    ]
!  ;
!
! You should never use EventList itself; you should always instantiate objects of
! one of its child classes -- ShuffledEventList, CyclicEventList, StopEventList, or
! the more powerful DistributedEventList.
!
! To use an EventList, create an EventList object of one of these types, give it an
! event_list containing one or more events, and then call the object's run_script
! method each time you want an event to fire off:
!
!  my_list.run_script();
!
! In the unlikely event that you need more than 16 events in a list, you can extend
! the numList, which is also a property array, in your EventList object itself, like this:
! 
! numList 0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23,
!
! The code above can be used with event_list arrays of up to 24 events.
!
! In a ShuffledEventList, the items in event_list will print or run the first time in
! the order given in the source code, after which the list will be shuffled randomly and
! rerun in the shuffled order. Each time all of the items in the list have been used, the
! list will be shuffled again. (In fact, the list isn't shuffled -- what's shuffled is
! the integers in numList, which amounts to the same thing, because they're used as
! pointers to what to print next.)
!
! In a CyclicEventList, each call to runScript causes an event to fire and then advances
! the pointer to the next item in the list. When the end of the list is reached, the
! pointer goes back to the beginning of the list and starts over.
!
! In a StopEventList, the items in event_list will print or run in the order given in
! the source code, and when the last item is reached, it will always print or run
! thereafter: The list is never shuffled or restarted from the top, although you can
! restart it in your own code if you need to by setting listPtr = 0 on the StopEventList
! object. Doing this reset on a ShuffledEventList won't reset the list to its initial
! order in the source code, but it will cause the list to start over and do a complete
! run-through of the events in their current random order, whatever that happens to be.
!
! A StopEventList is the tool of choice for conversation systems in which you want an
! NPC to respond to a question in a chatty manner the first time (and possibly again on
! the second and third queries), but thereafter you want the output to be something along
! the lines of, "The butler has already told you everything he knows about Lord Downey
! -- namely that His Lordship is mad as a loon, and owns a shotgun."
!
! The main reason to use a property method in an EventList is because it allows you to change
! the game environment in arbitrary ways when a given event runs. In a conversation system,
! for instance, you could keep track of what the player knows as a result of the conversation
! by using property methods to set variables before printing the corresponding text.
!
! In theory, one could replace the contents of event_list at run-time, or add to it,
! but replacing the list with a shorter list would be treacherous unless one were
! to reset numList to its default order at the same time. A better solution would be
! to create two independent EventLists and switch between them using a DistributedEventList
! as the master object.
!
! --- THE DISTRIBUTEDEVENTLIST CLASS ---
!
! Using the DistributedEventList class, you can switch among two or more EventLists at
! run-time. This is useful, for instance, if you want an NPC to shuffle through three responses
! to a certain conversation topic during the first part of the game, and three entirely different
! responses to the same topic during a different part of the game. You could do this with if-tests
! in the NPC's life routine, but if you use a DistributedEventList, the conversation topics in the
! life routine can be very simple to read, and resistant to bugs.
!
! To use this functionality, create a DistributedEventList, and then create one or more
! EventLists children of the DistributedEventList, using -> notation. The children can
! even be further DistributedEventLists, if you think you can keep track of the permutations.
! Any of the child lists can be active or inactive at any point in the game. The
! DistributedEventList should also have one or more default events that it can use if no
! child EventLists are active. The DistributedEventList needs only one new property: In order
! to output events from its default list, it needs to know whether it's a CYCLIC, STOP, or
! SHUFFLED type. You might create it like this:
!
! DistributedEventList main_list
!     with
!     type STOP,
!     event_list
!     "Some text to be printed by default."
!     "More text to be printed by default."
! ;
!
! StopEventList -> sub_list
!     with
!     event_list
!     "Some text to be printed when this list is active."
!     "Other text to be printed when this list is active."
!     shut_self_off,
!     shut_self_off [;
!         self.active = false;
!         "Final text output from the sub_list before it shuts off.";
!     ]
! ;
!
! To invoke a DistributedEventList, just use its run_script() routine in the usual way.
!
! EventLists are active by default. They can be deactivated in three ways: You can set active = false
! for the list at any point in your code; or you can write an is_active routine as a property of the list,
! which will return true or false according to some set of conditions that you create; or you can move a list
! out of its master DistributedEventList by writing:
!
! remove sub_list;
!
! Conversely, you can restock a DistributedEventList by writing:
!
! move sub_list to main_list;
!
! As long as the sub-list is active, this will make its events available to the master list. Note,
! however, that the order of sub-lists in the master list is significant. The DistributedEventList will
! always fire an event from the first child list that is active. If you want to add a sub-list to a
! master list and make it the eldest child, you'll have to manually move any other sub-lists out of the
! master list and then move them back in. (Making them inactive is easier.)

! --- THE SYNCEVENTLIST CLASS ---
!
! A SyncEventList is designed to run an item with the same number as the item most recently run by
! another list. The SyncEventList can have either more events or fewer than its master list. (If it
! has more, the excess items will never run.) It has two new properties -- master_list and wrap.
! The master_list property tells it what list to sync to. This property can be changed at run-time.
!
! The wrap property is a boolean (true or false) value. When the master list contains more items than
! the SyncEventList, its most recently fired item will sometimes be past the end of the list of available
! events in the SyncEventList. If wrap is true, the SyncEventList will start over at its first event
! and proceed through its list again. If wrap is false, the SyncEventList will "pin" at its last event
! whenever the master list has just fired an event higher in its list.

Class EventList
  with
    is_active [; return self.active; ],
    active true,
    run_script [
      i len r;
      len = self.#event_list / WORDSIZE;
      i = self.&numList-->self.listPtr;
      self.prev_item_number = i;
      r = self.do_stuff(i);
      self.maybeAdjustPtr(len);
      return r;
    ],
    ! The get_item_number routine returns the next item that will be run when
    ! the list is called.
    prev_item_number 0,
    get_prev_item_number [; return self.prev_item_number; ],
    get_item_number      [; return self.&numList-->self.listPtr; ],
    get_list_length      [; return self.#event_list / WORDSIZE; ],
    maybeAdjustPtr [ len;
      self.listPtr++;
      if (self ofclass ShuffledEventList) {
        if (self.listPtr == len) {
          self.shuffle();
          self.listPtr = 0;
        }
      } else if (self ofclass CyclicEventList) {
        if (self.listPtr == len) self.listPtr = 0;
      } else if (self ofclass StopEventList) {
        if (self.listPtr >= len) self.listPtr = len - 1;
      } else {
        "Error -- attempting to operate an EventList of an unknown class.";
      }
    ],
    do_stuff [ i
      f;
      f = self.&event_list-->i;
      if (f ofclass String)       return f;
      else if (f ofclass Routine) return f();
      else {
        if (self provides f)      return self.f();
        else                      return "Error in EventList data.";
      }
    ],
    shuffle [
      i j len;
      len = self.#event_list / WORDSIZE;
      for (i = 0 : i < len : i++) {
        j = random(i + 1) - 1;
        self.&numList-->i = self.&numList-->j;
        self.&numList-->j = i;
      }
    ],
    ! This is a fake event_list, included simply to create the property on the class:
    event_list "Weasels ripped my flesh."
               "On the other hand, they might be giants.",
    numList 0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15,
    listPtr 0;
    
Class ShuffledEventList
  class EventList;

Class StopEventList class EventList
  with
    at_end [; return self.get_item_number() == self.get_list_length() - 1; ];

Class CyclicEventList
  class EventList;

Class SyncEventList class EventList
  with
    master_list 0,
    wrap false,
    run_script [
      i master len master_len;
      master = self.master_list;
      if (master) {
        i = master.get_prev_item_number();
        master_len = master.get_list_length();
        print "^Testing: i is ", i, ", master_len is ", master_len, ".^";
        
        len = self.get_list_length();
        ! Possibly the master list is longer than we are, and is now primed to
        ! run an event that we don't have:
      .ReduceAgain;
        if (i >= len) {
          if (self.wrap) i = i - len;
          else           i = len - 1;
        }
        if (i >= len) jump ReduceAgain;
      } else i = 0;
      self.do_stuff(i);
    ];

Constant SHUFFLED = 0;
Constant STOP = 1;
Constant CYCLIC = 2;

Class DistributedEventList class EventList
  with
    run_script [
      obj active;
      if (~~child(self)) {
        self.run_local_script();
        rtrue;
      }
      active = false;
      obj = child(self);
      do {
        active = obj.is_active();
        if (active) break;
        obj = sibling(obj);
      } until (obj == nothing);
      if (active) {
        obj.run_script();
        rtrue;
      }
      self.run_local_script();
      rtrue;
    ],
    ! The run_local_script routine does just what run_script does in the standard EventList,
    ! but in a DistributedEventList it will only be called if all of the available child objects
    ! are inactive.
    run_local_script [
      i len;
      len = self.#event_list / WORDSIZE;
      i = self.&numList-->self.listPtr;
      self.do_stuff(i);
      self.maybeAdjustPtr(len);
    ],
    ! The DistributedEventList can't use the ofclass test when adjusting its pointer to
    ! internal events, so it needs a type property, which can be SHUFFLED, CYCLIC, or STOP.
    ! By default, the class has type set to STOP.
    maybeAdjustPtr [ len;
      self.listPtr++;
      if (self.type == SHUFFLED) {
        if (self.listPtr == len) {
          self.shuffle();
          self.listPtr = 0;
        }
      } else if (self.type == CYCLIC) {
        if (self.listPtr == len) self.listPtr = 0;
      } else if (self.type == STOP) {                   ! We're a StopEventList:
        if (self.listPtr >= len) self.listPtr = len - 1;
      } else {
        "Error -- attempting to operate an EventList of an unknown class.";
      }
    ],
    type STOP;

! Dummy objects to shut off annoying compiler warnings:
ShuffledEventList pqr123;
CyclicEventList pqr234;
StopEventList pqr345;
DistributedEventList pqr456;
SyncEventList pqr567;

