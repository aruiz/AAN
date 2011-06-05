all: 9_perona_malik_p1 9_perona_malik_p2 9_perona_malik_p3

9_perona_malik_p1: 9_perona_malik.c float_utils.c aan_mascara.c float_utils.h aan_mascara.h ami.h ami_bmp.h aan_perona_malik.c aan_perona_malik.h
	gcc -lm -DP1 -o 9_perona_malik_p1 -g 9_perona_malik.c float_utils.c aan_mascara.c aan_perona_malik.c

9_perona_malik_p2: 9_perona_malik.c float_utils.c aan_mascara.c float_utils.h aan_mascara.h ami.h ami_bmp.h aan_perona_malik.c aan_perona_malik.h
	gcc -lm -DP2 -o 9_perona_malik_p2 -g 9_perona_malik.c float_utils.c aan_mascara.c aan_perona_malik.c

9_perona_malik_p3: 9_perona_malik.c float_utils.c aan_mascara.c float_utils.h aan_mascara.h ami.h ami_bmp.h aan_perona_malik.c aan_perona_malik.h
	gcc -lm -DP3 -o 9_perona_malik_p3 -g 9_perona_malik.c float_utils.c aan_mascara.c aan_perona_malik.c

videos:
	cd p1 && make
	cd p2 && make
	cd p3 && make

clean:
	rm -rf p*/*bmp p*/*avi
	rm -rf 9_perona_malik_p*
