# Repository Guidelines

## Project Structure & Module Organization

- `src/main.c` is the executable entry point and simple simulation loop.
- `src/cvs/` contains model types and CVS calculation modules.
- `src/cvs/cvs_model.h` defines shared enums, parameters, states, and simulation structs.
- `src/cvs/cvs_elastances.*` contains left- and right-ventricle elastance calculations.
- `src/cvs/cvs_pressures.*` contains node pressure calculations from elastance, volume, and unstressed volume.
- `src/cvs/cvs_flows.*` contains node outlet flow calculations from pressure differences and Simulink-derived conductances.
- `src/cvs/cvs_volumes.*` contains volume state initialization and Euler updates from flow balance.
- `src/cvs/cvs_output.*` writes simulation results to CSV for reference comparison.
- `Reference_CVS/` stores Simulink reference artifacts (`.slx`, `.slxc`, cache files).
- `bak/` contains older reference files, scripts, generated output, and historical backups.

Keep production C code under `src/`. Treat `Reference_CVS/` and `bak/` as reference material unless a task explicitly requires updating them.

## Build, Test, and Development Commands

- `make` builds `cvs.exe` from all `src/**/*.c` files into intermediate objects under `build/`.
- `make rebuild` removes build artifacts and rebuilds the executable.
- `make clean` removes the `build/` directory.
- `./cvs.exe` runs the current simulation executable after building.
- `./cvs.exe` writes the latest run to `output.csv` in the repository root.
- `make plot` rebuilds/runs `cvs.exe` when needed and opens a Matplotlib plot of `output.csv`.
- `make reference` exports `reference.csv` from internal `Reference_CVS` Simulink state, pressure, and flow vectors.
- `make compare-plot` runs `cvs.exe`, exports `reference.csv`, and opens MATLAB comparison plots for `output.csv` against `reference.csv`.
- `C:\msys64\ucrt64\bin\python.exe -m venv --system-site-packages .venv` creates the project-local Python virtual environment using MSYS2/UCRT64 Python.
- `C:\msys64\usr\bin\bash.exe -lc 'pacman -S --needed --noconfirm mingw-w64-ucrt-x86_64-python-matplotlib'` installs the MSYS2/UCRT64 plotting stack when needed.
- `.\.venv\bin\python.exe -m pip install -r requirements.txt` verifies Python plotting dependencies from the MSYS2/UCRT64 environment.
- `.\.venv\bin\python.exe scripts\plot_elastances.py --csv bak\output.csv` opens a Matplotlib plot of calculated `e_LV` and `e_RV` from saved CVS output.
- `.\.venv\bin\python.exe scripts\plot_cvs_output.py --csv output.csv` opens a Matplotlib plot of CVS volumes, pressures, flows, and elastances.
- `matlab -batch "addpath('scripts'); export_reference_cvs('reference.csv',40)"` writes Simulink-derived reference data from `Reference_CVS`.
- `matlab -batch "addpath('scripts'); plot_cvs_comparison('output.csv','reference.csv')"` opens MATLAB comparison plots and prints max/mean absolute errors.

The Makefile uses Unix-style helpers such as `find`, `mkdir -p`, and `rm -rf`; use Git Bash, MSYS2, WSL, or a compatible shell on Windows.
Use the project-local `.venv` for Python scripts to avoid mixing plotting dependencies with Windows system Python. The MSYS2/UCRT64 virtual environment uses `.venv/bin/`, not `.venv/Scripts/`.

## Coding Style & Naming Conventions

Use straightforward C. Follow the existing style: 4-space indentation in `.c` files, braces on their own line for functions and control blocks, and descriptive local variables.

Use the `cvs_` prefix for module functions, for example `cvs_calculate_e_RV`. Keep private helpers `static` inside their `.c` file. Use `CVS_` prefixes for public types and enums, and `NODE_*` names for node identifiers. Prefer `float` math and `math.h` functions ending in `f` when working with model values.

## Testing Guidelines

There is currently no automated test suite. Before submitting changes, run `make rebuild` and execute `./cvs.exe` to confirm the model still builds and runs. For numerical changes, compare output against Simulink-derived `reference.csv` from `Reference_CVS/`. Add focused tests or reproducible comparison scripts when introducing new model equations.

For elastance visualization, use `scripts/plot_elastances.py`. The saved `bak/output.csv` does not include elastance columns directly, so the script recomputes `e_LV` and `e_RV` from `t`, `V_LV`, and `V_RV` using the C/Simulink elastance equations. By default the script opens a Matplotlib window; pass `--output elastances.png --no-show` only when a saved image is needed.

## Commit & Pull Request Guidelines

Git history uses short, informal commit messages. For new work, prefer concise imperative messages such as `add LV elastance calculation` or `fix RV activation timing`.

Pull requests should include a brief description, the commands run for verification, and notes about any numerical behavior changes. Link related issues when available. Include plots, CSV snippets, or screenshots only when they help explain model output or Simulink comparisons.

## Agent-Specific Instructions

Do not rewrite generated Simulink cache files or backup artifacts unless requested. Keep edits small and localized, and preserve existing model terminology even when adding comments or documentation.
