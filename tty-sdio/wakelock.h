/* include/linux/wakelock.h
 *
 * Copyright (C) 2007-2012 Google, Inc.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

#ifndef _LINUX_WAKELOCK_H
#define _LINUX_WAKELOCK_H

#include <linux/ktime.h>
#include <linux/device.h>

/* A wake_lock prevents the system from entering suspend or other low power
 * states when active. If the type is set to WAKE_LOCK_SUSPEND, the wake_lock
 * prevents a full system suspend.
 */

enum {
	WAKE_LOCK_SUSPEND, /* Prevent suspend */
	WAKE_LOCK_TYPE_COUNT
};

struct wake_lock {
	struct wakeup_source ws;
};

 /**
 * wakeup_source_prepare - Prepare a new wakeup source for initialization.
 * @ws: Wakeup source to prepare.
 * @name: Pointer to the name of the new wakeup source.
 *
 * Callers must ensure that the @name string won't be freed when @ws is still in
 * use.
 */
void wakeup_source_prepare(struct wakeup_source *ws, const char *name)
{
	if (ws) {
		memset(ws, 0, sizeof(*ws));
		ws->name = name;
	}
}
EXPORT_SYMBOL_GPL(wakeup_source_prepare);


/**
 * wakeup_source_drop - Prepare a struct wakeup_source object for destruction.
 * @ws: Wakeup source to prepare for destruction.
 *
 * Callers must ensure that __pm_stay_awake() or __pm_wakeup_event() will never
 * be run in parallel with this function for the same wakeup source object.
 */
void wakeup_source_drop(struct wakeup_source *ws)
{
	if (!ws)
		return;

	__pm_relax(ws);
}
EXPORT_SYMBOL_GPL(wakeup_source_drop);

static inline void wakeup_source_init(struct wakeup_source *ws,
				      const char *name)
{
	wakeup_source_prepare(ws, name);
	wakeup_source_add(ws);
}

static inline void wake_lock_init(struct wake_lock *lock, int type,
				  const char *name)
{
	wakeup_source_init(&lock->ws, name);

}

static inline void wakeup_source_trash(struct wakeup_source *ws)
{
	wakeup_source_remove(ws);
	wakeup_source_drop(ws);
}

static inline void wake_lock_destroy(struct wake_lock *lock)
{
	wakeup_source_trash(&lock->ws);
}

static inline void wake_lock(struct wake_lock *lock)
{
	__pm_stay_awake(&lock->ws);
}

static inline void wake_lock_timeout(struct wake_lock *lock, long timeout)
{
	__pm_wakeup_event(&lock->ws, jiffies_to_msecs(timeout));
}

static inline void wake_unlock(struct wake_lock *lock)
{
	__pm_relax(&lock->ws);
}

static inline int wake_lock_active(struct wake_lock *lock)
{
	return lock->ws.active;
}

#endif
