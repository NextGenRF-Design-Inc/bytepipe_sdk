function BytePipe_WavformFileWrite( filename, iqData )


csvwrite(filename,[typecast(int16(real(iqData)'*32768),'uint16'),typecast(int16(imag(iqData)'*32768),'uint16')]);
