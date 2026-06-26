function reference = export_reference_cvs(referenceCsv, stopTime)
%EXPORT_REFERENCE_CVS Export Reference_CVS internal state signals to CSV.
%
%   export_reference_cvs()
%   export_reference_cvs("reference.csv", 40)
%
% The reference Simulink model stores the CVS states in the Unit Delay2
% vector as [LV, AO, V, RV, P, LA, RA]. This function logs that state vector,
% plus the pressure and flow vectors built inside the same subsystem, and
% writes them with the same column names/order used by output.csv.

if nargin < 1 || strlength(string(referenceCsv)) == 0
    referenceCsv = "reference.csv";
end

if nargin < 2 || isempty(stopTime)
    stopTime = 40;
end

referenceCsv = string(referenceCsv);
repoRoot = fileparts(fileparts(mfilename("fullpath")));
modelFile = fullfile(repoRoot, "Reference_CVS", "Reference_CVS.slx");
modelName = "Reference_CVS";

load_system(modelFile);

loggedPorts = enableReferenceSignalLogging(modelName);
cleanupObj = onCleanup(@()restoreReferenceSignalLogging(modelName, loggedPorts));

simInput = Simulink.SimulationInput(modelName);
simInput = simInput.setModelParameter( ...
    "StopTime", num2str(stopTime), ...
    "Solver", "ode1", ...
    "FixedStep", "0.0001", ...
    "SignalLogging", "on", ...
    "SignalLoggingName", "logsout");

simOutput = sim(simInput);
logs = simOutput.logsout;

xSignal = logs.getElement("xpre").Values;
pSignal = logs.getElement("P_tott").Values;
qSignal = logs.getElement("Q_tott").Values;

t = xSignal.Time(:);
x = xSignal.Data;
p = resampleVectorSignal(pSignal, t);
q = resampleVectorSignal(qSignal, t);

reference = buildReferenceTable(t, x, p, q);
writetable(reference, fullfile(repoRoot, referenceCsv));

fprintf("Reference CVS exported to %s (%d rows).\\n", referenceCsv, height(reference));
end

function loggedPorts = enableReferenceSignalLogging(modelName)
% Log only the internal vectors needed for C-vs-Simulink comparison.
signalInfo = {
    "Reference_CVS:1086", "xpre";   % Unit Delay2 state vector
    "Reference_CVS:1087", "P_tott"; % Pressure vector concatenate
    "Reference_CVS:1088", "Q_tott"; % Flow vector concatenate
};

loggedPorts = struct( ...
    "sid", strings(size(signalInfo, 1), 1), ...
    "handle", cell(size(signalInfo, 1), 1), ...
    "DataLogging", strings(size(signalInfo, 1), 1), ...
    "DataLoggingNameMode", strings(size(signalInfo, 1), 1), ...
    "DataLoggingName", strings(size(signalInfo, 1), 1));

for k = 1:size(signalInfo, 1)
    sid = signalInfo{k, 1};
    signalName = signalInfo{k, 2};
    fullName = Simulink.ID.getFullName(sid);
    portHandles = get_param(fullName, "PortHandles");
    portHandle = portHandles.Outport(1);

    loggedPorts(k).sid = sid;
    loggedPorts(k).handle = portHandle;
    loggedPorts(k).DataLogging = string(get_param(portHandle, "DataLogging"));
    loggedPorts(k).DataLoggingNameMode = string(get_param(portHandle, "DataLoggingNameMode"));
    loggedPorts(k).DataLoggingName = string(get_param(portHandle, "DataLoggingName"));

    set_param(portHandle, ...
        "DataLogging", "on", ...
        "DataLoggingNameMode", "Custom", ...
        "DataLoggingName", signalName);
end

set_param(modelName, "Dirty", "off");
end

function restoreReferenceSignalLogging(modelName, loggedPorts)
% Leave the Simulink model exactly as we found it; do not save any changes.
for k = 1:numel(loggedPorts)
    if isempty(loggedPorts(k).handle)
        continue;
    end

    set_param(loggedPorts(k).handle, ...
        "DataLogging", loggedPorts(k).DataLogging, ...
        "DataLoggingNameMode", loggedPorts(k).DataLoggingNameMode, ...
        "DataLoggingName", loggedPorts(k).DataLoggingName);
end

set_param(modelName, "Dirty", "off");
end

function data = resampleVectorSignal(signal, t)
% Pressure and flow signals should share time with xpre; interpolate if not.
signalTime = signal.Time(:);
signalData = signal.Data;

if numel(signalTime) == numel(t) && all(signalTime == t)
    data = signalData;
    return;
end

data = interp1(signalTime, signalData, t, "linear", "extrap");
end

function tableOut = buildReferenceTable(t, x, p, q)
% Simulink vector order is [LV, AO, V, RV, P, LA, RA].
V_LV = x(:, 1);
V_AO = x(:, 2);
V_V = x(:, 3);
V_RV = x(:, 4);
V_P = x(:, 5);
V_LA = x(:, 6);
V_RA = x(:, 7);

P_LV = p(:, 1);
P_AO = p(:, 2);
P_V = p(:, 3);
P_RV = p(:, 4);
P_P = p(:, 5);
P_LA = p(:, 6);
P_RA = p(:, 7);

Q_LV = q(:, 1);
Q_AO = q(:, 2);
Q_V = q(:, 3);
Q_RV = q(:, 4);
Q_P = q(:, 5);
Q_LA = q(:, 6);
Q_RA = q(:, 7);

tableOut = table(t, ...
    V_RA, V_RV, V_P, V_LA, V_LV, V_AO, V_V, ...
    P_RA, P_RV, P_P, P_LA, P_LV, P_AO, P_V, ...
    Q_RA, Q_RV, Q_P, Q_LA, Q_LV, Q_AO, Q_V);
end
