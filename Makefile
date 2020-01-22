PREFIX=/usr
BINDIR	= $(PREFIX)/bin
MANDIR	= $(PREFIX)/share/man/zh_TW/man1

PROGS		= cdate
CFLAGS		= -Wall -Wextra -pedantic -std=c89 -O2 -fPIE -fPIC -fstack-protector-strong


all: $(PROGS)
install: $(PROGS)
	mkdir -p $(DESTDIR)$(BINDIR)
	cp  $(PROGS) $(DESTDIR)$(BINDIR)
	mkdir -p $(DESTDIR)$(MANDIR)
	cp  $(PROGS).1 $(DESTDIR)$(MANDIR)
uninstall: $(PROGS)
	$(RM) $(DESTDIR)$(BINDIR)/$(PROGS)
	$(RM) $(DESTDIR)$(MANDIR)/$(PROGS).1
clean:
	$(RM) $(PROGS)
