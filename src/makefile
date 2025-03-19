all : chiral letterart colorstates drawit

chiral : chiral.c
	cc chiral.c -o chiral

#  for the programs that use ImageMagick
IMFLAGS = -I/usr/include/ImageMagick-6 -I/usr/include/arm-linux-gnueabihf/ImageMagick-6 -DMAGICKCORE_QUANTUM_DEPTH=16 -DMAGICKCORE_HDRI_ENABLE=0
IMLIBS = /usr/lib/arm-linux-gnueabihf/libMagickWand-6.Q16.so.6 /usr/lib/arm-linux-gnueabihf/libMagickCore-6.Q16.so.6

letterart : letterart.c
	cc $(IMFLAGS) letterart.c $(IMLIBS) -o letterart

drawit : drawit.c
	cc $(IMFLAGS) drawit.c $(IMLIBS) -o drawit

colorstates : colorstates.c
	cc $(IMFLAGS) colorstates.c $(IMLIBS) -o colorstates

clean:
	@rm -f chiral letterart drawit colorstates

