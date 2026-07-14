# power_domain_sample — common Zephyr / Renode commands.
# Lives in the project folder: `just` finds it when run from anywhere inside
# power_domain_sample/, and recipes run with the working directory set here — so
# the build dir is power_domain_sample/build/. Requires the `zephyr` pyenv
# virtualenv active (see ../.python-version) and the Zephyr SDK toolchain on PATH.

alias b := build
alias c := clean
alias r := run
alias w := web
alias d := debug

# App source dir (this folder) and the board Renode emulates.
app := "."
board := "nucleo_u5a5zj_q"

# The `west zvb-*` commands (from the zephyr-virtual-board module) auto-discover
# the per-board config from the build: they read the board name and look up
# <app>/boards/zvb/<board>/zvb.yml. So no --config is needed here; override
# with `west zvb-web --config <path>` if you ever need to.

# Show available recipes (default when running bare `just`).
default:
    @just --list

# Pristine build. Override the board: `just board=stm32u5g9j_dk2/stm32u5g9xx build`.
build:
    west build -b {{ board }} {{ app }}

# Remove the build directory.
clean:
    rip build

# Regenerate the Renode platform from the freshly-built devicetree (dts2repl,
# with the SysTick clock override). Run after a build whose devicetree changed.
repl:
    west zvb-repl

# Run the firmware under Renode (interactive console: monitor + console analyzer).
run:
    west zvb-run

# Browser dashboard: spawns headless Renode + serves http://localhost:8000
# (live LEDs, Zephyr shell, Renode monitor, GDB button, CPU low-power). Ctrl-C stops it.
web:
    west zvb-web

# Debug with GDB: headless Renode + GDB stub (CPU halted at reset) with
# arm-zephyr-eabi-gdb attached, so you drive from `main` (`break main`, `continue`,
# `step`, breakpoints, ...). Quitting gdb stops that Renode. Needs a build.
debug:
    west zvb-debug
