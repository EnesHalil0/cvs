import argparse
import csv
import math
from pathlib import Path

import matplotlib.pyplot as plt


def e_lv_min(hr, v_lv, heart_failure=False, exercise=False):
    hr2 = hr * hr

    if heart_failure:
        b_lv = ((-0.0333 * hr2) + (4.333 * hr) - 20.0) / 10000.0
        c_lv = ((2.444 * hr2) - (306.67 * hr) + 25600.0) / 100.0
    else:
        if exercise:
            b_lv = ((0.0108 * hr2) - (4.2917 * hr) + 398.5) / 10000.0
        else:
            b_lv = ((-4.3333 * hr) + 440.0) / 10000.0

        c_lv = (0.0446 * hr2) - (6.3542 * hr) + 365.75

    return (0.4641 * (math.exp((v_lv - c_lv) * b_lv) - 1.0)) + 0.1235


def e_lv_max(hr, heart_failure=False, exercise=False):
    hr2 = hr * hr

    if heart_failure:
        return ((-0.0028 * hr2) + (0.9167 * hr) + 45.0) / 100.0

    if exercise:
        return ((0.0027 * hr2) - (0.0767 * hr) + 24.76) / 10.0

    return 3.0


def normalized_e_lv(t, hr):
    freq = hr / 60.0
    period = 1.0 / freq
    t_beat = math.fmod(t, period)
    t_es = 0.35 / freq

    if t_beat < 0.0 or t_es <= 0.0:
        return 0.0

    t_n = t_beat / t_es
    numerator = 461.4567 * math.pow(t_n, 1.5)
    denominator = (
        126.2886
        + (333.3319 * math.pow(t_n, 1.9))
        + (0.3789 * math.pow(t_n, 37.0))
        + math.pow(t_n, 38.9)
    )

    if denominator == 0.0:
        return 0.0

    return numerator / denominator


def calculate_e_lv(t, v_lv, hr, heart_failure=False, exercise=False):
    e_min = e_lv_min(hr, v_lv, heart_failure, exercise)
    e_max = e_lv_max(hr, heart_failure, exercise)
    e_n = normalized_e_lv(t, hr)

    return (e_n * (e_max - e_min)) + e_min


def e_rv_min(hr, v_rv, heart_failure=False):
    hr2 = hr * hr

    if heart_failure:
        c_rv = ((hr * -333.33) + 110000.0) / 1000.0
    else:
        c_rv = (0.0167 * hr2) - (3.3333 * hr) + 290.0

    return 0.03752 * (math.exp((v_rv - c_rv) * 0.03776) + 1.0)


def e_rv_max(hr, heart_failure=False):
    hr2 = hr * hr

    if heart_failure:
        return ((0.0056 * hr2) - (0.6667 * hr) + 60.0) / 100.0

    return ((0.005 * hr2) + (0.25 * hr) + 17.0) / 100.0


def phi_rv(t, hr):
    freq = hr / 60.0
    period = 1.0 / freq
    t_beat = math.fmod(t, period)
    t_h = 1.0 / freq

    hr2 = hr * hr
    k_0 = ((-0.0041 * hr2) + (-0.0417 * hr) + 257.5) / 1000.0
    k_1 = ((0.0158 * hr2) + (-5.0417 * hr) + 445.5) / 1000.0
    t_ce = k_0 + (k_1 * t_h)

    if t_ce <= 0.0 or t_beat < 0.0 or t_beat >= t_ce:
        return 0.0

    x = math.pi * t_beat / t_ce
    return (0.9 * math.sin(x)) - (0.25 * math.sin(2.0 * x))


def calculate_e_rv(t, v_rv, hr, heart_failure=False):
    e_min = e_rv_min(hr, v_rv, heart_failure)
    e_max = e_rv_max(hr, heart_failure)
    phi = phi_rv(t, hr)

    return (e_max * phi) + (e_min * (1.0 - phi))


def read_elastances(csv_path, hr, heart_failure=False, exercise=False):
    times = []
    e_lvs = []
    e_rvs = []

    with csv_path.open(newline="") as file:
        reader = csv.DictReader(file)
        required = {"t", "V_LV", "V_RV"}
        missing = required.difference(reader.fieldnames or [])
        if missing:
            raise ValueError(f"CSV missing required columns: {', '.join(sorted(missing))}")

        for row in reader:
            t = float(row["t"])
            v_lv = float(row["V_LV"])
            v_rv = float(row["V_RV"])

            times.append(t)
            e_lvs.append(calculate_e_lv(t, v_lv, hr, heart_failure, exercise))
            e_rvs.append(calculate_e_rv(t, v_rv, hr, heart_failure))

    return times, e_lvs, e_rvs


def main():
    parser = argparse.ArgumentParser(description="Plot LV and RV elastances from CVS output CSV.")
    parser.add_argument("--csv", default="bak/output.csv", help="CSV file containing t, V_LV, and V_RV columns.")
    parser.add_argument("--hr", type=float, default=60.0, help="Heart rate in bpm.")
    parser.add_argument("--heart-failure", action="store_true", help="Use heart failure elastance equations.")
    parser.add_argument("--exercise", action="store_true", help="Use exercise elastance equations where applicable.")
    parser.add_argument("--output", help="Optional path for saving the plot image.")
    parser.add_argument("--no-show", action="store_true", help="Do not open the Matplotlib plot window.")
    args = parser.parse_args()

    csv_path = Path(args.csv)
    output_path = Path(args.output) if args.output else None

    times, e_lvs, e_rvs = read_elastances(
        csv_path,
        args.hr,
        heart_failure=args.heart_failure,
        exercise=args.exercise,
    )

    plt.figure(figsize=(11, 6))
    plt.plot(times, e_lvs, label="e_LV", linewidth=1.2)
    plt.plot(times, e_rvs, label="e_RV", linewidth=1.2)
    plt.grid(True, alpha=0.35)
    plt.xlabel("Time [s]")
    plt.ylabel("Elastance")
    plt.title("LV and RV Elastances")
    plt.legend()
    plt.tight_layout()
    if output_path:
        plt.savefig(output_path, dpi=160)
        print(f"Saved plot to {output_path}")

    if not args.no_show:
        plt.show()


if __name__ == "__main__":
    main()
