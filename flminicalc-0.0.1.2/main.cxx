/*
 * main.cxx
 */

#include "flminicalc.h"
#include "main.h"

#define MAXLEN 10
#define MAXLENS "10"

#define DEBUG 0

/* fl2str
 * value shown by calculator is stored as double
 * but we only display decimal part if it's there
 * hence this special routine
 * maybe ought to be in CalcUI. dunno.
 * returns NULL if an error occurs
 */
char *fl2str(double val) {

        static char str[MAXLEN+1];
	char *buf; int i, d, s, o = 0;

	if (val == (int)val) {
		/* integer */
		sprintf(str, "%" MAXLENS ".d", (int)val);

		/* sprintf fails to give a lone 0 */
		if(val == 0) {
			str[MAXLEN-1] = '0';
		}
#if DEBUG
		printf("str - '%s', val %f\n", str, val);
#endif
	} else {
		/* ecvt - p is precision, d is loc of dp, s is sign flag */
		buf = ecvt(val, MAXLEN, &d, &s);

		if(d < 1) {
			/* handle small numbers */
			if(d < -3) {
				if(snprintf(str, MAXLEN+1, "%" MAXLENS ".3g", val) == -1) {
					return NULL;
				}
			} else {
				if(snprintf(str, MAXLEN+1, "%" MAXLENS "." MAXLENS "g", val) == -1) {
					return NULL;
				}
			}
		} else if (d > (MAXLEN - 1)) {
			/* handle big numbers */
			/* if overflow, show scientific notation */
			/* Refuse to truncate the string */
			if(snprintf(str, MAXLEN+1, "%" MAXLENS ".3g", val) == -1) {
				return NULL;
			}
		} else {
			/* between 1 and 1eMAXLEN we handle ourselves */
			/* count zeros at end */
			o = -1;
			for(i = MAXLEN - 1; i > 0; i--) {
				if(buf[i] == '0') {
					o++;
				} else {
					break;
				}
			}
			
			/* jiggery pokery with o gets the right
			 * number of spaces at the beginning
			 */
			for(i = 0; i < (MAXLEN); i++) {
				if(i < o) {
					str[i] = ' ';
					if(s && (i+1 == o)) {
						str[i] = '-';
					}
					continue;
				}
				if((i - o) < d) {
					str[i] = buf[i - o];
				} else if((i - o) == d) {
					str[i] = '.';
				} else {
					str[i] = buf[i - o - 1];
				}
			}			       
		}
#if DEBUG
		printf("buf '%s' d %d, s %d, val %f, o %d, str '%s', \n", buf, d, s, val, o, str);
#endif
	}

	return str;

}

/* main
 * initialise the UI
 * show it
 * call FLTK loop
 */
int main(int argc, char **argv) {

	CalcUI *ui = new CalcUI;
	ui->show(argc, argv);
	/* call update win after showing to get 0 shown */
	ui->update_win();
	return Fl::run();

}
