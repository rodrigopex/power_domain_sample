#include <zephyr/kernel.h>
#include <zephyr/pm/pm.h>

static const char *const state_names[] = {
	[PM_STATE_ACTIVE] = "ACTIVE",
	[PM_STATE_RUNTIME_IDLE] = "RUNTIME_IDLE",
	[PM_STATE_SUSPEND_TO_IDLE] = "SUSPEND_TO_IDLE",
	[PM_STATE_STANDBY] = "STANDBY",
	[PM_STATE_SUSPEND_TO_RAM] = "SUSPEND_TO_RAM",
	[PM_STATE_SUSPEND_TO_DISK] = "SUSPEND_TO_DISK",
	[PM_STATE_SOFT_OFF] = "SOFT_OFF",
};

static void on_state_entry(enum pm_state state)
{
	/* About to enter `state` (ACTIVE → low-power), just before pm_state_set(). */
	const char *state_str = (state < ARRAY_SIZE(state_names) && state_names[state])
					? state_names[state]
					: "UNKNOWN";

	printk("Entering state %s\n", state_str);
}

static void on_state_exit(enum pm_state state)
{
	/* Just woke from `state` (low-power → ACTIVE) — may run in the wake ISR. */
	const char *state_str = (state < ARRAY_SIZE(state_names) && state_names[state])
					? state_names[state]
					: "UNKNOWN";

	printk("Exiting state %s\n", state_str);
}

static struct pm_notifier notifier = {
	.state_entry = on_state_entry,
	.state_exit = on_state_exit,
	.report_substate = true,
};

int pm_notifier_init(void)
{
	pm_notifier_register(&notifier);
	return 0;
}

SYS_INIT(pm_notifier_init, APPLICATION, 99);
