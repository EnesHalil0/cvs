clear; clc; close all;

data = readtable("output.csv");

%% Pressures
figure("Name", "CVS Pressures");
plot(data.t, data.P_LV, "LineWidth", 1.2); hold on;
plot(data.t, data.P_AO, "LineWidth", 1.2);
plot(data.t, data.P_LA, "LineWidth", 1.2);
plot(data.t, data.P_RA, "LineWidth", 1.2);
grid on;
xlabel("Time [s]");
ylabel("Pressure [mmHg]");
title("CVS Pressures");
legend("P_{LV}", "P_{AO}", "P_{LA}", "P_{RA}");

%% Volumes
figure("Name", "CVS Volumes");
plot(data.t, data.V_LV, "LineWidth", 1.2); hold on;
plot(data.t, data.V_RV, "LineWidth", 1.2);
plot(data.t, data.V_AO, "LineWidth", 1.2);
plot(data.t, data.V_V,  "LineWidth", 1.2);
grid on;
xlabel("Time [s]");
ylabel("Volume [mL]");
title("CVS Volumes");
legend("V_{LV}", "V_{RV}", "V_{AO}", "V_{V}");

%% Flows
figure("Name", "CVS Flows");
plot(data.t, data.Q_LV, "LineWidth", 1.2); hold on;
plot(data.t, data.Q_AO, "LineWidth", 1.2);
plot(data.t, data.Q_LA, "LineWidth", 1.2);
plot(data.t, data.Q_RA, "LineWidth", 1.2);
grid on;
xlabel("Time [s]");
ylabel("Flow [mL/s]");
title("CVS Outlet Flows");
legend("Q_{LV \rightarrow AO}", ...
       "Q_{AO \rightarrow V}", ...
       "Q_{LA \rightarrow LV}", ...
       "Q_{RA \rightarrow RV}");

%% LV pressure-volume loop
figure("Name", "LV Pressure-Volume Loop");
plot(data.V_LV, data.P_LV, "LineWidth", 1.2);
grid on;
xlabel("LV Volume [mL]");
ylabel("LV Pressure [mmHg]");
title("LV Pressure-Volume Loop");
