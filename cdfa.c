#include <stdlib.h>
#include <stdio.h>
#include <strings.h>
#include <unistd.h>
#include "graphviz/cgraph.h"
#include "graphviz/gvc.h"

void viz(Agraph_t *g)
{
	static GVC_t *gvc;
	if (!gvc)
		gvc = gvContext();
	gvLayout(gvc, g, "dot");
	gvRender(gvc, g, "svg", stdout);
	gvFreeLayout(gvc, g);
}

Agnode_t *delta(Agraph_t *g, Agnode_t *n, char c)
{
	Agedge_t *e;
	for (e = agfstout(g, n); e; e = agnxtout(g, e))
		if (strchr(agget(e, "label"), c)) {
			printf("\t%s\t-- %c -->\t", agnameof(n), c);
			printf("%s\t", agnameof(e->node));
			return e->node;
		}
	printf("No transition, this is an NFA\n");
	exit(1);
}

void process(Agraph_t *g)
{
	int i, j, len;
	char in[1024];
	Agnode_t *n, *fst;
	fst = agnode(g, "start", FALSE);
	if (fst)
		n = agfstout(g, fst)->node;
	else
		printf("Please create a `start' node");
	while (scanf("%s", in) != EOF) {
		len = strlen(in);
		for (i = 0; i < len; i++) {
			/* Scanned intput */
			for (j = len - i; j < len; j++)
				putchar(in[j]);
			n = delta(g, n, in[i]);
			/* Unscanned input */
			for (j = 0; j < len - i; j++)
				putchar(in[j]);
			putchar('\n');
		}
		if (!strcmp(agget(n, "shape"), "doublecircle"))
			printf("Accepted\n");
		else
			printf("Declined\n");
	}
}

char *usages = "usage: %s [-s?]\n\
	-v	print svg to stdout\n\
	-?	print usage\n";

void usage()
{
	printf(usages, "cdfa");
	exit(0);
}

int main(int argc, char **argv)
{
	Agraph_t *g;
	FILE *fd;
	char ch;
	int vflag;
	while ((ch = getopt(argc, argv, "v?")) != -1) {
		switch (ch) {
		case 'v':
			vflag = 1;
			break;
		case '?':
			usage();
		}
	}
	fd = fopen("even.dot", "r");
	g = agread(fd, NULL);
	fclose(fd);
	if (vflag)
		viz(g);
	else
		process(g);
	agclose(g);
	return 0;
}
