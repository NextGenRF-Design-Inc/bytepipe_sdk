![logo](../../docs/BytePipe_Logo.png)

# RFLAN QPSK - 
The rflan qpsk example utilizes how the basic [rflan build](https://github.com/NextGenRF-Design-Inc/bytepipe_sdk/src/rflan/README.md) can be used to send baseband information over a bandpass channel.

1. Generate symbols
2. Transform symbols in I and Q samples 
3. Write samples to bytepipe using the CLI (command line interface)
4. Bytepipe streams samples to ADRV9002 RF IC for modulation
5. Bandpass signal interacts with communication channel
6. Bytepipe streams demodulated samples from ADRV9002 RF IC 
7. Read samples from bytepipe using the CLI (command line interface)
8. transform I and Q samples into symbols
9. Compare received and transmitted symbols
