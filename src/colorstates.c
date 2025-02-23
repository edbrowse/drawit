/*
colorstates.c: color the states of the U.S. map
with various, mostly primary, colors.
Assumes a template black&white map in us50.png.

http://members.shaw.ca/el.supremo/MagickWand/floodfill.htm

Requires ImageMagick and ImageMagick-devel
CFLAGS = -I/usr/include/ImageMagick-6 -DMAGICKCORE_QUANTUM_DEPTH=16 -DMAGICKCORE_HDRI_ENABLE=0
LDLIBS = /lib/libMagickWand-6.Q16.so /lib/libMagickCore-6.Q16.so
*/

#include <stdio.h>
#include <string.h>
#include <ImageMagick-6/wand/magick_wand.h>
#include <ImageMagick-6/magick/MagickCore.h>

static MagickWand *m_wand;
static PixelWand *fc_wand;
static PixelWand *bc_wand;
static ChannelType channel;

static void fillState0(int x, int y, const char *color);
static void fillState1(const char *state, const char *color);

int main(int argc, char **argv)
{
	FILE *f;
	int x, y;
	char statecode[4];
	char colorname[32];

	if (argc != 3) {
		fprintf(stderr,
			"usage: colorStates assignment-file output-file\n");
		exit(1);
	}

	f = fopen(argv[1], "r");
	if (!f) {
		fprintf(stderr, "colorstates cannot open assignment file %s\n",
			argv[1]);
		exit(1);
	}

	MagickWandGenesis();
	m_wand = NewMagickWand();

	// PixelWands for the fill and bordercolor 
	fc_wand = NewPixelWand();
	bc_wand = NewPixelWand();

	// Convert "rgba" to the enumerated ChannelType required by the floodfill function
	channel = ParseChannelOption("rgba");

// read in the black&white U.S. map
	if (MagickReadImage(m_wand, "us50.png") == MagickFalse) {
		fprintf(stderr,
			"colorstates cannot read the original black & white image of the U.S., in us50.png\n");
		exit(1);
	}

	while (fscanf(f, "%s %s\n", statecode, colorname) == 2)
		fillState1(statecode, colorname);
	fclose(f);

	if (MagickWriteImage(m_wand, argv[2]) == MagickFalse) {
		fprintf(stderr,
			"colorstates cannot create the output file %s\n",
			argv[2]);
		exit(1);
	}

	/* and we're done so destroy the magick wand etc. */
	fc_wand = DestroyPixelWand(fc_wand);
	bc_wand = DestroyPixelWand(bc_wand);
	m_wand = DestroyMagickWand(m_wand);
	MagickWandTerminus();
	exit(0);
}

static void fillState0(int x, int y, const char *color)
{
	PixelSetColor(bc_wand, "white");
	PixelSetColor(fc_wand, color);
	//      The bordercolor=bc_wand (with fuzz of 20 applied) is replaced 
	// by the fill colour=fc_wand starting at the given coordinate.
	// Normally the last argument is MagickFalse so that the colours are matched but
	// if it is MagickTrue then it floodfills any pixel that does *not* match 
	// the target color
	MagickFloodfillPaintImage(m_wand, channel, fc_wand, 20, bc_wand, x, y,
				  MagickFalse);
}

/* approved names for colors, understood by ImageMagick */
static const char *const ok_colors[] = {
	"red", "blue", "green", "yellow", "orange", "pink", "purple",
	    "royalblue",
	0
};

int stringInList(const char *const *list, const char *s)
{
	int i = 0;
	while (*list) {
		if (!strcmp(s, *list))
			return i;
		++i, ++list;
	}
	return -1;
}				/* stringInList */

/* the points that are interior to each state */
struct INSIDE {
	char state[4];
	int x, y;
};

const struct INSIDE inlist[] = {
{"ak",162, 1032},
{"al",1465, 876},
{"ar",1233, 849},
{"az",450, 816},
{"ca",110, 462},
{"co",727, 605},
{"ct",1953, 379},
{"dc",1827, 559},
{"de",1882, 552},
{"fl",1730, 1127},
{"ga",1629, 929},
{"hi",414, 1096},
{"hi",500, 1132},
{"hi",604, 1180},
{"hi",648, 1244},
{"id",416, 302},
{"il",1327, 533},
{"in",1440, 526},
{"ia",1180, 450},
{"ks",1000, 660},
{"ky",1536, 669},
{"la",1237, 1007},
{"ma",1955, 342},
{"md",1826, 529},
{"me",2013, 163},
{"mi",1364, 239},
{"mi",1504, 356},
{"mn",1118, 271},
{"mo",1224, 646},
{"ms",1346, 906},
{"mt",641, 167},
{"nc",1803, 736},
{"nd",906, 216},
{"ne",945, 490},
{"nh",1964, 269},
{"nj",1907, 478},
{"nm",674, 860},
{"nv",284, 534},
{"ny",1849, 352},
{"oh",1550, 524},
{"ok",1026, 830},
{"or",184, 246},
{"pa",1783, 444},
{"ri",1989, 364},
{"sc",1707, 832},
{"sd",947, 349},
{"tn",1452, 776},
{"tx",983, 1009},
{"ut",468, 581},
{"va",1778, 647},
{"vt",1918, 251},
{"wa",238, 102},
{"wi",1309, 311},
{"wv",1679, 609},
{"wy",651, 396},
{"", 0, 0}
};

static void fillState1(const char *state, const char *color)
{
	const struct INSIDE *s;
	int found;

	if (stringInList(ok_colors, color) < 0) {
		fprintf(stderr, "invalid color %s\n", color);
		exit(2);
	}

	found = 0;
	for (s = inlist; s->x; ++s) {
		if (strcmp(state, s->state))
			continue;
		fillState0(s->x, s->y, color);
		found = 1;
	}

	if (!found) {
		fprintf(stderr,
			"colorstates does not recognize the state abbreviation %s\n",
			state);
		exit(2);
	}
}
