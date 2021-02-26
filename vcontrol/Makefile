.PHONY: main doc upload dist clean veryclean

main:
	$(MAKE) -C build main

upload:
	$(MAKE) -C build upload_main

doc:
	$(MAKE) -C doc/report all

dist:
	$(MAKE) -C buid dist

clean:
	$(MAKE) -C build clean
	$(MAKE) -C doc/report clean
	cd src;  \rm -f *~ 
	cd test; \rm -f *~

veryclean: clean
	$(MAKE) -C build veryclean
	$(MAKE) -C doc/report veryclean
	cd src;  \rm -f \#*\#
	cd test; \rm -f \#*\#

