function stats = plot_cvs_comparison(outputCsv, referenceCsv)
%PLOT_CVS_COMPARISON Plot C CVS output against the reference CVS CSV.
%
%   plot_cvs_comparison()
%   plot_cvs_comparison("output.csv", "reference.csv")
%
% The script overlays the C output and reference signals in MATLAB figures.
% It also interpolates the C output to the reference time points and prints
% simple max/mean absolute-error statistics for shared V/P/Q variables.

if nargin < 1 || strlength(string(outputCsv)) == 0
    outputCsv = "output.csv";
end

if nargin < 2 || strlength(string(referenceCsv)) == 0
    referenceCsv = "reference.csv";
end

outputCsv = string(outputCsv);
referenceCsv = string(referenceCsv);

output = readtable(outputCsv, VariableNamingRule="preserve");
reference = readtable(referenceCsv, VariableNamingRule="preserve");

required = ["t"];
assert(all(ismember(required, string(output.Properties.VariableNames))), ...
    "Output CSV must contain column t.");
assert(all(ismember(required, string(reference.Properties.VariableNames))), ...
    "Reference CSV must contain column t.");

groups = {
    "Volumes",  ["V_RA" "V_RV" "V_P" "V_LA" "V_LV" "V_AO" "V_V"], "Volume [mL]";
    "Pressures",["P_RA" "P_RV" "P_P" "P_LA" "P_LV" "P_AO" "P_V"], "Pressure";
    "Flows",    ["Q_RA" "Q_RV" "Q_P" "Q_LA" "Q_LV" "Q_AO" "Q_V"], "Flow";
};

for groupIndex = 1:size(groups, 1)
    plotSignalGroup(output, reference, groups{groupIndex, 1}, groups{groupIndex, 2}, groups{groupIndex, 3});
end

if all(ismember(["e_LV" "e_RV"], string(output.Properties.VariableNames)))
    figure(Name="CVS Comparison - Elastances", Color="w");
    tiledlayout(2, 1, TileSpacing="compact", Padding="compact");

    nexttile;
    plot(output.t, output.e_LV, LineWidth=1.1);
    grid on;
    title("e\_LV");
    xlabel("Time [s]");
    ylabel("Elastance");

    nexttile;
    plot(output.t, output.e_RV, LineWidth=1.1);
    grid on;
    title("e\_RV");
    xlabel("Time [s]");
    ylabel("Elastance");
end

stats = calculateComparisonStats(output, reference);
disp(stats);

end

function plotSignalGroup(output, reference, figureTitle, columns, yLabelText)
figure(Name="CVS Comparison - " + figureTitle, Color="w");
layout = tiledlayout(4, 2, TileSpacing="compact", Padding="compact");

for column = columns
    nexttile;

    if ismember(column, string(output.Properties.VariableNames))
        [tOutput, yOutput] = downsampleForPlot(output.t, output.(column));
        plot(tOutput, yOutput, "-", LineWidth=1.0, DisplayName="C output");
        hold on;
    end

    if ismember(column, string(reference.Properties.VariableNames))
        [tReference, yReference] = downsampleForPlot(reference.t, reference.(column));
        plot(tReference, yReference, "--", LineWidth=1.0, DisplayName="Reference");
        hold on;
    end

    grid on;
    title(column, Interpreter="none");
    xlabel("Time [s]");
    ylabel(yLabelText);
    legend(Location="best");
end

title(layout, figureTitle + " - C Output vs Reference");
end

function [tPlot, yPlot] = downsampleForPlot(t, y)
maxPoints = 8000;
sampleCount = numel(t);

if sampleCount <= maxPoints
    tPlot = t;
    yPlot = y;
    return;
end

indices = unique(round(linspace(1, sampleCount, maxPoints)));
tPlot = t(indices);
yPlot = y(indices);
end

function stats = calculateComparisonStats(output, reference)
commonColumns = intersect(string(output.Properties.VariableNames), string(reference.Properties.VariableNames), "stable");
commonColumns = setdiff(commonColumns, "t", "stable");
commonColumns = commonColumns(startsWith(commonColumns, ["V_" "P_" "Q_"]));

tStart = max(min(output.t), min(reference.t));
tEnd = min(max(output.t), max(reference.t));
validReferenceRows = reference.t >= tStart & reference.t <= tEnd;

names = strings(0, 1);
maxAbsError = zeros(0, 1);
meanAbsError = zeros(0, 1);

for column = commonColumns
    outputAtReferenceTime = interp1(output.t, output.(column), reference.t(validReferenceRows), "linear");
    error = outputAtReferenceTime - reference.(column)(validReferenceRows);

    names(end + 1, 1) = column; %#ok<AGROW>
    maxAbsError(end + 1, 1) = max(abs(error), [], "omitmissing"); %#ok<AGROW>
    meanAbsError(end + 1, 1) = mean(abs(error), "omitmissing"); %#ok<AGROW>
end

stats = table(names, maxAbsError, meanAbsError, ...
    VariableNames=["Signal", "MaxAbsError", "MeanAbsError"]);
end
