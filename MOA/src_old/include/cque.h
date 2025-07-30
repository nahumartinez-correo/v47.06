/*
** que management macros
*/
#ifndef CQUE_H
#define CQUE_H

#define CQUE

#define initq(qh) { qh->next=qh;\
					qh->prev=qh;}

#define addeq(qh,np) {np->next=qh->prev->next;\
					  np->prev=qh->prev;\
					  qh->prev=qh->prev->next=np;}

#define insq(pp,np) {np->next=pp->next;\
					  np->prev=pp;\
					  pp->next=np->next->prev=np;}

#define addfq(qh,np)  {np->next = qh->next;\
					  np->prev = qh;\
					  qh->next = qh->next->prev = np;}

/*
#define addfq(qh,np)  {np->next=qh->prev->next;\
					  np->prev=qh->prev;\
					  qh->prev=qh->prev->next=np;}
*/

#define delq(np) {np->prev->next=np->next;\
				  np->next->prev=np->prev; \
				  np->next=np->prev=NULL;}

#define firstq(qh) ((qh->next == qh) ? NULL:qh->next)

#define lastq(qh) ((qh->prev == qh) ? NULL:qh->prev)

#define nextq(qh,np) ((np->next == qh) ? NULL:np->next)

#define prevq(qh,np) ((np->prev == qh) ? NULL:np->prev)

#endif
