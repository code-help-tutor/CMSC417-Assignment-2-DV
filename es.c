WeChat: cstutorcs
QQ: 749389476
Email: tutorcs@163.com
#ifndef _ES_C_
#define _ES_C_

/* $Id: es.c,v 1.1 2000/03/01 14:09:09 bobby Exp bobby $
 * ----
 * $Revision: 1.1 $
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/time.h>
#include <string.h>

#include "queue.h"
#include "common.h"
#include "es.h"
#include "ls.h"
#include "rt.h"
#include "n2h.h"

struct el *g_lst;

int init_new_el()
{
	InitDQ(g_lst, struct el);
	assert(g_lst);

	g_lst->es_head = 0x0;
	return (g_lst != 0x0);
}

void add_new_es()
{
	struct es *n_es;
	struct el *n_el = (struct el *)
		malloc(sizeof(struct el));

	struct el *tail = g_lst->prev;
	InsertDQ(tail, n_el);

	// added new es to tail
	// lets start a new queue here

	{
		struct es *nhead = tail->es_head;
		InitDQ(nhead, struct es);

		tail = g_lst->prev;
		tail->es_head = nhead;

		n_es = nhead;

		n_es->ev_ty = _es_null;
		n_es->peer0 = n_es->peer1 =
			n_es->port0 = n_es->port1 =
				n_es->cost = -1;
		n_es->name = 0x0;
	}
}

void add_to_last_es(e_type ev,
					node peer0, int port0,
					node peer1, int port1,
					int cost, char *name)
{
	struct el *tail = g_lst->prev;
	bool local_event = false;

	assert(tail->es_head);

	// check for re-defined link (for establish)
	// check for local event (for tear-down, update)
	switch (ev)
	{
	case _es_link:
		// a local event?
		if ((peer0 == get_myid()) || peer1 == get_myid())
			local_event = true;
		break;
	case _ud_link:
		// a local event?
		if (geteventbylink(name))
			local_event = true;
		break;
	case _td_link:
		// a local event?
		if (geteventbylink(name))
			local_event = true;
		break;
	default:
		printf("[es]\t\tUnknown event!\n");
		break;
	}

	if (!local_event)
	{
		printf("[es]\t Not a local event, skip\n");
		return;
	}

	printf("[es]\t Adding into local event\n");

	{
		struct es *es_tail = (tail->es_head)->prev;

		struct es *n_es = (struct es *)
			malloc(sizeof(struct es));

		n_es->ev_ty = ev;
		n_es->peer0 = peer0;
		n_es->port0 = port0;
		n_es->peer1 = peer1;
		n_es->port1 = port1;
		n_es->cost = cost;
		n_es->name = (char *)
			malloc(strlen(name) + 1);
		strcpy(n_es->name, name);

		InsertDQ(es_tail, n_es);
	}
}

/*
 * print out the whole event LIST
 */
void print_el()
{
	struct el *el;
	struct es *es_hd;
	struct es *es;

	assert(g_lst->next);

	printf("\n\n");
	printf("[es] >>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<<<<\n");
	printf("[es] >>>>>>>>>> Dumping all event sets  <<<<<<<<<<<<<\n");
	printf("[es] >>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<<<<\n");

	for (el = g_lst->next; el != g_lst; el = el->next)
	{
		assert(el);
		es_hd = el->es_head;
		assert(es_hd);

		printf("\n[es] ***** Dumping next event set *****\n");

		for (es = es_hd->next; es != es_hd; es = es->next)
			print_event(es);
	}
}

/*
 * print out one event: establish, update, or, teardown
 */
void print_event(struct es *es)
{
	assert(es);

	switch (es->ev_ty)
	{
	case _es_null:
		printf("[es]\t----- NULL event -----\n");
		break;
	case _es_link:
		printf("[es]\t----- Establish event -----\n");
		break;
	case _ud_link:
		printf("[es]\t----- Update event -----\n");
		break;
	case _td_link:
		printf("[es]\t----- Teardown event -----\n");
		break;
	default:
		printf("[es]\t----- Unknown event-----\n");
		break;
	}
	printf("[es]\t link-name(%s)\n", es->name);
	printf("[es]\t node(%d)port(%d) <--> node(%d)port(%d)\n",
		   es->peer0, es->port0, es->peer1, es->port1);
	printf("[es]\t cost(%d)\n", es->cost);
}

struct es *geteventbylink(char *lname)
{
	struct el *el;
	struct es *es_hd;
	struct es *es;

	assert(g_lst->next);
	assert(lname);

	for (el = g_lst->next; el != g_lst; el = el->next)
	{

		assert(el);
		es_hd = el->es_head;
		assert(es_hd);

		for (es = es_hd->next; es != es_hd; es = es->next)
			if (!strcmp(lname, es->name))
				return es;
	}
	return 0x0;
}

#endif
