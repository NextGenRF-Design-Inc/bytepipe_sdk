function iqData = BytePipe_WavformFileRead( filename )

iqData = csvread(filename);

iqData = iqData(:,1) + 1i*iqData(:,2);


