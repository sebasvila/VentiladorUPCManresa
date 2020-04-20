#ifndef _PT_DELAY_H_
#define _PT_DELAY_H_

#include "pt.h"
#include "ticker.h"


/*
 * The thread waits at least `time` centiseconds.
 *
 *
 * Caveat emptor: Allocates an static variable that, in some
 * conditions, would be better to share it between all delays in the
 * same thread. However, no clever way of doing it without disgusting
 * the end user was found.
 */
#define PT_DELAY(pt, time)			\
  do {						\
    static uint16_t from;			\
						\
    from = ticker_get();				\
    LC_SET((pt)->lc);				\
    if( (ticker_get() - from <= (time)) ) {	\
      return PT_WAITING;			\
    }						\
  } while(0)


#endif /* _PT_DELAY_H_ */


