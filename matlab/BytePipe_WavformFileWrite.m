function BytePipe_WavformFileWrite( filename, iqData )

csvwrite(filename,[typecast(int16(idata'*32768),'uint16'),typecast(int16(qdata'*32768),'uint16')]);
