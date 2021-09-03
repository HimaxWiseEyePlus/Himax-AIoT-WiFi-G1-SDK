/*
* 
* CONFIDENTIAL AND PROPRIETARY INFORMATION
* 
* Copyright (c) 2015-2017 Synopsys, Inc. All rights reserved.
* This software and documentation contain confidential and
* proprietary information that is the property of
* Synopsys, Inc. The software and documentation are
* furnished under a license agreement and may be used
* or copied only in accordance with the terms of the license
* agreement. No part of the software and documentation
* may be reproduced, transmitted, or translated, in any
* form or by any means, electronic, mechanical, manual,
* optical, or otherwise, without prior written permission
* of Synopsys, Inc., or as expressly provided by the license agreement.
* Reverse engineering is prohibited, and reproduction,
* disclosure or use without specific written authorization
* of Synopsys Inc. is strictly forbidden.
*/
/*==========================================================================
* Library DW_DFSS-1.1.6
* ========================================================================== */

#include "stdint.h"
#include "event.h"
#include "embARC_debug.h"
#include "arc_exception.h"

#define INFRA_EVT_PRIO_DEFAULT  (0xFF)

/* local function declarations */
void subs_proc_rtn_none();
void subs_proc_rtn_clear();
void subs_clear_pending(uint32_t idx);

typedef struct _evt_t {
	uint32_t        prio;
    infra_evt_cb_t   func;
} evt_t;

typedef void (*subs_loop_rtn_func_t)();

static evt_t   	subs_event_list[INFRA_EVT_MAX];

static uint8_t  subs_events[INFRA_EVT_MAX];
static uint32_t subs_events_pending_cnt = 0;
static uint32_t subs_events_created_cnt = 0;

static uint8_t  subs_raised_events_ISR[INFRA_EVT_MAX];
static uint32_t subs_raised_events_ISR_cnt = 0;

/* 
 *  subs_events[0..pending_cnt)                            : runnable events sort by priority, lowest priority is running
 *  subs_events[pending_cnt..subs_events_created_cnt)      : blocked events (unsorted); gets runnable, when event is raised
 *  subs_events[subs_events_created_cnt..INFRA_EVT_MAX)    : free events
 *  subs_raised_events_ISR[0..subs_raised_events_ISR_cnt)  : raised events from ISR context, not yet part of runnable events
 */

static uint8_t		subs_events_executing = 0;
static uint8_t		subs_events_aborted = 0;

static subs_loop_rtn_func_t subs_proc_rtn[2] = {
		&subs_proc_rtn_none,
		&subs_proc_rtn_clear
};

static uint8_t default_processing(void)
{
	return INFRA_EVT_RTN_CLEAR;
}

static void default_callback(void) { }
static infra_idle_cb_t subs_event_idle_func = default_callback;

/* API functions */
void infra_evt_create(infra_evt_t *evt)
{
    uint8_t	idx = INFRA_EVT_MAX;
	
	if (subs_events_created_cnt != INFRA_EVT_MAX)
	{
		idx = subs_events[subs_events_created_cnt];
		subs_event_list[idx].prio = INFRA_EVT_PRIO_DEFAULT;
		subs_event_list[idx].func = default_processing;
		subs_events_created_cnt++;
	}
	
	*evt = idx;
}

void infra_evt_delete(infra_evt_t evt)
{
	uint32_t    h = 0, k = 0;
	uint8_t  	evt2 = 0;
	
	h = 0; 
	while (evt != subs_events[h]) 
		{  h++; }

	/* note: cannot delete running event (assert on h == 0 when executing) */
	if ( ((h != 0) || (subs_events_executing == 0)) && (h >= subs_events_created_cnt) )
	{
		evt2 = subs_events[h];
		/* put 'h' on 'subs_events_created_cnt' and shift everything in between */
		for (k=h; k!=(subs_events_created_cnt-1); k++)
		{
		   subs_events[k] = subs_events[k+1];
		}
		if (h < subs_events_pending_cnt)
		{
			subs_events_pending_cnt--;
		}
		subs_events_created_cnt--;
		subs_events[subs_events_created_cnt] = evt2;
	}
}

void infra_evt_raise(infra_evt_t evt)
{
	uint32_t      h = 0, k = 0;
	uint32_t      prio = 0;
	
	h = subs_events_pending_cnt;
	while (  (h != subs_events_created_cnt)
          && (evt != subs_events[h])
          )
	{ 
	    h++; 
	}
	if (evt == subs_events[h])
	{
		subs_events[h] = subs_events[subs_events_pending_cnt];
		subs_events[subs_events_pending_cnt] = evt;
		subs_events_pending_cnt++;
		prio = subs_event_list[evt].prio;
		/* sort on priority */
		k = subs_events_pending_cnt-1;
		while (  (k != 0)
			  && (subs_event_list[subs_events[k-1]].prio > prio)
			  )
		{ 
		    subs_events[k] = subs_events[k-1];
			k--; 
		}
		subs_events[k] = evt;
	}
}

