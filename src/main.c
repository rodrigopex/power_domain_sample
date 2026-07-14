/*
 * Copyright (c) 2026 Rodrigo Peixoto
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdint.h>
#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(hello_world, LOG_LEVEL_INF);

/*
 * red_led_1 = PG2 is the only user LED not claimed by a power-domain
 * (PC7/PB7 are the regulator enables). user_button = PC13, the blue
 * USER button. Both come from the nucleo_u5a5zj_q board devicetree.
 */
static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(DT_NODELABEL(red_led_1), gpios);
static const struct gpio_dt_spec button = GPIO_DT_SPEC_GET(DT_NODELABEL(user_button), gpios);

static struct gpio_callback button_cb_data;

static int blink_timeout = 0;

static void button_pressed(const struct device *dev, struct gpio_callback *cb, uint32_t pins)
{
	LOG_INF("Button pressed at %" PRIu32, k_cycle_get_32());
	blink_timeout += 1;
	blink_timeout = blink_timeout % 6;
}

int main(void)
{
	if (!gpio_is_ready_dt(&led) || !gpio_is_ready_dt(&button)) {
		LOG_ERR("GPIO not ready");
		return 0;
	}

	gpio_pin_configure_dt(&led, GPIO_OUTPUT_INACTIVE);

	gpio_pin_configure_dt(&button, GPIO_INPUT);
	gpio_pin_interrupt_configure_dt(&button, GPIO_INT_EDGE_TO_ACTIVE);
	gpio_init_callback(&button_cb_data, button_pressed, BIT(button.pin));
	gpio_add_callback_dt(&button, &button_cb_data);

	LOG_INF("Blink demo: red LED (PG2) blinking");
	int count = 0;
	while (1) {
		gpio_pin_toggle_dt(&led);
		if (count == 10) {
			count = 0;
			k_msleep(3000);
		} else {
			k_msleep((blink_timeout + 1) * 250);
		}
		count++;
	}

	return 0;
}
