WeChat: cstutorcs
QQ: 749389476
Email: tutorcs@163.com
#ifndef _ES_H_
#define _ES_H_

#include "common.h"
#include <unistd.h>

/*
 * $Id$
 * -----
 * $Revision$
 *
 */

typedef enum
{
  _es_null = 0,
  _es_link,
  _ud_link,
  _td_link
} e_type;

struct es
{
  struct es *next;
  struct es *prev;

  e_type ev_ty;
  int peer0, port0, peer1, port1;
  int cost;
  char *name;
};

struct el
{
  struct el *next;
  struct el *prev;

  struct es *es_head;
};

int init_new_el(); // init once
void add_new_es(); // add a null set, cannot add to last set any more!
void add_to_last_es(e_type ev,
                    node peer0, int port0,
                    node peer1, int port1,
                    int cost, char *name);
void print_el();
void print_event(struct es *es);
struct es *geteventbylink(char *lname);

#endif