void infra_evt_raise_ISR(infra_evt_t evt)
{
	if (subs_raised_events_ISR_cnt < INFRA_EVT_MAX)
	{
		subs_raised_events_ISR[subs_raised_events_ISR_cnt++] = evt;
	}
}

void infra_evt_clear(infra_evt_t evt)
{
	uint32_t h = 0;
	
	h = 0; 
	while (  (h != subs_events_pending_cnt)
          && (evt != subs_events[h])
          )
	{ 
	    h++; 
	}

	/* note: cannot clear running event (assert on h == 0 when executing) */
	if ( ((h != 0) || (subs_events_executing == 0)) && (h != subs_events_pending_cnt) )
	{
		subs_clear_pending(h);
	}
}

void infra_evt_clear_ISR_pendingevent(infra_evt_t evt)
{
	uint32_t h = 0, k = 0;

	h = 0;
	while (  (h != subs_raised_events_ISR_cnt)
          && (evt != subs_raised_events_ISR[h])
          )
	{
	    h++;
	}

	/* note: cannot clear running event (assert on h == 0 when executing) */
	if (h < subs_raised_events_ISR_cnt )
	{
		/* put 'idx' on 'subs_events_pending_cnt' and shift everything in between */
		evt = subs_raised_events_ISR[h];
		for (k=h; k!=(subs_raised_events_ISR_cnt-1); k++)
		{
			subs_raised_events_ISR[k] = subs_raised_events_ISR[k+1];
		}
		subs_raised_events_ISR_cnt--;
	}
}


void infra_evt_set_priority(infra_evt_t evt, uint32_t prio)
{
	subs_event_list[evt].prio = prio;
}

void infra_evt_set_callback(infra_evt_t evt, infra_evt_cb_t evt_fct)
{
    subs_event_list[evt].func = evt_fct;
}

void infra_evts_init()
{
	uint32_t h = 0;

	for (h=0; h!=INFRA_EVT_MAX; h++) { subs_events[h] = h; }
	subs_events_pending_cnt = subs_events_created_cnt = subs_raised_events_ISR_cnt = 0;
}

void infra_evts_execute()
{
	uint8_t      status = 0;
	
	subs_events_aborted = 0;
	while ( !subs_events_aborted ) {
		while (subs_raised_events_ISR_cnt > 0)
		{
			uint32_t  interrupts;

			interrupts = arc_lock_save();        // disable interrupts
			subs_raised_events_ISR_cnt--;
		    infra_evt_raise( subs_raised_events_ISR[subs_raised_events_ISR_cnt] );
		    arc_unlock_restore(interrupts);
		}
		if (subs_events_pending_cnt > 0)
		{
			/* execute */
			subs_events_executing = 1;
			/* enable watchdog to detect deadlock */
			status = subs_event_list[subs_events[0]].func();
			subs_proc_rtn[status]();
		}
		else
		{
			/* sleep */
			if (subs_events_executing == 1)
			{
				/* disable watchdog */
			}
			subs_events_executing = 0;
			/* callback function */
			subs_event_idle_func();
		}
	}
}

void infra_evts_abort()
{
	subs_events_aborted = 1;
}


void infra_evts_set_callback(infra_idle_cb_t idle_fct)
{
	subs_event_idle_func = idle_fct;
}

void infra_evts_set_cycle_cnt()
{
	/* TODO */
}

/* local function implementations */

void subs_proc_rtn_none()
{
	uint32_t     h = 0;
	uint32_t     prio = 0;
	uint8_t		 evt = 0;

	evt = subs_events[0];
	prio = subs_event_list[evt].prio;

	h = 1;
	while (  (h != subs_events_pending_cnt)
		  && (prio == subs_event_list[subs_events[h]].prio)
		  )
	{
	   subs_events[h-1] = subs_events[h];
	   h++;
	}
	subs_events[h-1] = evt;
}

void subs_proc_rtn_clear()
{
	subs_clear_pending(0);
}

void subs_clear_pending(uint32_t idx)
{
	uint32_t      k = 0;
	uint8_t		  evt = 0;
	
	/* precondition: 0 <= idx < subs_events_pending_cnt, otherwise assert */

	/* put 'idx' on 'subs_events_pending_cnt' and shift everything in between */
	evt = subs_events[idx];
	for (k=idx; k!=(subs_events_pending_cnt-1); k++)
	{
		subs_events[k] = subs_events[k+1];
	}
	subs_events_pending_cnt--;
	subs_events[subs_events_pending_cnt] = evt;
}

