# TODOs

## Completed

- [x] Add elastance calculation module scaffold.
  - Implemented `src/cvs/cvs_elastances.c`.
  - Exposed public declarations in `src/cvs/cvs_elastances.h`.
- [x] Implement RV elastance calculation.
  - Added `cvs_calculate_e_RV`.
  - Matched RV minimum/maximum elastance and activation timing against the reference Simulink `eRV` subsystem.
- [x] Implement LV elastance calculation.
  - Added `cvs_calculate_e_LV`.
  - Matched LV minimum/maximum elastance and normalized activation curve against the reference Simulink `eLV` subsystem.
- [x] Implement combined elastance update.
  - Added `cvs_calculate_elastances`.
  - `main.c` now calls the combined elastance update and prints `e_LV` / `e_RV`.
- [x] Add Python elastance visualization helper.
  - Added `scripts/plot_elastances.py`.
  - Script recomputes `e_LV` and `e_RV` from `bak/output.csv` columns `t`, `V_LV`, and `V_RV`.
  - Uses MSYS2/UCRT64 Python through `.venv/bin/python.exe`.
- [x] Implement pressure calculations.
  - Added `src/cvs/cvs_pressures.c` and `src/cvs/cvs_pressures.h`.
  - Added `V0[NODE_COUNT]` to `CVS_Parameters`.
  - Pressure equation uses `P = e * (V - V0)` after LV/RV elastances are updated.
  - Ventricular `V0` follows the Simulink `Subsystem13` HR/HF switch instead of a hard-coded 10 mL.
- [x] Implement flow calculations.
  - Added `src/cvs/cvs_flows.c` and `src/cvs/cvs_flows.h`.
  - `Q[NODE_*]` stores the outlet flow from that node to the next circulation node.
  - Conductances `g[]` follow the Simulink `g Values` subsystems for normal, exercise, and heart-failure branches where present.
  - Valve outlets `RA->RV`, `RV->P`, `LA->LV`, and `LV->AO` clamp negative pressure drops with `max(0, dP)`, matching the Simulink `Max` blocks.
- [x] Implement volume/state updates.
  - Added `src/cvs/cvs_volumes.c` and `src/cvs/cvs_volumes.h`.
  - Initial volumes follow Simulink `Unit Delay2` initial condition `X_0 = [0, 0, 5000, 0, 0, 0, 0]`.
  - Volume update uses `V_next = V_current + dt * (Q_in - Q_out)` with a simultaneous temporary derivative vector.
  - Updated `main.c` timestep to the Simulink `stepsize = 0.0001`.
- [x] Complete non-ventricular elastance equations.
  - `eRA`, `eLA`, `eP`, `eAO`, and `eV` now follow the Simulink HR/HF/exercise switch branches instead of fixed legacy constants.
- [x] Add full simulation output/export.
  - Added `src/cvs/cvs_output.c` and `src/cvs/cvs_output.h`.
  - `./cvs.exe` writes `output.csv` with `t`, all `V`, all `P`, all `Q`, `e_LV`, and `e_RV`.
  - CSV node order matches the existing `bak/output.csv` order for `V/P/Q`.
- [x] Add one-command plotting.
  - Added `scripts/plot_cvs_output.py`.
  - Added `make plot` to run the simulator and open Matplotlib plots from `output.csv`.
- [x] Add MATLAB output/reference comparison plots.
  - Added `scripts/plot_cvs_comparison.m`.
  - Added `scripts/export_reference_cvs.m` to export `reference.csv` from the internal `Reference_CVS` Simulink state, pressure, and flow vectors.
  - Added `make reference` to regenerate `reference.csv`.
  - Added `make compare-plot` for MATLAB overlay plots of `output.csv` and `reference.csv`.

## In Progress / Needs Review

- [ ] Complete `cvs_model.h`.
  - Current status: basic node enum, parameters, states, and simulation structs exist.
  - Decide whether the shared model should also include resistances/conductances, unstressed volumes, total blood volume, valve state, and reference-condition flags.
  - Decide whether to introduce a single `CVS_Model` struct that owns `CVS_Parameters`, `CVS_States`, and `CVS_Simulation`.
- [ ] Improve README.
  - Current status: `README.md` only has a title.
  - Add project purpose, folder structure, build commands, run commands, MSYS2 requirements, Python venv setup, and plotting workflow.

## Next Model Equations

- [x] Add automated reference comparison.
  - Compare `output.csv` against Simulink-exported `reference.csv` generated from the same timestep and parameter flags.
  - Report max/mean absolute error for `V`, `P`, and `Q`.

## Verification Tasks

- [ ] Rebuild and run the C executable after each model stage.
  - Preferred command: `make rebuild && ./cvs.exe` from Git Bash/MSYS2/WSL.
  - PowerShell may not have `make` or GCC on `PATH`.
- [ ] Add a reproducible comparison workflow.
  - Save C output to CSV instead of only printing to stdout.
  - Plot C output against Simulink-exported `reference.csv`.
  - Track numerical differences for pressures, flows, volumes, and elastances.
- [ ] Add focused tests or regression checks.
  - At minimum, test elastance values at selected `HR`, `t`, `V_LV`, and `V_RV` points.
  - Include normal, heart failure, and exercise branches.
