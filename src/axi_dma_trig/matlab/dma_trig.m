clear all; close all; clc;

fs = 192e3;
clk = fs*256;

f_tone = fs/32;
t = (0:4*fs/f_tone-1)'/fs;
iq = 1/2 * (sin(2*pi*f_tone*t)+1i*cos(2*pi*f_tone*t));

iq = [repmat(complex(0,0),100,1) ; iq; repmat(complex(0,0),100,1)];


sampleTime = 1/fs;
numSteps = length(iq);
time = sampleTime*(0:numSteps-1);
time = time';
           
simin.time = time;
simin.signals.values = iq;