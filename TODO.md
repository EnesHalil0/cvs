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
- [ ] Design a solver-independent model API.
  - Add a `CVS_Model` owner struct for parameters, states, and simulation time.
  - Add a `CVS_SolverType` enum with at least `CVS_SOLVER_EULER`, `CVS_SOLVER_HEUN`, and `CVS_SOLVER_RK4`.
  - Add a `CVS_Solver` struct containing solver type and `dt`.
  - Add `cvs_model_init()` and `cvs_model_step()` so PC, benchmark, and STM32 code call the same model core.
  - Keep Euler as the baseline solver for Simulink `ode1` validation.
- [ ] Improve README.
  - Current status: `README.md` only has a title.
  - Add project purpose, folder structure, build commands, run commands, MSYS2 requirements, Python venv setup, and plotting workflow.

## TIPTEKNO Paper Tasks

- [ ] Rewrite the abstract around the contribution order.
  - Motivation: HMC/VAD testing and previous dSPACE/Simulink HMC.
  - Contribution 1: embedded-oriented C reimplementation of the seven-state CVS model.
  - Contribution 2: C-Euler validation against Simulink `ode1` using matched settings.
  - Contribution 3: C vs MATLAB/Simulink runtime benchmark.
  - Contribution 4: solver interface and Euler/Heun/RK4 trade-off evaluation.
  - Contribution 5: STM32 processor-in-the-loop preparation.
- [ ] Keep the main validation scope narrow.
  - Compare Simulink only against the C Euler implementation.
  - Run Simulink with fixed-step `ode1` / Euler, same `dt`, same duration, same initial conditions, and same parameter flags.
  - Do not claim RK4 or Heun is "closer to Simulink ode1"; that would be the wrong comparison target.
- [ ] Produce the minimum paper figures.
  - Workflow: Simulink reference -> C implementation -> validation -> benchmark -> STM32/PIL.
  - Seven-state CVS circulation diagram.
  - C software architecture diagram with model core, solver, runner, output, and embedded layer.
  - Simulink `ode1` vs C Euler pressure/volume overlays.
  - LV pressure-volume loop comparison.
- [ ] Produce the minimum paper tables.
  - Model states and units.
  - C module mapping to equations.
  - Simulink `ode1` vs C Euler error metrics.
  - C vs MATLAB/Simulink runtime and per-step timing.
  - Solver/dt accuracy-runtime trade-off.
- [ ] Write the paper discussion around honest claims.
  - C Euler reproduces the existing Simulink `ode1` reference.
  - Higher-order solvers are evaluated as alternative embedded integration choices, not as Simulink-validation replacements.
  - STM32 work should be described as "PIL target" or "feasibility" until real board timing is measured.

## Next Model Equations

- [x] Add automated reference comparison.
  - Compare `output.csv` against Simulink-exported `reference.csv` generated from the same timestep and parameter flags.
  - Report max/mean absolute error for `V`, `P`, and `Q`.
- [ ] Separate derivative calculation from integration.
  - Add a function that computes `dV/dt = Q_in - Q_out` without directly updating state.
  - Let Euler, Heun, and RK4 use this derivative function.
  - Keep pressure, elastance, and flow recalculation inside each derivative evaluation so intermediate RK stages are physically consistent.

## Solver Comparison Tasks

- [ ] Implement C Euler through the new `CVS_Solver` interface.
  - Confirm output is unchanged relative to the current Euler update.
  - Use this mode for Simulink `ode1` validation.
- [ ] Implement Heun solver.
  - Predictor: one Euler trial step.
  - Corrector: average derivative at current and predicted states.
  - Verify total blood volume conservation after each corrected step.
- [ ] Implement RK4 solver.
  - Compute four derivative stages with intermediate states.
  - Recompute elastance, pressure, and flow at each intermediate stage.
  - Verify runtime cost and memory usage remain acceptable for embedded use.
- [ ] Define a high-resolution offline reference for solver comparison.
  - Preferred reference: C RK4 with very small `dt`, for example 0.01 ms or 0.005 ms, using `double` if available.
  - Use this only for solver accuracy studies, not for Simulink equivalence claims.
- [ ] Evaluate solver quality with signal metrics.
  - RMSE and NRMSE for all V/P/Q signals.
  - MaxAbsError for switching-sensitive flow signals.
  - Optional phase/time-shift error for pressure peaks.
- [ ] Evaluate solver quality with physiological feature metrics.
  - MAP.
  - Systolic and diastolic aortic pressure.
  - LV EDV, ESV, SV, EF.
  - Cardiac output.
  - LV PV-loop area or shape error.
- [ ] Evaluate solver quality with embedded feasibility metrics.
  - Per-step runtime.
  - Real-time ratio.
  - Deadline margin for `dt = 1 ms`, `0.5 ms`, and `0.1 ms`.
  - Numerical stability at larger `dt`.
  - Absence of negative compartment volumes.

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

## STM32 / Processor-in-the-Loop Tasks

- [ ] Choose the first PIL host workflow.
  - Recommended: Python host using USB CDC/UART for command, telemetry, CSV logging, and plotting.
  - MATLAB/Simulink should generate `reference.csv` and can be used for final metrics/figures.
  - Avoid making the first PIL prototype depend on a live Simulink host loop.
- [ ] Define a simple serial protocol.
  - Host sends `init` with HR, scenario flags, `dt`, duration, and sample decimation.
  - Host sends `step` or `run N` command.
  - STM32 returns selected states/signals: time, V_LV, P_LV, P_AO, Q_LV, e_LV, e_RV.
  - Use binary packets later if CSV text telemetry is too slow.
- [ ] Build the STM32 timing experiment.
  - Run model step from a timer-driven loop or a controlled benchmark loop.
  - Measure per-step time using DWT cycle counter or a hardware timer.
  - Report average, maximum, and worst-case step time.
  - Compare step time against 1 ms, 0.5 ms, and 0.1 ms deadlines.
- [ ] Compare STM32 output against host reference.
  - Save `stm32_output.csv`.
  - Compare against C PC Euler output and Simulink `reference.csv`.
  - Report numerical drift and timing margin separately.
