#
#---------------------------------------------------------------------------
#       Makefile for the Queue Function Library
#---------------------------------------------------------------------------

AR      =ar
ARFLAGS =-r

CC      =gcc
#CCFLAGS =-Wall -g -std=c11
#CCFLAGS =-Wall -g -std=c99
#CCFLAGS =-Wall -g -std=c90
#CCFLAGS =-Wall -g -ansi -Wpedantic
CCFLAGS =-Wall -g

HDRS    =../dque.h

OBJS1   =dque_create.o dque_destroy.o dque_error.o dque_options.o
OBJS2   =dque_empty.o dque_size.o dque_max_size.o dque_key_erase.o dque_key_find.o dque_key_insert.o
OBJS3   =dque_back.o dque_front.o dque_get_data.o dque_set_data.o
OBJS4   =dque_begin.o dque_next.o dque_end.o dque_rbegin.o dque_rnext.o dque_rend.o
OBJS5	=dque_insert.o dque_erase.o dque_pop_back.o dque_pop_front.o dque_push_back.o dque_push_front.o
OBJS6   =dque_myalloc.o dque_mydelete.o dque_myfind.o dque_myinsert.o dque_myscan.o dque_mysearch.o
OBJS    =$(OBJS1) $(OBJS2) $(OBJS3) $(OBJS4) $(OBJS5) $(OBJS6)

LIBS    =../libdque.a

all:    $(LIBS) $(HDRS)

$(OBJS): %.o: %.c
	$(CC) -c $(CCFLAGS) $< -o $@

../dque.h:     dque.h
	cp dque.h ..

../libdque.a:  $(HDRS) mydque.h $(OBJS)
	rm -f ../libdque.a
	$(AR) $(ARFLAGS) ../libdque.a $(OBJS)

test:	test.c dque_util.c dque_util.h $(LIBS)
	$(CC) $(CCFLAGS) test.c dque_util.c $(LIBS) -o $@

dque_create.o:      dque_create.c     $(HDRS)

dque_destroy.o:     dque_destroy.c    $(HDRS)

dque_erase.o:       dque_erase.c      $(HDRS)

dque_error.o:       dque_error.c      $(HDRS)

dque_options.o:     dque_options.c    $(HDRS)

dque_empty.o:       dque_empty.c      $(HDRS)

dque_size.o:        dque_size.c       $(HDRS)

dque_max_size.o:    dque_max_size.c   $(HDRS)

dque_back.o:        dque_back.c       $(HDRS)

dque_front.o:       dque_front.c      $(HDRS)

dque_get_data.o:    dque_get_data.c   $(HDRS)

dque_set_data.o:    dque_set_data.c   $(HDRS)

dque_begin.o:       dque_begin.c      $(HDRS)

dque_next.o:        dque_next.c       $(HDRS)

dque_end.o:         dque_end.c        $(HDRS)

dque_rbegin.o:      dque_rbegin.c     $(HDRS)

dque_rnext.o:       dque_rnext.c      $(HDRS)

dque_rend.o:        dque_rend.c       $(HDRS)

dque_key_erase.o:   dque_key_erase.c  $(HDRS)

dque_key_find.o:    dque_key_find.c   $(HDRS)

dque_key_insert.o:  dque_key_insert.c $(HDRS)

dque_insert.o:      dque_insert.c     $(HDRS)

dque_pop_back.o:    dque_pop_back.c   $(HDRS)

dque_pop_front.o:   dque_pop_front.c  $(HDRS)

dque_push_back.o:   dque_push_back.c  $(HDRS)

dque_push_front.o:  dque_push_front.c $(HDRS)

dque_myalloc.o:     dque_myalloc.c    $(HDRS)

dque_mydelete.o:    dque_mydelete.c   $(HDRS)

dque_myfind.o:      dque_myfind.c     $(HDRS)

dque_myinsert.o:    dque_myinsert.c   $(HDRS)

dque_myscan.o:      dque_myscan.c     $(HDRS)

dque_mysearch.o:    dque_mysearch.c   $(HDRS)

.PHONY : clean
clean:
	rm -f *.o
	rm -f *~

# end
